/* Start Header -------------------------------------------------------
Copyright (C) 2013 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Message.h
Purpose: Defines the system interface
Language: C++
Platform: Windows
Project: CS529_twalton_FinalProject
Author: Tommy Walton, t.walton, 130000812
Creation date: 10/3/2013
- End Header -----------------------------------------------------*/
#pragma once

namespace Framework
{
    enum EVENT_TYPE
    {
        EVENT_TYPE_NONE,
        EVENT_TYPE_QUIT,
        EVENT_TYPE_CHARACTER_KEY,
        EVENT_TYPE_MOUSE_BUTTON,
        EVENT_TYPE_MOUSE_MOVE
    };

    // Base event class
    class Event
    {
    public:
        Event(EVENT_TYPE type) :
            m_Type(type)
        {};

        EVENT_TYPE m_Type;
        virtual ~Event(){};
    };
}
