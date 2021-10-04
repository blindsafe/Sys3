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

// is this a window we might want to kill?
bool is_kill_target_window(const Window_Tracking *wt) {
	// by painful discovery, some windows aren't to be canceled
	// but others definitely want to be!
	bool is_kill_target = false;

	// cout << "test  " << wt->titlebuff << " -- " << wt->namebuff << endl;

	if (string_contains(wt->titlebuff, "notepad"))
		is_kill_target = true;
	else if (string_contains(wt->titlebuff, "word"))
		is_kill_target = true;
	else if ((wt->titlebuff[0] == 'w') && (wt->titlebuff[1] == '\0')) {
		// cout << "this appears to be Word?" << endl;
		is_kill_target = true;
	} else if (string_contains(wt->titlebuff, "explorer")) {
		if (string_contains(wt->titlebuff, "file")) {
			// if we kill File Explorer we lose the ribbon at the bottom
			// what to do ??
		} else
			is_kill_target = true;
	} else if (string_contains(wt->titlebuff, "outlook"))
		is_kill_target = true;
	else if (string_contains(wt->titlebuff, "bing"))
		is_kill_target = true;
	else if (string_contains(wt->titlebuff, "edge"))
		is_kill_target = true;
	else if (string_contains(wt->titlebuff, "google"))
		is_kill_target = true;
	else if (string_contains(wt->titlebuff, "microsoft"))
		is_kill_target = true;
	else if (string_contains(wt->titlebuff, "maps"))
		is_kill_target = true;
	else if (string_contains(wt->titlebuff, "command"))
		is_kill_target = true;
	else if (string_contains(wt->titlebuff, "ccleaner"))
		is_kill_target = true;

	if (wt->debug_commentary && is_kill_target) {
		cout << "kill special = " << wt->filename << " =  " << wt->titlebuff
				<< endl;
		// system("pause");
	}

	return is_kill_target;
}

bool kill_window(Window_Tracking *wt) {
	//  kill the current window
	int killed = false;

	if (wt->debug_commentary) {
		cout << endl << "(**)Kill window " << wt->pid << wt->titlebuff << "=="
				<< wt->namebuff << endl;
		// system("pause");
	}

#if 0 // command kill
	{
	char syscmd[BUF_SIZE] = " taskkill /PID ";
		strcat(syscmd, wt->filename);
		strcat(syscmd, "  /F");
	system(syscmd);
	}
# else // api kill
	{
		HANDLE local_hWnd;
		PostMessage(wt->current_window, WM_CLOSE, 0, 0);
		Sleep(10);
		local_hWnd = OpenProcess(PROCESS_TERMINATE, 0, wt->pid);
		if (local_hWnd) {
			TerminateProcess(local_hWnd, 0);
			CloseHandle(local_hWnd);
		}

	}
#endif

	if (wt->debug_commentary) {
		cout << "and back" << endl << endl;
		// system("pause");
	}
	wt->win_killed_windows++;
	killed = true;

	return !!killed;
}
