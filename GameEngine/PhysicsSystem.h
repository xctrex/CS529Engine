/* Start Header -------------------------------------------------------
Copyright (C) 2013 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: PhysicsSystem.h
Purpose: Header file for physics system
Language: C++
Platform: Windows
Project: CS529_twalton_FinalProject
Author: Tommy Walton, t.walton, 130000812
Creation date: 12/8/2013
- End Header -----------------------------------------------------*/
#pragma once

#include "Engine.h"
#include "Containers.h"

#include "tinyXML2\tinyxml2.h"

namespace Framework
{
    // Graphics manager
    class PhysicsSystem : public ISystem
    {
    public:
        PhysicsSystem();
        ~PhysicsSystem();

        // ISystem class virtual functions
        virtual void Initialize();
        virtual void Update(float dt);
        virtual std::string GetName() { return "Physics"; }
        int GetCellValue(int X, int Y);

        std::list<RigidBody*> m_RigidBodyList;
        
    
    private:

        void HandleCollisions(float dt);
        int ImportMapDataFromFile(char *FileName);
        void FreeMapData();
        int m_BinaryMapWidth;
        int m_BinaryMapHeight;
        int** MapData;//TODO: Initialize MapData and BinaryCollisionArray
        int** BinaryCollisionArray;

    };

    // Global pointer to Graphics.
    extern PhysicsSystem* g_PHYSICS;
}
