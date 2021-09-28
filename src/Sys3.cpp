//============================================================================
// Name        : Sys3.cpp
// Author      : Charles Yarbroughe
// Description : Alpha version of BlindSafe
//============================================================================

#include "Sys3.hpp"

struct Window_Tracking wtrk;

void say_blindsafe_help() {
	// command list for the basic loop of main()
	cout << endl;
	cout << "blindsafe help" << endl << endl;
	cout << " 'h' for help" << endl;
	cout << " 's'  for shutdown, closing all windows" << endl;
	cout << " 'r'  for reboot , closing all windows" << endl;
	cout << " 'c'  for cleanup ,	 closing all windows" << endl;
	cout << "' l'  for launch or join an app" << endl;
	cout << " 'e'  exit blindsafe" << endl;
	cout << " 'd'  for developer, doing who knows what!!" << endl;
	cout << endl;
}

void do_developer_commands() {
	//separate  workbench/playground for developers to test things
	char cmd[BUF_SIZE];
	bool accepting_commands = true;
	while (accepting_commands) {
		cout << "developer_commands in" << endl;
		std::cin.getline(cmd, BUF_SIZE);
		cout << "you said " << cmd << endl;
		switch (cmd[0]) {
		case 'q': {  // back to blindsafe main loop
			accepting_commands = false;
			break;
		}
		case 's': { // system commands
			system(&cmd[1]);
			break;
		}
		case 'd': { // debug
			wtrk.debug_commentary = true;
			break;
		}
		default: {
			cout << "makes no sense to me" << endl;
			break;
		}
		}
	} // end of command loop
}

BOOL am_i_in_already() {
	BOOL in_already_or_not = false;
	cout << "am_i_in_alread?" << endl;
	system("pause");
	init_window_tracking(&wtrk);   // Pointer to global shared variables
	wtrk.search_for_window = true;
	// wtrk.list_window = true; // for debugging

	strcpy(wtrk.searchname, "blindsafe");
	do_window_enum();
	if (wtrk.search_window) {
		cout << "previous " << wtrk.extra_search_windows + 1
				<< " blindsafe windows" << endl;
		in_already_or_not = true;
	}
	return in_already_or_not;

}
int main(int argc, char *argv[]) {
	char cmd[BUF_SIZE];
	bool accepting_commands = true;

	if (am_i_in_already()) {
#if 0
		cout << "how do i switch over to " << wtrk.search_window << endl;
		if (wtrk.extra_search_windows) {
			cout << "and what do i do about the " << wtrk.extra_search_windows
					<< " xtras?" << endl;
		}
#endif
		cout << "we only need one of me, so I killed my older brothers" << endl;
		system("pause");
	}

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
			case 'r':  // reboot

			{
				do_window_enum_plus(command_char);
				break;
			}
			case 'e': { // exit
				accepting_commands = false;
				break;
			}
			case 'l': {
				// launch or join
				do_launch_or_join_window();
				break;
			}
			case 'd': { // for developers
				cout << "DEVEOPER? who are you trying to kid?" << endl;
				do_developer_commands();
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
