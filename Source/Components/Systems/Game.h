#pragma once
#include "SinEntity.h"
#include "GameObject.h"
#include "WindowSystem.h"
#include "Space.h"
class Game : public GameObject
{
public:
    ZilchDeclareDerivedType(Game, SinEntity);
    void Serialize(DataNode* node) override; //DataLevel
    void Create() override;
    void Initialize() override;
    void Update();

    void LoadLevel(DataLevel* level);
    void UnloadLevel(const String& levelName);

    ComponentPointer(WindowSystem);

    
    void Uninitialize() override;
    void Destroy() override;
    ~Game();
private:
    HashMap < String, Space* >  Spaces;
};

class UpdateEvent : public EventData
{
public:
    UpdateEvent(float dt = 0.0f) : EventData(), Dt(dt) {}
    float Dt;
};