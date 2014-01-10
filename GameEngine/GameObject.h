/* Start Header -------------------------------------------------------
Copyright (C) 2013 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: GameObject.h
Purpose: Header file for base gameobject class
Language: C++
Platform: Windows
Project: CS529_twalton_FinalProject
Author: Tommy Walton, t.walton, 130000812
Creation date: 12/15/2013
- End Header -----------------------------------------------------*/
#pragma once

#include "Precomp.h"
#include "Event.h"
#include "Component.h"

// Game Logic Components
#include "InputHandler.h"
#include "Life.h"
#include "GameStateManager.h"

// Graphics Components
#include "Sprite.h"
#include "Text.h"
#include "Transform.h"
#include "LineDrawing.h"

// Physics Components
#include "RigidBody.h"


typedef unsigned int GameObjectID;

namespace Framework
{
    // Define the maximum number of game objects allowed
    static const unsigned int MAX_GAME_OBJECTS = 1024;

    // Function for getting a component type from a c_str
    COMPONENT_TYPE GetComponentTypeFromName(const char* name);
    
    unsigned int FindFreeSlotInGameObjectHandleTable();
    unsigned int GetGameObjectHandleIndexFromUniqueID(GameObjectID);

    //
    class GameObject
    {
    public:
        GameObject(); //TODO: must assign a value to uniqueID and handleIndex. Could do this in Initialize, but then would have to add this to every single derived class, duplicating code.
        // The issue is that any derived class may have it's own initialization to do, but they will all share the game object initialization.
        ~GameObject();

        void Initialize(tinyxml2::XMLElement *txmlElement);
        void Serialize(tinyxml2::XMLDocument* txmlDoc);
        void Destroy();

        // Event will be passed to each component in this composition
        void OnEvent(Event* e);

        GameObjectID GetID(){ return m_UniqueID; }

        // Get the component if it exists, otherwise return NULL
        Component* GetComponent(COMPONENT_TYPE type);
        ComponentHandle GetComponentHandle(COMPONENT_TYPE type);
        ComponentHandle GameObject::CreateComponent(tinyxml2::XMLElement *txmlElement);
        bool m_Cleanup;
    private:
        GameObjectID m_UniqueID;
        unsigned int m_HandleIndex;
        std::string m_Name;
        int m_RecursionLevel;

        std::vector<ComponentHandle> m_ComponentVector;
        // Give the handle class access to GUID and index
        friend class GameObjectHandle;
    };

    //
    class GameObjectHandle
    {
    public:
        GameObjectHandle() :
            m_HandleIndex(MAX_GAME_OBJECTS),
            m_UniqueID(UINT_MAX)
        {}

        explicit GameObjectHandle(GameObject& object) :
            m_HandleIndex(object.m_HandleIndex),
            m_UniqueID(object.m_UniqueID)
        {}
        
        void Initialize(unsigned int index, GameObjectID id);

        // This function dereferences the handle
        GameObject* ToObject() const;
                
    private:
        unsigned int m_HandleIndex;
        GameObjectID m_UniqueID; // A unique ID avoids stale pointers
    };
    
    // Handle Table
    extern GameObject* g_GameObjectHandleTable[MAX_GAME_OBJECTS];
}
