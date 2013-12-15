#pragma once

#include "Component.h"
#include "Transform.h"

enum SHAPE
{
    SHAPE_CIRCLE,
    SHAPE_SQUARE,
    SHAPE_LINE,
    SHAPE_SPOON,
    SHAPE_SHIP
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
        virtual void Destroy();

        void Accelerate(float accel, float dt);
        void UpdatePosition(float dt);
        int CollidesWith(RigidBody* body2);

        void SetPosition(float x, float y) { m_pTransform->m_Position.x = x; m_pTransform->m_Position.y = y; }
        void SetRotation(float rot) { m_pTransform->m_Rotation = rot; }
        void SetVelocity(float x, float y) { m_Velocity.x = x; m_Velocity.y = y; }
        void SetPreviousPosition(float x, float y) { m_PreviousPosition.x = x; m_PreviousPosition.y = y; }
        void SetPreviousVelocity(float x, float y) { m_PreviousVelocity.x = x; m_PreviousVelocity.y = y; }
        float GetRadius(){ return m_Radius; }
        SHAPE GetShape(){ return m_Shape; }
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