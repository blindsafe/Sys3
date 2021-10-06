//============================================================================
// Name        : Sys3.cpp
// Author      : Charles Yarbroughe
// Description : Alpha version of BlindSafe
//============================================================================

#include "Sys3.hpp"

using namespace std;

struct Window_Tracking wtrk;  // this is it!! our global storage

void say_blindsafe_help() {
	// command list for the basic loop of main()
	cout << endl;
	cout << "blindSafe help" << endl << endl;
	cout << " 'h' for help" << endl;
	cout << " 's'  for shutdown, closing all windows" << endl;
	cout << " 'r'  for reboot , closing all windows" << endl;
	cout << " 'c'  for cleanup, closing all windows" << endl;
	cout << "' l'  for launch or join an app" << endl;
	cout << " 'e'  exit blindSafe" << endl;
	cout << " 'd'  for a developer, doing who knows what!!" << endl;
	cout << endl;
}

void do_developer_commands() {
	//separate  workbench/playground for developers to test things
	cout << "Developer Mode, use q to return to main loop" << endl;

	char cmd[BUF_SIZE] = { 'x', '\0' };
	bool accepting_commands = true;
	while (accepting_commands) {
		cout << endl << "developer_command --> ";

		memset(cmd, 0, sizeof(cmd));
		std::cin.getline(cmd, BUF_SIZE);
		const char command_char = cmd[0];
		cout << "developer said: '" << cmd << "', will use '" << command_char
				<< "'" << endl;
		if (command_char == '\0') {
			cout << "Nothing entered!" << endl;
		} else {
			switch (command_char) {
			case 'q': {  // back to blindsafe main loop
				accepting_commands = false;
				break;
			}
			case 'x': {  // help for developers
				cout << endl;
				cout << "---- blindSafe developer help ----" << endl << endl;
				cout << " 'q'  for returning to blindSafe main loop" << endl;
				cout << " 'x'  for help" << endl;
				cout << " 's'  for system command" << endl;
				cout << " 'd'  for setting debug mode to true" << endl;
				cout << " 'n'  for setting debug mode to false" << endl;
				cout << endl;
				break;
			}
			case 's': {  // system commands
				int r = system(&cmd[1]);
				cout << "-- RC: " << r << " --" << endl;
				system("pause");
				break;
			}
			case 'd': { // debug true
				wtrk.debug_commentary = true;
				break;
			}
			case 'n': { // debug off
				wtrk.debug_commentary = false;
				break;
			}
			default: {
				cout << "developer command '" << command_char
						<< "' makes no sense to me" << endl;
				accepting_commands = false;
				break;
			}
			}
		}
	} // end of command loop
}

BOOL am_i_in_already() {
	// blindsafe itself at launch time will kill any of itself it finds already there
	BOOL in_already_or_not = false;
	// we count of this being first thing in execution
	init_window_tracking(&wtrk);   // Pointer to global shared variables
	wtrk.search_for_window = true;
	strcpy(wtrk.searchname, "blindsafe");
	do_window_enum();
	if (wtrk.search_window) {
		cout << "previous " << wtrk.extra_search_windows + 1
				<< " blindsafe windows" << endl;
		in_already_or_not = true;
	}
	return (in_already_or_not);

}

#define APP_VERSION "1.0.2"

int main(int argc, char *argv[]) {
	cout << endl << "BlindSafe - Version " << APP_VERSION << endl;

	if (am_i_in_already()) {
		// currently, last in is the one surviving.
		// but maybe it should be first in, a kind of permanent resident
		// with its own permanent memory of what its done
		cout << "we only need one of me, so I killed my older brothers" << endl;
		// system("pause");
	}

	char cmd[BUF_SIZE] = { 'h', '\0' };
	bool accepting_commands = true;

	while (accepting_commands) {
		// blindSafe takes in one letter commands and does its best
		// to execute them until something causes an exit or shutdown
		cout << endl << "blindSafe: what is your wish? ('h' for help)" << endl
				<< "--> ";
		strncpy(cmd, "h", 2);   // default entry is HELP
		cin >> cmd;
		const char command_char = cmd[0];

		if (command_char == 'h') { // help doesnt need to pause
			say_blindsafe_help();
		} else {
			switch (command_char) {
			case 'c':    // cleanup
			case 's':    // shutdown
			case 'r': {  // reboot
				do_window_enum_plus(command_char);
				break;
			}
			case 'e': {  // exit
				accepting_commands = false;
				break;
			}
			case 'l': {  // launch or join
				do_launch_or_join_window();
				break;
			}
			case 'd': {  // for developers
				cout << "DEVEOPER? who are you trying to kid?" << endl;
				do_developer_commands();
				break;
			}
			default: {  // whatever
				cout << "don't tnow what you mean  by '" << cmd << "'" << endl;
				system("pause");
				say_blindsafe_help();
				break;
			}
			}
		} // end of command switch
	} // end main command loop

	cout << "That's all folks!" << endl;
	return (1);
}
