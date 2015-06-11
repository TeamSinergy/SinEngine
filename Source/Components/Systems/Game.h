#pragma once
#include "SinEntity.h"
#include "GameObject.h"
#include "Space.h"

class Game : public GameObject
{
public:
    ZilchDeclareDerivedType(Game, SinEntity);
    
    
    void Update();

    void LoadSpace(DataObject* spaceArchetype, DataLevel* level);
    void UnloadSpace(const String& levelName);
    
      
    ~Game();
private:
};

class UpdateEvent : public EventData
{
public:
    UpdateEvent(float dt = 0.0f) : EventData(), Dt(dt) {}
    float Dt;
};