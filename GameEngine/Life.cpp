/* Start Header -------------------------------------------------------
Copyright (C) 2013 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Life.cpp
Purpose: Implementation file for life component
Language: C++
Platform: Windows
Project: CS529_twalton_FinalProject
Author: Tommy Walton, t.walton, 130000812
Creation date: 12/15/2013
- End Header -----------------------------------------------------*/
#include "Life.h"

namespace Framework
{
    Life::Life() :
        m_HP(100)
    {
        m_Type = COMPONENT_TYPE_LIFE;

        g_ComponentHandleTable[this->GetHandleIndex()] = this;
    };

    void Life::Initialize(tinyxml2::XMLElement* txmlElement)
    {
        CommonComponentInitialization(txmlElement);

        //================================================================
        // Life specific initialization
        //================================================================
        if (txmlElement->Attribute("HP"))
        {
            m_MaxHP = txmlElement->IntAttribute("HP");
            m_HP = m_MaxHP;
        }

        if (m_RecursionLevel == 0)
        {
        }
        else
        {
            --m_RecursionLevel;
        }
    }

    void Life::Heal(int health)
    {
        m_HP += health;
        if (m_HP > m_MaxHP) m_HP = m_MaxHP;
    }

    void Life::TakeDamage(int damage)
    {
        m_HP -= damage;
        if (m_HP < 0)
        {
            g_CORE->AddToCleanupList(m_Parent);
        }
    }
}
