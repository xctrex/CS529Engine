#include "Text.h"
#include "Transform.h"
#include "PhysicsSystem.h"

namespace Framework
{
    RigidBody::RigidBody() :
        m_pTransform(NULL),
        m_Velocity({0.0f, 0.0f}),
        m_Shape(SHAPE_CIRCLE),
        m_Radius(0.0f),
        m_Weight(FLT_MAX),
        m_PreviousPosition(0.0f, 0.0f),
        m_PreviousVelocity({0.0f, 0.0f})
    {
        m_Type = COMPONENT_TYPE_RIGID_BODY;

        m_LineSegment.m_N = { 0.0f, 1.0f };
        m_LineSegment.m_NdotP0 = 1.0f;
        m_LineSegment.m_P0 = { 0.0f, 0.0f };
        m_LineSegment.m_P1 = { 0.0f, 0.0f };

        g_ComponentHandleTable[this->GetHandleIndex()] = this;
    };

    RigidBody::~RigidBody()
    {
        // Only free the transform if it does not belong to the parent of the text
        Transform* pTransform = static_cast<Transform*>(m_Parent->GetComponent(COMPONENT_TYPE_TRANSFORM));
        if (pTransform == NULL)
        {
            if (m_pTransform)
            {
                free(m_pTransform);
            }
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
            else
            {
                ThrowErrorIf(true, "Shape not recognized");
            }
        }
        if (txmlElement->Attribute("Radius"))
        {
            m_Radius = txmlElement->FloatAttribute("Radius");
        }
        if (txmlElement->Attribute("Weight"))
        {
            m_Weight = txmlElement->FloatAttribute("Weight");
        }

        // Line Segment initialization
        tinyxml2::XMLElement* txmlLineSegmentElement = txmlElement->FirstChildElement("LineSegment");
        if (txmlLineSegmentElement)
        {
            ThrowErrorIf(
                (!txmlLineSegmentElement->Attribute("P0X")
                || !txmlLineSegmentElement->Attribute("P0Y")
                || !txmlLineSegmentElement->Attribute("P1X")
                || !txmlLineSegmentElement->Attribute("P1Y")),
               // || !txmlLineSegmentElement->Attribute("NX")
                //|| !txmlLineSegmentElement->Attribute("NY")),
                "Line Segment not fully defined in the xml."
                );
            m_LineSegment.m_P0.x = txmlLineSegmentElement->FloatAttribute("P0X");
            m_LineSegment.m_P0.y = txmlLineSegmentElement->FloatAttribute("P0Y");
            m_LineSegment.m_P1.x = txmlLineSegmentElement->FloatAttribute("P1X");
            m_LineSegment.m_P1.y = txmlLineSegmentElement->FloatAttribute("P1Y");
            //m_LineSegment.m_N.x = txmlLineSegmentElement->FloatAttribute("NX");
            //m_LineSegment.m_N.y = txmlLineSegmentElement->FloatAttribute("NY");
            //m_LineSegment.m_NdotP0 = Vector2DDotProduct(m_LineSegment.m_N, m_LineSegment.m_P0);
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
        if (e->m_Type == EVENT_TYPE_COLLISION)
        {
            RigidBody* collider = static_cast<CollisionEvent*>(e)->m_pCollidedWith;
            float dt = static_cast<CollisionEvent*>(e)->m_FrameTime;
            // Whichever object is "lighter" will bounce, and the heavier object will be unaffected by the collision
            // This is not physically accurate, but allows us to establish a hierarchy of which objects bounce
            // and which ones don't (e.g., ship bounces off of both asteroids and walls, asteroids only bounce off walls,
            // and walls are unimpacted by collisions) If objects are equally weighted, both will bounce

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
                if (this->m_Shape == SHAPE_CIRCLE && collider->m_Shape == SHAPE_CIRCLE)
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
                else if (this->m_Shape == SHAPE_CIRCLE && collider->m_Shape == SHAPE_LINE)
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
                else if (this->m_Shape == SHAPE_LINE && collider->m_Shape == SHAPE_CIRCLE)
                {
                    // Ignore, only doing static lines for now
                    return;
                }

                // Get center position at time of impact
                Vector2D CenterAtImpact = this->m_PreviousPosition;
                CenterAtImpact.x += (this->m_PreviousPosition.x - thisNewPosition.x) * ti;
                CenterAtImpact.y += (this->m_PreviousPosition.y - thisNewPosition.y) * ti;
                // Update the position of this
                Vector2DAdd(this->m_pTransform->m_Position, CenterAtImpact, R);

                Vector2DNormalize(R, R);
                Vector2DScale(this->m_Velocity, R, 
                    (Vector2DLength(this->m_Velocity) + Vector2DLength(collider->m_Velocity)) * 0.5f
                    );
            }
        }
    }

    void RigidBody::Accelerate(float accel, float dt)
    {
        Vector2D directionVec = { 0.0f, 0.0f };
        
        // newVel = a * dt + currentVel
        Vector2DFromAngleDeg(directionVec, m_pTransform->m_Rotation);
        Vector2DScaleAdd(m_Velocity, directionVec, m_Velocity, accel * dt);
        // newVel *= 0.99
        Vector2DScale(m_Velocity, m_Velocity, 0.99f);
    }

    void RigidBody::UpdatePosition(float dt)
    {
        // Keep track of old position and velocity in case of collision
        m_PreviousPosition = m_pTransform->m_Position;
        m_PreviousVelocity = m_Velocity;

        // newPosition = velocity * dt + currentPosition
        Vector2DScaleAdd(m_pTransform->m_Position, m_Velocity, m_pTransform->m_Position, dt);
    }

    int RigidBody::CollidesWith(RigidBody* body2)
    {
        if (m_Shape == SHAPE_CIRCLE)
        {
            if (body2->m_Shape == SHAPE_CIRCLE)
            {
                return StaticCircleToStaticCircle(m_pTransform->m_Position, m_Radius, body2->m_pTransform->m_Position, body2->m_Radius);
            }
            else if (body2->m_Shape == SHAPE_SQUARE)
            {
                // TODO: Don't ignore circle to square collision
            }
            else if (body2->m_Shape == SHAPE_LINE)
            {
                return StaticCircleToStaticLineSegment(m_pTransform->m_Position, m_Radius, body2->m_LineSegment);
            }
        }
        else if (m_Shape == SHAPE_SQUARE)
        {
            // TODO: don't ignore square to x collision
        }
        else if (m_Shape == SHAPE_LINE)
        {
            if (body2->m_Shape == SHAPE_CIRCLE)
            {
                return StaticCircleToStaticLineSegment(body2->m_pTransform->m_Position, body2->m_Radius, m_LineSegment);
            }
        }
        return 0;
    }
}
