#include "InputHandler.h"
#include "WindowsSystem.h"
#include "GameLogicSystem.h"

namespace Framework
{
    InputHandler::InputHandler() :
        m_AccelerationSpeed(10.0f),
        m_DeccelerationSpeed(-1.0f),
        m_RotationSpeed(60.0f),
        m_pTransform(NULL),
        m_pRigidBody(NULL)
    { 
        m_Type = COMPONENT_TYPE_INPUT_HANDLER;
    };

    void InputHandler::Initialize(tinyxml2::XMLElement* txmlElement)
    {
        if (!m_pTransform)
        {
            m_pTransform = static_cast<Transform*>(m_Parent->GetComponent(COMPONENT_TYPE_TRANSFORM));
            if (!m_pTransform)
                m_pTransform = new Transform();
        }

        if (!m_pRigidBody)
        {
            m_pRigidBody = static_cast<RigidBody*>(m_Parent->GetComponent(COMPONENT_TYPE_RIGID_BODY));
            if (!m_pRigidBody)
                m_pRigidBody = new RigidBody();
        }

        if (txmlElement->Attribute("Name"))
        {
            m_Name = txmlElement->Attribute("Name");
        }
        if (txmlElement->Attribute("AccelerationSpeed"))
        {
            m_AccelerationSpeed = txmlElement->FloatAttribute("AccelerationSpeed");
        }
        if (txmlElement->Attribute("DeccelerationSpeed"))
        {
            m_DeccelerationSpeed = txmlElement->FloatAttribute("DeccelerationSpeed");
        }
        g_LOGIC->m_InputHandler = this;
    }

    void InputHandler::Update(float dt)
    {
        if (IsUpHeld())
        {
            m_pRigidBody->Accelerate(m_AccelerationSpeed, dt);
        }
        if (IsDownHeld())
        {
            m_pRigidBody->Accelerate(m_DeccelerationSpeed, dt);
        }
        if (IsLeftHeld())
            m_pTransform->m_Rotation += m_RotationSpeed * dt;
        if (IsRightHeld())
            m_pTransform->m_Rotation -= m_RotationSpeed * dt;
    }
}