/* Start Header -------------------------------------------------------
Copyright (C) 2013 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: GraphicsSystem.h
Purpose: Include file for graphics libraries
Language: C++
Platform: Windows
Project: CS529_twalton_FinalProject
Author: Tommy Walton, t.walton, 130000812
Creation date: 10/20/2013
- End Header -----------------------------------------------------*/
#include "Engine.h"
#include "WindowsIncludes.h"
#include "Containers.h"
#include <d3d11.h>
#include <DirectXMath.h>

#pragma comment(lib, "d3d11.lib")

namespace Framework
{
    // Graphics manager
    class GraphicsSystem : public ISystem
    {
    public:
        GraphicsSystem(HWND hw, int w, int h);
        ~GraphicsSystem();

        // ISystem class virtual functions
        //virtual void SendMessage(Message* message) {};
        virtual void Initialize();
        virtual void Update(float timeslice);
        virtual std::string GetName() {return "Windows";}

    private:
	
    ComPtr<IDXGISwapChain> m_spSwapChain;
	ComPtr<ID3D11Device> m_spDevice;
	ComPtr<ID3D11DeviceContext> m_spDeviceContext;
	ComPtr<ID3D11RenderTargetView> m_spRTVBackBuffer;

	// Window Properties
	HWND m_HWnd;
	int m_ScreenWidth;
	int m_ScreenHeight;
    };
}
