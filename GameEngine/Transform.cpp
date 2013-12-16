/* Start Header -------------------------------------------------------
Copyright (C) 2013 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Transform.cpp
Purpose: Implementation file for transform component
Language: C++
Platform: Windows
Project: CS529_twalton_FinalProject
Author: Tommy Walton, t.walton, 130000812
Creation date: 12/15/2013
- End Header -----------------------------------------------------*/
#include "Transform.h"

namespace Framework
{
    void Transform::Initialize(tinyxml2::XMLElement* txmlElement)
    {
        CommonComponentInitialization(txmlElement);

        //================================================================
        // Transform specific initialization
        //================================================================
        if (txmlElement->Attribute("PositionX"))
        {
            m_Position.x = txmlElement->FloatAttribute("PositionX");
        }
        if (txmlElement->Attribute("PositionY"))
        {
            m_Position.y = txmlElement->FloatAttribute("PositionY");
        }
        if (txmlElement->Attribute("Rotation"))
        {
            m_Rotation = txmlElement->FloatAttribute("Rotation");
        }
        if (txmlElement->Attribute("ScaleX"))
        {
            m_Scale.x = txmlElement->FloatAttribute("ScaleX");
        }
        if (txmlElement->Attribute("ScaleY"))
        {
            m_Scale.y = txmlElement->FloatAttribute("ScaleY");
        }
    }
}