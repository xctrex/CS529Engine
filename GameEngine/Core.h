/* Start Header -------------------------------------------------------
Copyright (C) 2013 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Core.h
Purpose: Header file for the CoreEngine
Language: C++
Platform: Windows
Project: CS529_twalton_FinalProject
Author: Tommy Walton, t.walton, 130000812
Creation date: 10/3/2013
- End Header -----------------------------------------------------*/
#pragma once

#include "Precomp.h"
#include "System.h"
#include "CoreTime.h"

namespace Framework
{
    class CoreEngine
    {
    public:
        CoreEngine();
        ~CoreEngine();

        // Add a new system to the core
        void AddSystem(ISystem* system);
        // Initialize all of the core's systems
        void InitializeSystems(void);
        // Update all the systems until the game is no longer active
        void GameLoop();

    private:
        // Tracks all the systems the game uses
        std::vector<ISystem*> Systems;
        // The last time the game was updated
        unsigned int LastTime;
        // True if the game is running, false otherwise
        bool GameActive;
        // Timer class
        CoreTime time;
    };
}
