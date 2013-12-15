#pragma once

#include "Component.h"
#include "Transform.h"

enum SHAPE
{
    SHAPE_CIRCLE,
    SHAPE_SQUARE,
    SHAPE_LINE
};

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
        virtual void OnEvent(Event* e);
        
        // TODO: implement the rest of the pure virtual functions
        virtual void Serialize(tinyxml2::XMLDocument* txmlDoc){};

        void Accelerate(float accel, float dt);
        void UpdatePosition(float dt);
        int CollidesWith(RigidBody* body2);
    private:

        
        Transform* m_pTransform;
        Vector2D m_Velocity;
        SHAPE m_Shape;
        float m_Radius;
        float m_Weight;
        Vector2D m_PreviousPosition;
        Vector2D m_PreviousVelocity;
        LineSegment2D m_LineSegment;
    };


    // Event signaling that a collision occured
    class CollisionEvent : public Event
    {
    public:
        CollisionEvent() : Event(EVENT_TYPE_COLLISION), m_pCollidedWith(NULL), m_FrameTime(0.0f) {};
        CollisionEvent(RigidBody* body, float dt) : Event(EVENT_TYPE_COLLISION), m_pCollidedWith(body), m_FrameTime(dt) {};
        RigidBody* m_pCollidedWith;
        float m_FrameTime;
    };
}