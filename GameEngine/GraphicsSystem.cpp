/* Start Header -------------------------------------------------------
Copyright (C) 2013 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: DirectX.h
Purpose: Implementation file for graphics libraries
Language: C++
Platform: Windows
Project: CS529_twalton_FinalProject
Author: Tommy Walton, t.walton, 130000812
Creation date: 10/20/2013
- End Header -----------------------------------------------------*/
#include "GraphicsSystem.h"

using namespace DirectX;

namespace Framework
{
    GraphicsSystem::GraphicsSystem(HWND hw, int w, int h) : 
		m_HWnd(hw),
		m_ScreenWidth(w),
		m_ScreenHeight(h) {}

    GraphicsSystem::~GraphicsSystem(){}

    
    void GraphicsSystem::Initialize()
    {
        // Struct to hold information about the swap chain
        DXGI_SWAP_CHAIN_DESC scd;
        // Initialize the entire scd to NULL so we don't have to initialize each member individually
        ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
        
        scd.BufferCount = 1; // One back buffer (double buffering)
        scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 32-bit RGBA color
        scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        scd.OutputWindow = m_HWnd;
        scd.SampleDesc.Count = 4;
        scd.Windowed = TRUE;

		HRESULT hr = S_OK;
		hr = D3D11CreateDeviceAndSwapChain(
			NULL,
			D3D_DRIVER_TYPE_HARDWARE,
			NULL,
			NULL,
			NULL,
			NULL,
			D3D11_SDK_VERSION,
			&scd,
		    m_spSwapChain.GetAddressOf(),
			m_spDevice.GetAddressOf(),
			NULL,
            m_spDeviceContext.GetAddressOf()
			);

		// Get the address of the back buffer
		ComPtr<ID3D11Texture2D> spBB;
        hr = m_spSwapChain->GetBuffer(
			0,
			__uuidof(ID3D11Texture2D),
			(LPVOID*)spBB.GetAddressOf()
			);

		// Use the back buffer address to create a render target
		hr = m_spDevice->CreateRenderTargetView(
			spBB.Get(),
			NULL,
            m_spRTVBackBuffer.GetAddressOf()
			);

		// Set the render target as the back buffer
        m_spDeviceContext->OMSetRenderTargets(
			1,
			m_spRTVBackBuffer.GetAddressOf(),
			NULL
			);

		// Set the viewport
		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = (float)m_ScreenWidth;
		viewport.Height = (float)m_ScreenHeight;

		m_spDeviceContext->RSSetViewports(1, &viewport);
    }

    void GraphicsSystem::Update(float timeslice)
	{
		HRESULT hr = S_OK;
        const float clearColor[4] = { 0.7f, 0.0f, 0.8f, 1.0f };
        m_spDeviceContext->ClearRenderTargetView(
			m_spRTVBackBuffer.Get(),
            clearColor
            );

        hr = m_spSwapChain->Present(0, 0);
    }
}
