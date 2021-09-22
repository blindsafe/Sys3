/*
 * Kill_Window.cpp
 *
 *  Created on: Sep 12, 2021
 *      Author: Charles Yarbrough
 */
#include "Kill_Window.hpp"

BOOL string_contains(char *target_string, const char *test_string) {
	char *match;
	match = strstr(target_string, test_string);
	if (match) {
		if (0) {
			cout << " test match = " << " for test_string " << test_string
					<< "and " << target_string << "and matches!" << endl;
		}
		return 1;
	} else {
		return 0;
	}
}

Window_Type special_window(Window_Tracking *wt) {
	// by painful discovery, some windows are not meant to be canceled
	// but others definitely want to be!
	//                   0,     1,      2,             3
	// Window_Type => W_NO, W_YES, W_KILL, W_YES_DEFAULT
	Window_Type is_special = W_NO; // 1=yes, dont kill, 2=do kill, 0 = dont know

	if (wt->is_blindsafe_window)
		is_special = W_YES;          // 1
	else if (string_contains(wt->titlebuff, "Default"))
		is_special = W_YES_DEFAULT; // 3 -> special recognition for this common case
	else if (string_contains(wt->namebuff, "Default"))
		is_special = W_YES_DEFAULT; // 3 -> special recognition for this common case
	// all these should be left alone
	else if (string_contains(wt->titlebuff, "Window"))
		is_special = W_YES;          // 1
	else if (string_contains(wt->titlebuff, "Battery Meter"))
		is_special = W_YES;          // 1
	else if (string_contains(wt->titlebuff, "{"))
		is_special = W_YES;          // 1
	else if (string_contains(wt->titlebuff, ".NET"))
		is_special = W_YES;          // 1
	else if (string_contains(wt->titlebuff, "#"))
		is_special = W_YES;          // 1

	// order is important here: get rid of lines with both
	else if (string_contains(wt->titlebuff, "Edge"))
		is_special = W_KILL;		 // 2
	else if (string_contains(wt->titlebuff, "Microsoft"))
		is_special = W_YES;          // 1

	else if (string_contains(wt->titlebuff, "Cicero"))
		is_special = W_YES;          // 1
	else if (string_contains(wt->titlebuff, "McPlatform"))
		is_special = W_YES;          // 1
	else if (string_contains(wt->titlebuff, "GDI"))
		is_special = W_YES;          // 1
	else if (string_contains(wt->titlebuff, "Realtek"))
		is_special = W_YES;          // 1
	else if (string_contains(wt->titlebuff, "SecurityHealth"))
		is_special = W_YES;          // 1
	else if (string_contains(wt->titlebuff, "AccEvent"))
		is_special = W_YES;          // 1
	else if (string_contains(wt->titlebuff, "Settings"))
		is_special = W_YES;          // 1
	else if (string_contains(wt->titlebuff, "The Event"))
		is_special = W_YES;          // 1
	else if (string_contains(wt->titlebuff, "fsATP"))
		is_special = W_YES;          // 1
	else if (string_contains(wt->titlebuff, "TaskHost"))
		is_special = W_YES;          // 1
	else if (string_contains(wt->titlebuff, "DWM"))
		is_special = W_YES;          // 1
	else if (string_contains(wt->titlebuff, "Network Flyout"))
		is_special = W_YES;
	else if (string_contains(wt->titlebuff, "Mail"))
		is_special = W_YES;
	else if (string_contains(wt->titlebuff, "Add an account"))
		is_special = W_YES;
	else if (string_contains(wt->titlebuff, "Form"))
		is_special = W_YES;
	else if (string_contains(wt->titlebuff, "CCleaner"))
		is_special = W_YES;
	else if (string_contains(wt->titlebuff, "DDE"))
		is_special = W_YES;
	else if (string_contains(wt->titlebuff, "OneDrive"))
		is_special = W_YES;
	else if (string_contains(wt->titlebuff, "PartR"))
		is_special = W_YES;
	else if (string_contains(wt->titlebuff, "Program Manager"))
		is_special = W_YES;
	else if (string_contains(wt->titlebuff, "MS_Web"))
		is_special = W_YES;
	else if (string_contains(wt->titlebuff, "Eclipse"))
		is_special = W_YES;
	else if (string_contains(wt->titlebuff, "Freedom"))
		is_special = W_YES;
	else if (string_contains(wt->titlebuff, "JFW"))
		is_special = W_YES;
	else if (string_contains(wt->titlebuff, "ITEM"))
		is_special = W_YES;
	else if (string_contains(wt->titlebuff, "FSIScanner"))
		is_special = W_YES;
	else if (string_contains(wt->titlebuff, "FSCAM"))
		is_special = W_YES;
	else if (string_contains(wt->titlebuff, "JAWS"))
		is_special = W_YES;
	else if (string_contains(wt->titlebuff, "File Explorer"))
		is_special = W_YES;

	if (!is_special) {	// these definitely want to be killed
		if (string_contains(wt->titlebuff, "Notepad"))
			is_special = W_KILL;
		else if (string_contains(wt->titlebuff, "Word"))
			is_special = W_KILL;
		else if ((wt->titlebuff[0] == 'W') && (wt->titlebuff[1] == '\0'))
			is_special = W_KILL;
		else if (string_contains(wt->titlebuff, "Explorer"))
			is_special = W_KILL;
		else if (string_contains(wt->titlebuff, "Outlook"))
			is_special = W_KILL;
		else if (string_contains(wt->titlebuff, "Bing"))
			is_special = W_KILL;
	}
	if (wt->debug_commentary && is_special) {
		cout << "special = " << is_special << " for " << wt->filename << " =  "
				<< wt->titlebuff << endl;
		//  system( "pause");
	}
	return is_special;
}

BOOL kill_window(Window_Tracking *wt) {
	// if fails a last minute exception test,  kill the current window
	int killed = 0;
	char syscmd[BUF_SIZE] = "taskkill /PID ";
	strcat(syscmd, wt->filename);
	strcat(syscmd, "  /F");

	if (wt->debug_commentary) {
		cout << "(?) Kill window, special= " << wt->is_special << "for "
				<< wt->namebuff << "with" << syscmd << endl;
	}

	if ((wt->is_special == 1) || (wt->is_special == 3)) {
		wt->win_saved_windows++;
	} else {  // (special == 2) | | (special == 0)
		{
			cout << "(**)Kill window " << wt->titlebuff << "==" << wt->namebuff
					<< "with" << syscmd << endl;
			// system("pause");
			system(syscmd);
			// cout << "and back"; system("pause");
			wt->win_killed_windows++;
			killed = 1;
		}
	}
	return killed;
}
