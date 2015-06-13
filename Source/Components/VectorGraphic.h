#pragma once
#include "Component.h"
#include "GraphicsSystem.h"
#include "GraphicsComponent.h"

class VectorGraphic : public GraphicsComponent
{
    ZilchDeclareDerivedType(VectorGraphic, Component);
    void Serialize(DataNode* node) override; //Serialize this through zilch
    void Create() override;
    void Initialize() override;

    void Update(UpdateEvent* event);

    void Uninitialize() override;
    void Destroy() override;

private:
    VSBufferDefault vertexMods;

    
};