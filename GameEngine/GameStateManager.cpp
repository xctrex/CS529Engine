/* Start Header -------------------------------------------------------
Copyright (C) 2013 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: GameStateManager.cpp
Purpose: Implementation file for GameStateManager component
Language: C++
Platform: Windows
Project: CS529_twalton_FinalProject
Author: Tommy Walton, t.walton, 130000812
Creation date: 12/15/2013
- End Header -----------------------------------------------------*/
#include "GameStateManager.h"
#include "Core.h"
#include "GameLogicSystem.h"
#include "GraphicsSystem.h"

namespace Framework
{
    GameStateManager::GameStateManager() :
        m_PreviousState(GAME_STATE_PLATFORMER),
        m_CurrentState(GAME_STATE_PLATFORMER),
        m_NextState(GAME_STATE_PLATFORMER),
        m_NumAsteroids(100),
        m_NumClicks(10)
    {
        m_Type = COMPONENT_TYPE_GAME_STATE_MANAGER;

        g_ComponentHandleTable[this->GetHandleIndex()] = this;
    };

    void GameStateManager::Initialize(tinyxml2::XMLElement *txmlElement)
    {
        CommonComponentInitialization(txmlElement);

        //================================================================
        // GameState specific initialization
        //================================================================
        InitializeAttribute(txmlElement, m_NumAsteroids, "NumAsteroids");
        InitializeAttribute(txmlElement, m_NumClicks, "NumClicks");

        if(m_RecursionLevel == 0)
        {
            g_LOGIC->m_hGameStateManager.Initialize(this->GetHandleIndex(), this->GetUniqueID());
        }
    }

    void GameStateManager::OnEvent(Event* e)
    {
        if (e->m_EventType == EVENT_TYPE_OBJECT_DEATH)
        {
            ObjectDeathEvent* pODE = static_cast<ObjectDeathEvent*>(e);
            if (pODE->m_ObjectType == OBJECT_TYPE_SHIP)
            {
                // GameOver
                g_CORE->m_GameActive = false;
                g_GRAPHICS->m_ShowLose = true;
                m_NumClicks = 1;
            }
            else if (pODE->m_ObjectType == OBJECT_TYPE_ASTEROID)
            {
                --m_NumAsteroids;
                if (m_NumAsteroids <= 0)
                {
                    // Win level
                    g_CORE->m_GameActive = false;
                }
            }
        }
        else if(e->m_EventType == EVENT_TYPE_MOUSE_BUTTON)
        {
            MouseButtonEvent* pMBE = static_cast<MouseButtonEvent*>(e);
            if(!pMBE->m_IsPressed && pMBE->m_MouseButtonIndex == MouseButtonEvent::RightMouse)
            {
                --m_NumClicks;
                if(m_NumClicks <= 0)
                {
                    // Win level
                    g_CORE->m_GameActive = false;
                }
            }
        }
    };
}
