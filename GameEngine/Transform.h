/* Start Header -------------------------------------------------------
Copyright (C) 2013 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Transform.h
Purpose: Header file for transform component class
Language: C++
Platform: Windows
Project: CS529_twalton_FinalProject
Author: Tommy Walton, t.walton, 130000812
Creation date: 12/15/2013
- End Header -----------------------------------------------------*/
#pragma once
#include "Component.h"
#include <DirectXMath.h>
#include "Vector2D.h"
#include "Math2D.h"

namespace Framework
{
    class Transform : public Component
    {
    public:
        Transform::Transform() :
            m_Position(0.0f, 0.0f),
            m_Rotation(0.0f),
            m_Scale(1.0f, 1.0f)
        {
            m_Type = COMPONENT_TYPE_TRANSFORM;
            g_ComponentHandleTable[this->GetHandleIndex()] = this;
        };
        virtual void Initialize(tinyxml2::XMLElement* txmlElement);
        virtual void Serialize(tinyxml2::XMLDocument* txmlDoc) {};
        virtual void OnEvent(Event* e){};
        virtual void Destroy(){};

        Vector2D m_Position;
        float m_Rotation;
        Vector2D m_Scale;
    };
}