/*
 * Enum_Windows.cpp
 *
 *  Created on: Sep 7, 2021
 *      Author: Charles Yarbrough
 */

#include "Kill_Window.hpp"

struct Window_Tracking *wt;  // global storage

void setupKnownWIndows() {
	// one time, at the beginning
	// One window has the focus -- the one that's first in line to get keyboard events.
	// The outer window (that the user can drag around the screen) is "active"
	// if one of its subwindows has the focus, but it might or might not have focus itself.

	wt->active_window = GetActiveWindow(); // top level window associated with focus
	cout << "   Active Window    : " << std::setw(10) << std::hex
			<< wt->active_window << endl;
	wt->focus_window = GetFocus();
	cout << "   Focus Window     : " << std::setw(10) << std::hex
			<< wt->focus_window << endl;
	wt->forground_window = GetForegroundWindow();
	cout << "   Foreground Window: " << std::setw(10) << std::hex
			<< wt->forground_window << endl;
}

void init_window_tracking(Window_Tracking *wtptr) {
	// at the beginning of every enum_proc we set up how to keep track
	// of things.
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

	wt->win_total = 0;
	wt->win_count = 0;
	wt->win_mixed = 0;
	wt->win_hidden_count = 0;
	wt->win_blindsafe_windows = 0;
	wt->win_saved_windows = 0;
	wt->win_killed_windows = 0;
}

Window_Tracking* get_window_tracking() {
	// someone wants to know where the global storage is
	return wt;
}

void check_if_blindsafe() {
	// maybe it's one of ours . . . just curiosity
	if (string_contains(wt->titlebuff, "blindsafe")) {
		wt->is_blindsafe_window = 1;
	} else if (string_contains(wt->namebuff, "blindsafe")) {
		wt->is_blindsafe_window = 1;
	} else if (string_contains(wt->titlebuff, "github")) {
		wt->is_blindsafe_window = 1;
	} else if (string_contains(wt->namebuff, "github")) {
		wt->is_blindsafe_window = 1;
	}
	if (wt->is_blindsafe_window) {
		wt->win_blindsafe_windows++;
		wt->marks[5] = 'G';
	}
}

void describe_window(HWND hWnd) {
	// look it over and decide what it is

	wt->has_title = false;
	wt->has_name = false;
	wt->is_blindsafe_window = false;
	wt->is_kill_target = false;
	wt->current_window = hWnd;
	strcpy(wt->marks, "      ");

	strcpy(wt->titlebuff, "");
	GetWindowText(hWnd, (LPSTR) wt->titlebuff, sizeof(wt->titlebuff) - 1);
	if (strlen(wt->titlebuff) < 1) {
		strcpy(wt->titlebuff, "[NO TITLE]");
	} else {
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
	long unsigned int pid = 0;
	GetWindowThreadProcessId(hWnd, &pid);
	string filename = to_string(pid);
	strcpy(wt->filename, &filename[0]);
	if (wt->list_window) {
		{
			cout << std::setw(3) << std::dec << wt->win_count << "/"
					<< std::setw(4) << std::dec << wt->win_total << "."
					<< wt->marks << setw(10) << "pid= " << pid << std::hex
					<< " = " << hWnd << " --> " << wt->titlebuff << " ==> "
					<< wt->namebuff << endl;
		}
	}
	if (wt->show_window) {
		char syscmd[BUF_SIZE] = " tasklist /svc /FI \"PID eq ";
		strcat(syscmd, &filename[0]);
		system(syscmd);
	}

	if (wt->kill_window) {
		if (wt->is_kill_target) {
			kill_window(wt);
		} else {
			// leave sleeping dogs lie
		}
	}
	if (wt->search_for_window) {
		// return the first window that matches
		// with the notable exception of blindsafe itself!
		if (string_contains(wt->titlebuff, wt->searchname)) {
			cout << "this might be it we're looking for " << wt->titlebuff
					<< " at " << wt->forground_window << " vs " << hWnd << endl;
			if (wt->forground_window == hWnd) {
				// what we expect for blindsafe, and nothing else
				// which means that for blindsafe, we return the first previous windows
				cout << " That's us!!' " << endl;
			} else {
				if (wt->search_window) {
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
	return TRUE;
}

