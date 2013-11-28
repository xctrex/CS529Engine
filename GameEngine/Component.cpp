#include "Component.h"

namespace Framework
{
    Component* g_ComponentHandleTable[MAX_COMPONENTS];

    std::hash<std::string> g_HashString;

    ComponentID GetUniqueIDFromString(std::string str)
    {
        return g_HashString(str);
    }

    unsigned int FindFreeSlotInComponentHandleTable()
    {
        for (unsigned int i = 0; i < MAX_COMPONENTS; ++i)
        {
            if (g_ComponentHandleTable[i] == NULL)
                return i;
        }
        return MAX_COMPONENTS;
    }

    Component::Component() :
        m_Type(COMPONENT_TYPE_NONE),
        m_Name("DefaultComponentName")
    {
        m_UniqueID = GetUniqueIDFromString(m_Name);
        m_HandleIndex = FindFreeSlotInComponentHandleTable();
        ThrowErrorIf(m_HandleIndex == MAX_COMPONENTS, "Exceeded maximum number of components");
        //g_ComponentHandleTable[m_HandleIndex] = this;
    }

    //================================================================
    // ComponentHandle
    //================================================================

    void ComponentHandle::Initialize(unsigned int index, ComponentID id)
    {
        m_HandleIndex = index;
        m_UniqueID = id;
    }

    Component* ComponentHandle::ToObject() const
    {
        Component* pObject = g_ComponentHandleTable[m_HandleIndex];
        if (pObject != NULL
            && pObject->m_UniqueID == m_UniqueID)
        {
            return pObject;
        }
        return NULL;
    }
}
