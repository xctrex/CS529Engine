#include "GameObject.h"

namespace Framework
{
    GameObject* g_GameObjectHandleTable[MAX_GAME_OBJECTS];

    // TODO: This could be replaced by a component factory class
    ComponentHandle CreateComponent(tinyxml2::XMLElement* txmlElement)
    {
        ComponentHandle c;
        if (strcmp(txmlElement->Name(), "Sprite") == 0)
        {
            Sprite* pSprite = new Sprite();
            pSprite->Initialize(txmlElement);
            c.Initialize(pSprite->GetHandleIndex(), pSprite->GetUniqueID());
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
        m_UniqueID = AssignUniqueObjectID();
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
        }
        if (txmlGameObjectElement->Attribute("UniqueID"))
        {
            m_UniqueID = txmlGameObjectElement->UnsignedAttribute("UniqueID");
        }

        // For each child, create a component and add it to the object
        txmlGameObjectElement = txmlGameObjectElement->FirstChildElement();
        while (txmlGameObjectElement != NULL)
        {
            m_ComponentVector.push_back(CreateComponent(txmlGameObjectElement));
            txmlGameObjectElement = txmlGameObjectElement->NextSiblingElement();
        }
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
