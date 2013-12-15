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
    // Forward declaration of game object class
    class GameObject;

    class CoreEngine
    {
    public:
        CoreEngine();
        ~CoreEngine();

        // Add a new system to the core
        void AddSystem(ISystem* system);
        // Initialize all of the core's systems
        void InitializeSystems();
        // Destroy all of the core's systems
        void DestroySystems();
        // Update all the systems until the game is no longer active
        void GameLoop();
        // Broadcasts an event to all systems
        void BroadcastEvent(Event* e);
        // Adds an object to the cleanup list
        void AddToCleanupList(GameObject* pObj);
        // Deletes objects that should no longer exist
        void Cleanup();
    private:
        // Tracks all the systems the game uses
        std::vector<ISystem*> m_Systems;
        // List of objects that should be deleted at the end of the frame
        std::list<GameObject*> m_CleanupList;

        // The last time the game was updated
        unsigned int LastTime;
        // True if the game is running, false otherwise
        bool m_GameActive;
        // Timer class
        CoreTime time;
    };

    // Global pointer to the core engine
    extern CoreEngine* g_CORE;
}
