#include "InputHandler.h"

namespace Framework
{
    void InputHandler::Update(float dt)
    {
        if (IsUpHeld())
            m_Position.y -= m_Speed * dt;
        if (IsDownHeld())
            m_Position.y += m_Speed * dt;
        if (IsLeftHeld())
            m_Position.x += m_Speed * dt;
        if (IsRightHeld())
            m_Position.x -= m_Speed * dt;
    }
}