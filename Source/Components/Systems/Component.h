#pragma once
#include <Precompiled.h>
#include "SinEntity.h"

#define Dependancy(comp) ErrorIf(comp == nullptr, "Component %s has a dependancy on Component %s.", Name.c_str(), String(#comp).Replace("Owner->", "").c_str())

class Game;
class Space;
class GameObject;

#define BindComponent(component) \
RegisterComponent(component);\
BindConstructor();\
BindMethod(Serialize);\
BindMethod(Create);\
BindMethod(Initialize);\
BindMethod(Uninitialize);\
BindMethod(Destroy);\
BindDestructor();

class Animal : public EventHandler
{
public:
    ZilchDeclareDerivedType(Animal, EventHandler);
    String Name;
};

class Dog : public Animal
{
public:
    ZilchDeclareDerivedType(Dog, Animal);
    
};

class Component : public SinEntity
{
public:
    ZilchDeclareDerivedType(Component, SinEntity);
    
    void Serialize(DataNode* node) override; //Serialize this through zilch
    void Create() override;
    void Initialize() override;

    void Uninitialize() override;
    void Destroy() override;

    GameObject* Owner;
    Space* Space;
    Game* GameSession;
};

