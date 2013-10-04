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
    namespace MID // Message ID
    {
        enum MessageIDType
        {
            None,
            Quit
        };
    }

    class Message
    {
    public:
        Message(MID::MessageIDType id) : MessageID(id){};
        MID::MessageIDType MessageID;
        virtual ~Message(){};
    };
}
