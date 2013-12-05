#pragma once

#include "Engine.h"
#include "WindowsSystem.h"

namespace Framework
{
    class InputHandler : public Component
    {
    public:
        InputHandler() : m_Speed(10.0f) {};
        ~InputHandler() {};
        virtual void Initialize(tinyxml2::XMLElement* txmlElement){};
        virtual void Serialize(tinyxml2::XMLDocument* txmlDoc){};
        virtual void OnEvent(Event* e){};

        void Update(float dt);

        float m_Speed;
        XMFLOAT2 m_Position;
    };
}