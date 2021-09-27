/*
 * Kill_Window.hpp
 *
 *  Created on: Sep 13, 2021
 *      Author: Don
 */

#ifndef KILL_WINDOW_HPP_
#define KILL_WINDOW_HPP_

#include <windows.h>
#include <iostream>
#include <IOmanip>
using namespace std;

#include "Enum_Windows.hpp"

bool string_contains(const char *target_string, const char *test_string);

bool kill_window(Window_Tracking *wt);

bool is_kill_target_window(const Window_Tracking *wt);

#endif /* KILL_WINDOW_HPP_ */
