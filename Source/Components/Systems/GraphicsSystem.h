#pragma once
#include <Precompiled.h>
#include "SinEntity.h"
#include "WindowSystem.h"
#include "Camera.h"

class UpdateEvent;
class WindowSystem;
class VectorGraphic;
class GraphicsComponent;

namespace Math
{
    template <typename T>
    static const XMVECTOR& ArrayToXMVector(const T& array)
    {
        return *(XMVECTOR*)&array;
    }

    template <typename T>
    static const T& XMVectorToArray(const XMVECTOR& xmvector)
    {
        return *(T*)&xmvector;
    }

    //template <typename To, typename From>
    //static const T& ArrayToXM(const From& xmvector)
    //{
    //    return *(To*)&xmvector;
    //}

    //template <typename To, typename From>
    //static const T& XMToArray(const From& xmvector)
    //{
    //    return *(To*)&xmvector;
    //}
};

struct Vertex
{
    Vertex(Real3 pos = Real3(), Real4 color = Real4(1, 1, 1, 1)) : Position(pos), Color(color){}
    Real3 Position;      // position
    Real4 Color;         // color
};

struct VSBufferDefault
{
    //Real3 Translation = Real3();
    //Real4 Color = Real4(1,1,1,1);
    Math::Matrix4 WorldViewProjection;
};
typedef D3D11_BUFFER_DESC DXBufferDescription;
typedef D3D11_VIEWPORT DXViewport;

class GraphicsSystem : public Component
{
public:
    static Array<DXViewport> AllViewports;

    ZilchDeclareDerivedType(GraphicsSystem, Component);
    void Serialize(DataNode* node) override; //DataLevel
    void Create() override;
    void Initialize() override;
    
    
    void SetPrimaryRenderTargetView();
    
    //void SetWindowAsViewport(WindowSystem* window);
    void OnLevelLoaded(EventData* event);
    void UpdateCameraViewports();

    void Uninitialize() override;
    void Destroy() override;

    
    

    DXDeviceInterface* GetDevice() const { return DeviceInterface; }
    DXDeviceContext* GetDeviceContext() const { return DeviceContext; }
    DXSwapChain* GetSwapChain() const { return SwapChain; }
    DXRenderTargetView* GetPrimaryRenderTarget() const { return RenderTarget; }
    bool IsVSyncEnabled() const { return VSync; }
    Camera* GetMainCamera() const { return MainCamera; }
    unsigned GetColorMode() const { return ColorMode; }
    /*DXRenderTargetView* GetPrimaryRenderTarget() const { return RenderTarget; }*/

    Camera* MainCamera;
private:
    WindowSystem* Window;
    //DIRECTX11
    int ColorMode = ColorMode::RGBa;
    int SampleRate;
    int QualityLevel;
    bool DebugMode;
    bool VSync;
    bool AntiAliasedLines;
    int ScalingMode;

    int VideoCardMemory;
    String VideoCardDescription;
    Integer2 RefreshRate;

    //FUNCTIONS
    void GetDeviceInformation();
    void CreateDeviceAndSwapChain();
    
    void CreateDepthStencilBuffer();
    void CreateDepthStencilState();
    void CreateDepthStencilView();
    void CreateRasterState();
    //SetRendertTarget
    //SetWindowAsViewport
    ///*void CalculateProjectionMatrix();
    //void CalculateWorldMatrix();
    //void CalculateOrthographicMatrix();
    //void CalculateViewMatrix();*/
    //void InitializePipeline();
    //DrawTriangle
    //RenderFrame
    
    DXDeviceInterface* DeviceInterface;
    DXDeviceContext* DeviceContext;
    DXFeatureLevel FeatureLevel; //Version of DX
    DXSwapChain* SwapChain;

    DXRenderTargetView* RenderTarget; //The Back-Buffer.

    DXTexture2D* DepthStencilBuffer;
    DXDepthStencilState* DepthStencilState;
    DXDepthStencilView* DepthStencilView;
    DXRasterizerState* RasterState;
    
    
    
};

#define ReleaseCOM(obj) if(obj != nullptr){obj->Release(); obj = nullptr;}