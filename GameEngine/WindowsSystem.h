/* Start Header -------------------------------------------------------
Copyright (C) 2013 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: WindowsSystem.h
Purpose: This is a system that wraps everything related to the Windows OS
Language: C++
Platform: Windows
Project: CS529_twalton_FinalProject
Author: Tommy Walton, t.walton, 130000812
Creation date: 10/3/2013
- End Header -----------------------------------------------------*/
#pragma once

#include "Engine.h"
#include "WindowsIncludes.h"
#include "Containers.h"

namespace Framework
{
    // Window manager. Implements Windows message pump and
    // broadcasts user input to all systems
    class WindowsSystem : public ISystem
    {
    public:
        WindowsSystem(int ClientWidth, int ClientHeight);
        ~WindowsSystem();

        void ActivateWindow();
        virtual void Update(float dt);
        virtual std::string GetName() {return "Windows";}

        HWND m_hWnd;
        HINSTANCE hInstance;
        POINTS m_MousePosition;
    private:
        std::string m_WindowsClassName;
    };

    bool IsUpHeld();
    bool IsDownHeld();
    bool IsLeftHeld();
    bool IsRightHeld();
    bool IsSpaceHeld();
    bool IsControlHeld();

    extern WindowsSystem* g_WINDOWSSYSTEM;
}
