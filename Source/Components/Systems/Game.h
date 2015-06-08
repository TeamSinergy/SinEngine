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

    ComponentPointer(WindowSystem);

    
    void Uninitialize() override;
    void Destroy() override;
    ~Game();
private:
    HashMap < String, Space* >  Spaces;
};