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
#include <d3d11_2.h>
#include <DirectXMath.h>
#include <d2d1_2.h>
#include <dwrite_2.h>
#include <wincodec.h>

#include "tinyXML2\tinyxml2.h"
#include <WICTextureLoader.h>
#include <SpriteBatch.h>

#include "Sprite.h"
#include "Text.h"

namespace Framework
{
    // Throw on error //TODO: Convert DirectX error codes to exceptions.
    inline void DXThrowIfFailed(HRESULT hr)
    {
        if (FAILED(hr))
        {
            // Set a breakpoint on this line to catch DX API errors.
            throw;
        }
    }

    // Graphics manager
    class GraphicsSystem : public ISystem
    {
    public:
        GraphicsSystem(HWND hw, int w, int h);
        ~GraphicsSystem();
        void Destroy();

        // ISystem class virtual functions
        //virtual void SendMessage(Message* message) {};
        virtual void Initialize();
        virtual void Update(float dt);
        virtual void OnEvent(Event* e);
        virtual std::string GetName() {return "Graphics";}

        void LoadTextures(tinyxml2::XMLElement* txmlElement);
        void LoadTexture(std::string TextureName, std::string TextureFilePath);
        ID3D11ShaderResourceView *GetTexture(std::string TextureName);
        void DrawSprites();
        void DrawText();

        Vector2D WorldCoordsToWindowCoords(Vector2D &WorldCoords);

        std::list<Sprite*> m_SpriteList;//TODO: investigate if we should use Sprite* here, allowing the sprites to be modified by something other than the graphics compononent
        GameObject* m_pControlsSprite;
        GameObject* m_pWinSprite;
        GameObject* m_pLoseSprite;
        
        std::list<Text> m_TextList;//TODO: investigate if we should use Text* here, 
        bool m_DrawDebug;
        bool m_ShowWin;
        bool m_ShowLose;
        bool m_ShowControls;
    private:

    void CreateDeviceIndependentResources();
    void CreateDeviceResources();
    void CreateWindowSizeDependentResources();
	
    // DXGI Resources
    ComPtr<IDXGISwapChain1> m_spSwapChain;

    // D3D Resources
    ComPtr<ID3D11Device2> m_spD3DDevice;
	ComPtr<ID3D11DeviceContext2> m_spD3DDeviceContext;
	ComPtr<ID3D11RenderTargetView> m_spD3DRenderTargetView;
    ComPtr<ID3D11DepthStencilView> m_spD3DDepthStencilView;
    D3D_FEATURE_LEVEL m_FeatureLevel;

    // D2D Resources
    ComPtr<ID2D1Factory2> m_spD2DFactory;
    ComPtr<ID2D1Device1> m_spD2DDevice;
    ComPtr<ID2D1DeviceContext1> m_spD2DDeviceContext;
    ComPtr<ID2D1RenderTarget> m_spD2DRenderTarget;
    ComPtr<ID2D1Bitmap1> m_spD2DTargetBitmap;
    ComPtr<ID2D1SolidColorBrush> m_spBlackBrush;

    // DWrite Resources
    ComPtr<IDWriteFactory2> m_spDWriteFactory;

    // WIC Resources
    ComPtr<IWICImagingFactory2> m_spWICFactory;

    std::unique_ptr<SpriteBatch> m_spSpriteBatch;
    std::hash_map<std::string, ID3D11ShaderResourceView* > m_TextureMap;
	// Window Properties
	HWND m_HWnd;
	int m_ScreenWidth;
	int m_ScreenHeight;
    float m_DPIX;
    float m_DPIY;
    };

    // Global pointer to Graphics.
    extern GraphicsSystem* g_GRAPHICS;
}
