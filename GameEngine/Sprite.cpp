#include "Sprite.h"
#include "GraphicsSystem.h"

namespace Framework
{
    Sprite::Sprite() : 
        m_spSRV(NULL),
        m_pSRV(NULL),
        m_TextureName("Default"),
        m_Position({0.0f, 0.0f}),
        m_Color(Colors::White),
        m_Rotation(0.0f),
        m_Origin({0.0f, 0.0f}),
        m_Scale({ 1.0f, 1.0f })
    {};
    Sprite::~Sprite() 
    {
        if (m_pSRV) // TODO: what if more than one sprite has the same SRV?
        {
            m_pSRV->Release();
        }
    };

    void Sprite::Initialize(tinyxml2::XMLElement *txmlElement)
    {
        if (txmlElement->Attribute("Archetype"))
        {
            //TODO: add code for parsing Archetype XML
            // Should use a recursive function, something like InitializeArchetype,
            // That can recursively keep looking for more archetypes
        }
        if (txmlElement->Attribute("TextureName"))
        {
            m_TextureName = txmlElement->Attribute("TextureName");
        }
        if (txmlElement->Attribute("PositionX"))
        {
            m_Position.x = txmlElement->FloatAttribute("PositionX");
        }
        if (txmlElement->Attribute("PositionY"))
        {
            m_Position.y = txmlElement->FloatAttribute("PositionY");
        }
        //TODO: add initialization for m_Color
        if (txmlElement->Attribute("Rotation"))
        {
            m_Rotation = txmlElement->FloatAttribute("Rotation");
        }
        //TODO: add initialization for origin and scale

        m_pSRV = GRAPHICS->GetTexture(m_TextureName);
        GRAPHICS->m_SpriteList.push_back(*this);
    }

    void Sprite::Draw(std::unique_ptr<SpriteBatch> &spSpriteBatch)
    {
        //spSpriteBatch->Draw(m_pSRV, XMFLOAT2(0, 0));
        spSpriteBatch->Draw(
            m_pSRV,
            m_Position,
            NULL,
            m_Color,
            m_Rotation,
            m_Origin,
            m_Scale,
            SpriteEffects::SpriteEffects_None,
            0.0f
            );
    }


    void Sprite::SetPosition(float x, float y)
    {
        m_Position.x = x;
        m_Position.y = y;
    }
}