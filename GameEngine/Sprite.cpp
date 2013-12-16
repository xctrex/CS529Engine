/* Start Header -------------------------------------------------------
Copyright (C) 2013 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Sprite.cpp
Purpose: Implementation file for sprite component
Language: C++
Platform: Windows
Project: CS529_twalton_FinalProject
Author: Tommy Walton, t.walton, 130000812
Creation date: 12/15/2013
- End Header -----------------------------------------------------*/
#include "Sprite.h"
#include "Transform.h"
#include "GraphicsSystem.h"
#include "WindowsSystem.h"

namespace Framework
{
    Sprite::Sprite() :
        m_spSRV(NULL),
        m_pSRV(NULL),
        m_TextureName("Default"),
        m_Color(Colors::White),
        m_Origin(128.0f, 128.0f),
        m_pTransform(NULL),
        m_Layer(0.0f),
        m_SpriteRotation(0.0f)
    {
        m_Type = COMPONENT_TYPE_SPRITE;        
     
        g_ComponentHandleTable[this->GetHandleIndex()] = this;
    };
    Sprite::~Sprite() 
    {
        
    };
    void Sprite::Destroy()
    {
        g_GRAPHICS->m_SpriteList.remove(ComponentHandle(*this));
    }

    void Sprite::Initialize(tinyxml2::XMLElement *txmlElement)
    {

        CommonComponentInitialization(txmlElement);

        //================================================================
        // Sprite specific initialization
        //================================================================
        if (txmlElement->Attribute("TextureName"))
        {
            m_TextureName = txmlElement->Attribute("TextureName");
        }
        //TODO: add initialization for m_Color
        if (txmlElement->Attribute("SpriteRotation"))
        {
            m_SpriteRotation = txmlElement->FloatAttribute("SpriteRotation");
        }
        if (txmlElement->Attribute("SpriteWidth"))
        {
            m_Origin.x = txmlElement->FloatAttribute("SpriteWidth") / 2.0f;
        }
        if (txmlElement->Attribute("SpriteHeight"))
        {
            m_Origin.y = txmlElement->FloatAttribute("SpriteHeight") / 2.0f;
        }
        if (txmlElement->Attribute("Layer"))
        {
            m_Layer = txmlElement->FloatAttribute("Layer");
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
            ThrowErrorIf(!m_pTransform, "Parent of a sprite component must have a transform");
        }
        if (txmlElement->Attribute("ScaleX"))
        {
            m_pTransform->m_Scale.x = txmlElement->FloatAttribute("ScaleX");
        }
        if (txmlElement->Attribute("ScaleY"))
        {
            m_pTransform->m_Scale.y = txmlElement->FloatAttribute("ScaleY");
        }

               
        if (m_RecursionLevel == 0)
        {
            m_pSRV = g_GRAPHICS->GetTexture(m_TextureName);
            ThrowErrorIf(!m_pSRV, "Failed to get texture from GRAPHICS");
            g_GRAPHICS->m_SpriteList.push_back(ComponentHandle(*this));
        }
        else
        {
            --m_RecursionLevel;
        }
    }

	void Sprite::Draw(std::unique_ptr<SpriteBatch> &spSpriteBatch)
    {
        //spSpriteBatch->Draw(m_pSRV, XMFLOAT2(0, 0));
        RECT srcRect;
        srcRect.left = 0;
        srcRect.top = 0;
        srcRect.right = (LONG)m_Origin.x * 2;
        srcRect.bottom = (LONG)m_Origin.y * 2;

        spSpriteBatch->Draw(
            m_pSRV,
            g_GRAPHICS->WorldCoordsToWindowCoords(m_pTransform->m_Position),
            NULL,
            m_Color,
            -1.0f * DegreesToRadians(m_pTransform->m_Rotation + m_SpriteRotation),
            m_Origin,
            m_pTransform->m_Scale,
            SpriteEffects::SpriteEffects_None,
            m_Layer
            );        
    }

    void Sprite::DrawDebug(ComPtr<ID2D1DeviceContext> &spD2DDeviceContext, ComPtr<ID2D1SolidColorBrush> &spSolidColorBrush)
    {
        RigidBody* pBody = static_cast<RigidBody*>(m_Parent->GetComponent(COMPONENT_TYPE_RIGID_BODY));
        if (pBody && m_pTransform)
        {
            if (pBody->GetShape() == SHAPE_CIRCLE || pBody->GetShape() == SHAPE_SHIP || pBody->GetShape() == SHAPE_SPOON)
            {
                // Draw a circle around the object
                spD2DDeviceContext->DrawEllipse(
                    D2D1::Ellipse(
                        D2D1::Point2F(
                            g_GRAPHICS->WorldCoordsToWindowCoords(m_pTransform->m_Position).x,
                            g_GRAPHICS->WorldCoordsToWindowCoords(m_pTransform->m_Position).y
                        ),
                        pBody->GetRadius(),
                        pBody->GetRadius()
                    ),
                    spSolidColorBrush.Get(),
                    3.0f
                    );

				/*
                ID3D11ShaderResourceView* pSRV = g_GRAPHICS->GetTexture("Circle");
                ThrowErrorIf(!pSRV, "Failed to get circle texture from GRAPHICS");
                Vector2D origin(128.0f, 128.0f);
                float scale = pBody->GetRadius() / 128.0f;
                spSpriteBatch->Draw(
                    pSRV,
                    g_GRAPHICS->WorldCoordsToWindowCoords(m_pTransform->m_Position),
                    NULL,
                    m_Color,
                    0.0f,
                    origin,
                    m_pTransform->m_Scale,
                    SpriteEffects::SpriteEffects_None,
                    m_Layer
                    );*/
            }
        }
    }


    void Sprite::SetPosition(float x, float y)
    {
        m_pTransform->m_Position.x = x;
        //m_Position.x = x;
        m_pTransform->m_Position.y = y;
        //m_Position.y = y;
    }
}