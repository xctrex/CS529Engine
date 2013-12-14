#include "Sprite.h"
#include "Transform.h"
#include "GraphicsSystem.h"

namespace Framework
{
    Sprite::Sprite() :
        m_spSRV(NULL),
        m_pSRV(NULL),
        m_TextureName("Default"),
        m_Color(Colors::White),
        m_Origin({128.0f, 128.0f}),
        m_pTransform(NULL),
        m_Layer(0.0f),
        m_SpriteRotation(0.0f)
    {
        m_Type = COMPONENT_TYPE_SPRITE;
        

        g_ComponentHandleTable[this->GetHandleIndex()] = this;
    };
    Sprite::~Sprite() 
    {
        if (m_pSRV) // TODO: what if more than one sprite has the same SRV?
        {
            m_pSRV->Release();
        }

        // Only free the transform if it does not belong to the parent of the sprite
        Transform* pTransform = static_cast<Transform*>(m_Parent->GetComponent(COMPONENT_TYPE_TRANSFORM));
        if (pTransform == NULL)
        {
            if (m_pTransform)
            {
                free(m_pTransform);
            }
        }
    };

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
        if (txmlElement->Attribute("SpriteHeight"))
        {
            m_Origin.x = txmlElement->FloatAttribute("SpriteHeight") / 2.0f;
        }
        if (txmlElement->Attribute("SpriteWidth"))
        {
            m_Origin.y = txmlElement->FloatAttribute("SpriteWidth") / 2.0f;
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
            g_GRAPHICS->m_SpriteList.push_back(this);
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


    void Sprite::SetPosition(float x, float y)
    {
        m_pTransform->m_Position.x = x;
        //m_Position.x = x;
        m_pTransform->m_Position.y = y;
        //m_Position.y = y;
    }
}