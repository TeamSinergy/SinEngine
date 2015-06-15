#pragma once
#include "SinEntity.h"
#include "DataLevel.h"

class Game;
typedef Space ObjectSpace;

class Space : public GameObject
{
public:
    ZilchDeclareDerivedType(Space, GameObject);
    Space() :GameObject(){}
    Space(const String& name) : GameObject(name){}
    
    void InitializeSpace();

    void OnObjectCreated(EventData* data);
    void OnObjectDestroyed(EventData* data);

    void LoadLevel(DataLevel* level);
    
    void UninitializeSpace();

    const Array<Handle>& ObjectList = GetAllChildren();

    ~Space() {};
private:
    unsigned ObjectCount = 0;
    
};