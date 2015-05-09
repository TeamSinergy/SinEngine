#include <Precompiled.h>
#include "WindowSystem.h"

DefineType(WindowSystem, SinningZilch)
{
    BindConstructor();
    BindDestructor();
    RegisterComponent(WindowSystem);
}

void WindowSystem::Serialize(DataNode* data)
{
    ((Game*)Owner)->WindowSystem = this;
    DataComponent* settings = static_cast<DataComponent*>(data);
    Name = "SinEngine";
    SerializeValueName(settings, Name, "Name");
    WindowClassName = "Window1";
    SerializeValue(settings, WindowClassName);
    Visible = true;
    SerializeValue(settings, Visible);
    int xPosition = CW_USEDEFAULT;
    SerializeValue(settings, xPosition);
    int yPosition = CW_USEDEFAULT;
    SerializeValue(settings, yPosition);
    Position = Integer2(xPosition, yPosition);
    int Width = 500;
    SerializeValue(settings, Width);
    int Height = 400;
    SerializeValue(settings, Height);
    Dimensions = Integer2(Width, Height);
    defaultSize = Dimensions;
    IconPath = "icon.ico";
    SerializeValue(settings, IconPath);
    Resizable = true;
    SerializeValue(settings, Resizable);
    int fullscreen = Windowed;
    SerializeValueName(settings, fullscreen, "ScreenMode");
    CursorVisible = true;
    SerializeValue(settings, CursorVisible);
    ColorMode = 0;
    SerializeValue(settings, ColorMode);
    ClearColor = Real4(255, 255, 255, 1);
    SerializeValue(settings, ClearColor);
    GraphicsManager::NormalizeColor(ClearColor, ColorMode);
    SampleRate = 4;
    SerializeValue(settings, SampleRate);
    DebugMode = false;
    SerializeValue(settings, DebugMode);
    VSync = true;
    SerializeValue(settings, VSync);
    NearPlane = 1;
    SerializeValue(settings, NearPlane);
    FarPlane = 1000;
    SerializeValue(settings, FarPlane);
    AntiAliasedLines = true;
    SerializeValue(settings, AntiAliasedLines);
}

void WindowSystem::Create()
{
    ShowCursor(CursorVisible);

    unsigned winStyle = WindowStyles::BorderedWindowStyle;
    if (!Resizable)
    {
        winStyle = winStyle ^ WS_THICKFRAME;
    }

    Icon* icon = ResourceManager::FindResourceType<Icon>(IconPath);

    ZeroMemory(&info, sizeof(info));

    info.cbSize = sizeof(WindowInfo);
    info.style = CS_HREDRAW | CS_VREDRAW; //Redraw the window if it is resized
    info.lpfnWndProc = WindowProc;
    info.hInstance = GetModuleHandle(0); //MUST BE CHANGED FOR MULTIPLE WINDOWS
    info.hCursor = LoadCursor(NULL, IDC_ARROW);
    //info.hbrBackground = (HBRUSH)COLOR_WINDOW; //The background style and color
    info.lpszClassName = WindowClassName.c_str();
    info.hIcon = icon->StoredIcon;

    RECT wr = { 0, 0, Dimensions.x, Dimensions.y };
    AdjustWindowRect(&wr, info.style, false);

    // register the window class
    RegisterClassEx(&info);

    handle = CreateWindowEx(NULL,
        WindowClassName.c_str(),    // name of the window class
        Name.c_str(),   // title of the window
        winStyle,    // window style
        Position.x,    // x-position of the window
        Position.y,    // y-position of the window
        wr.right - wr.left,    // width of the window
        wr.bottom - wr.top,    // height of the window
        NULL,    // we have no parent window, NULL
        NULL,    // we aren't using menus, NULL
        info.hInstance,    // application handle
        NULL);    // used with multiple windows, NULL

    ErrorIf(handle == nullptr, "Failed to create window %s", Name.c_str());
    //GetClassInfoEx(instance, WindowClassName.c_str(), handle);
    // display the window on the screen
    ShowWindow(handle, Visible);
    SetFullscreen(screenMode);
}

void WindowSystem::Initialize()
{
    InitializeDX11();
    SetRenderTarget();
}

void WindowSystem::Uninitialize()
{
    UninitializeDX11();
}

void WindowSystem::Destroy()
{
}

void WindowSystem::SetWidth(int width)
{
    RECT wr = { 0, 0, width, Dimensions.y };
    AdjustWindowRect(&wr, info.style, false);
    SetWindowPos(handle, 0, 0, 0, wr.right - wr.left, wr.bottom - wr.top, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
    Dimensions.x = width;
}
void WindowSystem::SetHeight(int height)
{
    RECT wr = { 0, 0, Dimensions.x, height };
    AdjustWindowRect(&wr, info.style, false);
    SetWindowPos(handle, 0, 0, 0, wr.right - wr.left, wr.bottom - wr.top, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
    Dimensions.y = height;
}

void WindowSystem::SetDimensions(Integer2 dimensions)
{
    RECT wr = { 0, 0, dimensions.x, dimensions.y };
    AdjustWindowRect(&wr, info.style, false);
    SetWindowPos(handle, 0, 0, 0, wr.right - wr.left, wr.bottom - wr.top, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
    Dimensions = dimensions;
}

void WindowSystem::SetPosition(Integer2 position)
{
    SetWindowPos(handle, 0, position.x, position.y, Dimensions.x, Dimensions.y, SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER);
}

void WindowSystem::SetFullscreen(int newScreenMode)
{
    if (screenMode == newScreenMode)
    {
        return;
    }
    int style;
    screenMode = newScreenMode;

    switch (newScreenMode)
    {
    case Windowed:
    {
        SetDimensions(defaultSize);
        style = BorderedWindowStyle;
        if (!Resizable)
        {
            style = style ^ WS_THICKFRAME;
        }
        info.style = style;
        SetWindowLongPtr(handle, GWL_STYLE, style);
        ShowWindow(handle, Visible);
    }break;
    case Fullscreen:
    {
        //Same as borderless window mode with a few changes
    }
    case BorderlessWindow:
    {
        Integer2 resolution = GraphicsManager::GetDesktopResolution(handle);
        SetDimensions(resolution);
        style = BorderlessWindowStyle;
        info.style = style;
        SetWindowLongPtr(handle, GWL_STYLE, style);
        SetPosition(GraphicsManager::GetDesktopOrigin(handle));

        ShowWindow(handle, Visible);

    }break;
    case BorderedWindow:
    {
        Integer2 resolution = GraphicsManager::GetDesktopResolution(handle);
        style = BorderedWindowStyle;
        if (!Resizable)
        {
            style = style ^ WS_THICKFRAME;
        }
        info.style = style;
        SetWindowLongPtr(handle, GWL_STYLE, style);
        SetDimensions(resolution);
        SetPosition(GraphicsManager::GetDesktopOrigin(handle));
        ShowWindow(handle, SW_MAXIMIZE);
    }break;
    }

}

WindowRef WindowSystem::GetHandle()
{
    return handle;
}

//////////////////////////////////
////        DIRECTX11         ////
//////////////////////////////////

void WindowSystem::InitializeDX11()
{
    HRESULT result;
    IDXGIFactory* factory;
    IDXGIAdapter* adapter;
    IDXGIOutput* adapterOutput;
    unsigned int numModes, i, numerator, denominator, stringLength;
    DXGI_MODE_DESC* displayModeList;
    DXGI_ADAPTER_DESC adapterDesc;
    int error;
    
    D3D_FEATURE_LEVEL featureLevel;
    

    // Create a DirectX graphics interface factory.
    result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
    ErrorIf(FAILED(result), "Failed to Initialize DirectX11");

    // Use the factory to create an adapter for the primary graphics interface (video card).
    result = factory->EnumAdapters(0, &adapter);
    ErrorIf(FAILED(result), "Failed to Initialize DirectX11");

    // Enumerate the primary adapter output (monitor).
    result = adapter->EnumOutputs(0, &adapterOutput);
    ErrorIf(FAILED(result), "Failed to Initialize DirectX11"); //May need to be changed?

    // Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
    result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
    ErrorIf(FAILED(result), "Failed to Initialize DirectX11");

    // Create a list to hold all the possible display modes for this monitor/video card combination.
    displayModeList = new DXGI_MODE_DESC[numModes];
    MemCheck(displayModeList, "DirectX11 DisplayModeList");

    // Now fill the display mode list structures.
    result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
    ErrorIf(FAILED(result), "Failed to Initialize DirectX11");

    // Now go through all the display modes and find the one that matches the screen width and height.
    // When a match is found store the numerator and denominator of the refresh rate for that monitor.
    for (i = 0; i<numModes; i++)
    {
        if (displayModeList[i].Width == (unsigned int)Dimensions.x)
        {
            if (displayModeList[i].Height == (unsigned int)Dimensions.y)
            {
                numerator = displayModeList[i].RefreshRate.Numerator;
                denominator = displayModeList[i].RefreshRate.Denominator;
            }
        }
    }

    // Get the adapter (video card) description.
    result = adapter->GetDesc(&adapterDesc);
    ErrorIf(FAILED(result), "Failed to Initialize DirectX11");

    // Store the dedicated video card memory in megabytes.
    VideoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

    char m_videoCardDescription[128];
    // Convert the name of the video card to a character array and store it.
    error = wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128);
    ErrorIf(error != 0, "Failed to Initialize DirectX11");

    VideoCardDescription = String(m_videoCardDescription);

    // create a struct to hold information about the swap chain
    DXGI_SWAP_CHAIN_DESC scd; //SwapChainDescription
    // clear out the struct for use
    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
    bool fullscreen = false;
    if (GetScreenMode() == ScreenMode::Fullscreen)
    {
        fullscreen = true;
    }
    int flags = 0;//DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    if (DebugMode)
    {
        flags = flags | D3D11_CREATE_DEVICE_DEBUG;
    }
    // Release the display mode list.
    delete[] displayModeList;
    displayModeList = nullptr;

    // Release the adapter output.
    adapterOutput->Release();
    adapterOutput = nullptr;

    // Release the adapter.
    adapter->Release();
    adapter = nullptr;
    
    // Release the factory.
    factory->Release();
    factory = nullptr;

    // fill the swap chain description struct
    scd.BufferCount = 1;                                    // one back buffer
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
    scd.OutputWindow = handle;                 // the window to be used
    scd.Windowed = !fullscreen;                                    // windowed/full-screen mode

    scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    // Discard the back buffer contents after presenting.
    scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    // Set the refresh rate of the back buffer.
    if (VSync)
    {
        scd.BufferDesc.RefreshRate.Numerator = numerator;
        scd.BufferDesc.RefreshRate.Denominator = denominator;
    }
    else
    {
        scd.BufferDesc.RefreshRate.Numerator = 0;
        scd.BufferDesc.RefreshRate.Denominator = 1;
    }
    //Set the MSAA settings
    if (SampleRate >= AntiAiliasingModes::MSAA && SampleRate <= AntiAiliasingModes::MSAAx4)
    {
        // how many multisamples
        scd.SampleDesc.Count = SampleRate;
        scd.SampleDesc.Quality = D3D11_STANDARD_MULTISAMPLE_PATTERN;
    }
    else
    {
        scd.SampleDesc.Count = 1;
        scd.SampleDesc.Quality = 0;
    }
    scd.Flags = flags;

    // Set the feature level to DirectX 11.
    featureLevel = D3D_FEATURE_LEVEL_11_0;
    // create a device, device context and swap chain using the information in the scd struct
    result = D3D11CreateDeviceAndSwapChain(NULL,
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
        NULL,
        &featureLevel,
        1,
        D3D11_SDK_VERSION,
        &scd,
        &SwapChain,
        &DeviceInterface,
        NULL,
        &DeviceContext);
    ErrorIf(FAILED(result), "Failed to Initialize DirectX11");
    //unsigned qualityLevel = 0;
    //DeviceInterface->CheckMultisampleQualityLevels(scd.BufferDesc.Format, scd.SampleDesc.Count, &qualityLevel);
    //if (MSQualityLevel > qualityLevel)
    //{
    //    MSQualityLevel = static_cast<int>(qualityLevel);
    //}
    //SwapChain->GetDesc(&scd);
    //scd.SampleDesc.Quality = static_cast<unsigned>(MSQualityLevel);
    //SwapChain->GetDesc(&scd);

    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
    bd.ByteWidth = sizeof(Vertex) * 3;             // size is the VERTEX struct * 3
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

    DeviceInterface->CreateBuffer(&bd, NULL, &VertexBuffer);       // create the buffer
}
void WindowSystem::SetRenderTarget(DXTexture2D* target)
{
    HRESULT result;
    D3D11_TEXTURE2D_DESC depthBufferDesc;
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
    D3D11_RASTERIZER_DESC rasterDesc;
    

    if (target == nullptr) //Set the render target to the back buffer
    {
        //The index of the back buffer we are rendering to, 
        //the unique id of the Texture2D type, 
        //and a void* to the where the object will be stored.
        result = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&target);
        ErrorIf(FAILED(result), "Failed to Initialize DirectX11");
    }
    // use the buffer address to create the render target
    result = DeviceInterface->CreateRenderTargetView(target, NULL, &RenderTarget);
    ErrorIf(FAILED(result), "Failed to Initialize DirectX11");
    
    // set the render target as the buffer
    //Number of render targets
    //Pointer to an array of render targets
    target->Release();

    //Depth buffer
    // Initialize the description of the depth buffer.
    ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

    // Set up the description of the depth buffer.
    depthBufferDesc.Width = Dimensions.x;
    depthBufferDesc.Height = Dimensions.y;
    depthBufferDesc.MipLevels = 1;
    depthBufferDesc.ArraySize = 1;
    depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthBufferDesc.SampleDesc.Count = 1;
    depthBufferDesc.SampleDesc.Quality = 0;
    depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthBufferDesc.CPUAccessFlags = 0;
    depthBufferDesc.MiscFlags = 0;

    // Create the texture for the depth buffer using the filled out description.
    result = DeviceInterface->CreateTexture2D(&depthBufferDesc, NULL, &DepthStencilBuffer);
    ErrorIf(FAILED(result), "Failed to Initialize DirectX11");
    


    // Initialize the description of the stencil state.
    ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

    // Set up the description of the stencil state.
    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

    depthStencilDesc.StencilEnable = true;
    depthStencilDesc.StencilReadMask = 0xFF;
    depthStencilDesc.StencilWriteMask = 0xFF;

    // Stencil operations if pixel is front-facing.
    depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // Stencil operations if pixel is back-facing.
    depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // Create the depth stencil state.
    result = DeviceInterface->CreateDepthStencilState(&depthStencilDesc, &DepthStencilState);
    ErrorIf(FAILED(result), "Failed to Initialize DirectX11");

    // Set the depth stencil state.
    DeviceContext->OMSetDepthStencilState(DepthStencilState, 1);

    // Initailze the depth stencil view.
    ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

    // Set up the depth stencil view description.
    depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Texture2D.MipSlice = 0;

    // Create the depth stencil view.
    result = DeviceInterface->CreateDepthStencilView(DepthStencilBuffer, &depthStencilViewDesc, &DepthStencilView);

    // Bind the render target view and depth stencil buffer to the output render pipeline.
    DeviceContext->OMSetRenderTargets(1, &RenderTarget, DepthStencilView);

    // Setup the raster description which will determine how and what polygons will be drawn.
    rasterDesc.AntialiasedLineEnable = AntiAliasedLines;
    rasterDesc.CullMode = D3D11_CULL_BACK;
    rasterDesc.DepthBias = 0;
    rasterDesc.DepthBiasClamp = 0.0f;
    rasterDesc.DepthClipEnable = true;
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.FrontCounterClockwise = false;
    rasterDesc.MultisampleEnable = (SampleRate >= AntiAiliasingModes::MSAA || SampleRate <= AntiAiliasingModes::MSAAx4);
    rasterDesc.ScissorEnable = false;
    rasterDesc.SlopeScaledDepthBias = 0.0f;

    // Create the rasterizer state from the description we just filled out.
    result = DeviceInterface->CreateRasterizerState(&rasterDesc, &RasterState);
    ErrorIf(FAILED(result), "Failed to Initialize DirectX11");

    // Now set the rasterizer state.
    DeviceContext->RSSetState(RasterState);
}

void WindowSystem::SetWindowAsViewport(WindowSystem* window)
{
    // Set the viewport to the size of the window
    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

    Integer2 Dimensions = GetDimensions();

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = static_cast<float>(Dimensions.x);
    viewport.Height = static_cast<float>(Dimensions.y);
    viewport.MaxDepth = 1;
    viewport.MinDepth = 0;

    //number of viewports being used,
    //pointer to array of vieports.
    DeviceContext->RSSetViewports(1, &viewport);

    float fieldOfView, screenAspect;
    // Setup the projection matrix.
    fieldOfView = (float)XM_PI / 4.0f;
    screenAspect = (float)Dimensions.x / (float)Dimensions.y;

    // Create the projection matrix for 3D rendering.
    ProjectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, NearPlane, FarPlane);
    
    // Initialize the world matrix to the identity matrix.
    WorldMatrix = XMMatrixIdentity();

    // Create an orthographic projection matrix for 2D rendering.
    OrthographicMatrix = XMMatrixOrthographicLH((float)Dimensions.x, (float)Dimensions.y, NearPlane, FarPlane);
}

void WindowSystem::InitializePipeline()
{
    VertexShader* VS = ResourceManager::FindResourceType<VertexShader>("VertexTest.vert");
    PixelShader* PS = ResourceManager::FindResourceType<PixelShader>("PixelTest.pix");
    DeviceInterface->CreateVertexShader(VS->StoredShader->GetBufferPointer(), VS->StoredShader->GetBufferSize(), NULL, &pVS);
    DeviceInterface->CreatePixelShader(PS->StoredShader->GetBufferPointer(), PS->StoredShader->GetBufferSize(), NULL, &pPS);

    // set the shader objects
    DeviceContext->VSSetShader(pVS, 0, 0);
    DeviceContext->PSSetShader(pPS, 0, 0);

    //The element description
    D3D11_INPUT_ELEMENT_DESC inputLayout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    
    DeviceInterface->CreateInputLayout(inputLayout, 2, VS->StoredShader->GetBufferPointer(), VS->StoredShader->GetBufferSize(), &InputLayout);
    DeviceContext->IASetInputLayout(InputLayout);
}

void WindowSystem::DrawDebugTriangle()
{
    Vertex OurVertices[] =
    {
        { Real3(0.0f, 0.5f, 0.0f), Real4(1.0f, 0.0f, 0.0f, 1.0f) },
        { Real3(0.45f, -0.5, 0.0f), Real4(0.0f, 1.0f, 0.0f, 1.0f) },
        { Real3(-0.45f, -0.5f, 0.0f), Real4(0.0f, 0.0f, 1.0f, 1.0f) }
    };


    D3D11_MAPPED_SUBRESOURCE ms;
    DeviceContext->Map(VertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);   // map the buffer
    memcpy(ms.pData, OurVertices, sizeof(OurVertices));                           // copy the data
    DeviceContext->Unmap(VertexBuffer, NULL);                                     // unmap the buffer
    
}

void WindowSystem::RenderFrame()
{
    // clear the back buffer to the clearcolor
    float color[4] = { ClearColor.x, ClearColor.y, ClearColor.z, ClearColor.w };
    DeviceContext->ClearRenderTargetView(RenderTarget, color);

    // Clear the depth buffer.
    DeviceContext->ClearDepthStencilView(DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

    // do 3D rendering on the back buffer here
    unsigned stride = sizeof(Vertex);
    unsigned offset = 0;
    DeviceContext->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);
    DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    DeviceContext->Draw(3, 0);

    // Present the back buffer to the screen since rendering is complete.
    if (VSync)
    {
        // Lock to screen refresh rate.
        SwapChain->Present(1, 0);
    }
    else
    {
        // Present as fast as possible.
        SwapChain->Present(0, 0);
    }
}


void WindowSystem::UninitializeDX11()
{
    SwapChain->SetFullscreenState(FALSE, NULL);    // switch to windowed mode

    // close and release all existing COM objects

    pVS->Release();
    pPS->Release(); //TO BE MADE HASMAPS
    
   /* VertexBuffer->Release();
    InputLayout->Release();
    SwapChain->Release();
    RenderTarget->Release();
    DeviceInterface->Release();
    DeviceContext->Release();*/

    // Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
    if (SwapChain)
    {
        SwapChain->SetFullscreenState(false, NULL);
    }

    if (RasterState)
    {
        RasterState->Release();
        RasterState = nullptr;
    }

    if (DepthStencilView)
    {
        DepthStencilView->Release();
        DepthStencilView = nullptr;
    }

    if (DepthStencilState)
    {
        DepthStencilState->Release();
        DepthStencilState = nullptr;
    }

    if (DepthStencilBuffer)
    {
        DepthStencilBuffer->Release();
        DepthStencilBuffer = nullptr;
    }

    if (RenderTarget)
    {
        RenderTarget->Release();
        RenderTarget = nullptr;
    }

    if (DeviceContext)
    {
        DeviceContext->Release();
        DeviceContext = nullptr;
    }

    if (DeviceInterface)
    {
        DeviceInterface->Release();
        DeviceInterface = nullptr;
    }

    if (SwapChain)
    {
        SwapChain->Release();
        SwapChain = 0;
    }
}
