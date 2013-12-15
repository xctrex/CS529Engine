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
    // A global pointer to the windows system
    WindowsSystem* g_WINDOWSSYSTEM = NULL;

    // Message handling procedure for the game
    LRESULT WINAPI MessageHandler(
        HWND hWnd,
        UINT msg,
        WPARAM wParam,
        LPARAM lParam
        )
    {
        switch (msg)
        {
        case WM_CHAR: //A character key was pressed
        {
            //Create a key event
            CharacterKeyEvent key;
            //Set the character pressed (the wParam is the ascii value)
            key.m_Character = wParam;
            //Broadcast the message to all systems
            g_CORE->BroadcastEvent(&key);
            break;
        }
        case WM_LBUTTONDOWN:
        {
            MouseButtonEvent m(MouseButtonEvent::LeftMouse, true, Vector2D(g_WINDOWSSYSTEM->m_MousePosition.x, g_WINDOWSSYSTEM->m_MousePosition.y));
            g_CORE->BroadcastEvent(&m);
            break;
        }
        case WM_RBUTTONDOWN:
        {
                               MouseButtonEvent m(MouseButtonEvent::RightMouse, true, Vector2D(g_WINDOWSSYSTEM->m_MousePosition.x, g_WINDOWSSYSTEM->m_MousePosition.y));
            g_CORE->BroadcastEvent(&m);
            break;
        }
        case WM_LBUTTONUP:
        {
                             MouseButtonEvent m(MouseButtonEvent::LeftMouse, false, Vector2D(g_WINDOWSSYSTEM->m_MousePosition.x, g_WINDOWSSYSTEM->m_MousePosition.y));
            g_CORE->BroadcastEvent(&m);
            break;
        }
        case WM_RBUTTONUP:
        {
                             MouseButtonEvent m(MouseButtonEvent::RightMouse, false, Vector2D(g_WINDOWSSYSTEM->m_MousePosition.x, g_WINDOWSSYSTEM->m_MousePosition.y));
            g_CORE->BroadcastEvent(&m);
            break;
        }
        case WM_MOUSEMOVE:
        {
            g_WINDOWSSYSTEM->m_MousePosition = MAKEPOINTS(lParam);
            MouseMovedEvent m(Vector2D(g_WINDOWSSYSTEM->m_MousePosition.x, g_WINDOWSSYSTEM->m_MousePosition.y));
            g_CORE->BroadcastEvent(&m);
            break;
        }
        case WM_KEYDOWN: //A key was pressed
            //TODO: Handle any key logic you might need for game controls
            //Use virtual key codes (VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN, VK_SPACE, etc.)
            //to detect specific keys (the wParam is the key pressed) and then
            //broadcast whatever message you need
            break;
        case WM_KEYUP: //A key was released
            //TODO: Handle any key logic you might need for game controls
            break;
            // this message is read when the window is closed
            case WM_DESTROY:
            {
                // close the application entirely
                PostQuitMessage(0);
                return 0;
            } break;
        }
        // The engine didn't completely handle the message, so pass it on for Windows to handle.
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    WindowsSystem::WindowsSystem(int ClientWidth, int ClientHeight) :
        m_WindowsClassName("GameEngineWindowClass")
    {
        // Set the global pointer for the windows system
        g_WINDOWSSYSTEM = this;

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
            m_WindowsClassName.c_str(),           // Name of window class
            NULL                                // Small class icon - NULL
        };
        
        RegisterClassEx(&wcx);

        // Store the handle to the instance
        hInstance = wcx.hInstance;

        // Create the game's window
        hWnd = CreateWindow(
            m_WindowsClassName.c_str(),
            "GameWindow",
            WS_OVERLAPPEDWINDOW,
            0,
            0,
            ClientWidth,
            ClientHeight,
            HWND_DESKTOP,
            NULL,
            hInstance,
            (LPVOID) NULL
            );
    }

    WindowsSystem::~WindowsSystem()
    {
        // Unregister the window class
        UnregisterClass(m_WindowsClassName.c_str(), hInstance);
    }

    void WindowsSystem::Update(float dt)
    {
		MSG msg;
		//Look for any pending windows messages, remove them, then handle them
		//The second parameter is the window handle--NULL just means get any message from the current thread
		//The third and forth parameters are the start and end message types to process
		//The last parameter determines whether or not the message is removed
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) //It is important to get all windows messages available not just one
		{
			TranslateMessage(&msg);	//Makes sure WM_CHAR and similar messages are generated
			DispatchMessage(&msg);	//Calls the message procedure (see below) with this message

			//If we get a quit message, broadcast it to all systems
			if (msg.message == WM_QUIT)
			{
				QuitEvent q;
				g_CORE->BroadcastEvent(&q);
			}
		}
    }
    
    void WindowsSystem::ActivateWindow()
    {
        // Show the window
        ShowWindow(hWnd, SW_SHOW);
        // Send a WM_PAINT message to the window
        UpdateWindow(hWnd);
    }    

    bool IsUpHeld(){ return GetKeyState(VK_UP) < 0; }
    bool IsDownHeld(){ return GetKeyState(VK_DOWN) < 0; }
    bool IsLeftHeld(){ return GetKeyState(VK_LEFT) < 0; }
    bool IsRightHeld(){ return GetKeyState(VK_RIGHT) < 0; }
    bool IsSpaceHeld(){ return GetKeyState(VK_SPACE) < 0; }
}