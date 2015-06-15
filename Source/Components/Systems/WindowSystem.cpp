#include <Precompiled.h>
#include "WindowSystem.h"


DefineType(WindowSystem, SinningZilch)
{
    BindComponent(WindowSystem);
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
    screenMode = Windowed;
    SerializeValueName(settings, screenMode, "ScreenMode");
    CursorVisible = true;
    SerializeValue(settings, CursorVisible);
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
    int currentMode = screenMode;
    screenMode = 0;
    SetFullscreen(currentMode);
    DesktopResolution = GraphicsManager::GetDesktopResolution(handle);
}

void WindowSystem::Initialize()
{
    
}

void WindowSystem::Uninitialize()
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

void WindowSystem::SetDimensions(const Integer2& dimensions)
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




void WindowSystem::Destroy()
{
    PostQuitMessage(0);
    DestroyWindow(handle);
    handle = nullptr;
}