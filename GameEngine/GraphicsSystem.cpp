/* Start Header -------------------------------------------------------
Copyright (C) 2013 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: GraphicsSystem.cpp
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
		m_hWnd(hw),
		m_ScreenWidth(w),
		m_ScreenHeight(h),
        m_DPIX(96.0f),
        m_DPIY(96.0f),
        m_DrawDebug(false),
        m_ShowControls(false),
        m_ShowWin(false),
        m_ShowLose(false),
        m_pControlsSprite(NULL),
        m_pLoseSprite(NULL),
        m_pWinSprite(NULL),
        m_ChocolateMilkWeight(0.0f)
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

    void GraphicsSystem::Destroy()
    {
        m_TextureMap.clear();
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
				__uuidof(ID2D1Factory1),
				&options,
				&m_spD2DFactory
				)
			);

        DXThrowIfFailed(
            DWriteCreateFactory(
                DWRITE_FACTORY_TYPE_SHARED,
                __uuidof(IDWriteFactory1),
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
        // BGRA_SUPPORT flag adds support for surfaces with a different color channel ordering
        // than the API default. It is required for compatibility with Direct2D.
        UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

        // SINGLETHREADED flag improves performance. It will need to change if we make multithreaded graphics calls
        creationFlags |= D3D11_CREATE_DEVICE_SINGLETHREADED;

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
        HRESULT hr = D3D11CreateDevice(
                nullptr,                    // Specify nullptr to use the default adapter.
                D3D_DRIVER_TYPE_HARDWARE,
                0,
                creationFlags,              // Set debug and Direct2D compatibility flags.
                featureLevels,              // List of feature levels this app can support.
                ARRAYSIZE(featureLevels),
                D3D11_SDK_VERSION,          // Always set this to D3D11_SDK_VERSION for Windows Store apps.
                &spDevice,                  // Returns the Direct3D device created.
                &m_FeatureLevel,            // Returns feature level of device created.
                &spContext                  // Returns the device immediate context.
            );

        // Fall back to software if hardware creation fails
		if(DXGI_ERROR_UNSUPPORTED == hr)
		{
			DXThrowIfFailed(
				D3D11CreateDevice(
					nullptr,                    // Specify nullptr to use the default adapter.
					D3D_DRIVER_TYPE_WARP,
					0,
					creationFlags,              // Set debug and Direct2D compatibility flags.
					featureLevels,              // List of feature levels this app can support.
					ARRAYSIZE(featureLevels),
					D3D11_SDK_VERSION,          // Always set this to D3D11_SDK_VERSION for Windows Store apps.
					&spDevice,                  // Returns the Direct3D device created.
					&m_FeatureLevel,            // Returns feature level of device created.
					&spContext                  // Returns the device immediate context.
                )
			);
		}

        // Get the Direct3D 11.1 API device and context interfaces.
        DXThrowIfFailed(
            spDevice.As(&m_spD3DDevice1)
            );

        DXThrowIfFailed(
            spContext.As(&m_spD3DDeviceContext1)
            );

        // Get the underlying DXGI device of the Direct3D device.
		ComPtr<IDXGIDevice2> spDXGIDevice;
        DXThrowIfFailed(
            m_spD3DDevice1.As(&spDXGIDevice)
            );

        // Create the Direct2D device object and a corresponding context.
        DXThrowIfFailed(
            m_spD2DFactory->CreateDevice(spDXGIDevice.Get(), &m_spD2DDevice)
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

            swapChainDesc.Width = m_ScreenWidth;                            // Use automatic sizing.
            swapChainDesc.Height = m_ScreenHeight;
            swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;              // This is the most common swap chain format.
            swapChainDesc.Stereo = false;
            swapChainDesc.SampleDesc.Count = 4;                             // Use 4x MSAA (all D3D 11 devices support 4x MSAA)
            swapChainDesc.SampleDesc.Quality = 0;
            swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;    // Use RENDER_TARGET_OUTPUT because we are going to be rendering to the back buffer

            // When you call IDXGIFactory::CreateSwapChain to create a full-screen swap chain, you typically include the front buffer in this value.
            swapChainDesc.BufferCount = 1;                           // Use one back buffer for double-buffering to minimize latency.
            swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
            swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;    // Let the display driver select the most efficient presentation method.
            swapChainDesc.Flags = 0;
            swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;

			ComPtr<IDXGIDevice2> spDXGIDevice;
            DXThrowIfFailed(
                m_spD3DDevice1.As(&spDXGIDevice)
                );

            ComPtr<IDXGIAdapter> spDXGIAdapter;
            DXThrowIfFailed(
                spDXGIDevice->GetAdapter(&spDXGIAdapter)
                );

            ComPtr<IDXGIFactory2> spDXGIFactory;
            DXThrowIfFailed(
                spDXGIAdapter->GetParent(IID_PPV_ARGS(&spDXGIFactory))
                );

            
            //CoreWindow window = m_window.Get();
            DXThrowIfFailed(
                spDXGIFactory->CreateSwapChainForHwnd(
                    m_spD3DDevice1.Get(),
                    m_hWnd,
                    &swapChainDesc,
                    nullptr,
                    nullptr,
                    &m_spSwapChain1
                    )
                );

            // Ensure that DXGI does not queue more than one frame at a time. This both reduces latency and
            // ensures that the application will only render after each VSync, minimizing power consumption.
            DXThrowIfFailed(
                spDXGIDevice->SetMaximumFrameLatency(1)
                );
        //}

        // Create a Direct3D render target view of the swap chain back buffer.
        ComPtr<ID3D11Texture2D> spBackBuffer;
        DXThrowIfFailed(
            m_spSwapChain1->GetBuffer(0, IID_PPV_ARGS(&spBackBuffer))
            );

        DXThrowIfFailed(
            m_spD3DDevice1->CreateRenderTargetView(
                spBackBuffer.Get(),
                nullptr,
                &m_spD3DRenderTargetView
                )
            );

        // Cache the rendertarget dimensions in our helper class for convenient use.
        D3D11_TEXTURE2D_DESC backBufferDesc = { 0 };
        spBackBuffer->GetDesc(&backBufferDesc);

        // Create a depth stencil view for use with 3D rendering if needed.
        CD3D11_TEXTURE2D_DESC depthStencilDesc(
            DXGI_FORMAT_D24_UNORM_S8_UINT,
            backBufferDesc.Width,
            backBufferDesc.Height,
            1, // One MipMap level
            1, // One texture in the texture array
            D3D11_BIND_DEPTH_STENCIL
            );

        ComPtr<ID3D11Texture2D> spDepthStencil;
        DXThrowIfFailed(
            m_spD3DDevice1->CreateTexture2D(
                &depthStencilDesc,
                nullptr,
                &spDepthStencil
                )
            );

        auto viewDesc = CD3D11_DEPTH_STENCIL_VIEW_DESC(D3D11_DSV_DIMENSION_TEXTURE2D);
        DXThrowIfFailed(
            m_spD3DDevice1->CreateDepthStencilView(
                spDepthStencil.Get(),
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

        m_spD3DDeviceContext1->RSSetViewports(1, &viewport);

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
            m_spSwapChain1->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer))
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
    
    // Create Brushes
    void GraphicsSystem::CreateBrushes()
    {
        m_spD2DDeviceContext->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::White),
            m_spWhiteBrush.GetAddressOf()
            );

        m_spD2DDeviceContext->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::Red),
            m_spDebugBrush.GetAddressOf()
            );
    }

	void GraphicsSystem::CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ComPtr<ID3DBlob> &m_spBlobOut)
	{
		HRESULT hr = S_OK;

		DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
		// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
		// Setting this flag improves the shader debugging experience, but still allows 
		// the shaders to be optimized and to run exactly the way they will run in 
		// the release configuration of this program.
		dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

		ComPtr<ID3DBlob> spErrorBlob;
		hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
			dwShaderFlags, 0, &m_spBlobOut, &spErrorBlob);
		if (FAILED(hr))
		{
			if (spErrorBlob)
			{
				OutputDebugStringA(reinterpret_cast<const char*>(spErrorBlob->GetBufferPointer()));
			}
			DXThrowIfFailed(hr);
		}
	}

	void GraphicsSystem::CreateShaders()
	{
		// Compile the vertex shader
		ComPtr<ID3DBlob> spVSBlob;
		CompileShaderFromFile(L"SimpleShader.fx", "VS", "vs_5_0", spVSBlob);

		// Create the vertex shader
		DXThrowIfFailed(
			m_spD3DDevice1->CreateVertexShader(
				spVSBlob->GetBufferPointer(),
				spVSBlob->GetBufferSize(),
				nullptr, m_spVertexShader.GetAddressOf())
			);
		
		// Define the input layout
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		/*D3D11_INPUT_ELEMENT_DESC layout[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};*/
		UINT numElements = ARRAYSIZE(layout);

		// Create the input layout
		DXThrowIfFailed(
			m_spD3DDevice1->CreateInputLayout(
				layout,
				numElements,
				spVSBlob->GetBufferPointer(),
				spVSBlob->GetBufferSize(),
                m_spVertexLayout.GetAddressOf()
				)
			);
		
		// Set the input layout
		m_spD3DDeviceContext1->IASetInputLayout(m_spVertexLayout.Get());

		// Compile the pixel shader
		ComPtr<ID3DBlob> spPSBlob;
		CompileShaderFromFile(L"SimpleShader.fx", "PS", "ps_5_0", spPSBlob);

		// Create the pixel shader
		DXThrowIfFailed(
			m_spD3DDevice1->CreatePixelShader(spPSBlob->GetBufferPointer(), spPSBlob->GetBufferSize(), nullptr, m_spPixelShader.GetAddressOf())
			);
	}

	void GraphicsSystem::CreateBuffers()
	{
		// Create vertex buffer
		/*SimpleCubeVertex vertices[] =
		{
			{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
			{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
			{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },
			{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
			{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
			{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },
			{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
			{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
		};
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(SimpleCubeVertex)* 8;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;*/
		// Create vertex buffer
		SimpleCubeVertex vertices[] =
		{
            { XMFLOAT3(0.0f, 0.5f, 0.5f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
            { XMFLOAT3(0.5f, -0.5f, 0.5f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
            { XMFLOAT3(-0.5f, -0.5f, 0.5f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		};
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(SimpleCubeVertex)* 3;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = vertices;
		DXThrowIfFailed(
			m_spD3DDevice1->CreateBuffer(
				&bd, 
				&InitData, 
                m_spVertexBuffer.GetAddressOf()
				)
			);

		// Set vertex buffer
		UINT stride = sizeof(SimpleCubeVertex);
		//UINT stride = sizeof(SimpleVertex);
		UINT offset = 0;
		m_spD3DDeviceContext1->IASetVertexBuffers(0, 1, m_spVertexBuffer.GetAddressOf(), &stride, &offset);

		/*// Create index buffer
		WORD indices[] =
		{
			3, 1, 0,
			2, 1, 3,

			0, 5, 4,
			1, 5, 0,

			3, 4, 7,
			0, 4, 3,

			1, 6, 5,
			2, 6, 1,

			2, 7, 6,
			3, 7, 2,

			6, 4, 5,
			7, 4, 6,
		};
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(WORD)* 36;        // 36 vertices needed for 12 triangles in a triangle list
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		InitData.pSysMem = indices;
		DXThrowIfFailed(
			m_spD3DDevice->CreateBuffer(&bd, &InitData, m_spIndexBuffer.ReleaseAndGetAddressOf())
			);

		// Set index buffer
		m_spD3DDeviceContext->IASetIndexBuffer(m_spIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
		*/
		// Set primitive topology
		m_spD3DDeviceContext1->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		// Create the constant buffer
		/*bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(ConstantBuffer);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;
		DXThrowIfFailed(
			m_spD3DDevice->CreateBuffer(
				&bd,
				nullptr,
                m_spConstantBuffer.ReleaseAndGetAddressOf()
				)
			);*/
	}

	void GraphicsSystem::InitializeMatrices()
	{
		// Initialize the world matrix
		m_CB.world = m_World = XMMatrixIdentity();

		// Initialize the view matrix
		XMVECTOR Eye = XMVectorSet(0.0f, 1.0f, -5.0f, 0.0f);
		XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		m_CB.view = m_View = XMMatrixLookAtLH(Eye, At, Up);

        RECT rc;
        GetClientRect(m_hWnd, &rc);
        UINT width = rc.right - rc.left;
        UINT height = rc.bottom - rc.top;

		// Initialize the projection matrix
		m_CB.projection = m_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, width / (FLOAT)height, 0.01f, 100.0f);
	}
	
    void GraphicsSystem::LoadResources()
    {
        CreateDeviceIndependentResources();
        CreateDeviceResources();
        CreateWindowSizeDependentResources();
        CreateBrushes();

		HRESULT hr = S_OK;

		RECT rc;
		GetClientRect(m_hWnd, &rc);
		UINT width = rc.right - rc.left;
		UINT height = rc.bottom - rc.top;
		
		m_spD3DDeviceContext1->OMSetRenderTargets(1, m_spD3DRenderTargetView.GetAddressOf(), nullptr);

		// Setup the viewport
		D3D11_VIEWPORT vp;
		vp.Width = (FLOAT)m_ScreenWidth;
		vp.Height = (FLOAT)m_ScreenHeight;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		m_spD3DDeviceContext1->RSSetViewports(1, &vp);

        CreateShaders();
        CreateBuffers();
        InitializeMatrices();
    }

    void GraphicsSystem::Initialize()
    {
        m_ChocolateMilkWeight = 0.0f;
        // Initialize the sprite batch
        m_spSpriteBatch = std::unique_ptr<SpriteBatch>(new SpriteBatch(m_spD3DDeviceContext1.Get()));
    }

    void GraphicsSystem::LoadTextures(tinyxml2::XMLElement *txmlElement)
    {                
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
                m_spD3DDevice1.Get(),
                m_spD3DDeviceContext1.Get(),
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
        // Display a controls screen
        if (m_ShowControls)
        {            
            Sprite* pSprite = static_cast<Sprite*>(m_pControlsSprite->GetComponent(COMPONENT_TYPE_SPRITE));
            if (pSprite)
            {
                m_spSpriteBatch->Begin();
                pSprite->Draw(m_spSpriteBatch);
                m_spSpriteBatch->End();
            }
        }
        // Display a "You Win" message
        else if (m_ShowWin)
        {            
            Sprite* pSprite = static_cast<Sprite*>(m_pWinSprite->GetComponent(COMPONENT_TYPE_SPRITE));
            if (pSprite)
            {
                m_spSpriteBatch->Begin();
                pSprite->Draw(m_spSpriteBatch);
                m_spSpriteBatch->End();
            }
        }
        // Display a game over screen
        else if (m_ShowLose)
        {
            Sprite* pSprite = static_cast<Sprite*>(m_pLoseSprite->GetComponent(COMPONENT_TYPE_SPRITE));
            if (pSprite)
            {
                m_spSpriteBatch->Begin();
                pSprite->Draw(m_spSpriteBatch);
                m_spSpriteBatch->End();
            }
        }
        else
        {
            m_spSpriteBatch->Begin();
            //Iterate through the link list of sprites
            std::list<ComponentHandle>::iterator it = m_SpriteList.begin();
            for (; it != m_SpriteList.end(); ++it)
                static_cast<Sprite*>((*it).ToComponent())->Draw(m_spSpriteBatch);

            m_spSpriteBatch->End();
        }        
    }

    void GraphicsSystem::DrawText()
    {
        m_spD2DDeviceContext->BeginDraw();

        //Iterate through the linked list of sprites
        std::list<ComponentHandle>::iterator it = m_TextList.begin();
        for (; it != m_TextList.end(); ++it)
            static_cast<Text*>(it->ToComponent())->Draw(m_spD2DDeviceContext, m_spWhiteBrush, m_spDWriteFactory);

        DXThrowIfFailed(
            m_spD2DDeviceContext->EndDraw()
            );
    }

    void GraphicsSystem::DrawLines()
    {
        m_spD2DDeviceContext->BeginDraw();

        //Iterate through the linked list of sprites
        std::list<ComponentHandle>::iterator it = m_LineDrawingList.begin();
        for (; it != m_LineDrawingList.end(); ++it)
            static_cast<LineDrawing*>(it->ToComponent())->Draw(m_spD2DDeviceContext, m_spDebugBrush);

        DXThrowIfFailed(
            m_spD2DDeviceContext->EndDraw()
            );
    }

    void GraphicsSystem::DrawModels()
    {
        //
        // Update variables
        //
//        m_CB.world = XMMatrixTranspose(m_World);
  //      m_CB.view = XMMatrixTranspose(m_View);
    //    m_CB.projection = XMMatrixTranspose(m_Projection);
       /* m_spD3DDeviceContext->UpdateSubresource(m_spConstantBuffer.Get(), 0, nullptr, &m_CB, 0, 0);

        //
        // Renders a triangle
        //
        m_spD3DDeviceContext->VSSetShader(m_spVertexShader.Get(), nullptr, 0);
        m_spD3DDeviceContext->VSSetConstantBuffers(0, 1, m_spConstantBuffer.GetAddressOf());
        m_spD3DDeviceContext->PSSetShader(m_spPixelShader.Get(), nullptr, 0);
        m_spD3DDeviceContext->DrawIndexed(36, 0, 0);        // 36 vertices needed for 12 triangles in a triangle list*/

		// Clear the back buffer 
		m_spD3DDeviceContext1->ClearRenderTargetView(m_spD3DRenderTargetView.Get(), Colors::MidnightBlue);

		// Render a triangle
		m_spD3DDeviceContext1->VSSetShader(m_spVertexShader.Get(), nullptr, 0);
		m_spD3DDeviceContext1->PSSetShader(m_spPixelShader.Get(), nullptr, 0);
		m_spD3DDeviceContext1->Draw(3, 0);

    }

    void GraphicsSystem::DrawDebug()
    {      
        m_spD2DDeviceContext->BeginDraw();

        // Iterate through the list of sprites and draw the debug info for each one
        std::list<ComponentHandle>::iterator it = m_SpriteList.begin();
        for (; it != m_SpriteList.end(); ++it)
            static_cast<Sprite*>((*it).ToComponent())->DrawDebug(m_spD2DDeviceContext, m_spDebugBrush);

        DXThrowIfFailed(
            m_spD2DDeviceContext->EndDraw()
            );
    }

    void GraphicsSystem::Update(float timeslice)
	{
/*        m_spD3DDeviceContext->OMSetRenderTargets(1, m_spD3DRenderTargetView.GetAddressOf(), m_spD3DDepthStencilView.Get());
		HRESULT hr = S_OK;

        const float chocolateColor[4] = { 0.54296875f, 0.26593f, 0.074218f, 1.0f };

        float r = ((1.0f - m_ChocolateMilkWeight) + chocolateColor[0] * m_ChocolateMilkWeight);
        float g = ((1.0f - m_ChocolateMilkWeight) + chocolateColor[1] * m_ChocolateMilkWeight);
        float b = ((1.0f - m_ChocolateMilkWeight) + chocolateColor[2] * m_ChocolateMilkWeight);

        const float clearColor[4] = {r, g, b, 1.0f};

        m_spD3DDeviceContext->ClearRenderTargetView(
			m_spD3DRenderTargetView.Get(),
            clearColor
            );
			*/
        DrawModels();

        DrawLines();

        //DrawSprites();
        
        //DrawText();

        if(m_DrawDebug)
            DrawDebug();

        DXThrowIfFailed(
            m_spSwapChain1->Present(1, 0)
            );
        // Discard the contents of the render target.
        // This is a valid operation only when the existing contents will be entirely
        // overwritten. If dirty or scroll rects are used, this call should be removed.
        m_spD3DDeviceContext1->DiscardView(m_spD3DRenderTargetView.Get());

        // Discard the contents of the depth stencil.
        m_spD3DDeviceContext1->DiscardView(m_spD3DDepthStencilView.Get());
        
    }

    void GraphicsSystem::OnEvent(Event* e)
    {
        if (e->m_EventType == EVENT_TYPE_MOUSE_BUTTON)
        {
            MouseButtonEvent* mbe = static_cast<MouseButtonEvent*>(e);
            if (!mbe->m_IsPressed && mbe->m_MouseButtonIndex == MouseButtonEvent::RightMouse)
            {
                m_DrawDebug = !m_DrawDebug;
            }
            else if(!mbe->m_IsPressed && mbe->m_MouseButtonIndex == MouseButtonEvent::LeftMouse)
            {
                m_ShowControls = !m_ShowControls;
            }
        }
        else if(e->m_EventType == EVENT_TYPE_CHOCOLATE_MILK)
        {
            m_ChocolateMilkWeight += 0.001f;
            if(m_ChocolateMilkWeight > 1.0f) m_ChocolateMilkWeight = 1.0f;
        }
    }

    // Convert WorldCoords (from -ScreenSize/2 to +ScreenSize/2, with ScreenHeight/2 at the top and -ScreenHeight/2 at the bottom)
    // to WindowCoords (from 0 to ScreenSize, with 0 at the top and ScreenHeight at the bottom)
    Vector2D GraphicsSystem::WorldCoordsToWindowCoords(Vector2D &WorldCoords)
    {
        Vector2D WindowCoords(0.0f, 0.0f);

        WindowCoords.x = m_ScreenWidth / 2.0f + WorldCoords.x;
        WindowCoords.y = m_ScreenHeight / 2.0f - WorldCoords.y;

        return WindowCoords;
    }

    Vector2D GraphicsSystem::WindowCoordsToWorldCoords(Vector2D &WindowCoords)
    {
        Vector2D WorldCoords(0.0f, 0.0f);

        WorldCoords.x =  WindowCoords.x - m_ScreenWidth / 2.0f;
        WorldCoords.y = -WindowCoords.y  + m_ScreenHeight / 2.0f;

        return WorldCoords;
    }
}
