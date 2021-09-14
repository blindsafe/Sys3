/*
 * Kill_Window.cpp
 *
 *  Created on: Sep 12, 2021
 *      Author: Charles Yarbrough
 */
#include "Enum_Windows.hpp"
#include "Kill_Window.hpp"
#include <cstring>

BOOL strbegin( char *target_string, const char *test_string) {
	char *match;
	match = strstr( target_string, test_string);
	if ( match  ) {
		if ( 0 ) {
		cout << " test match = "
			<< " for test_string " << test_string
			<< "and " << target_string
			<< "and matches!" << endl; }
		return 1;
	}
	else {
		return 0;
	}
}

BOOL special_window(Window_Tracking *wt) {
	// by painful discovery, some windows arent to be canceled
	int is_special = 0;

	if ( strbegin(wt->titlebuff, "Window" ) ) is_special = 1;
	else if ( strbegin(wt->titlebuff, "Battery Meter" ) ) is_special = 1;
	else if ( strbegin(wt->titlebuff, "{" ) ) is_special = 1;
	else if ( strbegin(wt->titlebuff, ".NET" ) ) is_special = 1;
	else if ( strbegin(wt->titlebuff, "#" ) ) is_special = 1;
	else if ( strbegin(wt->titlebuff, "Microsoft" ) ) is_special = 1;
	else if ( strbegin(wt->titlebuff, "Cicero" ) ) is_special = 1;
	else if ( strbegin(wt->titlebuff, "McPlatform" ) ) is_special = 1;
	else if ( strbegin(wt->titlebuff, "GDI" ) ) is_special = 1;
	else if ( strbegin(wt->titlebuff, "Realtek" ) ) is_special = 1;
	else if ( strbegin(wt->titlebuff, "SecurityHealth" ) ) is_special = 1;
	else if ( strbegin(wt->titlebuff, "AccEvent" ) ) is_special = 1;
	else if ( strbegin(wt->titlebuff, "Settings" ) ) is_special = 1;
	else if ( strbegin(wt->titlebuff, "The Event" ) ) is_special = 1;
	else if ( strbegin(wt->titlebuff, "fsATP" ) ) is_special = 1;
	else if ( strbegin(wt->titlebuff, "TaskHost" ) ) is_special = 1;
	else if ( strbegin(wt->titlebuff, "DWM" ) ) is_special = 1;
	else if ( strbegin(wt->titlebuff, "Network Flyout" ) ) is_special = 1;
	else if ( strbegin(wt->titlebuff, "Mail" ) ) is_special = 1;
	else if ( strbegin(wt->titlebuff, "Add an account" ) ) is_special = 1;
	else if ( strbegin(wt->titlebuff, "Form" ) ) is_special = 1;
	else if ( strbegin(wt->titlebuff, "CCleaner" ) ) is_special = 1;
	else if ( strbegin(wt->titlebuff, "DDE" ) ) is_special = 1;
	else if ( strbegin(wt->titlebuff, "OneDrive" ) ) is_special = 1;
	else if ( strbegin(wt->titlebuff, "PartR" ) ) is_special = 1;
	else if ( strbegin(wt->titlebuff, "Default" ) ) is_special = 1;
	else if ( strbegin(wt->titlebuff, "Program Manager" ) ) is_special = 1;
	else if ( strbegin(wt->titlebuff, "MS_Web" ) ) is_special = 1;
	else if ( strbegin(wt->titlebuff, "Eclipse" ) ) is_special = 1;

	if (wt->debug_commentary ) { //  &&  is_special ) {
		cout  << "special = " << is_special  << " for " << wt->filename
		   << " =  " << wt->titlebuff << endl;
		// system( "pause");
	}
return is_special;
}

BOOL kill_window(Window_Tracking *wt) {
	// if fails a last minute exception test,  kill the current window
	int  is_special = 0;
	int killed = 0;
	char syscmd[BUF_SIZE] =  " taskkill /PID ";
	strcat( syscmd, wt->filename);
	strcat(syscmd, "  /F");
	if ( wt->debug_commentary ) {
				cout << "(?)Kill window " << wt->titlebuff << "==" << wt->namebuff << "with"
				<< syscmd << endl;
			}
	is_special = special_window( wt );
	if ( is_special ) {
		wt->win_saved_windows++;
	}
	else {
			if ( wt->kill_window == 2) {
				cout <<"system( " << syscmd << " )" << endl;
			}
			else {
				cout << "(**)Kill window " << wt->titlebuff << "==" << wt->namebuff << "with"
				<< syscmd << endl;
				system("pause");
				system( syscmd );
				cout << "and back";
				system("pause");
			wt->win_killed_windows++;
			killed = 1;
			} }
	return killed;
}
