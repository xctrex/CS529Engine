#pragma once

#include "Component.h"
#include "Transform.h"

#include <SpriteBatch.h>

using namespace DirectX;

namespace Framework
{
    // Forward declaration of Transform class
    class Transform;

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
        virtual void OnEvent(Event* e){};
        virtual void Serialize(tinyxml2::XMLDocument* txmlDoc){};
    private:
        ComPtr<ID3D11ShaderResourceView> m_spSRV;
        ID3D11ShaderResourceView *m_pSRV;
        std::string m_TextureName;
        Transform* m_pTransform;
        float m_SpriteRotation;

        XMVECTOR m_Color;
        Vector2D m_Origin;
        float m_Layer;
    };
}
