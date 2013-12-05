#pragma once

#include "Engine.h"

namespace Framework
{
    class InputHandler : public Component
    {
    public:
        InputHandler();
        ~InputHandler() {};
        virtual void Initialize(tinyxml2::XMLElement* txmlElement);
        virtual void Serialize(tinyxml2::XMLDocument* txmlDoc){};
        virtual void OnEvent(Event* e){};

        void Update(float dt);

        float m_Speed;
        Transform* m_pTransform;
    };
}