/*
 * Enum_Windows.cpp
 *
 *  Created on: Sep 7, 2021
 *      Author: Charles Yarbrough
 */

#include "Enum_Windows.hpp"
#include "Kill_Window.hpp"

//
// Global variable representing "object state" that would have been set in a constructor
struct Window_Tracking *wt;

//
// Expose "object state"
Window_Tracking* get_window_tracking() {
	return wt;
}

void setupKnownWIndows() {
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
	wt = wtptr;
	if (wt->active_window == 0) {
		// first time in
		setupKnownWIndows();
	}

	wt->list_window = false;
	wt->show_window = false;
	wt->kill_window = 0;

	wt->win_total = 0;
	wt->win_count = 0;
	wt->win_mixed = 0;
	wt->win_hidden_count = 0;
	wt->win_blindsafe_windows = 0;
	wt->win_saved_windows = 0;
	wt->win_killed_windows = 0;

	wt->focus_window = 0;
	wt->forground_window = 0;
}

//
// See example: https://stackoverflow.com/questions/15515088/how-to-check-if-string-starts-with-certain-string-in-c/15515276
BOOL string_begin(const char full_string[], const char test_string[]) {
	// does test_string start full_string?
	int test_len = strlen(test_string);
	int full_len = strlen(full_string);
	BOOL is_same = FALSE;
	if (full_len >= test_len) {
// NO NO NO, never modify input, even if restored!
//		char save_char = full_string[test_len];
//		full_string[test_len] = 0;
//		if (strcmp(full_string, test_string) == 0) {
//			is_same = TRUE;
//		}
//		full_string[test_len] = save_char;
		if (strncmp(full_string, test_string, test_len) == 0) {
			is_same = TRUE;
		}
	}
	return is_same;
}

BOOL is_exe(const char input_string[]) {
	// name might end in ".exe" or ".exe)"
	char exe_string[] = ".exe";
	char end_string[5];
	BOOL rtn = FALSE;
	BOOL copied_5 = FALSE;
	// if ( wt->debug_commentary ) cout << "--?is_exe("
	// << std::dec << len << ": " << string << " vs " << exe_string << ".";
	const int len = strlen(input_string);
	if (len >= 4) {
		if (len >= 5) {
			if (input_string[len - 1] == ')') {
				strcpy(end_string, &input_string[len - 5]);
				end_string[4] = '\0';
				copied_5 = TRUE;
			}
		}
		if (!copied_5) {
			strcpy(end_string, &input_string[len - 4]);
		}
		if (wt->debug_commentary)
			cout << ") with " << end_string << " vs " << exe_string << ".";
		if (strcmp(end_string, exe_string) == 0)
			rtn = TRUE;
	}
	// if ( wt->debug_commentary ) {
	//	if (rtn ) cout << " yes"; else cout << " no"; cout << endl;
	//}
	return rtn;
}

void describe_window(const HWND hWnd) {
	BOOL should_kill_window = TRUE; // various reasons to not kill  window, tested below
	wt->has_title = TRUE;
	wt->has_name = TRUE;
	wt->is_blindsafe_window = FALSE;

	wt->current_window = hWnd;
	strcpy(wt->marks, "      ");

	strcpy(wt->titlebuff, "");
	wt->exe_title = 0;
	GetWindowText(hWnd, (LPSTR) wt->titlebuff, sizeof(wt->titlebuff) - 1);
	if (strlen(wt->titlebuff) < 1) {
		strcpy(wt->titlebuff, "[NO TITLE]");
		wt->has_title = 0;
	} else {
		wt->exe_title = is_exe(wt->titlebuff);
	}
	strcpy(wt->namebuff, "");
	wt->exe_name = 0;
	GetWindowModuleFileName(hWnd, (LPSTR) wt->namebuff,
			sizeof(wt->namebuff) - 1);
	if (strlen(wt->namebuff) < 1) {
		strcpy(wt->namebuff, "[NO MODULE]");
		wt->has_name = 0;
	} else {
		wt->exe_name = is_exe(wt->namebuff);
	}

	if (string_contains(wt->titlebuff, "blindsafe")) {
		wt->is_blindsafe_window = TRUE;
	} else if (string_contains(wt->namebuff, "blindsafe")) {
		wt->is_blindsafe_window = TRUE;
	} else if (string_contains(wt->titlebuff, "github")) {
		wt->is_blindsafe_window = TRUE;
	} else if (string_contains(wt->namebuff, "github")) {
		wt->is_blindsafe_window = TRUE;
	}

	if (wt->is_blindsafe_window) {
		cout << "blindsafe " << wt->titlebuff << "and" << wt->namebuff << endl;
		wt->win_blindsafe_windows++;
		wt->marks[5] = 'G';
	}

	if (wt->has_title || wt->has_name) {
		wt->win_count++;
		wt->is_special = special_window(wt);
		if ((wt->is_special == 0) || (wt->is_special == 2))
			should_kill_window = TRUE;
		wt->marks[3] = 'W';
		if (IsWindowVisible(hWnd)) {
			wt->marks[3] = 'V';

			if (hWnd == wt->active_window) {
				wt->marks[0] = 'A';
				should_kill_window = FALSE;
			}
			if (hWnd == wt->focus_window) {
				wt->marks[1] = 'F';
				should_kill_window = FALSE;
			}
			if (hWnd == wt->forground_window) {
				wt->marks[2] = '*';
				should_kill_window = FALSE;
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

	if (wt->is_blindsafe_window) {
		should_kill_window = FALSE;
	}
	if (should_kill_window) {
		if (!wt->has_name && !wt->has_title) {
			cout << " . . .Why kill this empty  window? ";
			should_kill_window = FALSE;
		}
	}

	wt->could_kill_window = should_kill_window;

	if (wt->debug_commentary) {
		BOOL killed_a_window = kill_window(NULL);
		cout << " describe out with  " << killed_a_window << "  "
				<< wt->could_kill_window << "  " << wt->kill_window
				<< " with marks " << wt->marks << "and names = " << wt->namebuff
				<< ", " << wt->titlebuff << endl;
	}
}

void do_window(const HWND hWnd) {
// everything below is about the HWND we have in hand
	long unsigned int pid = 0;
	string filename;
	GetWindowThreadProcessId(hWnd, &pid);
	filename = to_string(pid);
	strcpy(wt->filename, &filename[0]);
	if (wt->list_window) {
		{
			cout << std::setw(3) << std::dec << wt->win_count << "/"
					<< std::setw(4) << std::dec << wt->win_total << "."
					<< wt->marks << setw(10) << "pid= " << pid << std::hex
					<< hWnd << " --> " << wt->titlebuff << " ==> "
					<< wt->namebuff << endl;
		}
	}
	if (wt->show_window) {
		char syscmd[BUF_SIZE] = " tasklist /svc /FI \"PID eq ";
		strcat(syscmd, &filename[0]);
		system(syscmd);
	}

	// cout  << "kills " << wt->kill_window << " " <<  wt->could_kill_window << endl;
	if (wt->kill_window && wt->could_kill_window) {
		kill_window(wt);
	}
}

BOOL CALLBACK EnumWindowsProc(const HWND hWnd, const long lParam) {
	wt->win_total++;
	describe_window(hWnd);
	do_window(hWnd);
	return TRUE;
}
