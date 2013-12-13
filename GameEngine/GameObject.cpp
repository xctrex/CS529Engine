#include "GameObject.h"

namespace Framework
{
    GameObject* g_GameObjectHandleTable[MAX_GAME_OBJECTS];

    // TODO: This could be replaced by a component factory class
    ComponentHandle GameObject::CreateComponent(tinyxml2::XMLElement* txmlElement)
    {
        ComponentHandle c;
        if (strcmp(txmlElement->Name(), "Sprite") == 0)
        {
            Sprite* pSprite = new Sprite();
            pSprite->m_Parent = this;
            pSprite->Initialize(txmlElement);
            c.Initialize(pSprite->GetHandleIndex(), pSprite->GetUniqueID());
        }
        else if (strcmp(txmlElement->Name(), "Transform") == 0)
        {
            Transform* pTransform = new Transform();
            pTransform->m_Parent = this;
            pTransform->Initialize(txmlElement);
            c.Initialize(pTransform->GetHandleIndex(), pTransform->GetUniqueID());
        }
        else if (strcmp(txmlElement->Name(), "InputHandler") == 0)
        {
            InputHandler* pInput = new InputHandler();
            pInput->m_Parent = this;
            pInput->Initialize(txmlElement);
            c.Initialize(pInput->GetHandleIndex(), pInput->GetUniqueID());
        }
        else if (strcmp(txmlElement->Name(), "Text") == 0)
        {
            Text* pText = new Text();
            pText->m_Parent = this;
            pText->Initialize(txmlElement);
            c.Initialize(pText->GetHandleIndex(), pText->GetUniqueID());
        }
        else if (strcmp(txmlElement->Name(), "RigidBody") == 0)
        {
            RigidBody* pRigidBody = new RigidBody();
            pRigidBody->m_Parent = this;
            pRigidBody->Initialize(txmlElement);
            c.Initialize(pRigidBody->GetHandleIndex(), pRigidBody->GetUniqueID());
        }
        else
        {
            ThrowErrorIf(true, "Component Type not recognized");
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
        m_Name("DefaultGameObjectName")
    {
        m_ComponentVector.clear();
        m_UniqueID = GetUniqueIDFromString(m_Name); //TODO: All game objects that are not given a unique name will not have unique IDs
        m_HandleIndex = FindFreeSlotInGameObjectHandleTable();
        g_GameObjectHandleTable[m_HandleIndex] = this;
    }

    GameObject::~GameObject()
    {
        g_GameObjectHandleTable[m_HandleIndex] = NULL;
    }

    void GameObject::Initialize(tinyxml2::XMLElement* txmlElement)
    {
        tinyxml2::XMLElement* txmlGameObjectElement = txmlElement;
        // Get the attributes and override values (UniqueID, Name, etc.)
        if (txmlGameObjectElement->Attribute("Name"))
        {
            m_Name = txmlGameObjectElement->Attribute("Name");
            m_UniqueID = GetUniqueIDFromString(m_Name);
        }

        // For each child, create a component and add it to the object
        txmlGameObjectElement = txmlGameObjectElement->FirstChildElement();
        while (txmlGameObjectElement != NULL)
        {
            m_ComponentVector.push_back(CreateComponent(txmlGameObjectElement));
            txmlGameObjectElement = txmlGameObjectElement->NextSiblingElement();
        }
    }
    
    void GameObject::OnEvent(Event* e)
    {
        // Send the event to each component
        for (size_t i = 0; i < m_ComponentVector.size(); ++i)
        {
            m_ComponentVector[i].ToObject()->OnEvent(e);
        }
    }

    Component* GameObject::GetComponent(COMPONENT_TYPE type)
    {
        for (size_t i = 0; i < m_ComponentVector.size(); ++i)
        {
            if (m_ComponentVector[i].ToObject()->m_Type == type)
                return m_ComponentVector[i].ToObject();
        }
        return NULL;
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
