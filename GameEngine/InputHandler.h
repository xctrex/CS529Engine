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
        virtual void OnEvent(Event* e){};
        virtual void Destroy(){};

        void Update(float dt);
        void CreateBullet();
    private:
        float m_AccelerationSpeed;
        float m_DeccelerationSpeed;
        float m_RotationSpeed;
        float m_BulletSpeed;
        Transform* m_pTransform;
        RigidBody* m_pRigidBody;
    };
}