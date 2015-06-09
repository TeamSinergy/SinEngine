#pragma once
#include "SinEntity.h"

class GameObject : public SinEntity
{
public:
    GameObject() :SinEntity(){};
    GameObject(const String& name) : SinEntity(name){}

    ZilchDeclareDerivedType(GameObject, SinEntity);
    void Serialize(DataNode* node) override; //DataLevel
    void Create() override;
    void Initialize() override;
    void Uninitialize() override;
    void Destroy() override;
protected:
    Array<Handle> Components;
    Array<GameObject*> Children;
    GameObject* Parent;
};