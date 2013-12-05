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
        m_RecursionLevel(0),
        m_Layer(0.0f)
    {
        m_Type = COMPONENT_TYPE_SPRITE;
        
        m_Position = { 0.0f, 0.0f };
        m_Rotation = 0.0f;
        m_Scale = { 1.0f, 1.0f };

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
        if (txmlElement->Attribute("Rotation"))
        {
            m_pTransform->m_Rotation = txmlElement->FloatAttribute("Rotation");
            //m_Rotation = txmlElement->FloatAttribute("Rotation");
        }
        if (txmlElement->Attribute("ScaleX"))
        {
            m_pTransform->m_Scale.x = txmlElement->FloatAttribute("ScaleX");
            //m_Scale.x = txmlElement->FloatAttribute("ScaleX");
        }
        if (txmlElement->Attribute("ScaleY"))
        {
            m_pTransform->m_Scale.y = txmlElement->FloatAttribute("ScaleY");
            //m_Scale.y = txmlElement->FloatAttribute("ScaleY");
        }
        if (txmlElement->Attribute("Layer"))
        {
            m_Layer = txmlElement->FloatAttribute("Layer");
        }
        //TODO: add initialization for origin and scale
        if (txmlElement->Attribute("Parent"))
        {
            /* At this point, I have a handle to the parent, but no way to give the sprite or
            component class a handle to their parents.

            GameObjectHandle c;
            c.Initialize(
                GetGameObjectHandleIndexFromUniqueID(txmlElement->UnsignedAttribute("Parent")),
                txmlElement->UnsignedAttribute("Parent")
                );

            SetParent(c);
            */
        }
        
        if (m_RecursionLevel == 0)
        {
            m_pSRV = g_GRAPHICS->GetTexture(m_TextureName);
            ThrowErrorIf(!m_pSRV, "Failed to get texture from GRAPHICS");
            g_GRAPHICS->m_SpriteList.push_back(*this);
        }
        else
        {
            --m_RecursionLevel;
        }
    }

    void Sprite::Draw(std::unique_ptr<SpriteBatch> &spSpriteBatch)
    {
        //spSpriteBatch->Draw(m_pSRV, XMFLOAT2(0, 0));
            spSpriteBatch->Draw(
            m_pSRV,
            m_pTransform->m_Position,
            //m_Position,
            NULL,
            m_Color,
            m_pTransform->m_Rotation,
            //m_Rotation,
            m_Origin,
            m_pTransform->m_Scale,
            //m_Scale,
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