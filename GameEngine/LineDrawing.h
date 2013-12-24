/* Start Header -------------------------------------------------------
Copyright (C) 2013 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: LineDrawing.h
Purpose: Header file for graphical line drawing component class
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

using namespace DirectX;

namespace Framework
{
    // Text class has a destination rect, font, content, and transform
    class LineDrawing : public Component
    {
    public:
        LineDrawing();
        ~LineDrawing(){};
        virtual void Initialize(tinyxml2::XMLElement *txmlElement);
        void Draw(ComPtr<ID2D1DeviceContext> sp_DeviceContext, ComPtr<ID2D1SolidColorBrush> sp_Brush);

        // TODO: implement the rest of the pure virtual functions
        virtual void OnEvent(Event* e){};
        virtual void Serialize(tinyxml2::XMLDocument* txmlDoc){};
        virtual void Destroy();
    private:
        ComponentHandle m_hRigidBody;
    };
}
