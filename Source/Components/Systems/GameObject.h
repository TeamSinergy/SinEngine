#pragma once
#include "SinEntity.h"

class GameObject : public SinEntity
{
public:
    GameObject() :GameObject(){};
    GameObject(const String& name) : GameObject(name){}

    ZilchDeclareDerivedType(GameObject, SinEntity);
    void Serialize(DataNode* node) override; //DataLevel
    void Create() override;
    void Initialize() override;
    void Uninitialize() override;
    void Destroy() override;
protected:
    Array<Handle> Components;
    GameObject* Parent;
};