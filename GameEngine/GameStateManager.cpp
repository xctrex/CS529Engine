#include "GameStateManager.h"
#include "GameLogicSystem.h"

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
        g_LOGIC->m_pGameStateManager = this;
    }

    void GameStateManager::Update(float dt)
    {

    }

    void GameStateManager::OnEvent(Event* e)
    {
        if (e->m_EventType == EVENT_TYPE_OBJECT_DEATH)
        {
            ObjectDeathEvent* pODE = static_cast<ObjectDeathEvent*>(e);
            if (pODE->m_ObjectType == OBJECT_TYPE_SHIP)
            {
                // GameOver
            }
            else if (pODE->m_ObjectType == OBJECT_TYPE_ASTEROID)
            {
                --m_NumAsteroids;
                if (m_NumAsteroids <= 0)
                {
                    // Win level
                }
            }
        }
    };
}
