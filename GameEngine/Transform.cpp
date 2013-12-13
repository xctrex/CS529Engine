#include "Transform.h"

namespace Framework
{
    void Transform::Initialize(tinyxml2::XMLElement* txmlElement)
    {
        if (txmlElement->Attribute("Name"))
        {
            m_Name = txmlElement->Attribute("Name");
        }
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