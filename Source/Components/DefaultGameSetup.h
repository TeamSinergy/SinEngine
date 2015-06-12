#pragma once
#include "SinEntity.h"

class DefaultGameSetup : public Component
{
    ZilchDeclareDerivedType(DefaultGameSetup, Component);
    void Serialize(DataNode* node) override; //Serialize this through zilch
    void Create() override;
    void Initialize() override;
    void Uninitialize() override;
    void Destroy() override;

private:
    String SpaceArchetype = "Space";
    String LevelName = "Level";
};