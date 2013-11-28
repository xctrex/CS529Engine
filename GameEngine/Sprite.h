#pragma once

#include "WindowsIncludes.h"
#include <d3d11.h>
#include <SpriteBatch.h>
#include "Component.h"
#include "GameObject.h"

using namespace DirectX;

namespace Framework
{
    // Forward declaration for GameObjectHandle
    class GameObjectHandle;

    // Sprite class has a texture, size and transform
    class Sprite : public Component
    {
    public:
        Sprite();
        ~Sprite();
        virtual void Initialize(tinyxml2::XMLElement *txmlElement);
        void Draw(std::unique_ptr<SpriteBatch> &spSpriteBatch);
        void SetPosition(float x, float y);

        // TODO: implement the rest of the pure virtual functions
        //virtual void SendEvent(){};
        //virtual void OnEvent(){};
        virtual void Serialize(tinyxml2::XMLDocument *txmlDoc){};
    private:
        ComPtr<ID3D11ShaderResourceView> m_spSRV;
        ID3D11ShaderResourceView *m_pSRV;
        std::string m_TextureName;
        XMFLOAT2 m_Position;
        XMVECTOR m_Color;
        float m_Rotation;
        XMFLOAT2 m_Origin;
        XMFLOAT2 m_Scale;

//        GameObjectHandle m_Parent;
    };
}