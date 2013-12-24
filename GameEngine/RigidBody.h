/* Start Header -------------------------------------------------------
Copyright (C) 2013 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: RigidBody.h
Purpose: Header file for RigidBody class
Language: C++
Platform: Windows
Project: CS529_twalton_FinalProject
Author: Tommy Walton, t.walton, 130000812
Creation date: 12/15/2013
- End Header -----------------------------------------------------*/
#pragma once

#include "Component.h"
#include "Transform.h"

enum SHAPE
{
    SHAPE_CIRCLE,
    SHAPE_SQUARE,
    SHAPE_LINE,
    SHAPE_SPOON,
    SHAPE_SHIP,
    SHAPE_BOMB
};

enum ASTEROID_TYPE
{
    ASTEROID_TYPE_NORMAL,
    ASTEROID_TYPE_SHRINKER,
    ASTEROID_TYPE_SPLITTER
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
        void ApplyGravity(float dt);
        void UpdatePosition(float dt);
        void BinaryMapCollision(float dt);
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
        ASTEROID_TYPE m_AsteroidType;
        float m_Radius;
        float m_Weight;
        Vector2D m_PreviousPosition;
        Vector2D m_PreviousVelocity;
        LineSegment2D m_LineSegment;
        float m_BombSecondsLeft;
        int m_BombsLeft;
        float m_Gravity;
        float m_MaxVelocity;
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