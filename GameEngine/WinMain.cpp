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

using namespace Framework;

// TODO: get computer's resolution
const int ClientWidth = 1920;
const int ClientHeight = 1080;

// Application entry point
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Create the core engine
    CoreEngine* engine = new CoreEngine();

    // Create the windows system
    WindowsSystem* windows = new WindowsSystem(ClientWidth, ClientHeight);
	// Create the graphics system
	GraphicsSystem* graphics = new GraphicsSystem(windows->hWnd, ClientWidth, ClientHeight);

    // Add the systems
    engine->AddSystem(windows);
	engine->AddSystem(graphics);

    // Initialize the engine and it's systems
    engine->InitializeSystems();

    tinyxml2::XMLDocument txmlDoc;
    ErrorIf(
        tinyxml2::XML_SUCCESS != txmlDoc.LoadFile("Assets\\Level0.xml"),
        "Failed to load Assets\\Level0.xml"
        );

    // Get the first sprite out of Level0.xml
    tinyxml2::XMLElement* textureElement = txmlDoc.FirstChildElement("Sprite");

    // Add a sprite to GRAPHICS
    // TODO: sprites should be managed elsewhere
    Sprite s;
    s.Initialize(textureElement);

    // Move on to the next sprite
    textureElement = textureElement->NextSiblingElement("Sprite");

    Sprite s2;
    s2.SetPosition(50.0f, 100.0f);
    s2.Initialize(textureElement);

    // Activate the window
    windows->ActivateWindow();

    // Run the game
    engine->GameLoop();

    // Delete all the game objects

    // Delete all the systems

    // Delete the engine itself
    delete engine;

    // Close the application
    return 0;
}