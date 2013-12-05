#pragma once
#include "Precomp.h"
#include "tinyXML2\tinyxml2.h"
#include "DebugDiagnostic.h"
#include "Event.h"

typedef unsigned int ComponentID;

enum COMPONENT_TYPE
{
    COMPONENT_TYPE_NONE = 0,
    COMPONENT_TYPE_SPRITE,
    COMPONENT_TYPE_TRANSFORM,
    COMPONENT_TYPE_INPUT_HANDLER
};

//TODO: having handles for components and handles for game objects duplicates a lot of code
namespace Framework
{
    // Forward declaration for GameObjectHandle
    class GameObject;

    // Define the size of the handle table and thus the maximum number of game objects
    static const unsigned int MAX_COMPONENTS = 1024;

    unsigned int FindFreeSlotInComponentHandleTable();
    ComponentID GetUniqueIDFromString(std::string str);

    class Component
    {
    public:
        Component();
        virtual void Initialize(tinyxml2::XMLElement *txmlElement) = 0;
        virtual void Serialize(tinyxml2::XMLDocument *txmlDoc) = 0;
        virtual void OnEvent(Event* e) = 0;
        ComponentID GetUniqueID(){ return m_UniqueID; }
        unsigned int GetHandleIndex(){ return m_HandleIndex; }

 //       void SetParent(GameObjectHandle handle){ m_Parent = handle; }
//        GameObjectHandle GetParent(){ return m_Parent; }

        COMPONENT_TYPE m_Type;
        GameObject* m_Parent;
    private:
        ComponentID m_UniqueID;
        unsigned int m_HandleIndex;
        std::string m_Name;
//TODO: Enable Component class to have a handle to its parent
//        GameObjectHandle m_Parent;
        // Give the handle class access to GUID and index
        friend class ComponentHandle;
    };

    //
    class ComponentHandle
    {
    public:
        ComponentHandle() :
            m_HandleIndex(MAX_COMPONENTS),
            m_UniqueID(UINT_MAX)
        {}

        explicit ComponentHandle(Component& component) :
            m_HandleIndex(component.m_HandleIndex),
            m_UniqueID(component.m_UniqueID)
        {}

        void Initialize(unsigned int index, ComponentID id);

        // This function dereferences the handle
        Component* ToObject() const;

    private:
        unsigned int m_HandleIndex;
        ComponentID m_UniqueID; // A unique ID avoids stale pointers
    };


    extern Component* g_ComponentHandleTable[MAX_COMPONENTS]; //TODO: with the current implementation, dynamic casting does not work. Only static casting. This may be acceptable.
}