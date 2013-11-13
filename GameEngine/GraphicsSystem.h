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
#pragma once

#include "Engine.h"
#include "WindowsIncludes.h"
#include "Containers.h"
#include <d3d11.h>
#include <DirectXMath.h>

#include <WICTextureLoader.h>
#include <SpriteBatch.h>

#include "Sprite.h"

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

        void LoadTextures();
        ID3D11ShaderResourceView *GetTexture(std::string TextureName);
        void DrawSprites();

        std::list<Sprite> m_SpriteList;
    private:
	
    ComPtr<IDXGISwapChain> m_spSwapChain;
	ComPtr<ID3D11Device> m_spDevice;
	ComPtr<ID3D11DeviceContext> m_spD3DDeviceContext;
	ComPtr<ID3D11RenderTargetView> m_spRTVBackBuffer;

    std::unique_ptr<SpriteBatch> m_spSpriteBatch;
    std::hash_map<std::string, ID3D11ShaderResourceView* > m_TextureMap;
	// Window Properties
	HWND m_HWnd;
	int m_ScreenWidth;
	int m_ScreenHeight;
    };

    // Global pointer to Graphics.
    extern GraphicsSystem* GRAPHICS;
}
