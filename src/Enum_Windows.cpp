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

BOOL CALLBACK EnumWindowsProc(HWND hWnd, long lParam) {
	const int N = 256;
	char textbuff[N];
	char namebuff[N];
	char marks[] = { ' ', ' ', ' ', ' ', ' ', '\0' };

	wt->win_total++;

	strcpy(textbuff, "");
	GetWindowText(hWnd, (LPSTR) textbuff, sizeof(textbuff) - 1);
	if (strlen(textbuff) < 1) {
		strcpy(textbuff, "[NO TITLE]");
	}

	strcpy(namebuff, "");
	GetWindowModuleFileName(hWnd, (LPSTR) namebuff, sizeof(namebuff) - 1);
	if (strlen(namebuff) < 1) {
		strcpy(namebuff, "[NO MODULE]");
	}

	if (IsWindowVisible(hWnd)) {
		if (strlen(textbuff) > 0) {
			wt->win_count++;
			if (hWnd == wt->active_window) {
				marks[0] = 'A';
			}
			if (hWnd == wt->focus_window) {
				marks[1] = 'F';
			}
			if (hWnd == wt->forground_window) {
				marks[2] = 'G';
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
	return TRUE;
}

