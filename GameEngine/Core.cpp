/* Start Header -------------------------------------------------------
Copyright (C) 2013 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Core.cpp
Purpose: Implementation file for the CoreEngine
Language: C++
Platform: Windows
Project: CS529_twalton_FinalProject
Author: Tommy Walton, t.walton, 130000812
Creation date: 10/3/2013
- End Header -----------------------------------------------------*/
#include "Core.h"

namespace Framework
{
    CoreEngine::CoreEngine() : LastTime(0), GameActive(true){};
    CoreEngine::~CoreEngine(){};

    // Add a new system to the core
    void CoreEngine::AddSystem(ISystem* system)
    {
        Systems.push_back(system);
    }
    
    // Initialize all of the core's systems
    void CoreEngine::InitializeSystems()
    {
        for (size_t i = 0; i < Systems.size(); ++i)
            Systems[i]->Initialize();
    }

    // Update all the systems until the game is no longer active
    void CoreEngine::GameLoop()
    {
        // Initialize the time for what is considered the "previous frame" before starting the game loop
        // Otherwise the first frame time will be calculated to be very large
        time.GetFrameTimeInMilliseconds();

        while (GameActive)
        {
            // Calculate the frame time dt (in seconds)
            float dt = time.GetFrameTimeInMilliseconds() / 1000.0f;

            // Update every system and pass in the frame time
            for (size_t i = 0; i < Systems.size(); ++i)
                Systems[i]->Update(dt);
        }
    }
}