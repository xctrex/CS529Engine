/* Start Header -------------------------------------------------------
Copyright (C) 2013 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: GameObject.cpp
Purpose: Implementation file for base game object class
Language: C++
Platform: Windows
Project: CS529_twalton_FinalProject
Author: Tommy Walton, t.walton, 130000812
Creation date: 12/15/2013
- End Header -----------------------------------------------------*/
#include "GameObject.h"

namespace Framework
{
    GameObject* g_GameObjectHandleTable[MAX_GAME_OBJECTS];

    COMPONENT_TYPE GetComponentTypeFromName(const char* name)
    {
        if (strcmp(name, "Sprite") == 0)
            return COMPONENT_TYPE_SPRITE;
        else if (strcmp(name, "Transform") == 0)
            return COMPONENT_TYPE_TRANSFORM;
        else if (strcmp(name, "InputHandler") == 0)
            return COMPONENT_TYPE_INPUT_HANDLER;
        else if (strcmp(name, "Text") == 0)
            return COMPONENT_TYPE_TEXT;
        else if (strcmp(name, "RigidBody") == 0)
            return COMPONENT_TYPE_RIGID_BODY;
        else if (strcmp(name, "Life") == 0)
            return COMPONENT_TYPE_LIFE;
        else if (strcmp(name, "GameStateManager") == 0)
            return COMPONENT_TYPE_GAME_STATE_MANAGER;
        else
            return COMPONENT_TYPE_NONE;
    }

    // TODO: This could be replaced by a component factory class
    ComponentHandle GameObject::CreateComponent(tinyxml2::XMLElement *txmlElement)
    {
        ComponentHandle c;
        COMPONENT_TYPE type = GetComponentTypeFromName(txmlElement->Name());
        switch (type)
        {
        case COMPONENT_TYPE_SPRITE:
        {
            Sprite* pSprite = new Sprite();
            pSprite->m_Parent = this;
            pSprite->Initialize(txmlElement);
            c.Initialize(pSprite->GetHandleIndex(), pSprite->GetUniqueID());
            break;
        }
        case COMPONENT_TYPE_TRANSFORM:
        {
            Transform* pTransform = new Transform();
            pTransform->m_Parent = this;
            pTransform->Initialize(txmlElement);
            c.Initialize(pTransform->GetHandleIndex(), pTransform->GetUniqueID());
            break;
        }
        case COMPONENT_TYPE_INPUT_HANDLER:
        {
            InputHandler* pInput = new InputHandler();
            pInput->m_Parent = this;
            pInput->Initialize(txmlElement);
            c.Initialize(pInput->GetHandleIndex(), pInput->GetUniqueID());
            break;
        }
        case COMPONENT_TYPE_TEXT:
        {
            Text* pText = new Text();
            pText->m_Parent = this;
            pText->Initialize(txmlElement);
            c.Initialize(pText->GetHandleIndex(), pText->GetUniqueID());
            break;
        }
        case COMPONENT_TYPE_RIGID_BODY:
        {
            RigidBody* pRigidBody = new RigidBody();
            pRigidBody->m_Parent = this;
            pRigidBody->Initialize(txmlElement);
            c.Initialize(pRigidBody->GetHandleIndex(), pRigidBody->GetUniqueID());
            break;
        }
        case COMPONENT_TYPE_LIFE:
        {
            Life* pLife = new Life();
            pLife->m_Parent = this;
            pLife->Initialize(txmlElement);
            c.Initialize(pLife->GetHandleIndex(), pLife->GetUniqueID());
            break;
        }
        case COMPONENT_TYPE_GAME_STATE_MANAGER:
        {
            GameStateManager* pGSM = new GameStateManager();
            pGSM->m_Parent = this;
            pGSM->Initialize(txmlElement);
            c.Initialize(pGSM->GetHandleIndex(), pGSM->GetUniqueID());
            break;
        }
        default :
            ThrowErrorIf(true, "Component Type not recognized");
            break;
        }
        return c;
    }

    unsigned int FindFreeSlotInGameObjectHandleTable()
    {
        for (unsigned int i = 0; i < MAX_GAME_OBJECTS; ++i)
        {
            if (g_GameObjectHandleTable[i] == NULL)
                return i;
        }
        ThrowErrorIf(true, "Exceeded maximum number of game objects");
        return MAX_GAME_OBJECTS;
    }

    unsigned int GetGameObjectHandleIndexFromUniqueID(GameObjectID id)
    {
        for (unsigned int i = 0; i < MAX_GAME_OBJECTS; ++i)
        {
            if (g_GameObjectHandleTable[i]->GetID() == id)
                return i;
        }
        ThrowErrorIf(true, "GameObjectNotFound");
        return MAX_GAME_OBJECTS;
    }

    GameObject::GameObject() :
        m_Name("DefaultGameObjectName"),
        m_RecursionLevel(0),
        m_Cleanup(false)
    {
        m_ComponentVector.clear();
        m_UniqueID = GetUniqueIDFromString(m_Name); //TODO: All game objects that are not given a unique name will not have unique IDs
        m_HandleIndex = FindFreeSlotInGameObjectHandleTable();
        g_GameObjectHandleTable[m_HandleIndex] = this;
    }

    GameObject::~GameObject()
    {
        
    }

    void GameObject::Destroy()
    {
        for (size_t i = 0; i < m_ComponentVector.size(); ++i)
        {
            m_ComponentVector[i].Destroy();
        }
        g_GameObjectHandleTable[m_HandleIndex] = NULL;
    }

    void GameObject::Initialize(tinyxml2::XMLElement* txmlElement)
    {
        //TODO: this code is repeated in a lot of places. See if it can be turned into a function
        if (txmlElement->Attribute("Archetype"))
        {
            tinyxml2::XMLDocument txmlDoc;
            ThrowErrorIf(
                tinyxml2::XML_SUCCESS != txmlDoc.LoadFile("Assets\\Archetypes.xml"),
                "Failed to load Assets\\Archetypes.xml"
                );

            tinyxml2::XMLElement* txmlRecursiveElement = txmlDoc.FirstChildElement();
            bool foundArchetype = false;
            while (txmlRecursiveElement)
            {
                if (strcmp(txmlElement->Attribute("Archetype"), txmlRecursiveElement->Name()) == 0)
                {
                    ++m_RecursionLevel;
                    this->Initialize(txmlRecursiveElement);
                    foundArchetype = true;
                    break;
                }
                txmlRecursiveElement = txmlRecursiveElement->NextSiblingElement();
            }
            ThrowErrorIf(!foundArchetype, "Failed to find a game object archetype by that name.");
        }

        tinyxml2::XMLElement* txmlGameObjectElement = txmlElement;
        // Get the attributes and override values (Name, etc.)
        if (txmlGameObjectElement->Attribute("Name"))
        {
            m_Name = txmlGameObjectElement->Attribute("Name");
            m_UniqueID = GetUniqueIDFromString(m_Name);
        }

        // For each child, create a component and add it to the object
        txmlGameObjectElement = txmlGameObjectElement->FirstChildElement();
        while (txmlGameObjectElement != NULL)
        {
            COMPONENT_TYPE type = GetComponentTypeFromName(txmlGameObjectElement->Name());
            Component* component = this->GetComponent(type);
            // If ComponentVector already has a component of that type, override that components attributes
            if (component)
            {
                // Override attributes with another initialization call
                component->Initialize(txmlGameObjectElement);
            }
            // Otherwise, add a new component
            else
            {
                m_ComponentVector.push_back(CreateComponent(txmlGameObjectElement));
            }
            txmlGameObjectElement = txmlGameObjectElement->NextSiblingElement();
        }
    }
    
    void GameObject::OnEvent(Event* e)
    {
        if (e->m_EventType == EVENT_TYPE_OBJECT_CLEANUP)
        {
            // If object isn' already slated for cleanup
            if (!this->m_Cleanup)
            {
                // Add to cleanup list
                this->m_Cleanup = true;
                g_CORE->AddToCleanupList(this);
            }
            return;
        }
        else if (e->m_EventType == EVENT_TYPE_DAMAGE)
        {
            // If this object has a life component
            Life* pLife = static_cast<Life*>(this->GetComponent(COMPONENT_TYPE_LIFE));
            if (pLife)
            {
                // Take damage
                DamageEvent* de = static_cast<DamageEvent*>(e);
                pLife->TakeDamage(de->m_Damage);
            }
        }
        // Send the event to each component
        for (size_t i = 0; i < m_ComponentVector.size(); ++i)
        {
            m_ComponentVector[i].ToComponent()->OnEvent(e);
        }
    }

    Component* GameObject::GetComponent(COMPONENT_TYPE type)
    {
        for (size_t i = 0; i < m_ComponentVector.size(); ++i)
        {
            if (m_ComponentVector[i].ToComponent())
            {            
                if (m_ComponentVector[i].ToComponent()->m_Type == type)
                    return m_ComponentVector[i].ToComponent();
            }
        }
        return NULL;
    }
    ComponentHandle GameObject::GetComponentHandle(COMPONENT_TYPE type)
    {
        for (size_t i = 0; i < m_ComponentVector.size(); ++i)
        {
            if (m_ComponentVector[i].ToComponent())
            {
                if (m_ComponentVector[i].ToComponent()->m_Type == type)
                    return m_ComponentVector[i];
            }
        }
        ComponentHandle ch;
        return ch;
    }
    //================================================================
    // GameObjectHandle
    //================================================================

    void GameObjectHandle::Initialize(unsigned int index, GameObjectID id)
    {
        m_HandleIndex = index;
        m_UniqueID = id;
    }

    GameObject* GameObjectHandle::ToObject() const
    {
        GameObject* pObject = g_GameObjectHandleTable[m_HandleIndex];
        if (pObject != NULL
            && pObject->m_UniqueID == m_UniqueID)
        {
            return pObject;
        }
        return NULL;
    }
}
