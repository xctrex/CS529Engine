/* Start Header -------------------------------------------------------
Copyright (C) 2013 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: RigidBody.cpp
Purpose: Implementation file for rigid body component
Language: C++
Platform: Windows
Project: CS529_twalton_FinalProject
Author: Tommy Walton, t.walton, 130000812
Creation date: 12/15/2013
- End Header -----------------------------------------------------*/
#include "Text.h"
#include "Transform.h"
#include "PhysicsSystem.h"
#include "GameLogicSystem.h"

#define BULLET_TO_ASTEROID_DAMAGE 10
#define ASTEROID_TO_SHIP_DAMAGE 20
#define BOMB_TO_ASTEROID_DAMAGE 2000
#define EPSILON 0.00001f

//Collision flags
#define	COLLISION_LEFT		0x00000001	//0001
#define	COLLISION_RIGHT		0x00000002	//0010
#define	COLLISION_TOP		0x00000004	//0100
#define	COLLISION_BOTTOM	0x00000008	//1000


namespace Framework
{
    int CheckInstanceBinaryMapCollision(float PosX, float PosY, float scaleX, float scaleY)
    {
        // Initialize flag
        int flag = 0;


        // Build hotspots
        Vector2D TopL, TopR, BottomL, BottomR, LeftT, LeftB, RightT, RightB;
        TopL.y = TopR.y = PosY + 0.5f * scaleY;// + EPSILON;
        TopL.x = BottomL.x = PosX - 0.25f * scaleX;// - EPSILON;
        TopR.x = BottomR.x = PosX + 0.25f * scaleX;// + EPSILON;
        BottomL.y = BottomR.y = PosY - 0.5f * scaleY;// - EPSILON;

        LeftT.x = LeftB.x = PosX - 0.5f * scaleX;// - EPSILON;
        LeftT.y = RightT.y = PosY + 0.25f * scaleY;// + EPSILON;
        LeftB.y = RightB.y = PosY - 0.25f * scaleY;// - EPSILON;
        RightT.x = RightB.x = PosX + 0.5f * scaleX;// + EPSILON;

        // Check collision for hotspots and set flag in case of collision
        if (g_PHYSICS->GetCellValue((int)TopL.x, (int)TopL.y) == 1 || g_PHYSICS->GetCellValue((int)TopR.x, (int)TopR.y) == 1)
            flag |= COLLISION_TOP;
        if (g_PHYSICS->GetCellValue((int)BottomL.x, (int)BottomL.y) == 1 || g_PHYSICS->GetCellValue((int)BottomR.x, (int)BottomR.y) == 1)
            flag |= COLLISION_BOTTOM;
        if (g_PHYSICS->GetCellValue((int)LeftT.x, (int)LeftT.y) == 1 || g_PHYSICS->GetCellValue((int)LeftB.x, (int)LeftB.y) == 1)
            flag |= COLLISION_LEFT;
        if (g_PHYSICS->GetCellValue((int)RightT.x, (int)RightT.y) == 1 || g_PHYSICS->GetCellValue((int)RightB.x, (int)RightB.y) == 1)
            flag |= COLLISION_RIGHT;

        return flag;
    }

    void SnapToCell(float *Coordinate)
    {
        *Coordinate = (float)((int)*Coordinate) + 0.5f;
    }

    RigidBody::RigidBody() :
        m_pTransform(NULL),
        m_Velocity(0.0f, 0.0f),
        m_Shape(SHAPE_CIRCLE),
        m_AsteroidType(ASTEROID_TYPE_NORMAL),
        m_Radius(0.0f),
        m_Weight(FLT_MAX),
        m_PreviousPosition(0.0f, 0.0f),
        m_PreviousVelocity(0.0f, 0.0f),
        m_BombSecondsLeft(3.0f),
        m_Gravity(-200.0f),
        m_MovementVelocity(40.0f),
        m_MaxVelocity(400.0f),
        m_JumpVelocity(110.0f)
    {
        m_Type = COMPONENT_TYPE_RIGID_BODY;

		m_LineSegment.m_N = Vector2D(0.0f, 1.0f);
        m_LineSegment.m_NdotP0 = 1.0f;
        m_LineSegment.m_P0 = Vector2D(0.0f, 0.0f);
        m_LineSegment.m_P1 = Vector2D(0.0f, 0.0f);

        g_ComponentHandleTable[this->GetHandleIndex()] = this;
    };

    RigidBody::~RigidBody()
    {
    };
    void RigidBody::Destroy()
    {
        g_PHYSICS->m_RigidBodyList.remove(this);
        if (m_Shape == SHAPE_SHIP)
        {
            // Inform the game that the ship has been destroyed
            ObjectDeathEvent ode(OBJECT_TYPE_SHIP);
            g_LOGIC->OnEvent(&ode);
            
        }
        if (m_Shape == SHAPE_CIRCLE)
        {
            // Update number of asteroids remaining
            ObjectDeathEvent ode(OBJECT_TYPE_ASTEROID);
            g_LOGIC->OnEvent(&ode);
        }
    };

    void RigidBody::Initialize(tinyxml2::XMLElement *txmlElement)
    {
        CommonComponentInitialization(txmlElement);
        
        //================================================================
        // RigidBody specific initialization
        //================================================================
        if (txmlElement->Attribute("Shape"))
        {
            if (strcmp(txmlElement->Attribute("Shape"), "Circle") == 0)
            {
                m_Shape = SHAPE_CIRCLE;
            }
            else if (strcmp(txmlElement->Attribute("Shape"), "Square") == 0)
            {
                m_Shape = SHAPE_SQUARE;
            }
            else if (strcmp(txmlElement->Attribute("Shape"), "Line") == 0)
            {
                m_Shape = SHAPE_LINE;
            }
            else if (strcmp(txmlElement->Attribute("Shape"), "Spoon") == 0)
            {
                m_Shape = SHAPE_SPOON;
            }
            else if (strcmp(txmlElement->Attribute("Shape"), "Ship") == 0)
            {
                m_Shape = SHAPE_SHIP;
            }
            else if (strcmp(txmlElement->Attribute("Shape"), "Bomb") == 0)
            {
                m_Shape = SHAPE_BOMB;
            }
            else
            {
                ThrowErrorIf(true, "Shape not recognized");
            }
        }
        if (txmlElement->Attribute("AsteroidType"))
        {
            if (strcmp(txmlElement->Attribute("AsteroidType"), "Normal") == 0)
            {
                m_AsteroidType = ASTEROID_TYPE_NORMAL;
            }
            else if (strcmp(txmlElement->Attribute("AsteroidType"), "Shrinker") == 0)
            {
                m_AsteroidType = ASTEROID_TYPE_SHRINKER;
            }
            else if (strcmp(txmlElement->Attribute("AsteroidType"), "Splitter") == 0)
            {
                m_AsteroidType = ASTEROID_TYPE_SPLITTER;
            }
            else
            {
                ThrowErrorIf(true, "Asteroid type not recognized");
            }
        }
        InitializeAttribute(txmlElement, m_Radius, "Radius");
        InitializeAttribute(txmlElement, m_Weight, "Weight");
        InitializeAttribute(txmlElement, m_Velocity.x, "VelocityX");
        InitializeAttribute(txmlElement, m_Velocity.y, "VelocityY");
        InitializeAttribute(txmlElement, m_Gravity, "Gravity");
        InitializeAttribute(txmlElement, m_MovementVelocity, "MovementVelocity");
        InitializeAttribute(txmlElement, m_MaxVelocity, "MaxVelocity");
        InitializeAttribute(txmlElement, m_JumpVelocity, "JumpVelocity");

        // Line Segment initialization
        tinyxml2::XMLElement* txmlLineSegmentElement = txmlElement->FirstChildElement("LineSegment");
        if (txmlLineSegmentElement)
        {
            ThrowErrorIf(
                (!txmlLineSegmentElement->Attribute("P0X")
                || !txmlLineSegmentElement->Attribute("P0Y")
                || !txmlLineSegmentElement->Attribute("P1X")
                || !txmlLineSegmentElement->Attribute("P1Y")),
                "Line Segment not fully defined in the xml."
                );
            m_LineSegment.m_P0.x = txmlLineSegmentElement->FloatAttribute("P0X");
            m_LineSegment.m_P0.y = txmlLineSegmentElement->FloatAttribute("P0Y");
            m_LineSegment.m_P1.x = txmlLineSegmentElement->FloatAttribute("P1X");
            m_LineSegment.m_P1.y = txmlLineSegmentElement->FloatAttribute("P1Y");
        }

        //================================================================
        // Transform attributes
        //================================================================
        // Can override values from the parent's transform here.
        // TODO: Not sure if this is desireable
        if (!m_pTransform)
        {
            // Get the transform from the parent, otherwise throw
            m_pTransform = static_cast<Transform*>(m_Parent->GetComponent(COMPONENT_TYPE_TRANSFORM));

            // TODO: Would be better to enforce this in the .xml rather than the code, but this will do for now
            ThrowErrorIf(!m_pTransform, "Parent of a rigid body component must have a transform");
        }        

        if (m_RecursionLevel == 0)
        {
            g_PHYSICS->m_RigidBodyList.push_back(this);
        }
        else
        {
            --m_RecursionLevel;
        }
    }

    void RigidBody::OnEvent(Event* e)
    {
        if (e->m_EventType == EVENT_TYPE_COLLISION)
        {
            RigidBody* collider = static_cast<CollisionEvent*>(e)->m_pCollidedWith;
            
            if (this->m_Shape == SHAPE_SHIP && collider->m_Shape == SHAPE_CIRCLE)
            {
                // Do nothing (I only put this here so I could set a breakpoint
                collider->GetHandleIndex();
            }
            // Handle Line collision logic
            if (this->m_Shape == SHAPE_LINE)
            {
                // Lines don't need to do anything
                return;
            }

            // Whichever object is "lighter" will bounce, and the heavier object will be unaffected by the collision
            // This is not physically accurate, but allows us to establish a hierarchy of which objects bounce
            // and which ones don't (e.g., ship bounces off of both asteroids and walls, asteroids only bounce off walls,
            // and walls are unimpacted by collisions) If objects are equally weighted, both will bounce
            float dt = static_cast<CollisionEvent*>(e)->m_FrameTime;
            Vector2D Pi;
            Vector2D R;            
            if (this->m_Weight <= collider->m_Weight)
            {
                // Need to calculate the original updated position from previous velocity because
                // the current position might have already been modified by this collision
                Vector2D thisNewPosition;
                thisNewPosition.x = this->m_PreviousPosition.x + this->m_PreviousVelocity.x * dt;
                thisNewPosition.y = this->m_PreviousPosition.y + this->m_PreviousVelocity.y * dt;
                Vector2D colliderNewPosition;
                colliderNewPosition.x = collider->m_PreviousPosition.x + collider->m_PreviousVelocity.x * dt;
                colliderNewPosition.y = collider->m_PreviousPosition.y + collider->m_PreviousVelocity.y * dt;

                float ti = 0.0f;
                if (this->m_Shape == SHAPE_CIRCLE && collider->m_Shape == SHAPE_CIRCLE
                    || this->m_Shape == SHAPE_CIRCLE && collider->m_Shape == SHAPE_SHIP
                    || this->m_Shape == SHAPE_SHIP && collider->m_Shape == SHAPE_CIRCLE)
                {
                    ti = ReflectAnimatedCircleOnAnimatedCircle(
                        this->m_PreviousPosition,
                        thisNewPosition,
                        this->m_Radius,
                        collider->m_PreviousPosition,
                        colliderNewPosition,
                        collider->m_Radius,
                        Pi,
                        R
                        );
                }
                else if (this->m_Shape == SHAPE_CIRCLE && collider->m_Shape == SHAPE_LINE
                    || this->m_Shape == SHAPE_SHIP && collider->m_Shape == SHAPE_LINE)
                {
                    ti = ReflectAnimatedCircleOnStaticLineSegment(
                        this->m_PreviousPosition,
                        thisNewPosition,
                        this->m_Radius,
                        collider->m_LineSegment,
                        Pi,
                        R
                        );
                }
                
                if (ti < 0) return;
                // Get center position at time of impact
                Vector2D CenterAtImpact = this->m_PreviousPosition;
                CenterAtImpact.x += (this->m_PreviousPosition.x - thisNewPosition.x) * ti;
                CenterAtImpact.y += (this->m_PreviousPosition.y - thisNewPosition.y) * ti;

                Vector2DSub(R, CenterAtImpact, Pi);
                // Update the position of this
                //Vector2DAdd(this->m_pTransform->m_Position, CenterAtImpact, R);
                //Vector2DSet(this->m_pTransform->m_Position, CenterAtImpact.x, CenterAtImpact.y);
                // Determine the angle of the collision and use this information to know if a rigid body was hit vertically or horizontally
                float angle = AngleDegFromVector2D(R);
                
                // If the body collided horizontally, set the x velocity to 0
                if ((0.0f <= angle && angle <= 45.0f)
                    || (315.0f <= angle && angle <= 360.0f))
                {
                    m_Velocity.x = 0.0f;
                    if (collider->m_Shape == SHAPE_LINE)
                        m_pTransform->m_Position.x = collider->m_pTransform->m_Position.x + m_Radius;
                }
                else if (135.0f <= angle && angle <= 225.0f)
                {
                    m_Velocity.x = 0.0f;
                    if (collider->m_Shape == SHAPE_LINE)
                        m_pTransform->m_Position.x = collider->m_pTransform->m_Position.x - m_Radius;
                }
                // If the body collided vertically, set the y velocity to 0
                if ((45.0f <= angle && angle <= 135.0f)
                    )
                {
                    m_Velocity.y = 0.0f;
                    if (collider->m_Shape == SHAPE_LINE)
                        m_pTransform->m_Position.y = collider->m_pTransform->m_Position.y + m_Radius;
                }
                else if (225.0f <= angle && angle <= 315.0f)
                {
                    m_Velocity.y = 0.0f;
                    if (collider->m_Shape == SHAPE_LINE)
                        m_pTransform->m_Position.y = collider->m_pTransform->m_Position.y - m_Radius;
                }
            }
        }
    }

    void RigidBody::Accelerate(float accel, float dt)
    {
        Vector2D directionVec(0.0f, 0.0f);
        
        if (m_pTransform)
        {
            // newVel = a * dt + currentVel
            Vector2DFromAngleDeg(directionVec, m_pTransform->m_Rotation);
            Vector2DScaleAdd(m_Velocity, directionVec, m_Velocity, accel * dt);
            // newVel *= 0.99
            Vector2DScale(m_Velocity, m_Velocity, 0.99f);
        }
    }

    void RigidBody::AccelerateLeft()
    {
        m_Velocity.x -= m_MovementVelocity;
        if (m_Velocity.x < -m_MovementVelocity)
        {
            m_Velocity.x = -m_MovementVelocity;
        }
    }

    void RigidBody::AccelerateRight()
    {
        m_Velocity.x += m_MovementVelocity;
        if (m_Velocity.x > m_MovementVelocity)
        {
            m_Velocity.x = m_MovementVelocity;
        }
    }

    void RigidBody::DecelerateHorizontal()
    {
        if (m_Velocity.x > 0)
            m_Velocity.x -= min(m_Velocity.x, m_MovementVelocity / 10.0f);
        else if (m_Velocity.x < 0)
            m_Velocity.x += min(-(m_Velocity.x), m_MovementVelocity / 10.0f);
    }

    void RigidBody::Jump()
    {
        m_Velocity.y = m_JumpVelocity;
    }

    void RigidBody::ApplyGravity(float dt)
    {
        if (m_Shape != SHAPE_LINE)
        {
            // Velocity Y = Gravity * Frame Time + Velocity Y
            m_Velocity.y += m_Gravity * dt;
            if (m_Velocity.y < -m_MaxVelocity)
                m_Velocity.y = -m_MaxVelocity;
        }
    }

    void RigidBody::UpdatePosition(float dt)
    {
        // Keep track of old position and velocity in case of collision
        m_PreviousPosition = m_pTransform->m_Position;
        m_PreviousVelocity = m_Velocity;

        // newPosition = velocity * dt + currentPosition
        Vector2DScaleAdd(m_pTransform->m_Position, m_Velocity, m_pTransform->m_Position, dt);
    }

    void RigidBody::BinaryMapCollision(float dt)
    {
        int gridCollisionFlag = CheckInstanceBinaryMapCollision(m_pTransform->m_Position.x, m_pTransform->m_Position.y, m_pTransform->m_Scale.x + EPSILON, m_pTransform->m_Scale.y + EPSILON);
        if (0 != (gridCollisionFlag & COLLISION_BOTTOM) ||
            0 != (gridCollisionFlag & COLLISION_TOP))
        {
            SnapToCell(&(m_pTransform->m_Position.y));
            m_Velocity.y = 0;
        }
        if (0 != (gridCollisionFlag & COLLISION_LEFT) ||
            0 != (gridCollisionFlag & COLLISION_RIGHT))
        {
            SnapToCell(&(m_pTransform->m_Position.x));
            m_Velocity.x = 0;
        }
    }

    int RigidBody::CollidesWith(RigidBody* body2)
    {
        if (m_Shape == SHAPE_CIRCLE)
        {
            if (body2->m_Shape == SHAPE_CIRCLE || body2->m_Shape == SHAPE_SHIP || body2->m_Shape == SHAPE_SPOON || body2->m_Shape == SHAPE_BOMB)
            {
                return StaticCircleToStaticCircle(m_pTransform->m_Position, m_Radius, body2->m_pTransform->m_Position, body2->m_Radius);
            }
            else if (body2->m_Shape == SHAPE_LINE)
            {
                return StaticCircleToStaticLineSegment(m_pTransform->m_Position, m_Radius, body2->m_LineSegment);
            }
        }
        else if (m_Shape == SHAPE_LINE)
        {
            if (body2->m_Shape == SHAPE_CIRCLE || body2->m_Shape == SHAPE_SHIP || body2->m_Shape == SHAPE_SPOON)
            {
                return StaticCircleToStaticLineSegment(body2->m_pTransform->m_Position, body2->m_Radius, m_LineSegment);
            }
        }
        else if (m_Shape == SHAPE_SPOON)
        {
            if (body2->m_Shape == SHAPE_CIRCLE)
            {
                return StaticCircleToStaticCircle(m_pTransform->m_Position, m_Radius, body2->m_pTransform->m_Position, body2->m_Radius);
            }
            else if (body2->m_Shape == SHAPE_LINE)
            {
                return StaticCircleToStaticLineSegment(m_pTransform->m_Position, m_Radius, body2->m_LineSegment);
            }
        }
        else if (m_Shape == SHAPE_SHIP)
        {
            if (body2->m_Shape == SHAPE_CIRCLE)
            {
                return StaticCircleToStaticCircle(m_pTransform->m_Position, m_Radius, body2->m_pTransform->m_Position, body2->m_Radius);
            }
            else if (body2->m_Shape == SHAPE_LINE)
            {
                return StaticCircleToStaticLineSegment(m_pTransform->m_Position, m_Radius, body2->m_LineSegment);
            }
        }
        return 0;
    }
}
