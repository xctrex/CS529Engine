#pragma once

#include "Component.h"

enum GAME_STATE
{
    GAME_STATE_
};

namespace Framework
{
    class GameStateManager : public Component
    {
    public:
        GameStateManager();
        ~GameStateManager() {};
        virtual void Initialize(tinyxml2::XMLElement* txmlElement);
        virtual void Serialize(tinyxml2::XMLDocument* txmlDoc){};
        virtual void OnEvent(Event* e);
        virtual void Destroy(){};
        void Update(float dt);
    private:
        int m_CurrentState;
        int m_NumAsteroids;
    };
}
