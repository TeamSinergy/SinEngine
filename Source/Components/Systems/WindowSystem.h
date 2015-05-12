#pragma once
#include <Precompiled.h>
#include <windowsx.h>  //For creating a window
#include "GraphicsManager.h"
#include "Component.h"

using namespace DirectX;

//GraphicsTypedefs
typedef IDXGISwapChain1 DXSwapChain;
typedef ID3D11Device DXDeviceInterface;
typedef ID3D11DeviceContext DXDeviceContext;
typedef D3D_FEATURE_LEVEL DXFeatureLevel;
typedef ID3D11RenderTargetView DXRenderTargetView;
typedef ID3D11Texture2D DXTexture2D;
typedef ID3D11Buffer DXVertexBuffer;
typedef ID3D11InputLayout DXInputLayout;
typedef XMMATRIX DXMatrix;
typedef ID3D11DepthStencilState DXDepthStencilState;
typedef ID3D11DepthStencilView DXDepthStencilView;
typedef ID3D11RasterizerState DXRasterizerState;
typedef ID3D11Buffer DXBuffer;

struct MatrixBufferType
{
	DXMatrix world;
	DXMatrix view;
	DXMatrix projection;
};

enum AntiAiliasingModes
{
    FXAA,
    MSAA,
    MSAAx2,
    MSAAx3,
    MSAAx4,
    None
};

struct Vertex
{
    Vertex(Real3 pos, Real4 color) : Position(pos), Color(color){}
    Real3 Position;      // position
    Real4 Color;         // color
};

class WindowSystem : public Component
{
public:
    ZilchDeclareDerivedType(WindowSystem, Component);

    void Serialize(DataNode* node) override; //Serialize this through zilch
    void Create() override;
    void Initialize() override;
    void Uninitialize() override;
    void Destroy() override;

    void SetWidth(int width);
    void SetHeight(int height);
    void SetDimensions(Integer2 dimesions);

    void SetPosition(Integer2 position);

    void SetFullscreen(int screenMode);

    int GetScreenMode(){ return screenMode; }

    Integer2 GetDimensions(){ return Dimensions; }

    WindowRef GetHandle();

    //DX11 functions
    void InitializeDX11();
    //TO BE CHANGED
    void SetRenderTargetView(DXTexture2D* target = nullptr);
    //TO BE CHANGED
    void SetWindowAsViewport(WindowSystem* window);

    void InitializePipeline();

    void DrawDebugTriangle();


    void RenderFrame();
    void UninitializeDX11();
    


    ~WindowSystem(){};
private:
    String Name;
    String WindowClassName;

    WindowRef handle;
    WindowInfo info;

    String IconPath;

    Integer2 Position;

    Integer2 Dimensions;
    Integer2 DesktopResolution;
    Integer2 defaultSize;

    Real4 ClearColor;

    int screenMode;

    bool Visible;
    bool Resizable;
    bool CursorVisible;

    //DIRECTX11
    int ColorMode;
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
