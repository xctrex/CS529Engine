/* Start Header -------------------------------------------------------
Copyright (C) 2013 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: GameLogicSystem.cpp
Purpose: Implementation file for game logic system
Language: C++
Platform: Windows
Project: CS529_twalton_FinalProject
Author: Tommy Walton, t.walton, 130000812
Creation date: 12/15/2013
- End Header -----------------------------------------------------*/
#include "GameLogicSystem.h"

namespace Framework
{
    // Global pointer to the game logic
    GameLogicSystem* g_LOGIC = NULL;

    GameLogicSystem::GameLogicSystem()
    {
        //Set up the global pointer
        ThrowErrorIf(g_LOGIC != NULL, "Logic already initialized");
        g_LOGIC = this;
    }
    void GameLogicSystem::Destroy()
    {
        if(m_hGameStateManager.ToComponent())
        {
            m_hGameStateManager.Destroy();
        }
    }

    void GameLogicSystem::Update(float dt)
    {
        // Update the input handler
        static_cast<InputHandler*>(m_hInputHandler.ToComponent())->Update(dt);
    }

    void GameLogicSystem::OnEvent(Event* e)
    {
        if (m_hGameStateManager.ToComponent())
        {
            m_hGameStateManager.ToComponent()->OnEvent(e);
        }
    }
}