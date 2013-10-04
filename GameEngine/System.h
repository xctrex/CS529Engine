/* Start Header -------------------------------------------------------
Copyright (C) 2013 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: System.h
Purpose: Defines the system interface
Language: C++
Platform: Windows
Project: CS529_twalton_FinalProject
Author: Tommy Walton, t.walton, 130000812
Creation date: 10/3/2013
- End Header -----------------------------------------------------*/
#pragma once

#include "Message.h"

namespace Framework
{
    class ISystem
    {
    public:
        virtual void SendMessage(Message* message) {};
        virtual void Update(float timeslice) = 0;
        virtual std::string GetName() = 0;
        virtual void Initialize(){};
        virtual ~ISystem(){};
    };
}
