#pragma once

#include "WindowsIncludes.h"
#include <d3d11.h>
#include <SpriteBatch.h>

using namespace DirectX;

namespace Framework
{
    // Sprite class has a texture, size and transform
    class Sprite
    {
    public:
        Sprite();
        ~Sprite();
        void Initialize();
        void Draw(std::unique_ptr<SpriteBatch> &spSpriteBatch);
    private:
        ComPtr<ID3D11ShaderResourceView> m_spSRV;
        ID3D11ShaderResourceView *m_pSRV;
        std::string m_TextureName;
        XMFLOAT2 m_Position;
        XMVECTOR m_Color;
        float m_Rotation;
        XMFLOAT2 m_Origin;
        XMFLOAT2 m_Scale;
    };
}