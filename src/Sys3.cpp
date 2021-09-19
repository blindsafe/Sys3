//============================================================================
// Name        : Sys3.cpp
// Author      : Charles Yarbrough
// Version     : 0.1.0
// Copyright   : (c) 2021
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "Sys3.hpp"

#include <minwindef.h>
#include <process.h>
#include <winuser.h>

struct Window_Tracking wtrk;

int main(int argc, char *argv[]) {
	char cmd[BUF_SIZE];
	BOOL accepting_commands = true;

	while (accepting_commands) {
		cout << "your wish is my command" << endl << "-->";
		strncpy(cmd, "?", 2);   // default entry is HELP
		cin >> cmd;
		const char command_char = cmd[0];
		cout << "you said " << cmd << endl;
		system("pause");

		switch (command_char) {
		case 't': { // tasklist
			system("tasklist");
			break;
		}
		case 'q': { // quit
			accepting_commands = false;
			break;
		}
		case 'c': { //cmd
			system(cmd);
			break;
		}
		case 'e': { // emergency shutdown
			system("shutdown /r /f /t 0");
			break;
		}
		case 'k':
		case 'l':
		case 's':
		case 'r': {
			// kill, list, show, review
		const char subcmd_char = cmd[0];
			init_window_tracking(&wtrk);   // Pointer to global shared variables
			switch (subcmd_char) { // different options as we tour all the windows
			case 'k': {  // actually kill it with killtask
			    wtrk.list_window = true;
				wtrk.kill_window = 1;
				break;
			}
			case 'l': { // short description
				wtrk.list_window = true;
				break;
			}
			case 's': { // show, means listtask
				wtrk.show_window = true;
				break;
			}
			case 'r': { // review means show what would be killed
				wtrk.list_window = 1;
				wtrk.kill_window = 2;
				break;
			}
			default: {
				cout
						<< "while reviewing windows, I don't know what you mean by '"
						<< subcmd_char << "'" << endl;
				break;
			}
			} // end of reviewing windows sub-command loop

			cout << endl << "---- Starting enumeration loop! ----" << endl;
			BOOL result = EnumWindows(EnumWindowsProc, 0);
			cout << endl << "Done [result: " << result << "] and  " << std::dec
					<< wtrk.win_count << " of " << wtrk.win_total
					<< " with mixed " << wtrk.win_mixed << " hidden "
					<< wtrk.win_hidden_count << endl << "   and  blindsafe "
					<< wtrk.win_blindsafe_windows << "  killed "
					<< wtrk.win_killed_windows << "  saved "
					<< wtrk.win_saved_windows << endl;
			system("pause");
			break;
		}
		default: {
			cout << "I don't know what you mean by '" << command_char << "'"
					<< endl;
			break;
		}
		} // end of command switch
	} // end main command loop
	cout << "That's all folks!" << endl;
	return 1;
}

