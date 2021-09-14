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
	char cmd[BUF_SIZE];
	int doit = 1;

	while ( doit ) {
		cout << "your wish is my command" << endl <<  "-->";
		cin >> cmd;
		cout << "you said " << cmd << endl;
		system("pause");
		switch ( cmd[0] ) {
		case 't':  { // tasklist
			system( "tasklist");
			break;
		}
		case 'q': { // quit
			doit = 0;
			break;
		}
		case 'c': { //cmd
			system(cmd);
			break;
		}
		case 'k': case 'l': case 's': case 'r': {
			// kill, list, show, review
			cout  <<  "review all windows" << endl;
			init_window_tracking(&wtrk);   // Pointer to global shared variables
			switch ( cmd[0] ) {  // different options as we tour all the windows
			case 'k': {  // actually kill it will killtask
				wtrk.list_window = 1;
				wtrk.kill_window = 1;
				break; }
			case 'l': { // short description
				wtrk.list_window = 1;
				break; }
			case 's' :{ // show, means listtask
				wtrk.show_window = 1;
				break; }
			case 'r': { // review means show what would be billed
				wtrk.list_window = 1;
				wtrk.kill_window = 2;
				break; }
			}
			BOOL result = EnumWindows(EnumWindowsProc, 0);
				cout << endl  << "Done with "<<  result  << " and  "
						<< std::dec << wtrk.win_count
						<< " of " << wtrk.win_total
						 << " with mixed " <<  wtrk.win_mixed
						 << " hidden " << wtrk.win_hidden_count
						 << " github " <<  wtrk.win_github_windows
						 << " and killed "  << wtrk.win_killed_windows
						 << endl;
				system("pause");
				break;
		}
		default: {
			cout <<  "I don't know what you mean by " << endl;
			break;
		}
		}
	}
	cout << "That's all folks!" << endl;
return 1;
}

#if 0 // original test bench
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
#endif
