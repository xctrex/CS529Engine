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

        HWND hWnd;
        HINSTANCE hInstance;
        POINTS m_MousePosition;
    private:
        std::string m_WindowsClassName;
    };

    // Event signaling that a key is pressed
    class CharacterKeyEvent : public Event
    {
    public:
        CharacterKeyEvent() : Event(EVENT_TYPE_CHARACTER_KEY) {};
        int m_Character;
    };

    // Event signaling that a mouse button state has changed
    class MouseButtonEvent : public Event
    {
    public:
        enum MouseButtonIndexID
        {
            LeftMouse,
            RightMouse
        };

        MouseButtonEvent(MouseButtonIndexID button, bool state, Vector2D position) :
            Event(EVENT_TYPE_MOUSE_BUTTON),
            m_MouseButtonIndex(button),
            m_IsPressed(state),
            m_MousePosition(position)
        {};

        MouseButtonIndexID m_MouseButtonIndex;
        bool m_IsPressed;
        Vector2D m_MousePosition;
    };

    // Event signaling that the mouse has moved
    class MouseMovedEvent : public Event
    {
    public:
        MouseMovedEvent(Vector2D position) :
            Event(EVENT_TYPE_MOUSE_MOVE),
            m_MousePosition(position)
        {};

        Vector2D m_MousePosition;
    };


    bool IsUpHeld();
    bool IsDownHeld();
    bool IsLeftHeld();
    bool IsRightHeld();
    bool IsSpaceHeld();

    extern WindowsSystem* g_WINDOWSSYSTEM;
}
