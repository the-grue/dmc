// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993 - 1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   init.c
//
//  PURPOSE:   Performs application and instance specific initialization.
//
//  FUNCTIONS:
//    InitApplication() - Initializes window data and registers window.
//
//  COMMENTS:
//

#include <windows.h>            // required for all Windows applications
#include "globals.h"            // prototypes specific to this application
#include "palctrl.h"            // prototype for RegisterPalCtrlClass
#include "resource.h"

// Global variables for application info
HINSTANCE hInst;                // current instance
char      szAppName[9];         // the name of this application
HMENU     hMenu;                // handle of application menu
HICON     hIcon;                // handle of application icon
HWND      hWndClient;           // handle of client window

// palette-related global variables
BOOL      bPalDevice = FALSE;   // indicates palette support of display device
HPALETTE  hPalette = NULL;      // handle of current palette
                                  
// drawing-related global variables                                  
LOGPEN   logPen;                // structure for pen attributes
LOGBRUSH logBrush;              // structure for brush attributes
                                  
// Global variables for current DIB section
char     szCurrentFile[255]; 
HBITMAP  hBitmap  = NULL;
HANDLE   hDIBInfo = NULL;
LPVOID   lpvBits  = NULL;
BOOL     fChanges = FALSE;

//
//  FUNCTION: InitApplication(HINSTANCE, int)
//
//  PURPOSE: Initializes window data and registers window class.
//
//  PARAMETERS:
//    hInstance - The handle to the instance of this application that
//                is currently being executed.
//    nCmdShow  - Specifies how the main window is to be displayed.
//
//  RETURN VALUE:
//    TRUE  - Success
//    FALSE - Initialization failed
//
//  COMMENTS:
//
//    This function is called at application initialization time.  It
//    performs initialization tasks for the current application instance.
//    Unlike Win16, in Win32, each instance of an application must register
//    window classes.
//
//    In this function, we initialize a window class by filling out a data
//    structure of type WNDCLASS and calling the Windows RegisterClass()
//    function.  Then we create the main window and show it.
//
//

BOOL InitApplication(HINSTANCE hInstance, int nCmdShow)
{
    WNDCLASSEX wc;
    HWND       hwnd;        // Main window handle.
    char       szTitle[40]; // The title bar text
    
    // Load the application name and description strings.

    LoadString(hInstance, IDS_APPNAME, szAppName, sizeof(szAppName));
    LoadString(hInstance, IDS_DESCRIPTION, szTitle, sizeof(szTitle));

    // Save the instance handle in static variable, which will be used in
    // many subsequence calls from this application to Windows.

    hInst = hInstance; // Store instance handle in our global variable 
    
    hIcon = LoadIcon(hInstance, szAppName); // Store application icon handle

    // Fill in window class structure with parameters that describe the
    // main window.

    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = CS_HREDRAW | CS_VREDRAW; // Class style(s).
    wc.lpfnWndProc   = (WNDPROC)WndProc;        // Window Procedure
    wc.cbClsExtra    = 0;                       // No per-class extra data.
    wc.cbWndExtra    = 0;                       // No per-window extra data.
    wc.hInstance     = hInstance;               // Owner of this class
    wc.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPICON)); // Icon name from .RC
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW); // Cursor
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // Default color
    wc.lpszMenuName  = szAppName;               // Menu name from .RC
    wc.lpszClassName = szAppName;               // Name to register as
    wc.hIconSm       = LoadImage(hInstance,         // Load small icon image
                                 MAKEINTRESOURCE(IDI_APPICON),
                                 IMAGE_ICON,
                                 16, 16,
                                 0);

    // Register the window class and return FALSE if unsuccesful.

    if (!RegisterClassEx(&wc))
    {
        //Assume we are running on NT where RegisterClassEx() is
        //not implemented, so let's try calling RegisterClass().

        if (!RegisterClass((LPWNDCLASS)&wc.style))
        	return FALSE;
    }

    // Register window class for the client window.
    
    wc.lpfnWndProc   = ClientWndProc;
    wc.hIcon         = NULL;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // Default color
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = "ClientWndClass";

    if (!RegisterClass((LPWNDCLASS)&wc.style))
    {
        return FALSE;
    }
          
    // Register palette control class
    if (!RegisterPalCtrlClass(hInstance))
    {
        return FALSE;
    }
              
    // Create a main window for this application instance.
    hwnd = CreateWindow(szAppName,           // See RegisterClass() call
                        szTitle,             // Text for window title bar
                        WS_OVERLAPPEDWINDOW, // Window style
                        CW_USEDEFAULT, 0,    // Use default positioning
                        CW_USEDEFAULT, 0,    // Use default size
                        NULL,                // Overlapped has no parent
                        NULL,                // Use the window class menu
                        hInstance,           // This instance owns this window
                        NULL                 // Don't need data in WM_CREATE
    );
    
    if (!hwnd)
        // window could not be created, return "failure"
        return FALSE;
                                                   
    // Make the window visible; update its client area; and return "success"
    ShowWindow(hwnd, nCmdShow);  // Show the window
    UpdateWindow(hwnd);          // Sends WM_PAINT message
    
    return TRUE;           
}
