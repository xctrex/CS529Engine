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
        CommonComponentInitialization(txmlElement);

        if (!m_pTransform)
        {
            // Get the transform from the parent, otherwise throw
            m_pTransform = static_cast<Transform*>(m_Parent->GetComponent(COMPONENT_TYPE_TRANSFORM));

            // TODO: Would be better to enforce this in the .xml rather than the code, but this will do for now
            ThrowErrorIf(!m_pTransform, "Parent of an input handler component must have a transform");
        }

        if (!m_pRigidBody)
        {
            // Get the transform from the parent, otherwise throw
            m_pRigidBody = static_cast<RigidBody*>(m_Parent->GetComponent(COMPONENT_TYPE_RIGID_BODY));

            // TODO: Would be better to enforce this in the .xml rather than the code, but this will do for now
            ThrowErrorIf(!m_pRigidBody, "Parent of an input handler component must have a rigid body");
        }


        //================================================================
        // InputHandler specific initialization
        //================================================================
        if (txmlElement->Attribute("AccelerationSpeed"))
        {
            m_AccelerationSpeed = txmlElement->FloatAttribute("AccelerationSpeed");
        }
        if (txmlElement->Attribute("DeccelerationSpeed"))
        {
            m_DeccelerationSpeed = txmlElement->FloatAttribute("DeccelerationSpeed");
        }
        if (txmlElement->Attribute("RotationSpeed"))
        {
            m_RotationSpeed = txmlElement->FloatAttribute("RotationSpeed");
        }

        if (m_RecursionLevel == 0)
        {
            g_LOGIC->m_InputHandler = this;
        }
        else
        {
            --m_RecursionLevel;
        }
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