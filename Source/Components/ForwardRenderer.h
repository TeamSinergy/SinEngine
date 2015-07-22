#pragma once
#include "SinEntity.h"
#include "GraphicsSpace.h"

enum ScalingModes
{
    Fill

};

class ForwardRenderer : public Component
{
    ZilchDeclareDerivedType(ForwardRenderer, Component);
    void Serialize(DataNode* node) override; //Serialize this through zilch
    void Create() override;
    void Initialize() override;
    void Uninitialize() override;
    void Destroy() override;

    void SetRenderTargetView(DXTexture2D* target = nullptr);

    void RenderSpace(UpdateEvent* event);

private:
    Real4 ClearColor = Real4(1, 1, 1, 1);

    const GraphicsSystem* Graphics;
    const GraphicsSpace* GraphicsSpace;

    DXRenderTargetView* RenderTarget;
};

