/* Start Header -------------------------------------------------------
Copyright (C) 2013 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Component.cpp
Purpose: Implementation file for base component class
Language: C++
Platform: Windows
Project: CS529_twalton_FinalProject
Author: Tommy Walton, t.walton, 130000812
Creation date: 12/15/2013
- End Header -----------------------------------------------------*/
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
        m_Parent(NULL),
        m_Name("DefaultComponentName"),
        m_RecursionLevel(0)
    {
        m_UniqueID = GetUniqueIDFromString(m_Name);
        m_HandleIndex = FindFreeSlotInComponentHandleTable();
        ThrowErrorIf(m_HandleIndex == MAX_COMPONENTS, "Exceeded maximum number of components");
        g_ComponentHandleTable[m_HandleIndex] = this;
    }

    // Initialization that is relevant to all component types
    void Component::CommonComponentInitialization(tinyxml2::XMLElement* txmlElement)
    {
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
            ThrowErrorIf(!foundArchetype, "Failed to find a component archetype by that name.");
        }

        if (txmlElement->Attribute("Name"))
        {
            m_Name = txmlElement->Attribute("Name");
        }
    }

    //================================================================
    // ComponentHandle
    //================================================================

    void ComponentHandle::Initialize(unsigned int index, ComponentID id)
    {
        m_HandleIndex = index;
        m_UniqueID = id;
    }
    
    void ComponentHandle::Destroy()
    {
        if (g_ComponentHandleTable[m_HandleIndex])
        {
            // Call destroy on the component
            g_ComponentHandleTable[m_HandleIndex]->Destroy();
            // Delete the point to the component
            delete g_ComponentHandleTable[m_HandleIndex];
            // Set the value in the handle table to NULL
            g_ComponentHandleTable[m_HandleIndex] = NULL;
        }
        g_ComponentHandleTable[m_HandleIndex] = NULL;        
    }

    Component* ComponentHandle::ToComponent() const
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
