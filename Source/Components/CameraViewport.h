#pragma once
#include "SinEntity.h"
class CameraViewport : public Component
{

    ZilchDeclareDerivedType(CameraViewport, Component);
    void Serialize(DataNode* node) override; //Serialize this through zilch
    void Create() override;
    void Initialize() override;
    void Uninitialize() override;
    void Destroy() override;

private:
    WindowSystem* Window;
    GraphicsSystem* Graphics;

    DXViewport* Viewport;

    Integer2 AspectRatio = Integer2(1600, 900);
    unsigned ScalingMode = ScalingModes::Fill;
    Real4 MarginColor = Real4(1, 1, 1, 1);
    unsigned Layer = 0;
    Real2 NormalizedSize = Real2();
    Real2 NormalizedOffset = Real2();
};