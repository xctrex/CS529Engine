#pragma once

#include "GameObject.h"

namespace Framework
{
    class Transform : public Component
    {
    public:
        Transform::Transform() :
            m_Position({ 0.0f, 0.0f }),
            m_Rotation(0.0f),
            m_Scale({ 1.0f, 1.0f })
        {
            m_Type = COMPONENT_TYPE_TRANSFORM;
        };
        virtual void Initialize(tinyxml2::XMLElement* txmlElement);
        virtual void Serialize(tinyxml2::XMLDocument* txmlDoc) {};
        virtual void OnEvent(Event* e){};

        DirectX::XMFLOAT2 m_Position;
        float m_Rotation;
        DirectX::XMFLOAT2 m_Scale;
    };
}