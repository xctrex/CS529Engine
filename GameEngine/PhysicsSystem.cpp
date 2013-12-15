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
   //     bool CollisionsResolved = false;
        
   //     while (!CollisionsResolved)
   //     {
   //         CollisionsResolved = true;
            std::list<RigidBody*>::iterator it1 = m_RigidBodyList.begin();
            for (; it1 != m_RigidBodyList.end(); ++it1)
            {
                std::list<RigidBody*>::iterator it2 = it1;
                ++it2;
                for (; it2 != m_RigidBodyList.end(); ++it2)
                {
                    // Detect a collision between it1 and it2
                    if ((*it1)->CollidesWith(*it2))
                    {
                        // Set collisions resolved to false, leading to (at least) one more round of collision detection
 //                       CollisionsResolved = false;

                        // If there is a collision, create collision events and send them to the parents
                        CollisionEvent CollidedWithIt2(*it2, dt);
                        CollisionEvent CollidedWithIt1(*it1, dt);

                        // it1 collided with it2
                        (*it1)->m_Parent->OnEvent(&CollidedWithIt2);
                        // it2 collided with it1
                        (*it2)->m_Parent->OnEvent(&CollidedWithIt1);                    
                    }
                }
            }
       // }
    }
}