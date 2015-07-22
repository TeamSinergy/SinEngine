#include <Precompiled.h>
#include "CameraViewport.h"



DefineType(CameraViewport, SinningZilch)
{
    BindComponent(CameraViewport);
}

void CameraViewport::Serialize(DataNode* node)
{
    Owner->CameraViewport = this;
    DataComponent* data = static_cast<DataComponent*>(node);
    SerializeValue(data, AspectRatio);
    SerializeValue(data, ScalingMode);
    SerializeValue(data, MarginColor);

    
}
void CameraViewport::Create()
{
    
    AssignDependancy(Window, GameSession->WindowSystem);
    AssignDependancy(Graphics, GameSession->GraphicsSystem);
    
    
    GraphicsSystem::AllViewports.push_back();
    Viewport = &GraphicsSystem::AllViewports.back();
    ZeroMemory(Viewport, sizeof(D3D11_VIEWPORT));

    Integer2 Dimensions = Window->GetDimensions();
    float Aspect = static_cast<float>(AspectRatio.y) / static_cast<float>(AspectRatio.x);

    Viewport->TopLeftX = 0;
    Viewport->TopLeftY = 0;
    Viewport->Width = static_cast<float>(Dimensions.x);
    Viewport->Height = static_cast<float>(Dimensions.y);
    Viewport->MaxDepth = 1;
    Viewport->MinDepth = 0;

    
}
void CameraViewport::Initialize()
{
    
}

void CameraViewport::Uninitialize()
{

}
void CameraViewport::Destroy()
{
    GraphicsSystem::AllViewports.pop_back();
}
