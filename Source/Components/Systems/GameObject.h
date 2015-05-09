#pragma once
#include "SinEntity.h"

class GameObject : public SinEntity
{
public:
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