/* Start Header -------------------------------------------------------
Copyright (C) 2013 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: GraphicsSystem.h
Purpose: Include file for graphics libraries
Language: C++
Platform: Windows
Project: CS529_twalton_FinalProject
Author: Tommy Walton, t.walton, 130000812
Creation date: 12/15/2013
- End Header -----------------------------------------------------*/
#pragma once

#include "Engine.h"
#include "WindowsIncludes.h"
#include "Containers.h"

#include <d3d11_1.h>
#include <d2d1_1.h>
#include <dwrite_1.h>
#include <wincodec.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>

#include "tinyXML2\tinyxml2.h"
#include <WICTextureLoader.h>
#include <SpriteBatch.h>

#include "Sprite.h"
#include "Text.h"
#include "LineDrawing.h"

namespace Framework
{
    struct Vertex
    {
        XMFLOAT3 Pos;
        XMFLOAT3 Normal;
        XMFLOAT4 Color;
    };

	struct SimpleCubeVertex
	{
		XMFLOAT3 Pos;   // position
		XMFLOAT4 Color; // color
	};

	struct SimpleVertex
	{
		XMFLOAT3 Pos;
	};

	struct ConstantBuffer
	{
		XMFLOAT4X4 world;
        XMFLOAT4X4 view;
        XMFLOAT4X4 projection;
	};

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
        virtual void Initialize();
        virtual void Update(float dt);
        virtual void OnEvent(Event* e);
        virtual std::string GetName() {return "Graphics";}

        void LoadResources();
        void LoadTextures(tinyxml2::XMLElement *txmlElement);
        void LoadTexture(std::string TextureName, std::string TextureFilePath);
        ID3D11ShaderResourceView *GetTexture(std::string TextureName);
        void DrawSprites();
        void DrawText();
        void DrawLines();
        void DrawModels();
        void DrawDebug();

        Vector2D WorldCoordsToWindowCoords(Vector2D &WorldCoords);
        Vector2D WindowCoordsToWorldCoords(Vector2D &WindowCoords);

        std::list<ComponentHandle> m_SpriteList;
        GameObject* m_pControlsSprite;
        GameObject* m_pWinSprite;
        GameObject* m_pLoseSprite;
        GameObject* m_pHUDText;
        
        std::list<ComponentHandle> m_TextList;
        std::list<ComponentHandle> m_LineDrawingList;
        bool m_DrawDebug;
        bool m_ShowWin;
        bool m_ShowLose;
        bool m_ShowControls;
    private:
        float m_ChocolateMilkWeight;

    void CreateDeviceIndependentResources();
    void CreateDeviceResources();
    void CreateWindowSizeDependentResources();
    void CreateBrushes();

	// 3D Resource Creation
	void CreateShaders();
	void CreateBuffers();
	void InitializeMatrices();
	void CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ComPtr<ID3DBlob> &m_spBlobOut);
	ComPtr<ID3D11VertexShader> m_spVertexShader;
	ComPtr<ID3D11PixelShader> m_spPixelShader;
	ComPtr<ID3D11InputLayout> m_spVertexLayout;
	ComPtr<ID3D11Buffer> m_spVertexBuffer;
	ComPtr<ID3D11Buffer> m_spIndexBuffer;
	ComPtr<ID3D11Buffer> m_spConstantBuffer;
	XMFLOAT4X4    m_World;
    XMFLOAT4X4	m_View;
    XMFLOAT4X4	m_Projection;
    ConstantBuffer m_CB;
	D3D_DRIVER_TYPE m_DriverType;
    float m_Rotate;

    // DXGI Resources
	ComPtr<IDXGISwapChain1> m_spSwapChain1;
	ComPtr<IDXGISwapChain> m_spSwapChain;

    // D3D Resources
	ComPtr<ID3D11Device1> m_spD3DDevice1;
	ComPtr<ID3D11Device> m_spD3DDevice;
	ComPtr<ID3D11DeviceContext1> m_spD3DDeviceContext1;
	ComPtr<ID3D11DeviceContext> m_spD3DDeviceContext;
	ComPtr<ID3D11RenderTargetView> m_spD3DRenderTargetView;
    ComPtr<ID3D11DepthStencilView> m_spD3DDepthStencilView;
    D3D_FEATURE_LEVEL m_FeatureLevel;

    // D2D Resources	
	ComPtr<ID2D1Factory1> m_spD2DFactory;
	ComPtr<ID2D1Device> m_spD2DDevice;
	ComPtr<ID2D1DeviceContext> m_spD2DDeviceContext;
    ComPtr<ID2D1Bitmap1> m_spD2DTargetBitmap;
    ComPtr<ID2D1SolidColorBrush> m_spWhiteBrush;
    ComPtr<ID2D1SolidColorBrush> m_spDebugBrush;

    // DWrite Resources
	ComPtr<IDWriteFactory1> m_spDWriteFactory;

    // WIC Resources
    ComPtr<IWICImagingFactory2> m_spWICFactory;

    std::unique_ptr<SpriteBatch> m_spSpriteBatch;
    std::hash_map<std::string, ID3D11ShaderResourceView* > m_TextureMap;
	// Window Properties
	HWND m_hWnd;
	int m_ScreenWidth;
	int m_ScreenHeight;
    float m_DPIX;
    float m_DPIY;
    };

    // Global pointer to Graphics.
    extern GraphicsSystem* g_GRAPHICS;
}
