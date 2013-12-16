/* Start Header -------------------------------------------------------
Copyright (C) 2013 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: GameStateManager.h
Purpose: Header file for game state manager component
Language: C++
Platform: Windows
Project: CS529_twalton_FinalProject
Author: Tommy Walton, t.walton, 130000812
Creation date: 12/15/2013
- End Header -----------------------------------------------------*/
#pragma once

#include "Component.h"

enum GAME_STATE
{
    GAME_STATE_
};

namespace Framework
{
    class GameStateManager : public Component
    {
    public:
        GameStateManager();
        ~GameStateManager() {};
        virtual void Initialize(tinyxml2::XMLElement* txmlElement);
        virtual void Serialize(tinyxml2::XMLDocument* txmlDoc){};
        virtual void OnEvent(Event* e);
        virtual void Destroy(){};
    private:
        int m_CurrentState;
        int m_NumAsteroids;
        int m_NumClicks;
    };
}
