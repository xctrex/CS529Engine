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

    void GameLogicSystem::Update(float dt)
    {
        // Update the input handler
        m_InputHandler->Update(dt);
    }
}