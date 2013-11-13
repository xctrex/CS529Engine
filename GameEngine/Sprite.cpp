#include "Sprite.h"
#include "GraphicsSystem.h"

namespace Framework
{
    Sprite::Sprite() : 
        m_spSRV(NULL),
        m_pSRV(NULL),
        m_TextureName("Assets\\Default.png"),
        m_Position({0.0f, 0.0f}),
        m_Color(Colors::White),
        m_Rotation(0.0f),
        m_Origin({0.0f, 0.0f}),
        m_Scale({ 1.0f, 1.0f })
    {};
    Sprite::~Sprite() 
    {
        if (m_pSRV)
        {
            m_pSRV->Release();
        }
    };

    void Sprite::Initialize()
    {
        m_TextureName = "Default";
        m_pSRV = GRAPHICS->GetTexture(m_TextureName);
        GRAPHICS->m_SpriteList.push_back(*this);
    }

    void Sprite::Draw(std::unique_ptr<SpriteBatch> &spSpriteBatch)
    {
        spSpriteBatch->Draw(m_pSRV, XMFLOAT2(0, 0));
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
}