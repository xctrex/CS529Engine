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
        InitializeAttribute(txmlElement, m_Position.x, "PositionX");
        InitializeAttribute(txmlElement, m_Position.y, "PositionY");
        InitializeAttribute(txmlElement, m_Rotation, "Rotation");
        InitializeAttribute(txmlElement, m_Scale.x, "ScaleX");
        InitializeAttribute(txmlElement, m_Scale.y, "ScaleY");
    }
}