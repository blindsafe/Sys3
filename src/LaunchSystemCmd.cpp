/*
 * LaunchSystemCmd.cpp
 *
 *  Created on: May 30, 2023
 *      Author: Don
 */

#include <LaunchSystemCmd.hpp>

int main()
{
    // Define process information structures
    STARTUPINFO si = { sizeof(STARTUPINFO) };
    PROCESS_INFORMATION pi;

    // Create the process with elevated privileges and the SYSTEM user
    BOOL success = CreateProcessAsUserW(
        NULL,                               // Application name or command line
        L"C:\\Path\\to\\YourProgram.exe",    // Full path to the program you want to run
        NULL,                               // Process attributes
        NULL,                               // Thread attributes
        FALSE,                              // Inherit handles
        CREATE_NEW_CONSOLE,                 // Creation flags
        NULL,                               // Environment block
        NULL,                               // Current directory
        &si,                                // Startup information
        &pi                                 // Process information
    );

    if (success)
    {
        // Process created successfully
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
    }
    else
    {
        // Failed to create the process
        DWORD error = GetLastError();
        // Handle the error
    }

    return 0;
}



