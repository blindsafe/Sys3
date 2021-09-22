/*
 * Kill_Window.hpp
 *
 *  Created on: Sep 13, 2021
 *      Author: Don
 */

#ifndef KILL_WINDOW_HPP_
#define KILL_WINDOW_HPP_

#include <cstring>
#include <windows.h>
#include <iostream>
#include <IOmanip>
using namespace std;

#include "Enum_Windows.hpp"

BOOL string_contains(char *target_string, const char *test_string);

BOOL kill_window(Window_Tracking *wt);

Window_Type special_window(Window_Tracking *wt);

#endif /* KILL_WINDOW_HPP_ */
