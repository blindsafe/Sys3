//============================================================================
// Name        : Sys3.cpp
// Author      : Charles Yarbrough
// Version     : 0.1.0
// Copyright   : (c) 2021
// Description : Hello World in C++, Ansi-style
//============================================================================


#include "Sys3.hpp"

struct Window_Tracking wtrk;

void SetupKnownWIndows() {
	// One window has the focus -- the one that's first in line to get keyboard events.
	// The outer window (that the user can drag around the screen) is "active"
	// if one of its subwindows has the focus, but it might or might not have focus itself.

	wtrk.active_window = GetActiveWindow(); // top level window associated with focus
	cout << "   Active Window    : " << std::setw(10) << std::hex
			<< wtrk.active_window << endl;
	wtrk.focus_window = GetFocus();
	cout << "   Focus Window     : " << std::setw(10) << std::hex
			<< wtrk.focus_window << endl;
	wtrk.forground_window = GetForegroundWindow();
	cout << "   Foreground Window: " << std::setw(10) << std::hex
			<< wtrk.forground_window << endl;
}

int main() {
	cout << "***List Top Level Windows***" << endl;

	set_window_tracking(&wtrk);   // Pointer to global shared variables (sorry, bad!)
	SetupKnownWIndows();
	cout << endl;

	BOOL result = EnumWindows(EnumWindowsProc, 0);
	cout << endl << "Done with " << std::dec << wtrk.win_count
			<< " visible windows and " << wtrk.win_hidden_count
			<< " hidden windows, with " << wtrk.win_no_title
			<< " visible windows having no title." << endl;
	system("pause");
	cout << "***" << endl;

	return result ? 1 : 0;
}
