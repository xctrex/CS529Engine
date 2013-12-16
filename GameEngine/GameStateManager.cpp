#include "GameStateManager.h"
#include "Core.h"
#include "GameLogicSystem.h"
#include "GraphicsSystem.h"

namespace Framework
{
    GameStateManager::GameStateManager() :
        m_CurrentState(100),
        m_NumAsteroids(100)
    {
        m_Type = COMPONENT_TYPE_GAME_STATE_MANAGER;

        g_ComponentHandleTable[this->GetHandleIndex()] = this;
    };

    void GameStateManager::Initialize(tinyxml2::XMLElement* txmlElement)
    {
        CommonComponentInitialization(txmlElement);

        //================================================================
        // GameState specific initialization
        //================================================================
        if (txmlElement->Attribute("NumAsteroids"))
        {
            m_NumAsteroids = txmlElement->IntAttribute("NumAsteroids");
        }

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
    };
}
