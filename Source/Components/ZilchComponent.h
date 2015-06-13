#pragma once
#include "SinEntity.h"

class ZilchComponent : public Component
{
    ZilchDeclareDerivedType(ZilchComponent, Component);
    void Serialize(DataNode* node) override; //Serialize this through zilch
    void Create() override;
    void Initialize() override;
    void Uninitialize() override;
    void Destroy() override;

private:
    
};