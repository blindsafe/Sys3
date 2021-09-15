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

BOOL kill_window(  Window_Tracking *wt);

int special_window( HWND hWnd);

#endif /* KILL_WINDOW_HPP_ */
