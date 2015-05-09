#include <Precompiled.h>
#include "GraphicsManager.h"
#include "ResourceManager.h"
#include "WindowSystem.h"

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

    return Integer2(info.rcMonitor.left + (info.rcMonitor.right / 2), info.rcMonitor.top + (info.rcMonitor.bottom / 2));
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

void GraphicsManager::NormalizeColor(Real4& color)
{
    color.x /= 255;
    color.y /= 255;
    color.z /= 255;
    //ALPHA STAYS AS 0 to 1 BECAUSE USABILITY
    
}

void GraphicsManager::NormalizeColor(Real4& color, int colorMode)
{
    if (colorMode == ColorMode::rgba)
    {
        return;
    }
    color.x /= 255;
    color.y /= 255;
    color.z /= 255;
    //ALPHA STAYS AS 0 to 1 BECAUSE USABILITY
    if (colorMode == ColorMode::RGBA)
    {
        color.w /= 255;
    }

}


