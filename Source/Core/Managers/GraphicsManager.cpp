#include <Precompiled.h>
#include "GraphicsManager.h"
#include "ResourceManager.h"



Window* GraphicsManager::CreateGameWindow(EngineInstance instance, DataComponent* settings, WindowStyles style)
{
    Window* window = new Window(instance, settings, style);
    ScreenMode::Fullscreen;
    return window;
}

LRESULT CALLBACK WindowProc(WindowRef window, UINT message, WPARAM  windowParams, LPARAM  longParams)
{
    // sort through and find what code to run for the message given
    switch (message)
    {
        // this message is read when the window is closed
        case WM_DESTROY:
        {
            // close the application entirely
            PostQuitMessage(0);
            return 0;
        } break;
    }

    // Handle any messages the switch statement didn't
    return DefWindowProc(window, message, windowParams, longParams);
}

Integer2 GraphicsManager::GetDesktopResolution(WindowRef window)
{
    //get the nearest moniter;
    HMONITOR actualDesktop = MonitorFromWindow(window, MONITOR_DEFAULTTONEAREST);

    MONITORINFO info;
    info.cbSize = sizeof(MONITORINFO);
    GetMonitorInfo(actualDesktop, &info);
    
    return Integer2(info.rcMonitor.right - info.rcMonitor.left, info.rcMonitor.bottom - info.rcMonitor.top);
}

Integer2 GraphicsManager::GetDesktopCenter(WindowRef window)
{
    //get the nearest moniter;
    HMONITOR actualDesktop = MonitorFromWindow(window, MONITOR_DEFAULTTONEAREST);

    MONITORINFO info;
    info.cbSize = sizeof(MONITORINFO);
    GetMonitorInfo(actualDesktop, &info);

    return Integer2(info.rcMonitor.left + (info.rcMonitor.right / 2), info.rcMonitor.top + (info.rcMonitor.bottom/2));
}

Integer2 GraphicsManager::GetDesktopOrigin(WindowRef window)
{
    //get the nearest moniter;
    HMONITOR actualDesktop = MonitorFromWindow(window, MONITOR_DEFAULTTONEAREST);

    MONITORINFO info;
    info.cbSize = sizeof(MONITORINFO);
    GetMonitorInfo(actualDesktop, &info);

    return Integer2(info.rcMonitor.left, info.rcMonitor.top);
}

Window::Window(EngineInstance instance, DataComponent* settings, WindowStyles style)
{

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

    ShowCursor(CursorVisible);
        
    unsigned winStyle = style;
    if (!Resizable)
    {
        winStyle = style ^ WS_THICKFRAME;
    }

    Icon* icon = ResourceManager::FindResourceType<Icon>(IconPath);

    ZeroMemory(&info, sizeof(info));
    
    info.cbSize = sizeof(WindowInfo);
    info.style = CS_HREDRAW | CS_VREDRAW; //Redraw the window if it is resized
    info.lpfnWndProc = WindowProc;
    info.hInstance = instance;
    info.hCursor = LoadCursor(NULL, IDC_ARROW);
    info.hbrBackground = (HBRUSH)COLOR_WINDOW; //The background style and color
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
        xPosition,    // x-position of the window
        yPosition,    // y-position of the window
        wr.right - wr.left,    // width of the window
        wr.bottom - wr.top,    // height of the window
        NULL,    // we have no parent window, NULL
        NULL,    // we aren't using menus, NULL
        instance,    // application handle
        NULL);    // used with multiple windows, NULL

    ErrorIf(handle == nullptr, "Failed to create window %s", Name.c_str());
    //GetClassInfoEx(instance, WindowClassName.c_str(), handle);
    // display the window on the screen
    ShowWindow(handle, Visible);
    SetFullscreen(fullscreen);

    //EnableWindow(handle, true);
    //SetFocus(handle);
    //SetCapture(handle);
    
}

void Window::SetWidth(int width)
{
    RECT wr = { 0, 0, width, Dimensions.y };
    AdjustWindowRect(&wr, info.style, false);
    SetWindowPos(handle, 0, 0, 0, wr.right - wr.left, wr.bottom - wr.top, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
    Dimensions.x = width;
}
void Window::SetHeight(int height)
{
    RECT wr = { 0, 0, Dimensions.x, height };
    AdjustWindowRect(&wr, info.style, false);
    SetWindowPos(handle, 0, 0, 0, wr.right - wr.left, wr.bottom - wr.top, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
    Dimensions.y = height;
}

void Window::SetDimensions(Integer2 dimensions)
{
    RECT wr = { 0, 0, dimensions.x, dimensions.y };
    AdjustWindowRect(&wr, info.style, false);
    SetWindowPos(handle, 0, 0, 0, wr.right - wr.left, wr.bottom - wr.top, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
    Dimensions = dimensions;
}

void Window::SetPosition(Integer2 position)
{
    SetWindowPos(handle, 0, position.x, position.y, Dimensions.x, Dimensions.y, SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER);
}

void Window::SetFullscreen(int newScreenMode)
{
    if (screenMode == newScreenMode)
    {
        return;
    }
    int style;
    
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
            //graphics card stuff
        }break;
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