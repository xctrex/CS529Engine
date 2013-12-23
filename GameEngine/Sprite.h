/* Start Header -------------------------------------------------------
Copyright (C) 2013 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Sprite.h
Purpose: Header file for sprite component class
Language: C++
Platform: Windows
Project: CS529_twalton_FinalProject
Author: Tommy Walton, t.walton, 130000812
Creation date: 12/15/2013
- End Header -----------------------------------------------------*/
#pragma once

#include "Component.h"
#include "Transform.h"
#include <d2d1_1.h>
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
        void DrawDebug(ComPtr<ID2D1DeviceContext> &spDeviceContext, ComPtr<ID2D1SolidColorBrush> &spSolidColorBrush);
        void SetPosition(float x, float y);

        // TODO: implement the rest of the pure virtual functions
        virtual void OnEvent(Event* e){};
        virtual void Serialize(tinyxml2::XMLDocument* txmlDoc){};
        virtual void Destroy();
    private:
        ComPtr<ID3D11ShaderResourceView> m_spSRV;
        ID3D11ShaderResourceView *m_pSRV;
        std::string m_TextureName;
        Transform* m_pTransform;
        float m_SpriteRotation;

        XMVECTOR m_Color;
        Vector2D m_Origin;
        float m_Width;
        float m_Height;
        float m_Layer;
    };
}
