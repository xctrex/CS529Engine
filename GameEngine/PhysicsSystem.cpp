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
        std::list<RigidBody*>::iterator it1 = m_RigidBodyList.begin();
        for (; it1 != m_RigidBodyList.end(); ++it1)
        {
            std::list<RigidBody*>::iterator it2 = it1;
            ++it2;
            for (; it2 != m_RigidBodyList.end(); ++it2)
            {
                // Detect a collision between it1 and it2
                // TODO: If there is a collision
            }
        }
    }
}