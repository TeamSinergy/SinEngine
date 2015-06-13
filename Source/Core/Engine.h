#pragma once
#include <Precompiled.h>
#include "Game.h"

class Engine : public SinEntity
{
public:
    ZilchDeclareDerivedType(Engine, SinEntity);
    void Serialize(DataNode* node) override; //DataLevel
    void Create() override;
    void Initialize() override;
    void Uninitialize() override;
    void Destroy() override;
private:
    Array<Handle> GameSessions;
};
