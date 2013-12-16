/* Start Header -------------------------------------------------------
Copyright (C) 2013 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: GameLogicSystem.h
Purpose: Header file for game logic controllers
Language: C++
Platform: Windows
Project: CS529_twalton_FinalProject
Author: Tommy Walton, t.walton, 130000812
Creation date: 12/15/2013
- End Header -----------------------------------------------------*/
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
        virtual void Destroy();
        virtual void Initialize() {};
        virtual void Update(float dt);
        virtual void OnEvent(Event* e);
        virtual std::string GetName() { return "Windows"; }

        // TODO: For now, only allow one input handler
        InputHandler* m_pInputHandler;
        ComponentHandle m_hGameStateManager;
    };

    //A global pointer to the game logic so that it can be accessed from anywhere.
    extern GameLogicSystem* g_LOGIC;
}