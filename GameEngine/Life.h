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
