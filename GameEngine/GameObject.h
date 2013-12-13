#pragma once

#include "Precomp.h"
#include "Event.h"
#include "Component.h"

// Game Logic Components
#include "InputHandler.h"

// Graphics Components
#include "Sprite.h"
#include "Text.h"
#include "Transform.h"

// Physics Components
#include "RigidBody.h"


typedef unsigned int GameObjectID;

namespace Framework
{
    // Define the maximum number of game objects allowed
    static const unsigned int MAX_GAME_OBJECTS = 1024;

    // Function for creating components //TODO: could be replaced by a component factory class
    ComponentHandle CreateComponent(tinyxml2::XMLElement* txmlElement);

    unsigned int FindFreeSlotInGameObjectHandleTable();
    unsigned int GetGameObjectHandleIndexFromUniqueID(GameObjectID);

    //
    class GameObject
    {
    public:
        GameObject(); //TODO: must assign a value to uniqueID and handleIndex. Could do this in Initialize, but then would have to add this to every single derived class, duplicating code.
        // The issue is that any derived class may have it's own initialization to do, but they will all share the game object initialization.
        ~GameObject();

        void Initialize(tinyxml2::XMLElement* txmlElement);
        void Serialize(tinyxml2::XMLDocument* txmlDoc);

        // Event will be passed to each component in this composition
        void OnEvent(Event* e);

        GameObjectID GetID(){ return m_UniqueID; }

        // Get the component if it exists, otherwise return NULL
        Component* GetComponent(COMPONENT_TYPE type);
        ComponentHandle GameObject::CreateComponent(tinyxml2::XMLElement* txmlElement);
    private:
        GameObjectID m_UniqueID;
        unsigned int m_HandleIndex;
        std::string m_Name;

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
