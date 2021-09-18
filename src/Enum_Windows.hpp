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

#define BUF_SIZE 256  // universal buffer character size for names and commands

struct Window_Tracking {
	/*
	 * Global variables directing actions
	 */
	BOOL list_window = 0;
	BOOL show_window = 0;
	int kill_window = 0; //  0 = initial state, 1 = yes, 2 = just say what you would do
	int debug_commentary = 0;

	/*
	 * Global variables tracking  window counts
	 */
	int win_total = 0;                    // everything returned by enum_proc()
	int win_count = 0;                    // have both text and name
	int win_mixed = 0;                    // have one or the other
	int win_hidden_count = 0;  // there but not seen
	int win_blindsafe_windows = 0;         // kludge cause blindsafe has lots of stuff
	int win_killed_windows = 0;           // killed
	int win_saved_windows = 0;            // not killed by special intervention

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
	char titlebuff[BUF_SIZE];
	char namebuff[BUF_SIZE];
	char filename[BUF_SIZE];

	// marks[1] =
	char marks[7] = { ' ', ' ', ' ', ' ', ' ', ' ', '\0' };
	int exe_title = 0;
	int has_title = 0;
	int exe_name = 0;
	int has_name = 0;
	int is_special; // 0 = no 1 = yes, save, 2 = definitely kill, 3 = yes, save, is "default"
	int is_blindsafe_window;
};

void setupKnownWindows();

void init_window_tracking(Window_Tracking *wtptr);

Window_Tracking* get_window_tracking();

BOOL CALLBACK EnumWindowsProc(HWND hWnd, long lParam);

#endif /* ENUM_WINDOWS_HPP_ */
