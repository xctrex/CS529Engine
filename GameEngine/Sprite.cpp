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
        if (!m_pTransform)
        {
            // Get the transform from the parent, otherwise create a new one
            m_pTransform = static_cast<Transform*>(m_Parent->GetComponent(COMPONENT_TYPE_TRANSFORM));
            if (!m_pTransform)
                m_pTransform = new Transform();
        }

        if (txmlElement->Attribute("Archetype"))
        {
            //TODO: add code for parsing Archetype XML
            // Should use a recursive function, something like InitializeArchetype,
            // That can recursively keep looking for more archetypes
            tinyxml2::XMLDocument txmlDoc;
            ThrowErrorIf(
                tinyxml2::XML_SUCCESS != txmlDoc.LoadFile("Assets\\Archetypes.xml"),
                "Failed to load Assets\\Archetypes.xml"
                );

            tinyxml2::XMLElement* txmlRecursiveElement = txmlDoc.FirstChildElement();
            while (txmlRecursiveElement)
            {
                if (strcmp(txmlElement->Attribute("Archetype"), txmlRecursiveElement->Name()) == 0)
                {
                    ++m_RecursionLevel;
                    this->Initialize(txmlRecursiveElement);
                    break;
                }                
                txmlRecursiveElement = txmlRecursiveElement->NextSiblingElement();
            }
        }
        if (txmlElement->Attribute("Name"))
        {
            m_Name = txmlElement->Attribute("Name");
        }
        if (txmlElement->Attribute("TextureName"))
        {
            m_TextureName = txmlElement->Attribute("TextureName");
        }
        if (txmlElement->Attribute("PositionX"))
        {
            m_pTransform->m_Position.x = txmlElement->FloatAttribute("PositionX");
            //m_Position.x = txmlElement->FloatAttribute("PositionX");
        }
        if (txmlElement->Attribute("PositionY"))
        {
            m_pTransform->m_Position.y = txmlElement->FloatAttribute("PositionY");
            //m_Position.y = txmlElement->FloatAttribute("PositionY");
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

        if (txmlElement->Attribute("Rotation"))
        {
            m_pTransform->m_Rotation = txmlElement->FloatAttribute("Rotation");
        }
        if (txmlElement->Attribute("ScaleX"))
        {
            m_pTransform->m_Scale.x = txmlElement->FloatAttribute("ScaleX");
        }
        if (txmlElement->Attribute("ScaleY"))
        {
            m_pTransform->m_Scale.y = txmlElement->FloatAttribute("ScaleY");
        }
        
        if (txmlElement->Attribute("Layer"))
        {
            m_Layer = txmlElement->FloatAttribute("Layer");
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
        srcRect.left = 0.0f;
        srcRect.top = 0.0f;
        srcRect.right = m_Origin.x * 2;
        srcRect.bottom = m_Origin.y * 2;
        //m_Color = Colors::Black;
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