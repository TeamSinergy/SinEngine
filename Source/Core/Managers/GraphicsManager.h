#pragma once
#include <Precompiled.h>
#include <windowsx.h>  //For creating a window


typedef HWND WindowRef;
typedef WNDCLASSEX WindowInfo;
typedef HINSTANCE EngineInstance;
typedef WPARAM WindowParams;

//GraphicsTypedefs
typedef IDXGISwapChain DXSwapChain;
typedef ID3D11Device DXDeviceInterface;
typedef ID3D11DeviceContext DXDeviceContext;

LRESULT CALLBACK WindowProc(WindowRef window, UINT message, WPARAM  windowParams, LPARAM  longParams);

enum WindowStyles
{
    BorderedWindowStyle = WS_OVERLAPPEDWINDOW,
    BorderlessWindowStyle = WS_POPUP,
};



enum ScreenMode
{
    Fullscreen = 'F',//ConstantHash("Fullscreen") ,
    Windowed = 'W',
    BorderlessWindow = 'B',
    BorderedWindow = 'N',
};

class Window
{
public:
    Window(EngineInstance instance, DataComponent* settings, WindowStyles style);

    void SetWidth(int width);
    void SetHeight(int height);
    void SetDimensions(Integer2 dimesions);

    void SetPosition(Integer2 position);

    void SetFullscreen(int screenMode);

    WindowRef GetHandle();

    ~Window(){};
private:
    String Name;
    String WindowClassName;

    WindowRef handle;
    WindowInfo info;

    String IconPath;

    Integer2 Position;

    Integer2 Dimensions;
    Integer2 defaultSize;
    int screenMode;

    bool Visible;
    bool Resizable;
    bool CursorVisible;

};

class GraphicsManager
{
public:
    static Window* CreateGameWindow(EngineInstance instance, DataComponent* settings, WindowStyles style = WindowStyles::BorderedWindowStyle);
    static Integer2 GetDesktopResolution(WindowRef window);
    static Integer2 GetDesktopCenter(WindowRef window);
    static Integer2 GetDesktopOrigin(WindowRef window);

    static void InitializeDX11(Window* window);
    static void UninitializeDX11(Window* window);
private:
    static DXSwapChain* SwapChain;
    static DXDeviceInterface* DeviceInterface;
    static DXDeviceContext* DeviceContext;
};