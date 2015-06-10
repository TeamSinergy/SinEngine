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
    None,
    FXAA,
    MSAA,
    MSAAx2,
    MSAAx3,
    MSAAx4,
    MSAAx5,
    
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

    WindowSystem() : Component() {};

    void Serialize(DataNode* node) override; //Serialize this through zilch
    void Create() override;
    void Initialize() override;
    void Uninitialize() override;
    void Destroy() override;

    void SetWidth(int width);
    void SetHeight(int height);
    

    void SetPosition(Integer2 position);

    void SetFullscreen(int screenMode);

    int GetScreenMode(){ return screenMode; }

    const Integer2& GetDimensions() const{ return Dimensions; }
    void SetDimensions(const Integer2&);

    const Integer2& GetDesktopResolution() const{ return DesktopResolution; }

    WindowRef GetHandle();
    

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

    int screenMode;

    bool Visible;
    bool Resizable;
    bool CursorVisible;
    
};
