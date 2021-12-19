/*
 * Enum_Windows.hpp
 *
 *  Created on: Sep 7, 2021
 *      Author: Charles Yarbrough
 */

#ifndef ENUM_WINDOWS_HPP_
#define ENUM_WINDOWS_HPP_

#include <string.h>
#include <windows.h>
#include <iostream>
#include <IOmanip>

#define BUF_SIZE 256  // universal buffer character size for names and commands

// these apps get special attention from BlindSafe
// and except for unknown, these defines should match the enum just below
#define BS_OUTLOOK  "outlook"
#define BS_NOTEPAD  "notepad"
#define BS_EDGE     "edge"
#define BS_CHROME   "chrome"
#define BS_WORD     "word"

enum launch_app {
	outlook, notepad, edge, chrome, word, unknown
};

enum blind_op {
	bl_clean, bl_debug, bl_help, bl_launch, bl_quit, bl_restart,  bl_shutdown
};

// this is effectively the global memory of blindsafe.
// everything we need to know should be here
struct Window_Tracking {
	/*
	 * Global variables directing actions
	 */
	bool list_window = false;        // put short description into listfile
	bool show_window = false;        // put full detail into listfile
	bool kill_window = false;        // kill the process
	bool debug_commentary = false;   // say what we're doing
	bool search_for_window = false;  // we're actively looking for this window

	/*
	 * Global variables tracking  window counts
	 */
	int win_total = 0;             // everything returned by enum_proc()
	int win_count = 0;             // have both text and name
	int win_mixed = 0;             // have one or the other
	int win_hidden_count = 0;      // there but not seen
	int win_blindsafe_windows = 0; // kludge cause blindsafe has lots of stuff
	int win_killed_windows = 0;    // killed
	int win_saved_windows = 0;     // not killed by special intervention

	/*
	 * Global variables tracking window status
	 */
	HWND active_window = 0;
	HWND focus_window = 0;
	HWND forground_window = 0;
	HWND search_window = 0;

	/*
	 * Stuff about the current window
	 */
	HWND current_window;
	long unsigned int pid = 0;
	char titlebuff[BUF_SIZE];
	char namebuff[BUF_SIZE];
	char filename[BUF_SIZE];
	char searchname[BUF_SIZE];
	launch_app what_app = unknown;

	// marks[1] = for developers, subject to change at whim
	char marks[7] = { ' ', ' ', ' ', ' ', ' ', ' ', '\0' }; // @suppress("Avoid magic numbers")
	bool has_title = false;
	bool has_name = false;
	bool is_kill_target = false;
	bool is_blindsafe_window = false;
	int extra_search_windows = 0;
};

Window_Tracking* get_window_tracking();

void setupKnownWindows();

void init_window_tracking(Window_Tracking *wtptr);

BOOL CALLBACK EnumWindowsProc(const HWND hWnd, const long lParam);

bool do_window_enum();

void do_window_enum_plus(blind_op use_cmd);

void do_launch_or_join_window();

#endif /* ENUM_WINDOWS_HPP_ */
