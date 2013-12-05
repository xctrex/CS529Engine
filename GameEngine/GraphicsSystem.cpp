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
    GraphicsSystem* g_GRAPHICS = NULL;

    GraphicsSystem::GraphicsSystem(HWND hw, int w, int h) : 
		m_HWnd(hw),
		m_ScreenWidth(w),
		m_ScreenHeight(h) 
    {
        CoInitialize(NULL);
        m_TextureMap.clear();

        //Set up the global pointer
        ThrowErrorIf(g_GRAPHICS != NULL, "Graphics already initialized");
        g_GRAPHICS = this;
    }

    GraphicsSystem::~GraphicsSystem()
    {
        CoUninitialize();
    }

    void GraphicsSystem::CreateDeviceIndependentResources()
    {
        D2D1_FACTORY_OPTIONS options;
        ZeroMemory(&options, sizeof(D2D1_FACTORY_OPTIONS));

#if defined(_DEBUG)
        // If the project is in a debug build, enable Direct2D debugging via SDK Layers.
        options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif

        DXThrowIfFailed(
            D2D1CreateFactory(
                D2D1_FACTORY_TYPE_SINGLE_THREADED,
                __uuidof(ID2D1Factory2),
                &options,
                &m_spD2DFactory
                )
            );

        DXThrowIfFailed(
            DWriteCreateFactory(
                DWRITE_FACTORY_TYPE_SHARED,
                __uuidof(IDWriteFactory2),
                &m_spDWriteFactory
                )
            );
        
        DXThrowIfFailed(
            CoCreateInstance(
                CLSID_WICImagingFactory,
                nullptr,
                CLSCTX_INPROC_SERVER,
                IID_PPV_ARGS(&m_spWICFactory)
                )
            );
    }

    void GraphicsSystem::CreateDeviceResources()
    {
        // This flag adds support for surfaces with a different color channel ordering
        // than the API default. It is required for compatibility with Direct2D.
        UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
        /*
#if defined(_DEBUG)
        if (DX::SdkLayersAvailable())
        {
            // If the project is in a debug build, enable debugging via SDK Layers with this flag.
            creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
        }
#endif*/

        // This array defines the set of DirectX hardware feature levels this app will support.
        // Note the ordering should be preserved.
        // Don't forget to declare your application's minimum required feature level in its
        // description.  All applications are assumed to support 9.1 unless otherwise stated.
        D3D_FEATURE_LEVEL featureLevels[] =
        {
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_10_1,
            D3D_FEATURE_LEVEL_10_0,
            D3D_FEATURE_LEVEL_9_3,
            D3D_FEATURE_LEVEL_9_2,
            D3D_FEATURE_LEVEL_9_1
        };

        // Create the Direct3D 11 API device object and a corresponding context.
        ComPtr<ID3D11Device> spDevice;
        ComPtr<ID3D11DeviceContext> spContext;
        DXThrowIfFailed(
            D3D11CreateDevice(
                nullptr,                    // Specify nullptr to use the default adapter.
                D3D_DRIVER_TYPE_HARDWARE,
                0,
                creationFlags,              // Set debug and Direct2D compatibility flags.
                featureLevels,              // List of feature levels this app can support.
                ARRAYSIZE(featureLevels),
                D3D11_SDK_VERSION,          // Always set this to D3D11_SDK_VERSION for Windows Store apps.
                &spDevice,                    // Returns the Direct3D device created.
                &m_FeatureLevel,            // Returns feature level of device created.
                &spContext                    // Returns the device immediate context.
                )
            );

        // Get the Direct3D 11.1 API device and context interfaces.
        DXThrowIfFailed(
            spDevice.As(&m_spD3DDevice)
            );

        DXThrowIfFailed(
            spContext.As(&m_spD3DDeviceContext)
            );

        // Get the underlying DXGI device of the Direct3D device.
        ComPtr<IDXGIDevice3 > dxgiDevice;
        DXThrowIfFailed(
            m_spD3DDevice.As(&dxgiDevice)
            );

        // Create the Direct2D device object and a corresponding context.
        DXThrowIfFailed(
            m_spD2DFactory->CreateDevice(dxgiDevice.Get(), &m_spD2DDevice)
            );

        DXThrowIfFailed(
            m_spD2DDevice->CreateDeviceContext(
                D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
                &m_spD2DDeviceContext
                )
            );
    }

    // Allocate all memory resources that change on a window SizeChanged event.
    void GraphicsSystem::CreateWindowSizeDependentResources()
    {
        /*// Store the window bounds so the next time we get a SizeChanged event we can
        // avoid rebuilding everything if the size is identical.
        m_windowBounds = m_window->Bounds;

        if (m_swapChain != nullptr)
        {
            // If the swap chain already exists, resize it.
            HRESULT hr = m_swapChain->ResizeBuffers(2, 0, 0, DXGI_FORMAT_B8G8R8A8_UNORM, 0);

            if (hr == DXGI_ERROR_DEVICE_REMOVED)
            {
                // If the device was removed for any reason, a new device and swapchain will need to be created.
                HandleDeviceLost();

                // Everything is set up now. Do not continue execution of this method.
                return;
            }
            else
            {
                DX::ThrowIfFailed(hr);
            }
        }
        else
        {*/
            // Otherwise, create a new one using the same adapter as the existing Direct3D device.
            DXGI_SWAP_CHAIN_DESC1 swapChainDesc;

            ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC1));

            swapChainDesc.Width = m_ScreenWidth;                                     // Use automatic sizing.
            swapChainDesc.Height = m_ScreenHeight;
            swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;           // This is the most common swap chain format.
            swapChainDesc.Stereo = false;
            swapChainDesc.SampleDesc.Count = 1;                          // Don't use multi-sampling.
            swapChainDesc.SampleDesc.Quality = 0;
            swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

            // When you call IDXGIFactory::CreateSwapChain to create a full-screen swap chain, you typically include the front buffer in this value.
            swapChainDesc.BufferCount = 2;                               // Use double-buffering to minimize latency.
            swapChainDesc.Scaling = DXGI_SCALING_NONE;
            swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL; // All Windows Store apps must use this SwapEffect.
            swapChainDesc.Flags = 0;
            swapChainDesc.Scaling = DXGI_SCALING_NONE;
            swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;

            ComPtr<IDXGIDevice3> dxgiDevice;
            DXThrowIfFailed(
                m_spD3DDevice.As(&dxgiDevice)
                );

            ComPtr<IDXGIAdapter> dxgiAdapter;
            DXThrowIfFailed(
                dxgiDevice->GetAdapter(&dxgiAdapter)
                );

            ComPtr<IDXGIFactory2> dxgiFactory;
            DXThrowIfFailed(
                dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory))
                );

            
            //CoreWindow window = m_window.Get();
            DXThrowIfFailed(
                dxgiFactory->CreateSwapChainForHwnd(
                    m_spD3DDevice.Get(),
                    m_HWnd,
                    &swapChainDesc,
                    nullptr,
                    nullptr,
                    &m_spSwapChain
                    )
                );

            // Ensure that DXGI does not queue more than one frame at a time. This both reduces latency and
            // ensures that the application will only render after each VSync, minimizing power consumption.
            DXThrowIfFailed(
                dxgiDevice->SetMaximumFrameLatency(1)
                );
        //}

        // Create a Direct3D render target view of the swap chain back buffer.
        ComPtr<ID3D11Texture2D> backBuffer;
        DXThrowIfFailed(
            m_spSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer))
            );

        DXThrowIfFailed(
            m_spD3DDevice->CreateRenderTargetView(
                backBuffer.Get(),
                nullptr,
                &m_spD3DRenderTargetView
                )
            );

        // Cache the rendertarget dimensions in our helper class for convenient use.
        D3D11_TEXTURE2D_DESC backBufferDesc = { 0 };
        backBuffer->GetDesc(&backBufferDesc);

        // Create a depth stencil view for use with 3D rendering if needed.
        CD3D11_TEXTURE2D_DESC depthStencilDesc(
            DXGI_FORMAT_D24_UNORM_S8_UINT,
            backBufferDesc.Width,
            backBufferDesc.Height,
            1,
            1,
            D3D11_BIND_DEPTH_STENCIL
            );

        ComPtr<ID3D11Texture2D> depthStencil;
        DXThrowIfFailed(
            m_spD3DDevice->CreateTexture2D(
                &depthStencilDesc,
                nullptr,
                &depthStencil
                )
            );

        auto viewDesc = CD3D11_DEPTH_STENCIL_VIEW_DESC(D3D11_DSV_DIMENSION_TEXTURE2D);
        DXThrowIfFailed(
            m_spD3DDevice->CreateDepthStencilView(
                depthStencil.Get(),
                &viewDesc,
                &m_spD3DDepthStencilView
                )
            );
        
        // Set the 3D rendering viewport to target the entire window.
        CD3D11_VIEWPORT viewport(
            0.0f,
            0.0f,
            static_cast<float>(backBufferDesc.Width),
            static_cast<float>(backBufferDesc.Height)
            );

        m_spD3DDeviceContext->RSSetViewports(1, &viewport);

        // Create a Direct2D target bitmap associated with the
        // swap chain back buffer and set it as the current target.
        D2D1_BITMAP_PROPERTIES1 bitmapProperties =
            D2D1::BitmapProperties1(
                D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
                D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
                m_DPIX,
                m_DPIY
            );

        ComPtr<IDXGISurface2> dxgiBackBuffer;
        DXThrowIfFailed(
            m_spSwapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer))
            );

        DXThrowIfFailed(
            m_spD2DDeviceContext->CreateBitmapFromDxgiSurface(
                dxgiBackBuffer.Get(),
                &bitmapProperties,
                &m_spD2DTargetBitmap
                )
            );

        m_spD2DDeviceContext->SetTarget(m_spD2DTargetBitmap.Get());

        // Grayscale text anti-aliasing is recommended for all Windows Store apps.
        m_spD2DDeviceContext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
    }

    void GraphicsSystem::Initialize()
    {
        CreateDeviceIndependentResources();
        CreateDeviceResources();
        CreateWindowSizeDependentResources();
        
        // Initialize the sprite batch
        m_spSpriteBatch = std::unique_ptr<SpriteBatch>(new SpriteBatch(m_spD3DDeviceContext.Get()));
    }

    void GraphicsSystem::LoadTextures(tinyxml2::XMLElement *txmlElement)
    {
        /*tinyxml2::XMLDocument txmlDoc;
        ThrowErrorIf(
            tinyxml2::XML_SUCCESS != txmlDoc.LoadFile("Assets\\Textures.xml"), 
            "Failed to load Assets\\Textures.xml"
            );*/
                
        // Loop through all the textures and load each one
        tinyxml2::XMLElement* textureElement = txmlElement->FirstChildElement("Texture");
        while (textureElement != nullptr)
        {
            LoadTexture(
                textureElement->Attribute("Name"),
                textureElement->Attribute("Path")
                );

            textureElement = textureElement->NextSiblingElement("Texture");
        }
    }

    void GraphicsSystem::LoadTexture(std::string TextureName, std::string TextureFilePath)
    {
        HRESULT hr = S_OK;
        ID3D11ShaderResourceView *pSRV;

        // For starters, always load the same, hard coded texture
        // Will eventually create this object using metadata
        wchar_t filePath[MAX_PATH];
        swprintf(filePath, MAX_PATH, L"%hs", TextureFilePath.c_str());

        //Use D3DX to load the texture
        DXThrowIfFailed(
            DirectX::CreateWICTextureFromFile(
                m_spD3DDevice.Get(),
                m_spD3DDeviceContext.Get(),
                filePath,
                0,
                &pSRV
                )
            );
        
        //TODO: handle textures with different paths but the same name
        //Add to Graphics list of textures: Textures[texturefile.FileName.c_str()] = newTexture;
        //m_TextureMap[std::string("Default")] = spSRV;
        m_TextureMap.insert(std::pair<std::string, ID3D11ShaderResourceView* >(TextureName, pSRV));
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
        m_spD3DDeviceContext->OMSetRenderTargets(1, m_spD3DRenderTargetView.GetAddressOf(), m_spD3DDepthStencilView.Get());
		HRESULT hr = S_OK;
        const float clearColor[4] = { 0.7f, 0.0f, 0.8f, 1.0f };
        m_spD3DDeviceContext->ClearRenderTargetView(
			m_spD3DRenderTargetView.Get(),
            clearColor
            );

        DrawSprites();
        
        ComPtr<ID2D1SolidColorBrush> sp_BlackBrush;
        D2D1_COLOR_F color;
        color.a = 1.0f;
        color.r = 1.0f;
        color.g = 0.0f;
        color.b = 0.0f;

        m_spD2DDeviceContext->CreateSolidColorBrush(
        color,
        sp_BlackBrush.GetAddressOf()
        );

        m_spD2DDeviceContext->BeginDraw();

        m_spD2DDeviceContext->DrawRectangle(
        D2D1::RectF(10.0f, 10.0f, 500.0f, 500.0f),
        sp_BlackBrush.Get()
        );
        //TODO: check hrVERIFY_SUCCEEDED(
        m_spD2DDeviceContext->EndDraw();
        //     );

        DXThrowIfFailed(
            m_spSwapChain->Present(1, 0)
            );
        // Discard the contents of the render target.
        // This is a valid operation only when the existing contents will be entirely
        // overwritten. If dirty or scroll rects are used, this call should be removed.
        m_spD3DDeviceContext->DiscardView(m_spD3DRenderTargetView.Get());

        // Discard the contents of the depth stencil.
        m_spD3DDeviceContext->DiscardView(m_spD3DDepthStencilView.Get());
        
    }
}
