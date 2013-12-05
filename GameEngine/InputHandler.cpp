#include "InputHandler.h"
#include "WindowsSystem.h"
#include "GameLogicSystem.h"

namespace Framework
{
    InputHandler::InputHandler() :
        m_Speed(10.0f)
    { 
        m_Type = COMPONENT_TYPE_INPUT_HANDLER;
    };

    void InputHandler::Initialize(tinyxml2::XMLElement* txmlElement)
    {
        m_pTransform = static_cast<Transform*>(m_Parent->GetComponent(COMPONENT_TYPE_TRANSFORM));
        if (!m_pTransform)
            m_pTransform = new Transform();

        if (txmlElement->Attribute("Speed"))
        {
            m_Speed = txmlElement->FloatAttribute("Speed");
        }

        g_LOGIC->m_InputHandler = this;
    }

    void InputHandler::Update(float dt)
    {
        if (IsUpHeld())
        {
            m_pTransform->m_Position.y -= m_Speed * dt * cosf(m_pTransform->m_Rotation);
            m_pTransform->m_Position.x += m_Speed * dt * sinf(m_pTransform->m_Rotation);
        }
        if (IsDownHeld())
        {
            m_pTransform->m_Position.y += m_Speed * dt * cosf(m_pTransform->m_Rotation);
            m_pTransform->m_Position.x -= m_Speed * dt * sinf(m_pTransform->m_Rotation);
        }
        if (IsLeftHeld())
            m_pTransform->m_Rotation -= (m_Speed / 40.0f) * dt;
        if (IsRightHeld())
            m_pTransform->m_Rotation += (m_Speed / 40.0f) * dt;
    }
}