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

namespace Framework
{
    // Window manager. Implements Windows message pump and
    // broadcasts user input to all systems
    class WindowsSystem : public ISystem
    {
        WindowsSystem();
        ~WindowsSystem();

        void ActivateWindow();
        virtual void Update(float dt);
        virtual std::string GetName() {return "Windows";}
    };
}