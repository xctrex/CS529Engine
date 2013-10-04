/* Start Header -------------------------------------------------------
Copyright (C) 2013 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: WindowsSystem.cpp
Purpose: This is a system that wraps everything related to the Windows OS
Language: C++
Platform: Windows
Project: CS529_twalton_FinalProject
Author: Tommy Walton, t.walton, 130000812
Creation date: 10/3/2013
- End Header -----------------------------------------------------*/
#include "WindowsSystem.h"

namespace Framework
{
    void WindowsSystem::Update(float dt)
    {

    }

    // Message handling procedure for the game
    LRESULT WINAPI MessageHandler(
        HWND hWnd,
        UINT msg,
        WPARAM wParam,
        LPARAM lParam
        )
    {
        // The engine didn't completely handle the message, so pass it on for Windows to handle.
		return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    WindowsSystem::WindowsSystem(int ClientWidth, int ClientHeight) : WindowsClassName("GameEngineWindowClass")
    {
        // The size passed to CreateWindow is the full size including the windows border and caption 
		// AdjustWindowRect will adjust the provided rect so that the client size of the window is the desired size
		RECT fullWinRect = {0, 0, ClientWidth, ClientHeight};
        AdjustWindowRect(
            &fullWinRect,
            WS_OVERLAPPEDWINDOW,
            FALSE
            );            

        // Register the window class for the game
        WNDCLASSEX wcx =
        {
            sizeof(WNDCLASSEX),                 // The size of this structure
            CS_HREDRAW | CS_VREDRAW,            // Redraw if size changes
            MessageHandler,                     // Name of message handling function
            0,                                  // No extra class memory
            0,                                  // No extra window memory
            GetModuleHandle(NULL),              // Handle to the instance that has the windows procedure--NULL means use this file.
            LoadIcon(NULL, IDI_APPLICATION),    // Predefined app icon
            LoadCursor(NULL, IDC_ARROW),        // Predefined arrow
            NULL,                               // Background brush - NULL
            NULL,                               // Name of menu resource - NULL
            WindowsClassName.c_str(),           // Name of window class
            NULL                                // Small class icon - NULL
        };
        
        RegisterClassEx(&wcx);

        // Store the handle to the instance
        hInstance = wcx.hInstance;

        // Create the game's window
        hWnd = CreateWindow(
            WindowsClassName.c_str(),
            "GameWindow",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            NULL,
            NULL,
            hInstance,
            (LPVOID) NULL
            );
    }

    WindowsSystem::~WindowsSystem()
    {
        // Unregister the window class
        UnregisterClass(WindowsClassName.c_str(), hInstance);
    }

    void WindowsSystem::ActivateWindow()
    {
        // Show the window
        ShowWindow(hWnd, SW_SHOWMAXIMIZED);
        // Send a WM_PAINT message to the window
        UpdateWindow(hWnd);
    }
}