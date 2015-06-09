#pragma once
#include "SinEntity.h"
#include "DataLevel.h"

class Game;

class Space : public SinEntity
{
public:
    Space() :SinEntity(){};
    Space(const String& name) : SinEntity(name){}

    void Serialize(DataNode* node) override{}; //DataLevel
    void Create() override{};
    void Initialize() override{};
    void Update(){};

    void LoadLevel(DataLevel* level){};

    Game* GameSession;
    Array<GameObject*> ObjectList;


    void Uninitialize() override{};
    void Destroy() override{};
};