/* Start Header -------------------------------------------------------
Copyright (C) 2013 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: PhysicsSystem.cpp
Purpose: Implementation file for physics system
Language: C++
Platform: Windows
Project: CS529_twalton_FinalProject
Author: Tommy Walton, t.walton, 130000812
Creation date: 12/15/2013
- End Header -----------------------------------------------------*/
#include "PhysicsSystem.h"

namespace Framework
{
    // Global pointer to the game logic
    PhysicsSystem* g_PHYSICS = NULL;

    PhysicsSystem::PhysicsSystem() :
        m_BinaryMapHeight(0),
        m_BinaryMapWidth(0)
    {
        //Set up the global pointer
        ThrowErrorIf(g_PHYSICS != NULL, "Physics already initialized");
        g_PHYSICS = this;
    }

    PhysicsSystem::~PhysicsSystem()
    {
        FreeMapData();
    }

    void PhysicsSystem::Initialize()
    {
        ImportMapDataFromFile("Assets\\PlatformLayoutLevel0.txt");
    }

    int PhysicsSystem::ImportMapDataFromFile(char *FileName)
    {
        FILE * pFile;
        fopen_s(&pFile, FileName, "r");
        if (pFile != NULL)
        {
            int x, y;

            char tempstr[8];

            // Get the characters in the word Width and the following space
            for (x = 0; x < 6; ++x)
            {
                fgetc(pFile);
            }
            // Get the width
            //BINARY_MAP_WIDTH = fgetc(pFile) - '0';
            fgets(tempstr, 8, pFile);
            // Convert the string to an int
            m_BinaryMapWidth = cstrtoi(tempstr, 8);

            // Get the characters in the word Height and the following space
            for (x = 0; x < 7; ++x)
            {
                fgetc(pFile);
            }

            fgets(tempstr, 8, pFile);
            m_BinaryMapHeight = cstrtoi(tempstr, 8);

            // Allocate memory for MapData
            MapData = (int**)malloc(m_BinaryMapWidth * sizeof(int*));
            BinaryCollisionArray = (int**)malloc(m_BinaryMapWidth * sizeof(int*));
            if (MapData == NULL || BinaryCollisionArray == NULL) return 0;
            for (x = 0; x < m_BinaryMapWidth; ++x)
            {
                MapData[x] = (int*)malloc(m_BinaryMapHeight * sizeof(int));
                BinaryCollisionArray[x] = (int*)malloc(m_BinaryMapHeight * sizeof(int));
                if (MapData[x] == NULL || BinaryCollisionArray == NULL) return 0;
            }

            for (y = m_BinaryMapHeight - 1; y >= 0; --y)
            {
                for (x = 0; x < m_BinaryMapHeight; ++x)
                {
                    // Get the map data
                    int c = fgetc(pFile) - '0';
                    MapData[x][y] = c;
                    // Get the following space
                    fgetc(pFile);

                    // Set the binary collision array to 0 or 1
                    if (MapData[x][y] == 1) BinaryCollisionArray[x][y] = 1;
                    else BinaryCollisionArray[x][y] = 0;
                }
                // Get the newline
                fgetc(pFile);
            }

            fclose(pFile);
            return 1;
        }
        else
        {
            return 0;
        }
    }

    void PhysicsSystem::FreeMapData(void)
    {
        int x;
        for (x = 0; x < m_BinaryMapWidth; ++x)
        {
            free(MapData[x]);
            free(BinaryCollisionArray[x]);
        }
        free(MapData);
        free(BinaryCollisionArray);
    }

    void PhysicsSystem::Update(float dt)
    {
        std::list<RigidBody*>::iterator it = m_RigidBodyList.begin();
        for (; it != m_RigidBodyList.end(); ++it)
        {
            (*it)->ApplyGravity(dt);
            (*it)->UpdatePosition(dt);
        }
        // Update the input handler
        HandleCollisions(dt);
    }


    int PhysicsSystem::GetCellValue(int X, int Y)
    {
        int out;
        if (X < 0 || Y < 0 || X >= m_BinaryMapWidth || Y >= m_BinaryMapHeight) return 0;
        out = BinaryCollisionArray[X][Y];
        return out;
    }

    void PhysicsSystem::HandleCollisions(float dt)
    {
        std::list<RigidBody*>::iterator it1 = m_RigidBodyList.begin();
        for (; it1 != m_RigidBodyList.end(); ++it1)
        {
            std::list<RigidBody*>::iterator it2 = it1;
            ++it2;
            for (; it2 != m_RigidBodyList.end(); ++it2)
            {
                // Detect a collision between it1 and it2
                if ((*it1)->CollidesWith(*it2))
                {
                    // If there is a collision, create collision events and send them to the parents
                    CollisionEvent CollidedWithIt2(*it2, dt);
                    CollisionEvent CollidedWithIt1(*it1, dt);

                    // it1 collided with it2
                    (*it1)->m_Parent->OnEvent(&CollidedWithIt2);
                    // it2 collided with it1
                    (*it2)->m_Parent->OnEvent(&CollidedWithIt1);                    
                }
            }
        }
    }
}
