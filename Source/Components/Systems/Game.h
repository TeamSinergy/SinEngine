#pragma once
#include "SinEntity.h"
#include "GameObject.h"
#include "WindowSystem.h"
class Game : public GameObject
{
public:
    ZilchDeclareDerivedType(Game, SinEntity);
    void Serialize(DataNode* node) override; //DataLevel
    void Create() override;
    void Update();

    ComponentPointer(WindowSystem);

    void Initialize() override;
    void Uninitialize() override;
    void Destroy() override;
private:
};