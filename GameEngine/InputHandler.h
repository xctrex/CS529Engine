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

        void Update(float dt);
    private:
        float m_AccelerationSpeed;
        float m_DeccelerationSpeed;
        float m_RotationSpeed;
        Transform* m_pTransform;
        RigidBody* m_pRigidBody;
    };
}