/* Start Header -------------------------------------------------------
Copyright (C) 2013 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Text.h
Purpose: Header file for text component class
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
#include <dwrite_1.h>

using namespace DirectX;

namespace Framework
{
    // Forward declaration of Transform class
    class Transform;

    // Text class has a destination rect, font, content, and transform
    class Text : public Component
    {
    public:
        Text();
        ~Text();
        virtual void Initialize(tinyxml2::XMLElement *txmlElement);
		void Draw(ComPtr<ID2D1DeviceContext> sp_DeviceContext, ComPtr<ID2D1SolidColorBrush> sp_Brush, ComPtr<IDWriteFactory1> sp_DWriteFactory);
        void SetPosition(float x, float y);

        // TODO: implement the rest of the pure virtual functions
        virtual void OnEvent(Event* e){};
        virtual void Serialize(tinyxml2::XMLDocument* txmlDoc){};
        virtual void Destroy(){};
    private:
        void CreateFontFaceFromFontFile(ComPtr<IDWriteFactory1> &sp_DWriteFactory, ComPtr<IDWriteFontFace> &sp_FontFace);

        std::string m_TextContent;
        std::string m_Font;
        Transform* m_pTransform;

        D2D1_RECT_F m_Rect;
    };
}