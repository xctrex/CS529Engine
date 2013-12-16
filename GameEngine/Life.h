/* Start Header -------------------------------------------------------
Copyright (C) 2013 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Life.h
Purpose: Header file for Life component
Language: C++
Platform: Windows
Project: CS529_twalton_FinalProject
Author: Tommy Walton, t.walton, 130000812
Creation date: 12/15/2013
- End Header -----------------------------------------------------*/
#pragma once

#include "Component.h"
#include "Core.h"

namespace Framework
{
    class Life : public Component
    {
    public:
        Life();
        ~Life() {};
        virtual void Initialize(tinyxml2::XMLElement* txmlElement);
        virtual void Serialize(tinyxml2::XMLDocument* txmlDoc){};
        virtual void OnEvent(Event* e){};
        virtual void Destroy(){};

        void Heal(int health);
        void TakeDamage(int damage);
    private:
        int m_HP;
        int m_MaxHP;
    };
}
