/* Start Header -------------------------------------------------------
Copyright (C) 2013 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: InputHanlder.cpp
Purpose: Implementation file for input handler component
Language: C++
Platform: Windows
Project: CS529_twalton_FinalProject
Author: Tommy Walton, t.walton, 130000812
Creation date: 12/15/2013
- End Header -----------------------------------------------------*/
#include "InputHandler.h"
#include "WindowsSystem.h"
#include "GameLogicSystem.h"

namespace Framework
{
    InputHandler::InputHandler() :
        m_AccelerationSpeed(10.0f),
        m_DeccelerationSpeed(-1.0f),
        m_RotationSpeed(60.0f),
        m_BulletSpeed(100.0f),
        m_pRigidBody(NULL)
    { 
        m_Type = COMPONENT_TYPE_INPUT_HANDLER;
        g_ComponentHandleTable[this->GetHandleIndex()] = this;
    };

    void InputHandler::Initialize(tinyxml2::XMLElement* txmlElement)
    {
        CommonComponentInitialization(txmlElement);

        if (!m_hTransform.ToComponent())
        {
            // Get the transform from the parent, otherwise throw
            m_hTransform = ComponentHandle(*(m_Parent->GetComponent(COMPONENT_TYPE_TRANSFORM)));

            // TODO: Would be better to enforce this in the .xml rather than the code, but this will do for now
            ThrowErrorIf(!m_hTransform.ToComponent(), "Parent of an input handler component must have a transform");
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
        if (txmlElement->Attribute("BulletSpeed"))
        {
            m_BulletSpeed = txmlElement->FloatAttribute("BulletSpeed");
        }

        if (m_RecursionLevel == 0)
        {
            g_LOGIC->m_pInputHandler = this;
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
            static_cast<Transform*>(m_hTransform.ToComponent())->m_Rotation += m_RotationSpeed * dt;
        if (IsRightHeld())
            static_cast<Transform*>(m_hTransform.ToComponent())->m_Rotation -= m_RotationSpeed * dt;
        if (IsSpaceHeld())
            CreateBullet();
    }

    void InputHandler::CreateBullet()
    {
        // Create a new game object
        GameObject* pObj = new GameObject();

        // Open the Archetype doc to get the bullet archetype
        tinyxml2::XMLDocument txmlDoc;
        ThrowErrorIf(
            tinyxml2::XML_SUCCESS != txmlDoc.LoadFile("Assets\\Archetypes.xml"),
            "Failed to load Assets\\Archetypes.xml"
            );

        tinyxml2::XMLElement* txmlRecursiveElement = txmlDoc.FirstChildElement();
        bool foundArchetype = false;
        while (txmlRecursiveElement)
        {
            if (strcmp("BulletArchetype", txmlRecursiveElement->Name()) == 0)
            {
                pObj->Initialize(txmlRecursiveElement);
                foundArchetype = true;
                break;
            }
            txmlRecursiveElement = txmlRecursiveElement->NextSiblingElement();
        }
        ThrowErrorIf(!foundArchetype, "Failed to find the bullet archetype.");

        // Override the bullets position, velocity, and speed based on the current position, and direction of the ship
        RigidBody* pBody = static_cast<RigidBody*>(pObj->GetComponent(COMPONENT_TYPE_RIGID_BODY));
        ThrowErrorIf(!pBody, "BulletArchetype missing rigid body component.");
        ThrowErrorIf(!m_hTransform.ToComponent(), "BulletArchetype missing transformcomponent");
        pBody->SetPosition(static_cast<Transform*>(m_hTransform.ToComponent())->m_Position.x, static_cast<Transform*>(m_hTransform.ToComponent())->m_Position.y);
        pBody->SetPreviousPosition(static_cast<Transform*>(m_hTransform.ToComponent())->m_Position.x, static_cast<Transform*>(m_hTransform.ToComponent())->m_Position.y);
        pBody->SetRotation(static_cast<Transform*>(m_hTransform.ToComponent())->m_Rotation);
        Vector2D velocity;
        Vector2DFromAngleDeg(velocity, static_cast<Transform*>(m_hTransform.ToComponent())->m_Rotation);
        Vector2DScale(velocity, velocity, m_BulletSpeed);
        pBody->SetVelocity(velocity.x, velocity.y);
        pBody->SetPreviousVelocity(velocity.x, velocity.y);
    }
}