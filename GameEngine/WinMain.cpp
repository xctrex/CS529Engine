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

void LoadLevel(tinyxml2::XMLDocument* txmlDoc);
//void MakeConnections(tinyxml2::XMLElement* txmlElement);
std::vector<GameObject*> gameobjects;

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
/*
void MakeConnections(tinyxml2::XMLElement* txmlElement)
{
    // Loop through all the textures and load each one
    tinyxml2::XMLElement* connectionElement = txmlElement->FirstChildElement("Connection");
    while (connectionElement != nullptr)
    {
        if (strcmp(connectionElement->Attribute("SignalObject"), "WindowsSystem"))
        {
            if (strcmp(connectionElement->Attribute("SignalFunction"), "MoveUp"))
            {

            }
        }
        connectionElement = connectionElement->NextSiblingElement("Connection");
    }
}*/

// Application entry point
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Create the core engine
    CoreEngine* engine = new CoreEngine();

    // Create the windows system
    WindowsSystem* windows = new WindowsSystem(ClientWidth, ClientHeight);
	// Create the graphics system
	GraphicsSystem* graphics = new GraphicsSystem(windows->hWnd, ClientWidth, ClientHeight);
    // Create the logic system
    GameLogicSystem* logic = new GameLogicSystem();
    // Create the physics system
    PhysicsSystem* physics = new PhysicsSystem();

    // Add the systems
    engine->AddSystem(windows);
	engine->AddSystem(graphics);
    engine->AddSystem(logic);
    engine->AddSystem(physics);

    // Initialize the engine and it's systems
    engine->InitializeSystems();

    tinyxml2::XMLDocument txmlDoc;

    ThrowErrorIf(
        tinyxml2::XML_SUCCESS != txmlDoc.LoadFile("Assets\\Level0.xml"),
        "Failed to load Assets\\Level0.xml"
        );

    LoadLevel(&txmlDoc);

    // Activate the window
    windows->ActivateWindow();

    // Run the game
    engine->GameLoop();

    // Delete all the game objects
    for (size_t i = 0; i < MAX_GAME_OBJECTS; ++i)
    {
        if (g_GameObjectHandleTable[i])
        {
            g_GameObjectHandleTable[i]->Destroy();
            delete g_GameObjectHandleTable[i];
        }
    }

    // Delete all the systems
    engine->DestroySystems();

    // Delete the engine itself
    delete engine;

    // Close the application
    return 0;
}