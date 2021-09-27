/*
 * Kill_Window.cpp
 *
 *  Created on: Sep 12, 2021
 *      Author: Charles Yarbrough
 */
#include "Enum_Windows.hpp"
#include "Kill_Window.hpp"
#include <cstring>

bool string_contains(const char *target_string, const char *test_string) {
	return !!(strstr(target_string, test_string));
}

// is this a window we want to kill?
bool is_kill_target_window(const Window_Tracking *wt) {
	// by painful discovery, some windows aren't to be canceled
	// but others definitely want to be!
	bool is_kill_target = false;

	// cout << "test  " << wt->titlebuff << " -- " << wt->namebuff << endl;

	if (string_contains(wt->titlebuff, "Notepad"))
		is_kill_target = true;
	else if (string_contains(wt->titlebuff, "Word"))
		is_kill_target = true;
	else if ((wt->titlebuff[0] == 'W') && (wt->titlebuff[1] == '\0')) {
		cout << "this appears to be Word?" << endl;
		is_kill_target = true;
	} else if (string_contains(wt->titlebuff, "Explorer")) {
		if (string_contains(wt->titlebuff, "File")) {
			// if we kill File Explorer we lose the ribbon at the bottom
			// what to do ??
		} else
			is_kill_target = true;
	} else if (string_contains(wt->titlebuff, "Outlook"))
		is_kill_target = true;
	else if (string_contains(wt->titlebuff, "Bing"))
		is_kill_target = true;
	else if (string_contains(wt->titlebuff, "Edge"))
		is_kill_target = true;
	else if (string_contains(wt->titlebuff, "Google"))
		is_kill_target = true;
	else if (string_contains(wt->titlebuff, "Microsoft"))
		is_kill_target = true;
	else if (string_contains(wt->titlebuff, "Maps"))
		is_kill_target = true;
	else if (string_contains(wt->titlebuff, "Command"))
		is_kill_target = true;
	// }
	if (wt->debug_commentary && is_kill_target) {
		cout << "special = " << is_kill_target << " for " << wt->filename
				<< " =  " << wt->titlebuff << endl;
		system("pause");
	}
	return is_kill_target;
}

bool kill_window(Window_Tracking *wt) {
	//  kill the current window
	int killed = false;
	char syscmd[BUF_SIZE] = " taskkill /PID ";
	strcat(syscmd, wt->filename);
	strcat(syscmd, "  /F");

	if (wt->debug_commentary) {
		cout << "(**)Kill window " << wt->titlebuff << "==" << wt->namebuff
				<< "with" << syscmd << endl;
		system("pause");
	}
	system(syscmd);
	if (wt->debug_commentary) {
		cout << "and back" << endl;
		system("pause");
	}
	wt->win_killed_windows++;
	killed = true;

	return !!killed;
}
