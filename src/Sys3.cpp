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
	cout << "blindSafe help menu, type first letter:" << endl;
	cout << " help" << endl;
	cout << " shutdown closing all windows" << endl;
	cout << " reboot closing all windows" << endl;
	cout << " cleanup closing all windows" << endl;
	cout << " launch or join an app" << endl;
	cout << " quit blindSafe" << endl;
	cout << " debug, allowing a develor to do who knows what!!" << endl;
}

blind_op blind_help_char_to_op( const char command_char) {
	// hardcoded input character to blind_op
	// bl_clean, bl_debug, bl_help, bl_launch, bl_quit, bl_restart,  bl_shutdown

	blind_op rtn;
	// cout << "blind_help_char_to_op('" << command_char <<"')";

	switch ( command_char) {
	case 'c': { rtn = bl_clean; break; }
	case 'd': { rtn = bl_debug; break; }
	case 'h': { rtn = bl_help; break; }
	case 'l': { rtn = bl_launch; break; }
	case 'q': { rtn = bl_quit; break; }
	case 'r': { rtn = bl_restart; break; }
	case 's': { rtn = bl_shutdown; break; }
	default: {
		cout << "unknown command" << endl;
		rtn = bl_help; break; }
	}
	// cout << " returns " << (int)rtn << endl;
	return (rtn);
}

void do_developer_commands() {
	//separate  workbench/playground for developers to test things
	// primitive coding to be modified, altered, discarded at the
	// whim of whoever's in charge
	cout << "rogue Developer Mode, use q to return to main loop" << endl;

	char cmd[BUF_SIZE];  // why ? = { 'x', '\0' };
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
			case 'x': {  // try launching amazon
				char amazon[] = "chrome.exe";
				cout << "we will try " << amazon << endl;
				int r = system(amazon);
			    cout << "-- RC: " << r << " --" << endl;
				break;
			}
			case 'q': {  // back to blindsafe main loop
				accepting_commands = false;
				break;
			}
			case 'h': {  // help for developers
				cout << endl;
				cout << "---- blindSafe developer help ----" << endl << endl;
				cout << " 'q'  for returning to blindSafe main loop" << endl;
				cout << " 'h'  for help" << endl;
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
} // end do_developer_commands()

void do_blind_safe_cmd( blind_op use_cmd) {
	// main executor of all blind_safe operations
	bool do_pause = false;
	// cout << "do_blind_safe_cmd(" << (int) use_cmd << ")\n";
	switch ( use_cmd ) {
	case bl_help: {
		say_blindsafe_help();
		do_pause = false;
		break; }
	case bl_shutdown:
	case bl_restart:
	case bl_clean: {
		do_window_enum_plus(use_cmd);
		break; }
	case bl_launch: {
		do_launch_or_join_window();
		break; };
	case bl_quit: {
		do_launch_or_join_window();
		break; }
	case bl_debug: {
		cout << "DEVEOPER? who are you trying to kid?" << endl;
		do_developer_commands();
		break; }
	}
	if (do_pause)system("pause");
}

bool am_i_in_already() {
	// blindsafe itself at launch time will kill any of itself it finds already there
	bool in_already_or_not = false;
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

#define APP_VERSION "1.0.6"

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
	//	cin >> cmd;
		memset(cmd, 0, sizeof(cmd));
		std::cin.getline(cmd, BUF_SIZE);
		const char command_char = cmd[0];
		blind_op use_cmd = blind_help_char_to_op( command_char);
		do_blind_safe_cmd( use_cmd);

	} // end main command loop

	cout << "That's all folks!" << endl;
	return (1);
} // end of main()
