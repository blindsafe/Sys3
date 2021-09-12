//============================================================================
// Name        : Sys3.cpp
// Author      : Charles Yarbrough
// Version     : 0.1.0
// Copyright   : (c) 2021
// Description : Hello World in C++, Ansi-style
//============================================================================


#include "Sys3.hpp"

struct Window_Tracking wtrk;



int main() {
	cout << "***List Top Level Windows***" << endl;

	init_window_tracking(&wtrk);   // Pointer to global shared variables (sorry, bad!)
	wtrk.show_window = 1;


	// system("tasklist");	cout << "tasklist at beginning" << endl; system("pause");

	BOOL result = EnumWindows(EnumWindowsProc, 0);
	cout << endl  << "Done with "<<  result  << "and  " << std::dec << wtrk.win_count
			<< " visible windows and " <<  wtrk.win_github_windows << " my_windows and "
			<< wtrk.win_killed_windows << " killed windows and "
			<< wtrk.win_hidden_count << " hidden windows, with " << wtrk.win_no_title
			<< " visible windows having no title." << endl;
	system("pause");


	init_window_tracking(&wtrk);   // Pointer to global shared variables (sorry, bad!)
	// try again to see whats different
	wtrk.show_window = 1; wtrk.kill_window = 1;
    result = EnumWindows(EnumWindowsProc, 0);
	cout << endl  << "Done with "<<  result  << "and  " << std::dec << wtrk.win_count
			<< " visible windows and " <<  wtrk.win_github_windows << " my_windows and "
			<< wtrk.win_killed_windows << " killed windows and "
			<< wtrk.win_hidden_count << " hidden windows, with " << wtrk.win_no_title
			<< " visible windows having no title." << endl;
	system("pause");

	// system("tasklist") ; cout << "tasklist at end of 2nd go" << endl; 	system("pause");
	cout << "***" << endl;

	return result ? 1 : 0;
}
