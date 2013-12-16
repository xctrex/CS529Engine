/* Start Header -------------------------------------------------------
Copyright (C) 2013 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: WinMain.cpp
Purpose: This is the main entry point for the game engine final project
Language: C++
Platform: Windows
Project: CS529_twalton_FinalProject
Author: Tommy Walton, t.walton, 130000812
Creation date: 10/1/2013
- End Header -----------------------------------------------------*/

#include "Precomp.h"
#include "Engine.h"
#include "WindowsSystem.h"
#include "GraphicsSystem.h"
#include "GameLogicSystem.h"
#include "PhysicsSystem.h"

using namespace Framework;

// TODO: get computer's resolution
const int ClientWidth = 1280;
const int ClientHeight = 720;

void GetLevelList(std::vector<std::string> &LevelList);
void LoadLevel(tinyxml2::XMLDocument* txmlDoc);

void GetLevelList(std::vector<std::string> &LevelList)
{
    // Open up the list of levels
    tinyxml2::XMLDocument txmlDoc;

    ThrowErrorIf(
        tinyxml2::XML_SUCCESS != txmlDoc.LoadFile("Assets\\LevelList.xml"),
        "Failed to load Assets\\LevelList.xml"
        );
    
    tinyxml2::XMLElement* txmlElement = txmlDoc.FirstChildElement();
    txmlElement = txmlElement->FirstChildElement("Level");
    // Add each level to the list
    while (txmlElement)
    {
        LevelList.push_back(txmlElement->Attribute("Path"));
        txmlElement = txmlElement->NextSiblingElement();
    }
}

void LoadLevel(tinyxml2::XMLDocument* txmlDoc)
{
    tinyxml2::XMLElement* txmlElement = txmlDoc->FirstChildElement();
    while (txmlElement)
    {
        if (strcmp(txmlElement->Name(),"Textures") == 0)
        {
            g_GRAPHICS->LoadTextures(txmlElement);
        }
        if (strcmp(txmlElement->Name(), "Screens") == 0)
        {
            // Create and initialize the sprite object
            g_GRAPHICS->m_pWinSprite = new GameObject();
            g_GRAPHICS->m_pWinSprite->Initialize(txmlElement->FirstChildElement("WinScreenSprite"));
            // Remove the sprite from the typical rendering list
            g_GRAPHICS->m_SpriteList.remove(
                static_cast<Sprite*>(
                    g_GRAPHICS->m_pWinSprite->GetComponent(COMPONENT_TYPE_SPRITE)
                    )
                );

            // Create and initialize the sprite object
            g_GRAPHICS->m_pLoseSprite = new GameObject();
            g_GRAPHICS->m_pLoseSprite->Initialize(txmlElement->FirstChildElement("LoseScreenSprite"));
            // Remove the sprite from the typical rendering list
            g_GRAPHICS->m_SpriteList.remove(
                static_cast<Sprite*>(
                g_GRAPHICS->m_pLoseSprite->GetComponent(COMPONENT_TYPE_SPRITE)
                )
                );

            // Create and initialize the sprite object
            g_GRAPHICS->m_pControlsSprite = new GameObject();
            g_GRAPHICS->m_pControlsSprite->Initialize(txmlElement->FirstChildElement("ControlsScreenSprite"));
            // Remove the sprite from the typical rendering list
            g_GRAPHICS->m_SpriteList.remove(
                static_cast<Sprite*>(
                g_GRAPHICS->m_pControlsSprite->GetComponent(COMPONENT_TYPE_SPRITE)
                )
                );
        }
        else if (strcmp(txmlElement->Name(), "GameObject") == 0)
        {
            GameObject* pObj = new GameObject();
            pObj->Initialize(txmlElement);
        }
        txmlElement = txmlElement->NextSiblingElement();
    }
}

// Application entry point
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    //
    // Load (One time only setup goes here)
    //

    // Create the core engine
    CoreEngine* engine = new CoreEngine();

    // Create the windows system
    WindowsSystem* windows = new WindowsSystem(ClientWidth, ClientHeight);
    // Activate the window
    windows->ActivateWindow();

	// Create the graphics system
	GraphicsSystem* graphics = new GraphicsSystem(windows->m_hWnd, ClientWidth, ClientHeight);
    graphics->LoadResources();

    // Create the logic system
    GameLogicSystem* logic = new GameLogicSystem();

    // Create the physics system
    PhysicsSystem* physics = new PhysicsSystem();

    // Add the systems
    engine->AddSystem(windows);
	engine->AddSystem(graphics);
    engine->AddSystem(logic);
    engine->AddSystem(physics);


    // Get a list of all the levels
    std::vector<std::string> LevelList;
    GetLevelList(LevelList);
    
    

    for(size_t level = 0; level < LevelList.size(); ++level)
    {
        //
        // Initialize
        //

        // Initialize the engine and it's systems
        engine->InitializeSystems();

        tinyxml2::XMLDocument txmlDoc;

        ThrowErrorIf(
            tinyxml2::XML_SUCCESS != txmlDoc.LoadFile(LevelList[level].c_str()),
            "Failed to load Level"
            );

        LoadLevel(&txmlDoc);


        //
        // Update/Draw
        //
        engine->m_GameActive = true;
        // Run the game
        engine->GameLoop();

        //
        // Free
        //

        // Delete all the systems
        engine->DestroySystems();
    
        // Delete remaining game objects
        for (size_t i = 0; i < MAX_GAME_OBJECTS; ++i)
        {
            if (g_GameObjectHandleTable[i])
            {
                g_GameObjectHandleTable[i]->Destroy();
                delete g_GameObjectHandleTable[i];
            }
        }
    }

    //
    // Unload
    //

    // Delete the engine itself
    delete engine;

    // Close the application
    return 0;
}