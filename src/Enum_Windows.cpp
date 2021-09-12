/*
 * Enum_Windows.cpp
 *
 *  Created on: Sep 7, 2021
 *      Author: Charles Yarbrough
 */

#include "Enum_Windows.hpp"

struct Window_Tracking *wt;

void set_window_tracking(Window_Tracking *wtptr) {
	wt = wtptr;
}

Window_Tracking* get_window_tracking() {
	return wt;
}


int string_begin(char  full_string[], char test_string[]) {
	// does test_string start full_string?
	int test_len = strlen(test_string);
	int  full_len = strlen(full_string);
	int is_same = 0;
	char save_char;
 	if  (full_len >= test_len ) {
 		save_char = full_string[test_len];
 	   full_string[test_len] = 0;
 		if ( strcmp(full_string, test_string) == 0) {
 			is_same = 1;
 		}
 	     full_string[test_len] = save_char;
 	}
 return is_same;
 }

int is_exe( char string[]) {
	// name might end in ".exe" or ".exe)"
	int len = strlen(string);
	char exe_string[] = ".exe";
	char end_string[5];
	int rtn = 0; int copied_5 = 0;
	// cout << "--?is_exe(" << std::dec << len << ": " << string << " vs " << exe_string << ".";
	if ( len >= 4 ) {
		if ( len >= 5 ) {
			if ( string[len - 1] == ')' ) {
				strcpy(end_string, &string[len - 5]); end_string[4] = '\0';
				copied_5 = 1;
			}
		}
		if ( ! copied_5 ) {
			strcpy(end_string, &string[len - 4]);
		}
		// cout <<  ") with " << end_string << " vs " << exe_string  << ".";
		if ( strcmp( end_string, exe_string) == 0 ) rtn = 1;
	}
	// if (rtn ) cout << " yes"; else cout << " no"; cout << endl;
	return rtn;
}
BOOL CALLBACK EnumWindowsProc(HWND hWnd, long lParam) {
	const int N = 256;
	char textbuff[N];
	char namebuff[N];
	char marks[] = { ' ', ' ', ' ', ' ', ' ',' ', '\0' };
	char my_window[N];
	int is_github_window = 0;
	int kill_window = 0; // various reasons to not kill  window, tested below
	int has_title = 1; int exe_title = 0;
	int has_name = 1; int exe_name = 0;

	wt->win_total++;

	strcpy(textbuff, "");
	GetWindowText(hWnd, (LPSTR) textbuff, sizeof(textbuff) - 1);
	if (strlen(textbuff) < 1) {
		strcpy(textbuff, "[NO TITLE]"); has_title = 0;
	} else {
		exe_title = is_exe(textbuff);
	}
    strcpy(namebuff, "");
	GetWindowModuleFileName(hWnd, (LPSTR) namebuff, sizeof(namebuff) - 1);
	if (strlen(namebuff) < 1) {
		strcpy(namebuff, "[NO MODULE]"); has_name = 0;
	} else {
		exe_name = is_exe(namebuff);
	}

	 strcpy(my_window, "C:\\Users\\charlie\\github");
	 if ( string_begin(textbuff, my_window)) {
		is_github_window = 1;
	}
	 if ( string_begin(namebuff, my_window)) {
		is_github_window = 1;
	 }
	 if (is_github_window) {
		 wt->win_github_windows++;
		 marks[5] = 'G';
	 }


	if (IsWindowVisible(hWnd)) {
		if (strlen(textbuff) > 0) {
			wt->win_count++;
			marks[3] = 'V';
			kill_window = 1;
			if (hWnd == wt->active_window) {
				marks[0] = 'A';	kill_window = 0;
			}
			if (hWnd == wt->focus_window) {
				marks[1] = 'F';	kill_window = 0;
			}
			if (hWnd == wt->forground_window) {
				marks[2] = '*';	kill_window = 0;
			}

			if (IsWindowEnabled(hWnd)) {
				marks[4] = 'E';
			}
		} else {
			wt->win_no_title++;
		}
	} else {
		wt->win_hidden_count++;
	}

	// if (has_name || has_title) {
	if ( is_github_window) {
	//	cout << "ignoring github window" << endl;
		kill_window = 0;
	}
	else if ( exe_title || exe_name || kill_window   ) {
		cout << std::setw(3) << std::dec << wt->win_count << "/" << std::setw(4)
				<< std::dec << wt->win_total << "." << marks << setw(10)
				<< std::hex << hWnd << " --> " << textbuff << " ==> "
				<< namebuff << endl;

		if ( kill_window &&!has_name && !has_title  ) {
		//	cout << "Why kill this empty  window?";
			kill_window = 0;
		}
	}

	if ( kill_window) {
		long unsigned int  pid = 0;
		string filename;
	//	char syscmd[N] =  " tasklist /svc /FI \"PID eq ";
		char syscmd[N] =  " taskkill /PID ";
	    GetWindowThreadProcessId ( hWnd, &pid );
        filename = to_string(pid);
		strcat( syscmd, &filename[0]);
		strcat(syscmd, " /F");
		cout << "Kill window " << textbuff << "==" << namebuff << "with"
				<< syscmd << endl;
		system( syscmd );
		wt->win_killed_windows++;
	}

	return TRUE;
}

