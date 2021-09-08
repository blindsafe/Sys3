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

struct Window_Tracking {
	/*
	 * Global variables tracking  window counts
	 */
	int win_count = 0;
	int win_total = 0;
	int win_hidden_count = 0;
	int win_no_title = 0;

	/**
	 * Global variables tracking window status
	 */
	HWND active_window = 0;
	HWND focus_window = 0;
	HWND forground_window = 0;
};

void set_window_tracking(Window_Tracking *wtptr);

Window_Tracking* get_window_tracking();

BOOL CALLBACK EnumWindowsProc(HWND hWnd, long lParam);

#endif /* ENUM_WINDOWS_HPP_ */
