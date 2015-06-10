#pragma once
#include "SinEntity.h"

class Game;
class ObjectSpace;
class GameObject;

class Component : public SinEntity
{
public:
    ZilchDeclareDerivedType(Component, SinEntity);
    void Serialize(DataNode* node) override; //Serialize this through zilch
    void Create() override;
    void Initialize() override;
    void Uninitialize() override;
    void Destroy() override;

    GameObject* Owner;
    ObjectSpace* Space;
    Game* GameSession;
private:
};