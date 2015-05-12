#pragma once
#include "SinEntity.h"
#include "DataLevel.h"
class Space : public GameObject
{
public:
    Space():GameObject(){};
    Space(const String& name) : GameObject(name){}

    void Serialize(DataNode* node) override{}; //DataLevel
    void Create() override{};
    void Initialize() override{};
    void Update(){};

    void LoadLevel(DataLevel* level){};

    


    void Uninitialize() override{};
    void Destroy() override{};
};