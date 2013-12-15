#include "GameLogicSystem.h"

namespace Framework
{
    // Global pointer to the game logic
    GameLogicSystem* g_LOGIC = NULL;

    GameLogicSystem::GameLogicSystem() :
        m_pInputHandler(NULL),
        m_pGameStateManager(NULL)
    {
        //Set up the global pointer
        ThrowErrorIf(g_LOGIC != NULL, "Logic already initialized");
        g_LOGIC = this;
    }

    void GameLogicSystem::Update(float dt)
    {
        // Update the input handler
        m_pInputHandler->Update(dt);
        // Update the game state manager
        m_pGameStateManager->Update(dt);
    }

    void GameLogicSystem::OnEvent(Event* e)
    {
        if (m_pGameStateManager)
        {
            m_pGameStateManager->OnEvent(e);
        }
    }
}