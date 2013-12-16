#include "GameLogicSystem.h"

namespace Framework
{
    // Global pointer to the game logic
    GameLogicSystem* g_LOGIC = NULL;

    GameLogicSystem::GameLogicSystem() :
        m_pInputHandler(NULL)
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
        m_pInputHandler->Update(dt);
    }

    void GameLogicSystem::OnEvent(Event* e)
    {
        if (m_hGameStateManager.ToComponent())
        {
            m_hGameStateManager.ToComponent()->OnEvent(e);
        }
    }
}