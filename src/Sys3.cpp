//============================================================================
// Name        : Sys3.cpp
// Author      : Charles Yarbrough
// Version     : 0.1.0
// Copyright   : (c) 2021
// Description : Alpha version of BlindSafe
//============================================================================

#include "Sys3.hpp"

struct Window_Tracking wtrk;

void say_blindsafe_help() {
	cout << endl;
	cout << "blindsafe help" << endl << endl;

	cout << " 'h' for help" << endl;
	cout << " 's'  for shutdown, closing all windows" << endl;
	cout << " 'r'  for reboot , closing all windows" << endl;
	cout << " 'c'  for cleanup ,	 closing all windows" << endl;
	cout << " 'e'  exit blindsafe" << endl;
	cout << " 'd'  for developer, doing who knows what!!" << endl;

	cout << endl;
}

BOOL do_window_loop() {
	cout << endl << "---- Starting enumeration loop! ----" << endl;
	BOOL result = EnumWindows(EnumWindowsProc, 0);
	cout << endl << "Done [result: " << result << "] and  " << std::dec
			<< wtrk.win_count << " of " << wtrk.win_total << " with mixed "
			<< wtrk.win_mixed << " hidden " << wtrk.win_hidden_count << endl
			<< "   and  blindsafe " << wtrk.win_blindsafe_windows << "  killed "
			<< wtrk.win_killed_windows << "  saved " << wtrk.win_saved_windows
			<< endl;
	//  system("pause");
	return result;

}

void do_window_list(const char command_char) {
	init_window_tracking(&wtrk);   // Pointer to global shared variables
	switch (command_char) {
	case 's':
	case 'r':
	case 'c': {
		wtrk.list_window = true;
		wtrk.kill_window = 1;
		do_window_loop();
		switch (command_char) {
		case 's': { // shutdown
			system("shutdown /s /f /t 0");
			break;
		}
		case 'r': {
			system("shutdown /r /f /t 0");
			break;
		}
		case 'c': {
			break;
		}
		}
	}
	}
}

int main(int argc, char *argv[]) {
	char cmd[BUF_SIZE];
	BOOL accepting_commands = true;

	while (accepting_commands) {
		cout << "blindsafe 1.01 here: what is your wish? ('h' for help)" << endl
				<< "-->";
		strncpy(cmd, "h", 2);   // default entry is HELP
		cin >> cmd;
		const char command_char = cmd[0];

		if (command_char == 'h') { // help doesnt need to pause
			say_blindsafe_help();
		} else {
			cout << "you said you wanted  " << cmd << "." << endl;
			system("pause");
			switch (command_char) {
			case 'c':  // cleanup
			case 's':   // shutdown
			case 'r': { // reboot
						// kill, list, first
				do_window_list(command_char);
				break;
			}
			case 'e': { // exit
				accepting_commands = false;
				break;
			}
			case 'd': { // for developers
				cout << "DEVEOPER? who are you trying to kid?" << endl;
				break;
			}
			default: { // whatever
				cout << "i dont know what you mean  by " << cmd << endl;
				system("pause");
				say_blindsafe_help();
				break;
			}
			}
		} // end of command switch
	} // end main command loop
	cout << "That's all folks!" << endl;
	return 1;
}
