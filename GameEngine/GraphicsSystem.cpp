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
    // Global pointer to Graphics.
    GraphicsSystem* GRAPHICS = NULL;

    GraphicsSystem::GraphicsSystem(HWND hw, int w, int h) : 
		m_HWnd(hw),
		m_ScreenWidth(w),
		m_ScreenHeight(h) 
    {
        CoInitialize(NULL);
        m_TextureMap.clear();
        GRAPHICS = this;
    }

    GraphicsSystem::~GraphicsSystem()
    {
        CoUninitialize();
    }

    
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
        scd.BufferDesc.Width = m_ScreenWidth;
        scd.BufferDesc.Height = m_ScreenHeight;
        //scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // allow full screen switching

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
            m_spD3DDeviceContext.GetAddressOf()
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
        m_spD3DDeviceContext->OMSetRenderTargets(
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

		m_spD3DDeviceContext->RSSetViewports(1, &viewport);

        // Load the textures
        LoadTextures();

        // Initialize the sprite batch
        m_spSpriteBatch = std::unique_ptr<SpriteBatch>(new SpriteBatch(m_spD3DDeviceContext.Get()));
        // TODO: temporarily add a sprite to the list to make sure things work
        // The spritelist should be managed by the game logic
    }

    void GraphicsSystem::LoadTextures()
    {
        HRESULT hr = S_OK;
        ID3D11ShaderResourceView *pSRV;
        
        // For starters, always load the same, hard coded texture
        // Will eventually create this object using metadata

        //Use D3DX to load the texture
        hr = DirectX::CreateWICTextureFromFile(
            m_spDevice.Get(),
            m_spD3DDeviceContext.Get(),
            L"C:\\Users\\Tommy\\Documents\\GitHub\\CS529Engine\\GameEngine\\Assets\\Default.png",
            0,
            &pSRV
            );

        if (hr == S_OK)
        {
            //TODO: handle textures with different paths but the same name
            //Add to Graphics list of textures: Textures[texturefile.FileName.c_str()] = newTexture;
            //m_TextureMap[std::string("Default")] = spSRV;
            m_TextureMap.insert(std::pair<std::string, ID3D11ShaderResourceView* >("Default", pSRV));
        }
        else
        {
            //Error
            //ErrorIf(false, "Failed to load texture %s in %s", "C:\Assets\Default.png", "C:\Assets\Default");
        }
    }

    ID3D11ShaderResourceView* GraphicsSystem::GetTexture(std::string TextureName)
    {
        std::hash_map<std::string, ID3D11ShaderResourceView* >::iterator it = m_TextureMap.find(TextureName);
        if (it != m_TextureMap.end())
            return it->second;
        else
            return NULL;
    }

    void GraphicsSystem::DrawSprites()
    {
        m_spSpriteBatch->Begin();

        //Iterate through the link list of sprites
        std::list<Sprite>::iterator it = m_SpriteList.begin();
        for (; it != m_SpriteList.end(); ++it)
            it->Draw(m_spSpriteBatch);

        m_spSpriteBatch->End();
    }

    void GraphicsSystem::Update(float timeslice)
	{
		HRESULT hr = S_OK;
        const float clearColor[4] = { 0.7f, 0.0f, 0.8f, 1.0f };
        m_spD3DDeviceContext->ClearRenderTargetView(
			m_spRTVBackBuffer.Get(),
            clearColor
            );

        DrawSprites();

        hr = m_spSwapChain->Present(0, 0);
    }
}
