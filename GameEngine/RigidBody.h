#pragma once

#include "Component.h"
#include "Transform.h"

namespace Framework
{
    // Forward declaration of Transform class
    class Transform;

    // RigidBody has a shape and a transform, handles collisions
    class RigidBody : public Component
    {
    public:
        RigidBody();
        ~RigidBody();
        virtual void Initialize(tinyxml2::XMLElement *txmlElement);
        
        // TODO: implement the rest of the pure virtual functions
        virtual void OnEvent(Event* e){};
        virtual void Serialize(tinyxml2::XMLDocument* txmlDoc){};

        void Accelerate(float accel, float dt);
        void UpdatePosition(float dt);
    private:

        
        Transform* m_pTransform;
        Vector2D m_Velocity;
    };
}