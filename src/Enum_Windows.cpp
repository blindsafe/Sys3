/*
 * Enum_Windows.cpp
 *
 *  Created on: Sep 7, 2021
 *      Author: Charles Yarbrough
 */

#include "Enum_Windows.hpp"

struct Window_Tracking *wt;

void SetupKnownWIndows() {
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
	if ( wt->active_window == 0 ) {
		// first time in
		 SetupKnownWIndows();
	}
	{
		cout << "INITIALIZE!" << endl;
		  wt->list_window = 0;
		  wt->show_window = 0;
		  wt->kill_window = 0;


		  wt->win_total = 0;
		  wt->win_count = 0;
		  wt->win_mixed = 0;
		  wt->win_hidden_count = 0;
		  wt->win_github_windows = 0;
		  wt->win_killed_windows = 0;

		  wt->active_window = 0;
		  wt->focus_window = 0;
		  wt->forground_window = 0;
	}
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
	// if ( wt->debug_commentary ) cout << "--?is_exe("
	//                                                               << std::dec << len << ": " << string << " vs " << exe_string << ".";
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
		if ( wt->debug_commentary ) cout <<  ") with " << end_string << " vs " << exe_string  << ".";
		if ( strcmp( end_string, exe_string) == 0 ) rtn = 1;
	}
	// if ( wt->debug_commentary ) {
	//	if (rtn ) cout << " yes"; else cout << " no"; cout << endl;
   //}
	return rtn;
}

void describe_window( HWND hWnd) {

	char my_window[BUF_SIZE];
		int is_github_window = 0;
		int kill_window = 0; // various reasons to not kill  window, tested below
		int has_title = 1;
		int has_name = 1;

		wt->current_window = hWnd;
		strcpy(wt->marks, "      ");

		strcpy(wt->titlebuff, "") ;wt-> exe_title = 0;
		GetWindowText(hWnd, (LPSTR) wt->titlebuff, sizeof(wt->titlebuff) - 1);
			if (strlen(wt->titlebuff) < 1) {
				strcpy(wt->titlebuff, "[NO TITLE]"); has_title = 0;
			} else {
				wt->exe_title = is_exe(wt->titlebuff);
			}
		    strcpy(wt->namebuff, ""); wt->exe_name = 0;
			GetWindowModuleFileName(hWnd, (LPSTR) wt->namebuff, sizeof(wt->namebuff) - 1);
			if (strlen(wt->namebuff) < 1) {
				strcpy(wt->namebuff, "[NO MODULE]"); has_name = 0;
			} else {
				wt->exe_name = is_exe(wt->namebuff);
			}

			// special kludge coding for charlie so he doesnt kill github stuff
			 strcpy(my_window, "C:\\Users\\charlie\\github");
			 if ( string_begin(wt->titlebuff, my_window)) {
				is_github_window = 1;
			}
			 if ( string_begin(wt->namebuff, my_window)) {
				is_github_window = 1;
			 }
			 if (is_github_window) {
				 wt->win_github_windows++;
				 wt->marks[5] = 'G'; }

			 if ( has_title || has_name ) {
					 wt->win_count++;
					 kill_window = 1;
					if ( wt->debug_commentary ) cout << "KILL SET";
					wt->marks[3] = 'W';
						if (IsWindowVisible(hWnd)) {
						wt->marks[3] = 'V';

						if (hWnd == wt->active_window) {
							wt->marks[0] = 'A';	kill_window = 0;
						}
						if (hWnd == wt->focus_window) {
							wt->marks[1] = 'F';	kill_window = 0;
						}
						if (hWnd == wt->forground_window) {
							wt->marks[2] = '*';	kill_window = 0;
						}

						if (IsWindowEnabled(hWnd)) {
							wt->marks[4] = 'E';
						}
					}
					if ( !has_title || !has_name ) {
						wt->win_mixed++; wt->marks[4] = '!';
					}
				} else {
					wt->win_hidden_count++; wt->marks[4] = 'H';
				}

	if ( is_github_window) {
		if ( wt->debug_commentary ) cout << "ignoring github window" << endl;
		kill_window = 0;
	 }
	if ( kill_window ) {
		if (!has_name && !has_title ) {
			if ( wt->debug_commentary ) cout << "Why kill this empty  window?";
		kill_window = 0;
		}
		else if (!wt->exe_title && !wt->exe_name) {
			// if ( wt->debug_commentary ) cout << "Why kill this  window that's not an exe?";
			//	kill_window = 0;
		}
	}
	wt->could_kill_window = kill_window;
	if ( wt->debug_commentary ) cout << " three stooges " << kill_window
			 <<  "  " <<  wt->could_kill_window << "  " << wt->kill_window
			 << " with marks " << wt->marks
			 << "and names = " <<  wt->namebuff
			 << ", " << wt->titlebuff
			 << endl;
}

void do_window( HWND hWnd) {
// everything below is about the HWND we have in hand
	long unsigned int  pid = 0;
	string filename;
   GetWindowThreadProcessId ( hWnd, &pid );
     filename = to_string(pid);
	if (wt->list_window) {
		cout << std::setw(3) << std::dec << wt->win_count << "/" << std::setw(4)
				<< std::dec << wt->win_total << "." <<wt-> marks << setw(10)
				<< std::hex << hWnd << " --> " << wt->titlebuff << " ==> "
				<< wt->namebuff << endl;
	}
	if (wt->show_window ) {
		char syscmd[BUF_SIZE] =  " tasklist /svc /FI \"PID eq ";
		strcat( syscmd, &filename[0]);
		 system( syscmd );
	}

	if ( wt->kill_window &&	wt->could_kill_window ) {
			char syscmd[BUF_SIZE] =  " taskkill /PID ";
			strcat( syscmd, &filename[0]);
			strcat(syscmd, "  /F");
			if (wt->debug_commentary ) {
				cout << "(**)Kill window " << wt->titlebuff << "==" << wt->namebuff << "with"
				<< syscmd << endl;
			}
			if ( wt->kill_window == 2) {
				cout <<"system( " << syscmd << " )" << endl;
			}
			else {
				cout << "(**)Kill window " << wt->titlebuff << "==" << wt->namebuff << "with"
				<< syscmd << endl;
				system("pause");
				system( syscmd );
				cout << "and back";
				system("pause");
			}
		wt->win_killed_windows++;
	}
}

BOOL CALLBACK EnumWindowsProc(HWND hWnd, long lParam) {


	wt->win_total++;
    describe_window(hWnd);
	do_window(hWnd);
return TRUE;
}

