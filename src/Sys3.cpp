//============================================================================
// Name        : SimpleListWindows.cpp
// Author      : Don Trummell
// Version     :
// Copyright   : (c) 2021
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <windows.h>
#include <iostream>
#include <IOmanip>
using namespace std;
#include "helper.h"

int win_count = 0;
int win_hidden_count = 0;
int win_no_title = 0;

HWND active_window = 0;
HWND focus_window = 0;
HWND forground_window = 0;

void SetupKnownWIndows() {
	// One window has the focus -- the one that's first in line to get keyboard events.
	// The outer window (that the user can drag around the screen) is "active"
	// if one of its subwindows has the focus, but it might or might not have focus itself.

	active_window = GetActiveWindow(); // top level window associated with focus
	cout << "   Active Window    : " << std::setw(10) << std::hex
			<< active_window << endl;
	focus_window = GetFocus();
	cout << "   Focus Window     : " << std::setw(10) << std::hex
			<< focus_window << endl;
	forground_window = GetForegroundWindow();
	cout << "   Foreground Window: " << std::setw(10) << std::hex
			<< forground_window << endl;
}

BOOL CALLBACK EnumWindowsProc(HWND hWnd, long lParam) {
	const int N = 256;
	char buff[N];

	if (IsWindowVisible(hWnd)) {
		GetWindowText(hWnd, (LPSTR) buff, sizeof(buff) - 1);
		if (strlen(buff) > 0) {
			win_count++;

			char marks[] = { ' ', ' ', ' ', '\0' };
			if (hWnd == active_window) {
				marks[0] = 'A';
			}
			if (hWnd == focus_window) {
				marks[1] = 'F';
			}
			if (hWnd == forground_window) {
				marks[2] = 'G';
			}

			cout << std::setw(4) << std::dec << win_count << ". " << marks
					<< setw(10) << std::hex << hWnd << " --> " << buff << endl;
		} else {
			win_no_title++;
		}
	} else {
		win_hidden_count++;
	}

	return TRUE;
}

int main() {
	cout << "***List Top Level Windows***" << endl;
	show_message(MY_MESSAGE);

	SetupKnownWIndows();
	cout << endl;

	BOOL result = EnumWindows(EnumWindowsProc, 0);
	cout << endl << "Done with " << std::dec << win_count
			<< " visible windows and " << win_hidden_count
			<< " hidden windows, with " << win_no_title
			<< " visible windows having no title." << endl;
	system("pause");
	cout << "***" << endl;

	return result ? 1 : 0;
}
