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
    // Global pointer to the core
    CoreEngine* g_CORE = NULL;

    CoreEngine::CoreEngine() :
        LastTime(0),
        m_GameActive(true)
    {
        // Set the global pointer
        g_CORE = this;
    };

    CoreEngine::~CoreEngine(){};

    // Add a new system to the core
    void CoreEngine::AddSystem(ISystem* system)
    {
        m_Systems.push_back(system);
    }
    
    // Initialize all of the core's systems
    void CoreEngine::InitializeSystems()
    {
        for (size_t i = 0; i < m_Systems.size(); ++i)
            m_Systems[i]->Initialize();
    }

    // Update all the systems until the game is no longer active
    void CoreEngine::GameLoop()
    {
        // Initialize the time for what is considered the "previous frame" before starting the game loop
        // Otherwise the first frame time will be calculated to be very large
        time.GetFrameTimeInMilliseconds();

        while (m_GameActive)
        {
            // Calculate the frame time dt (in seconds)
            float dt = time.GetFrameTimeInMilliseconds() / 1000.0f;

            // Update every system and pass in the frame time
            for (size_t i = 0; i < m_Systems.size(); ++i)
                m_Systems[i]->Update(dt);
        }
    }

    void CoreEngine::BroadcastEvent(Event* e)
    {
        // Handle the quit event
        if (e->m_Type == EVENT_TYPE_QUIT)
            m_GameActive = false;

        // Iterate through each system and send the event
        for (unsigned i = 0; i < m_Systems.size(); ++i)
            m_Systems[i]->OnEvent(e);
    }
}