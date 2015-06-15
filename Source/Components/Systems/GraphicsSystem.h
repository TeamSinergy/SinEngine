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

    void AddGraphicsComponent(GraphicsComponent* comp, const DXBufferDescription& bufferDesc);
    void RemoveGraphicsComponent(GraphicsComponent* comp);
    DXBuffer* CreateBuffer(const DXBufferDescription& desc);
    void DestroyBuffer(DXBuffer* buffer);

    DXDeviceInterface* GetDevice() const { return DeviceInterface; }
    DXDeviceContext* GetDeviceContext() const { return DeviceContext; }

    Camera* MainCamera;
private:
    WindowSystem* Window;
    //DIRECTX11
    int ColorMode;
    Real4 ClearColor;
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
    void CalculateProjectionMatrix();
    void CalculateWorldMatrix();
    void CalculateOrthographicMatrix();
    void CalculateViewMatrix();
    //void InitializePipeline();
    //DrawTriangle
    //RenderFrame
    
    DXDeviceInterface* DeviceInterface;
    DXDeviceContext* DeviceContext;
    DXFeatureLevel FeatureLevel; //Version of DX
    DXSwapChain* SwapChain;

    DXRenderTargetView* RenderTarget; //The Back-Buffer.

    Array<GraphicsComponent*> GraphicsComponents; //For all the graphics components we are drawing. Maybe array of arrays?
    Array<DXBuffer*> VertexBuffers; //For all the stored vertices.
    DXBuffer* VSConstantBuffer; //For all the user-defined variables in the VertexShader.
    DXBuffer* PSConstantBuffer; //For all the user-defined variables in the PixelShader.
    DXBuffer* MatrixBuffer; //No idea yet

    DXInputLayout* InputLayout; //This holds the layout of our Vertex struct.

    DXTexture2D* DepthStencilBuffer;
    DXDepthStencilState* DepthStencilState;
    DXDepthStencilView* DepthStencilView;
    DXRasterizerState* RasterState;
    
    
    //MAKE A HASHMAP
    ID3D11VertexShader *pVS;    // the vertex shader
    ID3D11PixelShader *pPS;     // the pixel shader
};

#define ReleaseCOM(obj) if(obj != nullptr){obj->Release(); obj = nullptr;}