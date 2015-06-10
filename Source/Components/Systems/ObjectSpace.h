#pragma once
#include "SinEntity.h"
#include "DataLevel.h"

class Game;

class ObjectSpace : public GameObject
{
public:
    ZilchDeclareDerivedType(ObjectSpace, GameObject);
    ObjectSpace() :GameObject(){};
    ObjectSpace(const String& name) : GameObject(name){}
    
    

    void LoadLevel(DataLevel* level){};
    
    const Array<Handle>& ObjectList = Children;


    
    ~ObjectSpace() {};
};