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

using namespace Framework;

const int ClientWidth = 800;
const int ClientHeight = 600;

// Application entry point
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Create the core engine
    CoreEngine* engine = new CoreEngine();

    // Create the windows system
    WindowsSystem* windows = new WindowsSystem(ClientWidth, ClientHeight);

    // Add the systems
    engine->AddSystem(windows);

    // Initialize the engine and it's systems
    engine->InitializeSystems();

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