#pragma once

#include "tinyXML2\tinyxml2.h"
#include "GameObject.h"

//TODO: having handles for components and handles for game objects duplicates a lot of code
namespace Framework
{
    unsigned int FindFreeSlotInComponentHandleTable();

    class Component
    {
    public:
        Component();
        virtual void Initialize(tinyxml2::XMLElement *txmlElement) = 0;
        virtual void Serialize(tinyxml2::XMLDocument *txmlDoc) = 0;
        ComponentID GetUniqueID(){ return m_UniqueID; }
        unsigned int GetHandleIndex(){ return m_HandleIndex; }


    private:
        ComponentID m_UniqueID;
        unsigned int m_HandleIndex;

        // Give the handle class access to GUID and index
        friend class ComponentHandle;
    };

    //
    class ComponentHandle
    {
    public:
        explicit ComponentHandle(Component& component) :
            m_HandleIndex(component.m_HandleIndex),
            m_UniqueID(component.m_UniqueID)
        {}

        // This function dereferences the handle
        Component* ToObject() const;

    private:
        unsigned int m_HandleIndex;
        ComponentID m_UniqueID; // A unique ID avoids stale pointers
    };


    // Define the size of the handle table and thus the maximum number of game objects
    static const unsigned int MAX_COMPONENTS = 1024;
    extern Component* g_ComponentHandleTable[MAX_GAME_OBJECTS]; //TODO: with the current implementation, dynamic casting does not work. Only static casting. This may be acceptable.
}