#include "GameObject.h"

namespace Framework
{
    GameObject* g_GameObjectHandleTable[MAX_GAME_OBJECTS];

    GameObjectID AssignUniqueObjectID()
    {
        return rand(); // TODO: create a better GUID system using boost
    }
    unsigned int FindFreeSlotInGameObjectHandleTable()
    {
        for (unsigned int i = 0; i < MAX_GAME_OBJECTS; ++i)
        {
            if (g_GameObjectHandleTable[i] == NULL)
                return i;
        }
        return MAX_GAME_OBJECTS;
    }

    GameObject::GameObject()
    {
        m_UniqueID = AssignUniqueObjectID();
        m_HandleIndex = FindFreeSlotInGameObjectHandleTable();
        ThrowErrorIf(m_HandleIndex == MAX_GAME_OBJECTS, "Exceeded maximum number of game objects");
        g_GameObjectHandleTable[m_HandleIndex] = this;
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
