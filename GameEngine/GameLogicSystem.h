#pragma once

#include "System.h"
#include "InputHandler.h"
#include "GameStateManager.h"

namespace Framework
{
    class GameLogicSystem : public ISystem
    {
    public:
        GameLogicSystem();
        ~GameLogicSystem(){};
        virtual void Initialize() {};
        virtual void Update(float dt);
        virtual void OnEvent(Event* e);
        virtual std::string GetName() { return "Windows"; }

        // TODO: For now, only allow one input handler
        InputHandler* m_pInputHandler;
        GameStateManager* m_pGameStateManager;
    };

    //A global pointer to the game logic so that it can be accessed from anywhere.
    extern GameLogicSystem* g_LOGIC;
}