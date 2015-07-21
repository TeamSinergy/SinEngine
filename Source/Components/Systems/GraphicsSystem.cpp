#include <Precompiled.h>
#include "GraphicsSystem.h"
#include "Game.h"
#include "VectorGraphic.h"
#include "GraphicsComponent.h"
#include "DirectXMath.h"

DefineType(GraphicsSystem, SinningZilch)
{
    BindConstructor();
    BindDestructor();
    RegisterComponent(GraphicsSystem);
}

void GraphicsSystem::Serialize(DataNode* data)
{
    Owner->GraphicsSystem = this;
    
    DataComponent* settings = static_cast<DataComponent*>(data);
    ColorMode = 0;
    SerializeValue(settings, ColorMode);
    ClearColor = Real4(255, 255, 255, 1);
    SerializeValue(settings, ClearColor);
    GraphicsManager::NormalizeColor(ClearColor, ColorMode);
    SampleRate = 4;
    SerializeValue(settings, SampleRate);
    QualityLevel = 30;
    SerializeValue(settings, QualityLevel);
    DebugMode = false;
    SerializeValue(settings, DebugMode);
    VSync = true;
    SerializeValue(settings, VSync);
    AntiAliasedLines = true;
    SerializeValue(settings, AntiAliasedLines);
    ScalingMode = 0;
    SerializeValue(settings, ScalingMode);
}

//////////////////////////////////
////        DIRECTX11         ////
//////////////////////////////////
void GraphicsSystem::Create()
{
    Window = GameSession->WindowSystem;
    ErrorIf(Window == nullptr, "YA NEED A WINDOW TO DISPLAY GRAPHICS, DOOFUS!");

    GetDeviceInformation();
    CreateDeviceAndSwapChain();
    SetRenderTargetView();
}

void GraphicsSystem::Initialize()
{
    
    EventConnect(GameSession, Events::EngineUpdate, &GraphicsSystem::RenderFrame, this);

    SetWindowAsViewport(Window);
    //CreateDepthStencilBuffer();
    //CreateDepthStencilState();

    //CreateDepthStencilView();

    CreateRasterState();

    InitializePipeline();
    //CreateVertexBuffer();
    ////CalculateWorldMatrix();
    ////CalculateOrthographicMatrix();
    //DrawDebugTriangle();
}

void GraphicsSystem::GetDeviceInformation()
{
    HRESULT result;
    IDXGIFactory2* factory;
    IDXGIAdapter* adapter;
    IDXGIOutput* adapterOutput;
    unsigned int numModes, i, stringLength;
    DXGI_MODE_DESC* displayModeList;
    DXGI_ADAPTER_DESC adapterDesc;
    int error;

    // Create a DirectX graphics interface factory.
    result = CreateDXGIFactory(__uuidof(IDXGIFactory2), (void**)&factory);
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
        if (displayModeList[i].Width == (unsigned int)Window->GetDesktopResolution().x)
        {
            if (displayModeList[i].Height == (unsigned int)Window->GetDesktopResolution().y)
            {
                RefreshRate.x = displayModeList[i].RefreshRate.Numerator;
                RefreshRate.y = displayModeList[i].RefreshRate.Denominator;
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

    // Release the display mode list.
    delete[] displayModeList;
    displayModeList = nullptr;
    // Release the adapter output.
    ReleaseCOM(adapterOutput);
    // Release the adapter.
    ReleaseCOM(adapter);
    // Release the factory.
    ReleaseCOM(factory);
}

void GraphicsSystem::CreateDeviceAndSwapChain()
{
    D3D_FEATURE_LEVEL featureLevels[] = {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0 };
    // create a struct to hold information about the swap chain
    bool fullscreen = false;
    if (Window->GetScreenMode() == ScreenMode::Fullscreen)
    {
        fullscreen = true;
    }
    int flags = 0;//DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    if (DebugMode)
    {
        flags = flags | D3D11_CREATE_DEVICE_DEBUG;
    }

    D3D11CreateDevice(
        NULL,
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
        flags,
        featureLevels,
        ARRAYSIZE(featureLevels),
        D3D11_SDK_VERSION,
        &DeviceInterface,
        &FeatureLevel,
        &DeviceContext);

    DXGI_SWAP_CHAIN_DESC1 scd; //SwapChainDescription
    // clear out the struct for use
    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC1));
    // fill the swap chain description struct
    scd.Width = Window->GetDimensions().x;
    scd.Height = Window->GetDimensions().y;
    scd.BufferCount = 1;                                    // one back buffer
    scd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
    scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullScreenDesc; //SwapChainDescription
    // clear out the struct for use
    ZeroMemory(&fullScreenDesc, sizeof(DXGI_SWAP_CHAIN_FULLSCREEN_DESC));
    fullScreenDesc.Windowed = !fullscreen;                                    // windowed/full-screen mode    
    fullScreenDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    // Discard the back buffer contents after presenting.
    fullScreenDesc.Scaling = (DXGI_MODE_SCALING)ScalingMode; //Unspecified = 0, Centered = 1, Stretched = 2

    // Set the refresh rate of the back buffer.
    if (VSync)
    {
        fullScreenDesc.RefreshRate.Numerator = RefreshRate.x;
        fullScreenDesc.RefreshRate.Denominator = RefreshRate.y;
    }
    else
    {
        fullScreenDesc.RefreshRate.Numerator = 0;
        fullScreenDesc.RefreshRate.Denominator = 1;
    }
    //Set the MSAA settings
    //if MSAA is enabled
    if (SampleRate != AntiAiliasingModes::FXAA && SampleRate != AntiAiliasingModes::None)
    {
        // how many multisamples
        scd.SampleDesc.Count = SampleRate;
        unsigned qualityLevel;
        DeviceInterface->CheckMultisampleQualityLevels(scd.Format, SampleRate, &qualityLevel);
        if (qualityLevel == 0)
        {
            Error("MSAAx%i not supported on this device.", qualityLevel);
        }
        if (QualityLevel > static_cast<int>(qualityLevel))
        {
            QualityLevel = qualityLevel;
        }
        else if (QualityLevel < 0)
        {
            QualityLevel = 0;
        }
        scd.SampleDesc.Quality = QualityLevel - 1;
    }
    else
    {
        QualityLevel = 1;
        SampleRate = 1;
        scd.SampleDesc.Count = 1;
        scd.SampleDesc.Quality = 0;
    }
    scd.Flags = flags;



    HRESULT hr;
    IDXGIDevice* dxgiDevice = nullptr;
    ErrorIf(FAILED(hr = DeviceInterface->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice))));

    IDXGIAdapter* dxgiAdapter = nullptr;
    ErrorIf(FAILED(hr = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&dxgiAdapter))));

    IDXGIFactory2* dxgiFactory = nullptr;
    ErrorIf(FAILED(hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory))));

    ErrorIf(FAILED(hr = dxgiFactory->CreateSwapChainForHwnd(dxgiDevice, Window->GetHandle(), &scd, &fullScreenDesc, nullptr, &SwapChain)));


    ReleaseCOM(dxgiDevice);
    ReleaseCOM(dxgiAdapter);
    ReleaseCOM(dxgiFactory);
    //ReleaseCOM(md3dDebug);


}

void GraphicsSystem::CreateDepthStencilBuffer()
{
    HRESULT result;
    D3D11_TEXTURE2D_DESC depthBufferDesc;

    //Depth buffer
    // Initialize the description of the depth buffer.
    ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

    // Set up the description of the depth buffer.
    depthBufferDesc.Width = Window->GetDimensions().x;
    depthBufferDesc.Height = Window->GetDimensions().y;
    depthBufferDesc.MipLevels = 1;
    depthBufferDesc.ArraySize = 1;
    depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthBufferDesc.SampleDesc.Count = SampleRate;
    depthBufferDesc.SampleDesc.Quality = QualityLevel - 1;
    depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthBufferDesc.CPUAccessFlags = 0;
    depthBufferDesc.MiscFlags = 0;

    // Create the texture for the depth buffer using the filled out description.
    result = DeviceInterface->CreateTexture2D(&depthBufferDesc, NULL, &DepthStencilBuffer);
    ErrorIf(FAILED(result), "Failed to Initialize DirectX11");
}
void GraphicsSystem::CreateDepthStencilState()
{
    HRESULT result;
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
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
}
void GraphicsSystem::CreateDepthStencilView()
{
    HRESULT result;
    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
    ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

    // Set up the depth stencil view description.
    depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
    depthStencilViewDesc.Texture2D.MipSlice = 0;

    // Create the depth stencil view.
    result = DeviceInterface->CreateDepthStencilView(DepthStencilBuffer, &depthStencilViewDesc, &DepthStencilView);

    DeviceContext->OMSetRenderTargets(1, &RenderTarget, DepthStencilView);
}
void GraphicsSystem::CreateRasterState()
{
    HRESULT result;
    D3D11_RASTERIZER_DESC rasterDesc;
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

void GraphicsSystem::SetRenderTargetView(DXTexture2D* target)
{
    HRESULT result;
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
    // Initailze the depth stencil view.
    // Bind the render target view and depth stencil buffer to the output render pipeline.

    // set the render target as the back buffer
    DeviceContext->OMSetRenderTargets(1, &RenderTarget, NULL);


}

void GraphicsSystem::SetWindowAsViewport(WindowSystem* window)
{
    // Set the viewport to the size of the window
    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

    Integer2 Dimensions = Window->GetDimensions();

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = static_cast<float>(Dimensions.x);
    viewport.Height = static_cast<float>(Dimensions.y);
    viewport.MaxDepth = 1;
    viewport.MinDepth = 0;

    //number of viewports being used,
    //pointer to array of vieports.
    DeviceContext->RSSetViewports(1, &viewport);
}

void GraphicsSystem::CalculateProjectionMatrix()
{
    // Create the projection matrix for 3D rendering.
    //ProjectionMatrix = XMMatrixPerspectiveFovLH(FieldOfView, static_cast<float>(Window->GetDimensions().x) / static_cast<float>(Window->GetDimensions().y), NearPlane, FarPlane);
}
void GraphicsSystem::CalculateWorldMatrix()
{
    // Initialize the world matrix to the identity matrix.
    //WorldMatrix = XMMatrixIdentity();
}
void GraphicsSystem::CalculateOrthographicMatrix()
{
    // Create an orthographic projection matrix for 2D rendering.
    //OrthographicMatrix = XMMatrixOrthographicLH((float)Window->GetDimensions().x, (float)Window->GetDimensions().y, NearPlane, FarPlane);
}
void GraphicsSystem::CalculateViewMatrix()
{
    XMFLOAT3 up, position, lookAt;
    float yaw, pitch, roll;
    DXMatrix rotationMatrix;
    XMVECTOR Up, Position, LookAt;

    // Setup the vector that points upwards.
    up.x = 0.0f;
    up.y = 1.0f;
    up.z = 0.0f;

    Up = XMLoadFloat3(&up);

    // Setup the position of the camera in the world.
    position.x = 0;
    position.y = 0;
    position.z = -10;
    Position = XMLoadFloat3(&position);
    // Setup where the camera is looking by default.
    lookAt.x = 0.0f;
    lookAt.y = 0.0f;
    lookAt.z = 1.0f;
    LookAt = XMLoadFloat3(&lookAt);
    // Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
    pitch = 0 * 0.0174532925f;
    yaw = 0 * 0.0174532925f;
    roll = 0 * 0.0174532925f;

    // Create the rotation matrix from the yaw, pitch, and roll values.
    rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

    // Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
    LookAt = XMVector3TransformCoord(LookAt, rotationMatrix);
    Up = XMVector3TransformCoord(Up, rotationMatrix);

    // Translate the rotated camera position to the location of the viewer.
    LookAt = Position + LookAt;

    // Finally create the view matrix from the three updated vectors.
    //ViewMatrix = XMMatrixLookAtLH(Position, LookAt, Up);
}

void GraphicsSystem::InitializePipeline()
{

    //Should initialize them all;
    VertexShader* VS = ResourceManager::FindResourceType<VertexShader>("VertexTest.vert");
    PixelShader* PS = ResourceManager::FindResourceType<PixelShader>("PixelTest.pix");
    DeviceInterface->CreateVertexShader(VS->StoredShader->GetBufferPointer(), VS->StoredShader->GetBufferSize(), NULL, &pVS);
    DeviceInterface->CreatePixelShader(PS->StoredShader->GetBufferPointer(), PS->StoredShader->GetBufferSize(), NULL, &pPS);

    // set the shader objects
    DeviceContext->VSSetShader(pVS, nullptr, 0);
    DeviceContext->PSSetShader(pPS, nullptr, 0);
    

    //The element description
    D3D11_INPUT_ELEMENT_DESC inputLayout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    DeviceInterface->CreateInputLayout(inputLayout, ARRAYSIZE(inputLayout), VS->StoredShader->GetBufferPointer(), VS->StoredShader->GetBufferSize(), &InputLayout);
    DeviceContext->IASetInputLayout(InputLayout);


    //Perhaps I can add this to the GraphicsSystemClass
    // this can be part of the Init_Pipeline() function
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(VSBufferDefault) + sizeof(VSBufferDefault) % 16; //Must always be a multiple of 16
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    
    DeviceInterface->CreateBuffer(&bd, NULL, &VSConstantBuffer);
    DeviceContext->VSSetConstantBuffers(0, 1, &VSConstantBuffer);

    //INDEX BUFFER
    // Fill in a buffer description.
    D3D11_BUFFER_DESC bufferDesc;
    ZeroMemory(&bufferDesc, sizeof(bufferDesc));

    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = sizeof(unsigned int) * 3 * 2;
    bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;

    // Create indices.
    unsigned int indices[] = { 0, 1, 2, 0, 2, 3};
    // Define the resource data.
    D3D11_SUBRESOURCE_DATA InitData;
    InitData.pSysMem = indices;
    InitData.SysMemPitch = 0;
    InitData.SysMemSlicePitch = 0;

    // Create the buffer with the device.
    DeviceInterface->CreateBuffer(&bufferDesc, &InitData, &IndexBuffer);
    // Set the buffer.
    DeviceContext->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
    
}


void GraphicsSystem::AddGraphicsComponent(GraphicsComponent* comp, const DXBufferDescription& bufferDesc)
{
    GraphicsComponents.push_back(comp);
    comp->SetVertexBuffer(CreateBuffer(bufferDesc));
}
void GraphicsSystem::RemoveGraphicsComponent(GraphicsComponent* comp)
{
    GraphicsComponents.erase_value(comp);
    DestroyBuffer(comp->GetVertexBuffer());
}


DXBuffer* GraphicsSystem::CreateBuffer(const DXBufferDescription& desc)
{
    DXBuffer* buffer;
    DeviceInterface->CreateBuffer(&desc, NULL, &buffer);       // create the buffer
    switch (desc.BindFlags)
    {
        case D3D11_BIND_VERTEX_BUFFER:
        {
            VertexBuffers.push_back(buffer);
        }break;
    }
    return buffer;
}

void GraphicsSystem::DestroyBuffer(DXBuffer* buffer)
{
    DXBufferDescription desc;
    buffer->GetDesc(&desc);

    switch (desc.BindFlags)
    {
        case D3D11_BIND_VERTEX_BUFFER:
        {
            VertexBuffers.erase_value(buffer);
        }break;
    }
    
    ReleaseCOM(buffer);
}

void GraphicsSystem::DrawDebugTriangle()
{
    //WILL CAUSE A MEMORY LEAK!
    // create the vertex buffer
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
    bd.ByteWidth = sizeof(Vertex) * 3;             // size is the VERTEX struct * 3
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer
    DXBuffer* buffer = CreateBuffer(bd);

    Vertex OurVertices[] =
    {
        { Real3(0.0f, 0.5f, 1.0f), Real4(1.0f, 0.0f, 0.0f, 1.0f) },
        { Real3(0.45f, -0.5, 1.0f), Real4(0.0f, 1.0f, 0.0f, 1.0f) },
        { Real3(-0.45f, -0.5f, 1.0f), Real4(0.0f, 0.0f, 1.0f, 1.0f) },
    };

    // copy the vertices into the buffer
    D3D11_MAPPED_SUBRESOURCE ms;
    DeviceContext->Map(buffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
    memcpy(ms.pData, OurVertices, sizeof(OurVertices));                      // copy the data
    DeviceContext->Unmap(buffer, NULL);                                      // unmap the buffer

    

}

void GraphicsSystem::RenderFrame(UpdateEvent* event)
{

    // clear the back buffer to the clearcolor
    float color[4] = { ClearColor.x, ClearColor.y, ClearColor.z, ClearColor.w };
    DeviceContext->ClearRenderTargetView(RenderTarget, color);


    VSBufferDefault WorldViewProj;
    
    WorldViewProj.WorldViewProjection = GraphicsComponents[0]->Owner->Transform->GetWorldMatrix() * MainCamera->ViewProjectionMatrix();
    //MainCamera->ViewMatrix() * MainCamera->ProjectionMatrix();
    WorldViewProj.WorldViewProjection.Transpose();
    
    unsigned vertexCount = 0;
    for (unsigned i = 0; i < GraphicsComponents.size(); ++i)
    {
        vertexCount += GraphicsComponents[i]->GetVertexCount();
    }

    // set the new values for the constant buffer
    DeviceContext->UpdateSubresource(VSConstantBuffer, 0, 0, &WorldViewProj, 0, 0);
    //Prefer map/unmap ^
    

    

    // Clear the depth buffer.
    //DeviceContext->ClearDepthStencilView(DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
    
    // do 3D rendering on the back buffer here
    //This is the primary bottleneck
    unsigned stride[2] = { sizeof(Vertex), sizeof(Vertex) };
    unsigned offset[2] = { 0, 0 };
    DeviceContext->IASetVertexBuffers(0, VertexBuffers.size(), VertexBuffers.data(), stride, offset);
    DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    

    //DeviceContext->Draw(3, 0);
    
    //DeviceContext->IASetVertexBuffers(0, 1, &VertexBuffers[1], stride, offset);
    //DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    // Set the buffer.
    //DeviceContext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

    DeviceContext->DrawIndexed(6, 0, 0);

    WorldViewProj.WorldViewProjection = GraphicsComponents[1]->Owner->Transform->GetWorldMatrix() * MainCamera->ViewProjectionMatrix();
    //MainCamera->ViewMatrix() * MainCamera->ProjectionMatrix();
    WorldViewProj.WorldViewProjection.Transpose();
    DeviceContext->UpdateSubresource(VSConstantBuffer, 0, 0, &WorldViewProj, 0, 0);
    DeviceContext->DrawIndexed(6, 0, 0);
    //DeviceContext->DrawIndexedInstanced(3, 2, 0, 0, 0);
    //DeviceContext->Draw(3, 3);
    /*HRESULT result;
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    MatrixBufferType* dataPtr;
    
    unsigned int bufferNumber;*/

    

    ////// Set the position of the constant buffer in the vertex shader.
    //bufferNumber = 0;

    ////// Finanly set the constant buffer in the vertex shader with the updated values.
    //DeviceContext->VSSetConstantBuffers(bufferNumber, 1, &MatrixBuffer);

    ////// Set the vertex input layout.
    //DeviceContext->IASetInputLayout(InputLayout);


    //// Set the vertex and pixel shaders that will be used to render this triangle.

    //// Render the triangle.
    //DeviceContext->DrawIndexed(3, 0, 0);


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

    EventSend((EventHandler*)GameSession->Children[0].Dereference(), Events::FrameUpdate, event);
}
void GraphicsSystem::Uninitialize()
{

}

void GraphicsSystem::Destroy()
{
    EventDisconnect(GameSession, this, Events::EngineUpdate, this);
    // close and release all existing COM objects
    ReleaseCOM(pVS);
    ReleaseCOM(pPS) //TO BE MADE HASMAPS

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
    ReleaseCOM(RasterState);
    ReleaseCOM(DepthStencilView);
    ReleaseCOM(DepthStencilState);
    ReleaseCOM(DepthStencilBuffer);
    ReleaseCOM(RenderTarget);
    ReleaseCOM(DeviceContext);
    ReleaseCOM(DeviceInterface);
    ReleaseCOM(SwapChain);
    ReleaseCOM(InputLayout);
    ReleaseCOM(IndexBuffer);
    ReleaseCOM(VSConstantBuffer);
    ReleaseCOM(PSConstantBuffer);
    VertexBuffers.clear();
}