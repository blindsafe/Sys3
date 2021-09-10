/*
 * Enum_Windows.cpp
 *
 *  Created on: Sep 7, 2021
 *      Author: Charles Yarbrough
 */

#define NOPROBLEM 1

#include "Enum_Windows.hpp"

struct Window_Tracking *wt;

void set_window_tracking(Window_Tracking *wtptr) {
	wt = wtptr;
}

Window_Tracking* get_window_tracking() {
	return wt;
}

#if NOPROBLEM

int string_begin(char  full_string[], char test_string[]) {
	// does test_string start full_string?
	int test_len = strlen(test_string);
	int  full_len = strlen(full_string);
	int is_same = 0;
	char save_char;
 	// cout << "string_begin(" << full_string <<  ", "  << test_string << ")" <<endl;
 	if  (full_len >= test_len ) {
 		save_char = full_string[test_len];
 	   full_string[test_len] = 0;
 	  //  cout << "strcmp(" << full_string <<  ", "  << test_string << ")" <<endl;
 		if ( strcmp(full_string, test_string) == 0) {
 			is_same = 1;
 			// cout << "MATCH" << endl;
 		}
 	     full_string[test_len] = save_char;
 	}
 return is_same;
 }
#endif

BOOL CALLBACK EnumWindowsProc(HWND hWnd, long lParam) {
	const int N = 256;
	char textbuff[N];
	char namebuff[N];
	char marks[] = { ' ', ' ', ' ', ' ', ' ',' ', '\0' };
	char my_window[N];
	int is_my_window = 0;
	int kill_window = 0
			; // various reasons to not kill  window, tested below
	int no_title = 0;
	int no_name = 0;

	wt->win_total++;

	strcpy(textbuff, "");
	GetWindowText(hWnd, (LPSTR) textbuff, sizeof(textbuff) - 1);
	if (strlen(textbuff) < 1) {
		strcpy(textbuff, "[NO TITLE]"); no_title = 1;
	}

	strcpy(namebuff, "");
	GetWindowModuleFileName(hWnd, (LPSTR) namebuff, sizeof(namebuff) - 1);
	if (strlen(namebuff) < 1) {
		strcpy(namebuff, "[NO MODULE]"); no_name = 1;
	}

	if ( no_title && no_name) kill_window = 0;
#if NOPROBLEM
	 strcpy(my_window, "C:\\Users\\charlie\\github");
	 if ( string_begin(textbuff, my_window)) {
		// cout  << "textbuff matches!)";
		is_my_window = 1;
	}
	 if ( string_begin(namebuff, my_window)) {
		// cout  << "namebuff matches!)";
		is_my_window = 1;
	 }
	 if (is_my_window) {
		 wt->win_my_windows++;
		 marks[5] = 'M';
	 }
#endif

	if (IsWindowVisible(hWnd)) {
		if (strlen(textbuff) > 0) {
			wt->win_count++;
			kill_window = 1;
			if (hWnd == wt->active_window) {
				marks[0] = 'A';	kill_window = 0;
			}
			if (hWnd == wt->focus_window) {
				marks[1] = 'F';	kill_window = 0;
			}
			if (hWnd == wt->forground_window) {
				marks[2] = 'G';	kill_window = 0;
			}
			marks[3] = 'V';
			if (IsWindowEnabled(hWnd)) {
				marks[4] = 'E';
			}
		} else {
			wt->win_no_title++;
		}
	} else {
		wt->win_hidden_count++;
	}

	if (textbuff[0] || namebuff[0]) {
		cout << std::setw(3) << std::dec << wt->win_count << "/" << std::setw(4)
				<< std::dec << wt->win_total << "." << marks << setw(10)
				<< std::hex << hWnd << " --> " << textbuff << " ==> "
				<< namebuff << endl;
	}

	if ( kill_window ) {
		long unsigned int  pid = 0;
		string filename;
		char syscmd[N] =  " tasklist /svc /FI \"PID eq ";
		cout << "KILL this window? ";
		// does seem to actually close stuff  PostMessage(hWnd, WM_CLOSE, 0, 0);
	    GetWindowThreadProcessId ( hWnd, &pid );
		cout << "and back with pid =" << pid;
	//	 does this even exist? GetProcessImageFileNameA(hWnd, filename, sizeof(filename)-1);
        filename = to_string(pid);
		strcat( syscmd, &filename[0]);
		strcat(syscmd, "\"");
	    cout << "us'ung  "  << syscmd << endl;
		system( syscmd );
		wt->win_killed_windows++;
	}

	return TRUE;
}

