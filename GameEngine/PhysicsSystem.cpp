#include "PhysicsSystem.h"

namespace Framework
{
    // Global pointer to the game logic
    PhysicsSystem* g_PHYSICS = NULL;

    PhysicsSystem::PhysicsSystem()
    {
        //Set up the global pointer
        ThrowErrorIf(g_PHYSICS != NULL, "Physics already initialized");
        g_PHYSICS = this;
    }

    PhysicsSystem::~PhysicsSystem()
    {
        
    }

    void PhysicsSystem::Update(float dt)
    {
        std::list<RigidBody*>::iterator it = m_RigidBodyList.begin();
        for (; it != m_RigidBodyList.end(); ++it)
            (*it)->UpdatePosition(dt);
        // Update the input handler
        HandleCollisions(dt);
    }

    void PhysicsSystem::HandleCollisions(float dt)
    {
        /*std::list<RigidBody>::iterator it = m_RigidBodyList.begin();
        for (; it != m_RigidBodyList.end(); ++it)
            //TODO: collide*/
    }
}