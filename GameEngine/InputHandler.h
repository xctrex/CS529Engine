/* Start Header -------------------------------------------------------
Copyright (C) 2013 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: InputHandler.h
Purpose: Header file for input handler component
Language: C++
Platform: Windows
Project: CS529_twalton_FinalProject
Author: Tommy Walton, t.walton, 130000812
Creation date: 12/15/2013
- End Header -----------------------------------------------------*/
#pragma once

#include "Component.h"
#include "Transform.h"
#include "RigidBody.h"

namespace Framework
{
    class InputHandler : public Component
    {
    public:
        InputHandler();
        ~InputHandler() {};
        virtual void Initialize(tinyxml2::XMLElement* txmlElement);
        virtual void Serialize(tinyxml2::XMLDocument* txmlDoc){};
        virtual void OnEvent(Event* e);
        virtual void Destroy(){};

        void Update(float dt);
        void CreateBullet();
        void CreateBomb(Vector2D &position);
    private:
        float m_AccelerationSpeed;
        float m_DeccelerationSpeed;
        float m_RotationSpeed;
        float m_BulletSpeed;
        ComponentHandle m_hTransform;
        RigidBody* m_pRigidBody;
        int m_BombsLeft;
    };
}