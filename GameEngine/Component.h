/* Start Header -------------------------------------------------------
Copyright (C) 2013 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Component.h
Purpose: Header file for base component class
Language: C++
Platform: Windows
Project: CS529_twalton_FinalProject
Author: Tommy Walton, t.walton, 130000812
Creation date: 12/15/2013
- End Header -----------------------------------------------------*/
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
    COMPONENT_TYPE_TEXT,
    COMPONENT_TYPE_TRANSFORM,
    COMPONENT_TYPE_INPUT_HANDLER,
    COMPONENT_TYPE_RIGID_BODY,
    COMPONENT_TYPE_LIFE,
    COMPONENT_TYPE_GAME_STATE_MANAGER,
    COMPONENT_TYPE_LINE_DRAWING
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
        virtual void Serialize(tinyxml2::XMLDocument* txmlDoc) = 0;
        virtual void OnEvent(Event* e) = 0;
        virtual void Destroy() = 0;
        ComponentID GetUniqueID(){ return m_UniqueID; }
        unsigned int GetHandleIndex(){ return m_HandleIndex; }

        // Initialization that is common to all component types
        void CommonComponentInitialization(tinyxml2::XMLElement *txmlElement);
        // Initialize a variable with tinyxml
        void InitializeAttribute(tinyxml2::XMLElement *txmlElement, int &MemberVariable, char* AttributeName);
        void InitializeAttribute(tinyxml2::XMLElement *txmlElement, float &MemberVariable, char* AttributeName);
        void InitializeAttribute(tinyxml2::XMLElement *txmlElement, std::string &MemberVariable, char* AttributeName);
 //       void SetParent(GameObjectHandle handle){ m_Parent = handle; }
//        GameObjectHandle GetParent(){ return m_Parent; }

        COMPONENT_TYPE m_Type;
        std::string m_Name;
        GameObject* m_Parent;
        int m_RecursionLevel;
    private:
        ComponentID m_UniqueID;
        unsigned int m_HandleIndex;
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
        void Destroy();

        bool ComponentHandle::operator==(const ComponentHandle &rhs) const 
        {
            if(this->m_HandleIndex != rhs.m_HandleIndex
                || this->m_UniqueID != rhs.m_UniqueID)
            {
                return false;
            }
            return true;
        }

        // This function dereferences the handle
        Component* ToComponent() const;

    private:
        unsigned int m_HandleIndex;
        ComponentID m_UniqueID; // A unique ID avoids stale pointers
    };


    extern Component* g_ComponentHandleTable[MAX_COMPONENTS]; //TODO: with the current implementation, dynamic casting does not work. Only static casting. This may be acceptable.
}
