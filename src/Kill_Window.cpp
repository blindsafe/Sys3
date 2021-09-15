/*
 * Kill_Window.cpp
 *
 *  Created on: Sep 12, 2021
 *      Author: Charles Yarbrough
 */
#include "Enum_Windows.hpp"
#include "Kill_Window.hpp"
#include <cstring>

BOOL string_contains( char *target_string, const char *test_string) {
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
	// but others definitely want to be!
	int is_special = 0;  // 1=yes, dont kill, 2=do kill, 0 = dont know

	// all these should be left alone
	if ( string_contains(wt->titlebuff, "Window" ) ) is_special = 1;
	else if ( string_contains(wt->titlebuff, "Battery Meter" ) ) is_special = 1;
	else if ( string_contains(wt->titlebuff, "{" ) ) is_special = 1;
	else if ( string_contains(wt->titlebuff, ".NET" ) ) is_special = 1;
	else if ( string_contains(wt->titlebuff, "#" ) ) is_special = 1;
	else if ( string_contains(wt->titlebuff, "Microsoft" ) ) is_special = 1;
	else if ( string_contains(wt->titlebuff, "Cicero" ) ) is_special = 1;
	else if ( string_contains(wt->titlebuff, "McPlatform" ) ) is_special = 1;
	else if ( string_contains(wt->titlebuff, "GDI" ) ) is_special = 1;
	else if ( string_contains(wt->titlebuff, "Realtek" ) ) is_special = 1;
	else if ( string_contains(wt->titlebuff, "SecurityHealth" ) ) is_special = 1;
	else if ( string_contains(wt->titlebuff, "AccEvent" ) ) is_special = 1;
	else if ( string_contains(wt->titlebuff, "Settings" ) ) is_special = 1;
	else if ( string_contains(wt->titlebuff, "The Event" ) ) is_special = 1;
	else if ( string_contains(wt->titlebuff, "fsATP" ) ) is_special = 1;
	else if ( string_contains(wt->titlebuff, "TaskHost" ) ) is_special = 1;
	else if ( string_contains(wt->titlebuff, "DWM" ) ) is_special = 1;
	else if ( string_contains(wt->titlebuff, "Network Flyout" ) ) is_special = 1;
	else if ( string_contains(wt->titlebuff, "Mail" ) ) is_special = 1;
	else if ( string_contains(wt->titlebuff, "Add an account" ) ) is_special = 1;
	else if ( string_contains(wt->titlebuff, "Form" ) ) is_special = 1;
	else if ( string_contains(wt->titlebuff, "CCleaner" ) ) is_special = 1;
	else if ( string_contains(wt->titlebuff, "DDE" ) ) is_special = 1;
	else if ( string_contains(wt->titlebuff, "OneDrive" ) ) is_special = 1;
	else if ( string_contains(wt->titlebuff, "PartR" ) ) is_special = 1;
	else if ( string_contains(wt->titlebuff, "Default" ) ) is_special = 1;
	else if ( string_contains(wt->titlebuff, "Program Manager" ) ) is_special = 1;
	else if ( string_contains(wt->titlebuff, "MS_Web" ) ) is_special = 1;
	else if ( string_contains(wt->titlebuff, "Eclipse" ) ) is_special = 1;
	else if ( string_contains(wt->titlebuff,  "Freedom" ) ) is_special = 1;
	else if ( string_contains(wt->titlebuff,  "JFW" ) ) is_special = 1;
	else if ( string_contains(wt->titlebuff,  "ITEM" ) ) is_special = 1;
	else if ( string_contains(wt->titlebuff,  "FSIScanner" ) ) is_special = 1;
	else if ( string_contains(wt->titlebuff,  "FSCAM" ) ) is_special = 1;
	else if ( string_contains(wt->titlebuff,  "JAWS" ) ) is_special = 1;


	// these definitely want to be killed
	if ( string_contains(wt->titlebuff, "Notepad" ) ) is_special = 2;
	if ( string_contains(wt->titlebuff, "Explorer" ) ) is_special = 2;

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
	if ( is_special  == 1) {
		wt->win_saved_windows++;
	}
	else {  // (special == 2) | | (special == 0)
		if ( is_special == 2 ) {
			cout <<  "is_special == 2";
		}
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
		}
	}
	return killed;
}
