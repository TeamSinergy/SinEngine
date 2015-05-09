#pragma once
#include <Precompiled.h>
#include <windowsx.h>  //For creating a window

class WindowSystem;

typedef HWND WindowRef;
typedef WNDCLASSEX WindowInfo;
typedef HINSTANCE EngineInstance;
typedef WPARAM WindowParams;



LRESULT CALLBACK WindowProc(WindowRef window, UINT message, WPARAM  windowParams, LPARAM  longParams);

enum WindowStyles
{
    BorderedWindowStyle = WS_OVERLAPPEDWINDOW,
    BorderlessWindowStyle = WS_POPUP,
};

enum ColorMode
{
    RGBa,
    RGBA,
    rgba
};

enum ScreenMode
{
    Fullscreen = 'F',//ConstantHash("Fullscreen") ,
    Windowed = 'W',
    BorderlessWindow = 'B',
    BorderedWindow = 'N',
};


class GraphicsManager
{
public:
    static Integer2 GetDesktopResolution(WindowRef window);
    static Integer2 GetDesktopCenter(WindowRef window);
    static Integer2 GetDesktopOrigin(WindowRef window);

    static void NormalizeColor(Real4& color, int colorMode);
    static void NormalizeColor(Real4& color);

private:
};