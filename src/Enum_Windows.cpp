/*
 * Enum_Windows.cpp
 *
 *  Created on: Sep 7, 2021
 *      Author: Charles Yarbrough
 */

#include "Kill_Window.hpp"

using namespace std;

struct Window_Tracking *wt; // pointer to global storage, actually declared in sys3.ccp

void setupKnownWIndows() {
	// one time, at the beginning
	// One window has the focus -- the one that's first in line to get keyboard events.
	// The outer window (that the user can drag around the screen) is "active"
	// if one of its subwindows has the focus, but it might or might not have focus itself.

	wt->active_window = GetActiveWindow(); // top level window associated with focus
	cout << "   Active Window    : " << std::setw(10) << std::hex // @suppress("Avoid magic numbers")
			<< wt->active_window << endl;
	wt->focus_window = GetFocus();
	cout << "   Focus Window     : " << std::setw(10) << std::hex // @suppress("Avoid magic numbers")
			<< wt->focus_window << endl;
	wt->forground_window = GetForegroundWindow();
	cout << "   Foreground Window: " << std::setw(10) << std::hex // @suppress("Avoid magic numbers")
			<< wt->forground_window << endl;
}

void init_window_tracking(Window_Tracking *wtptr) {
	// at the beginning of every enum_proc we have to set up how to keep track
	// of things and wtptr is our global memory, our total current state
	wt = wtptr;
	if (wt->active_window == 0) {
		// first time in
		setupKnownWIndows();
	}

	wt->list_window = false;
	wt->show_window = false;
	wt->kill_window = false;
	wt->search_for_window = false;
	wt->extra_search_windows = 0;

	wt->search_window = 0;
	strcpy(wt->searchname, "");
	wt->what_app = unknown;

	wt->win_total = 0;
	wt->win_count = 0;
	wt->win_mixed = 0;
	wt->win_hidden_count = 0;
	wt->win_blindsafe_windows = 0;
	wt->win_saved_windows = 0;
	wt->win_killed_windows = 0;
}

Window_Tracking* get_window_tracking() {
	// someone wants to know where the global storage is, so we tell them
	return (wt);
}

void check_if_blindsafe() {
	// maybe it's one of ours . . . but that's just curiosity
	if (string_contains(wt->titlebuff, "blindsafe")) {
		wt->is_blindsafe_window = true;
	} else if (string_contains(wt->namebuff, "blindsafe")) {
		wt->is_blindsafe_window = true;
	} else if (string_contains(wt->titlebuff, "github")) {
		wt->is_blindsafe_window = true;
	} else if (string_contains(wt->namebuff, "github")) {
		wt->is_blindsafe_window = true;
	}
	if (wt->is_blindsafe_window) {
		wt->win_blindsafe_windows++;
		wt->marks[5] = 'G'; // @suppress("Avoid magic numbers")
	}
}

void tolower_string(char *s) {
	// makes name comparisons simpler
	char *ss = s;
	while (*ss) {
		*ss = tolower(*ss);
		ss++;
	}
}

void describe_window(HWND hWnd) {
	// look it over and decide what it is

	wt->has_title = false;
	wt->has_name = false;
	wt->is_blindsafe_window = false;
	wt->is_kill_target = false;
	wt->current_window = hWnd;
	wt->pid = 0;
	strcpy(wt->marks, "      ");

	strcpy(wt->titlebuff, "");
	GetWindowText(hWnd, static_cast<LPSTR>(wt->titlebuff),
			sizeof(wt->titlebuff) - 1);
	if (strlen(wt->titlebuff) < 1) {
		strcpy(wt->titlebuff, "[NO TITLE]");
	} else {
		tolower_string(wt->titlebuff);
		wt->has_title = true;
	}
	// NB: sometimes namebuff returns the previous name so that
	// we can get a titlebuff with "Google" but a namebuff that's the
	// previous .exe, sometimes a Github
	// see special coding in Github section
	strcpy(wt->namebuff, "");
	GetWindowModuleFileName(hWnd, (LPSTR) wt->namebuff,
			sizeof(wt->namebuff) - 1);
	if (strlen(wt->namebuff) < 1) {
		strcpy(wt->namebuff, "[NO MODULE]");
	} else {
		tolower_string(wt->namebuff);
		wt->has_name = true;
	}

	if (wt->has_title || wt->has_name) {
		wt->win_count++;
		wt->is_kill_target = is_kill_target_window(wt);
		if (!wt->is_kill_target) {
			check_if_blindsafe();
		}

		wt->marks[3] = 'W';
		if (IsWindowVisible(hWnd)) {
			wt->marks[3] = 'V';
			if (hWnd == wt->active_window) {
				wt->marks[0] = 'A';
			}
			if (hWnd == wt->focus_window) {
				wt->marks[1] = 'F';
			}
			if (hWnd == wt->forground_window) {
				wt->marks[2] = '*';
			}

			if (IsWindowEnabled(hWnd)) {
				wt->marks[4] = 'E';
			}
		}
		if (!wt->has_title || !wt->has_name) {
			wt->win_mixed++;
			wt->marks[4] = '!';
		}
	} else {
		wt->win_hidden_count++;
		wt->marks[4] = 'H';
	}

	if (wt->debug_commentary) {
		cout << " describe out with  " << wt->is_kill_target << "  "
				<< wt->kill_window << " with marks " << wt->marks
				<< "and names = " << wt->namebuff << ", " << wt->titlebuff
				<< endl;
	}
}

void do_window(HWND hWnd) {
// everything below is stuff to with with the HWND we have in hand

	GetWindowThreadProcessId(hWnd, &wt->pid);
	string filename = to_string(wt->pid);
	strcpy(wt->filename, &filename[0]);
	if (wt->list_window) {
		{
			cout << std::setw(3) << std::dec << wt->win_count << "/"
					// @suppress("Avoid magic numbers")
					<< std::setw(4) << std::dec << wt->win_total << "."
					// @suppress("Avoid magic numbers")
					<< wt->marks << setw(10) << "pid= " << wt->pid << std::hex
					// @suppress("Avoid magic numbers")
					<< " = " << hWnd << " --> " << wt->titlebuff << " ==> "
					<< wt->namebuff << endl;
		}
	}
	if (wt->show_window) {
		char syscmd[BUF_SIZE] = " tasklist /svc /FI \"PID eq ";
		strcat(syscmd, &filename[0]);
		system(syscmd);
	}

	if (wt->kill_window && wt->is_kill_target) {
		kill_window(wt);
	}
	if (wt->search_for_window) {
		// return the first window that matches
		// with the notable exception of blindsafe itself!
		if (string_contains(wt->titlebuff, wt->searchname)) {
			if (wt->forground_window == hWnd) {
				// what we expect for blindsafe, and nothing else
				// which means that for blindsafe, we return the first previous windows
			} else {
				if (wt->is_blindsafe_window) {
					// special case: we only need the current one
					cout << "previous blindsafe window to be discarded" << endl;
					kill_window(wt);
				} else if (wt->search_window) {
					++wt->extra_search_windows;
				} else {
					wt->search_window = hWnd;
				}
			}
		}
	}
}

BOOL CALLBACK EnumWindowsProc(HWND hWnd, long lParam) {
	wt->win_total++;
	describe_window(hWnd);
	do_window(hWnd);
	return (TRUE);
}

bool do_window_enum() {
	// all access to WindowsProcs comes thru here
	// switches and values set in wtrk by the caller specify whats to be done
	cout << endl << "---- do_window_loop() ----" << endl;
	bool result = EnumWindows(EnumWindowsProc, 0);
	cout << endl << "Done [result: " << result << "] and  " << std::dec
			<< wt->win_count << " of " << wt->win_total << " with mixed "
			<< wt->win_mixed << " hidden " << wt->win_hidden_count << endl
			<< "   and  blindsafe " << wt->win_blindsafe_windows << "  killed "
			<< wt->win_killed_windows << "  saved " << wt->win_saved_windows
			<< endl;
	//  system("pause");
	return result;
}

void do_window_enum_plus(const char command_char) {
	// various commands want a tour, sometimes then followed bymore to do
	init_window_tracking(wt);   // Pointer to global shared variables
	switch (command_char) {
	case 's':
	case 'r':
	case 'c': {
		//	wt->list_window = true;
		wt->kill_window = true;
		do_window_enum();
		switch (command_char) {
		case 's': { // shutdown
			system("shutdown /s /f /t 0");
			break;
		}
		case 'r': {
			system("shutdown /r /f /t 0");
			break;
		}
		case 'c': {
			break;
		}
		default: {
			cout << " unrecognized command '" << command_char << "', ignoring"
					<< endl;
			break;
		}
		}
	}
	}
}

void fill_appname(char *appname) {
	// various shortcuts will replace appname with it's regular name

	if (strlen(appname) > 1) {
		// for now, only single letter shortcuts
	} else {
		switch (appname[0]) {
		case 'o': {
			strcpy(appname, BS_OUTLOOK);
			break;
		}
		case 'n': {
			strcpy(appname, BS_NOTEPAD);
			break;
		}
		case 'e': {
			strcpy(appname, BS_EDGE);
			break;
		}
		case 'c': {
			strcpy(appname, BS_CHROME);
			break;
		}
		case 'w': {
			strcpy(appname, BS_WORD);
			break;
		}
		default: {
			cout << "unknown shortcut letter " << appname << endl;
			strcpy(appname, "");
			break;
		}
		}
	}
	// now check if we care about this name
	if (strcmp(appname, BS_OUTLOOK) == 0) {
		wt->what_app = outlook;
	} else if (strcmp(appname, BS_NOTEPAD) == 0) {
		wt->what_app = notepad;
	}
}

void do_launch_or_join_window() {
	// if the app already exists, move to it
	// else launch it
	// useful for things like outlook and notepad which start multiple copies
	char appname[BUF_SIZE];
	init_window_tracking(wt);   // Pointer to global shared variables
	cin >> appname;
	fill_appname(appname);

	if (appname[0]) {
		// we actualy have one
		wt->search_for_window = true;
		// wt->list_window = true; // for debugging
		strcpy(wt->searchname, appname);
		do_window_enum();
		if (wt->search_window) {
			cout << "we found one!, now what?" << endl;
			if (wt->extra_search_windows) {
				cout << "and what do i do about the "
						<< wt->extra_search_windows << " xtras?" << endl;
			}
			cout << "we will set focus on " << appname << endl;
			SetFocus(wt->search_window);
		} else {
			char syscmd[BUF_SIZE] = "start ";
			strcat(syscmd, appname);
			cout << "we will launch " << syscmd << endl;
			system(syscmd);
		}
		system("pause");
	}
}
