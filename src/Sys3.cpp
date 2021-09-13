//============================================================================
// Name        : Sys3.cpp
// Author      : Charles Yarbrough
// Version     : 0.1.0
// Copyright   : (c) 2021
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "Sys3.hpp"

struct Window_Tracking wtrk_old;

int main() {
	char cmd[BUF_SIZE];
	int doit = 1;

	while (doit) {
		cout << "your wish is my command" << endl << "-->";
		cin >> cmd;
		cout << "you said " << cmd << endl;
		system("pause");
		switch (cmd[0]) {
		case 't': { // tasklist
			system("tasklist");
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
		case 'k':
		case 'l':
		case 's':
		case 'r': {
			// kill, list, show, review
			cout << "review all windows" << endl;
			init_window_tracking(&wtrk_old); // Pointer to global shared variables
			switch (cmd[0]) {  // different options as we tour all the windows
			case 'k': {  // actually kill it will killtask
				wtrk_old.kill_window = 1;
				break;
			}
			case 'l': { // short description
				wtrk_old.list_window = 1;
				break;
			}
			case 's': { // show, means listtask
				wtrk_old.show_window = 1;
				break;
			}
			case 'r': { // review means show what would be billed
				wtrk_old.list_window = 1;
				wtrk_old.kill_window = 1;
				break;
			}
			}
			BOOL result = EnumWindows(EnumWindowsProc, 0);
			cout << endl << "Done with " << result << "and  " << std::dec
					<< wtrk_old.win_count << " visible windows and "
					<< wtrk_old.win_github_windows << " my_windows and "
					<< wtrk_old.win_killed_windows << " killed windows and "
					<< wtrk_old.win_hidden_count << " hidden windows, with "
					<< wtrk_old.win_no_title
					<< " visible windows having no title." << endl;
			system("pause");
			break;
		}
		default: {
			cout << "I don't know what you mean by " << endl;
			break;
		}
		}
	}
	cout << "That's all folks!" << endl;
	return 1;
}
