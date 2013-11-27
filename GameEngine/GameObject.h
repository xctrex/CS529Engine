#pragma once

#include "tinyXML2\tinyxml2.h"
#include "DebugDiagnostic.h"

typedef unsigned int GameObjectID;
typedef unsigned int ComponentID;

namespace Framework
{
    // Function for generating unique IDs //TODO: Should I differentiate between game object IDs and individual component IDs?
    GameObjectID AssignUniqueObjectID();
    unsigned int FindFreeSlotInGameObjectHandleTable();

    //
    class GameObject
    {
    public:
        GameObject(); //TODO: must assign a value to uniqueID and handleIndex. Could do this in Initialize, but then would have to add this to every single derived class, duplicating code.
        // The issue is that any derived class may have it's own initialization to do, but they will all share the game object initialization.

        virtual void Initialize(tinyxml2::XMLElement *txmlElement) = 0;
        virtual void Serialize(tinyxml2::XMLDocument *txmlDoc) = 0;

    private:
        GameObjectID m_UniqueID;
        unsigned int m_HandleIndex;

        // Give the handle class access to GUID and index
        friend class GameObjectHandle;
    };

    //
    class GameObjectHandle
    {
    public:
        explicit GameObjectHandle(GameObject& object) :
            m_HandleIndex(object.m_HandleIndex),
            m_UniqueID(object.m_UniqueID)
        {}
        
        // This function dereferences the handle
        GameObject* ToObject() const;        
        
    private:
        unsigned int m_HandleIndex;
        GameObjectID m_UniqueID; // A unique ID avoids stale pointers
    };


    // Define the size of the handle table and thus the maximum number of game objects
    static const unsigned int MAX_GAME_OBJECTS = 1024;
    extern GameObject* g_GameObjectHandleTable[MAX_GAME_OBJECTS];
}
