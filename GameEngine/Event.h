/* Start Header -------------------------------------------------------
Copyright (C) 2013 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Message.h
Purpose: Defines the event system
Language: C++
Platform: Windows
Project: CS529_twalton_FinalProject
Author: Tommy Walton, t.walton, 130000812
Creation date: 10/3/2013
- End Header -----------------------------------------------------*/
#pragma once
#include "Vector2D.h"

namespace Framework
{
    enum EVENT_TYPE
    {
        EVENT_TYPE_NONE,
        EVENT_TYPE_QUIT,
        EVENT_TYPE_CHARACTER_KEY,
        EVENT_TYPE_MOUSE_BUTTON,
        EVENT_TYPE_MOUSE_MOVE,
        EVENT_TYPE_COLLISION,
        EVENT_TYPE_OBJECT_CLEANUP,
        EVENT_TYPE_DAMAGE,
        EVENT_TYPE_OBJECT_DEATH
    };

    enum OBJECT_TYPE
    {
        OBJECT_TYPE_SHIP,
        OBJECT_TYPE_ASTEROID
    };

    // Base event class
    class Event
    {
    public:
        Event(EVENT_TYPE type) :
            m_EventType(type)
        {};

        EVENT_TYPE m_EventType;
        virtual ~Event(){};
    };

    // Event telling the object to take damage
    class DamageEvent : public Event
    {
    public:
        DamageEvent(int damage) : 
            Event(EVENT_TYPE_DAMAGE),
            m_Damage(damage)
        {};
        int m_Damage;
    };

    // Event telling the game logic that an object was destroyed
    class ObjectDeathEvent : public Event
    {
    public:
        ObjectDeathEvent(OBJECT_TYPE type) :
            Event(EVENT_TYPE_OBJECT_DEATH),
            m_ObjectType(type)
        {};
        OBJECT_TYPE m_ObjectType;
    };

    // Event telling the object to remove itself from the game
    class ObjectCleanupEvent : public Event
    {
    public:
        ObjectCleanupEvent() : Event(EVENT_TYPE_OBJECT_CLEANUP) {};
    };

    // Event telling the game to quit
    class QuitEvent : public Event
    {
    public:
        QuitEvent() : Event(EVENT_TYPE_QUIT) {};
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
}
