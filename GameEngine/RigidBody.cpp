#include "Text.h"
#include "Transform.h"
#include "PhysicsSystem.h"

namespace Framework
{
    RigidBody::RigidBody() :
        m_pTransform(NULL),
        m_Velocity({0.0f, 0.0f})
    {
        m_Type = COMPONENT_TYPE_RIGID_BODY;

        g_ComponentHandleTable[this->GetHandleIndex()] = this;
    };

    RigidBody::~RigidBody()
    {
        // Only free the transform if it does not belong to the parent of the text
        Transform* pTransform = static_cast<Transform*>(m_Parent->GetComponent(COMPONENT_TYPE_TRANSFORM));
        if (pTransform == NULL)
        {
            if (m_pTransform)
            {
                free(m_pTransform);
            }
        }
    };

    void RigidBody::Initialize(tinyxml2::XMLElement *txmlElement)
    {
        if (!m_pTransform)
        {
            // Get the transform from the parent, otherwise create a new one
            m_pTransform = static_cast<Transform*>(m_Parent->GetComponent(COMPONENT_TYPE_TRANSFORM));
            if (!m_pTransform)
                m_pTransform = new Transform();
        }

        if (txmlElement->Attribute("Archetype"))
        {
            tinyxml2::XMLDocument txmlDoc;
            ThrowErrorIf(
                tinyxml2::XML_SUCCESS != txmlDoc.LoadFile("Assets\\Archetypes.xml"),
                "Failed to load Assets\\Archetypes.xml"
                );

            tinyxml2::XMLElement* txmlRecursiveElement = txmlDoc.FirstChildElement();
            while (txmlRecursiveElement)
            {
                if (strcmp(txmlElement->Attribute("Archetype"), txmlRecursiveElement->Name()) == 0)
                {
                    ++m_RecursionLevel;
                    this->Initialize(txmlRecursiveElement);
                    break;
                }
                txmlRecursiveElement = txmlRecursiveElement->NextSiblingElement();
            }
        }
        
        if (txmlElement->Attribute("Name"))
        {
            m_Name = txmlElement->Attribute("Name");
        }
        if (txmlElement->Attribute("PositionX"))
        {
            m_pTransform->m_Position.x = txmlElement->FloatAttribute("PositionX");
            //m_Position.x = txmlElement->FloatAttribute("PositionX");
        }
        if (txmlElement->Attribute("PositionY"))
        {
            m_pTransform->m_Position.y = txmlElement->FloatAttribute("PositionY");
            //m_Position.y = txmlElement->FloatAttribute("PositionY");
        }
        if (txmlElement->Attribute("Rotation"))
        {
            m_pTransform->m_Rotation = txmlElement->FloatAttribute("Rotation");
            //m_Rotation = txmlElement->FloatAttribute("Rotation");
        }
        if (txmlElement->Attribute("ScaleX"))
        {
            m_pTransform->m_Scale.x = txmlElement->FloatAttribute("ScaleX");
        }
        if (txmlElement->Attribute("ScaleY"))
        {
            m_pTransform->m_Scale.y = txmlElement->FloatAttribute("ScaleY");
        }
        
        if (m_RecursionLevel == 0)
        {
            g_PHYSICS->m_RigidBodyList.push_back(this);
        }
        else
        {
            --m_RecursionLevel;
        }
    }

    void RigidBody::Accelerate(float accel, float dt)
    {
        Vector2D directionVec = { 0.0f, 0.0f };
        
        // newVel = a * dt + currentVel
        Vector2DFromAngleDeg(&directionVec, m_pTransform->m_Rotation);
        Vector2DScaleAdd(&m_Velocity, &directionVec, &m_Velocity, accel * dt);
        // newVel *= 0.99
        Vector2DScale(&m_Velocity, &m_Velocity, 0.99f);
    }

    void RigidBody::UpdatePosition(float dt)
    {
        // newPosition = velocity * dt + currentPosition
        Vector2DScaleAdd(&(m_pTransform->m_Position), &m_Velocity, &(m_pTransform->m_Position), dt);
    }
}
