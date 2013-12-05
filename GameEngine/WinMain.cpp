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

void LoadLevel(tinyxml2::XMLDocument* txmlDoc);
//void MakeConnections(tinyxml2::XMLElement* txmlElement);

void LoadLevel(tinyxml2::XMLDocument* txmlDoc)
{
    tinyxml2::XMLElement* txmlElement = txmlDoc->FirstChildElement();
    while (txmlElement)
    {
        if (strcmp(txmlElement->Name(),"Textures") == 0)
        {
            GRAPHICS->LoadTextures(txmlElement);
        }
        else if (strcmp(txmlElement->Name(), "GameObject") == 0)
        {
            GameObject* pObj = new GameObject();
            pObj->Initialize(txmlElement);
        }
        /*if (strcmp(txmlElement->Name(), "Textures") == 0)
        {
            MakeConnections(txmlElement);
        }*/
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

    // Add the systems
    engine->AddSystem(windows);
	engine->AddSystem(graphics);

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

    // Delete all the systems

    // Delete the engine itself
    delete engine;

    // Close the application
    return 0;
}