#pragma once
#include <Precompiled.h>
#include "SinEntity.h"
#include "WindowSystem.h"

class UpdateEvent;
class WindowSystem;

class GraphicsSystem : public Component
{
    ZilchDeclareDerivedType(GraphicsSystem, Component);
    void Serialize(DataNode* node) override; //DataLevel
    void Create() override;
    void Initialize() override;
    
    //TO BE CHANGED
    void SetRenderTargetView(DXTexture2D* target = nullptr);
    //TO BE CHANGED
    void SetWindowAsViewport(WindowSystem* window);

    void InitializePipeline();

    void DrawDebugTriangle();


    void RenderFrame(UpdateEvent* event);
    void Uninitialize() override;
    void Destroy() override;

    
private:
    WindowSystem* Window;
    //DIRECTX11
    int ColorMode;
    Real4 ClearColor;
    int SampleRate;
    int QualityLevel;
    bool DebugMode;
    bool VSync;
    float NearPlane;
    float FarPlane;
    float FieldOfView;
    bool AntiAliasedLines;
    int ScalingMode;

    int VideoCardMemory;
    String VideoCardDescription;
    Integer2 RefreshRate;

    //FUNCTIONS
    void GetDeviceInformation();
    void CreateDeviceAndSwapChain();
    void CreateVertexBuffer();
    void CreateDepthStencilBuffer();
    void CreateDepthStencilState();
    void CreateDepthStencilView();
    void CreateRasterState();
    //SetRendertTarget
    //SetWindowAsViewport
    void CalculateProjectionMatrix();
    void CalculateWorldMatrix();
    void CalculateOrthographicMatrix();
    void CalculateViewMatrix();
    //void InitializePipeline();
    //DrawTriangle
    //RenderFrame
    DXDeviceInterface* DeviceInterface;
    DXDeviceContext* DeviceContext;
    DXFeatureLevel FeatureLevel;
    DXSwapChain* SwapChain;

    DXRenderTargetView* RenderTarget;

    DXVertexBuffer* VertexBuffer;
    DXInputLayout* InputLayout;

    DXTexture2D* DepthStencilBuffer;
    DXDepthStencilState* DepthStencilState;
    DXDepthStencilView* DepthStencilView;
    DXRasterizerState* RasterState;
    DXMatrix ProjectionMatrix;
    DXMatrix WorldMatrix;
    DXMatrix OrthographicMatrix;
    DXMatrix ViewMatrix;
    DXBuffer* MatrixBuffer;
    //MAKE A HASHMAP
    ID3D11VertexShader *pVS;    // the vertex shader
    ID3D11PixelShader *pPS;     // the pixel shader
};

#define ReleaseCOM(obj) if(obj != nullptr){obj->Release(); obj = nullptr;}