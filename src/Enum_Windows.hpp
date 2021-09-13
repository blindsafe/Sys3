/*
 * Enum_Windows.hpp
 *
 *  Created on: Sep 7, 2021
 *      Author: Charles Yarbrough
 */

#ifndef ENUM_WINDOWS_HPP_
#define ENUM_WINDOWS_HPP_

#include <windows.h>
#include <iostream>
#include <IOmanip>
using namespace std;

#define BUF_SIZE 256  // universal size for names, commands

struct Window_Tracking {
	/*
	 * Global variables directing actions
	 */
	int list_window = 0;
	int show_window = 0;
	int kill_window = 0;   //  1= yes, 2 = just say what you would do
	int debug_commentary = 0;
	  /*
	  * Global variables tracking  window counts
	 */
	int win_count = 0;
	int win_total = 0;
	int win_hidden_count = 0;
	int win_no_title = 0;
	int win_github_windows = 0;
	int win_killed_windows = 0;
	/*
	 * Global variables tracking window status
	 */
	HWND active_window = 0;
	HWND focus_window = 0;
	HWND forground_window = 0;
	/*
	 * stuff about the current window
	 */
	HWND current_window;
	int could_kill_window = 0;
    char textbuff[BUF_SIZE];
	char namebuff[BUF_SIZE];
	char marks[7] = { ' ', ' ', ' ', ' ', ' ',' ', '\0' };
	int exe_title = 0;
	int exe_name = 0;
};

void SetupKnownWindows();

void init_window_tracking(Window_Tracking *wtptr);


Window_Tracking* get_window_tracking();

BOOL CALLBACK EnumWindowsProc(HWND hWnd, long lParam);

#endif /* ENUM_WINDOWS_HPP_ */
