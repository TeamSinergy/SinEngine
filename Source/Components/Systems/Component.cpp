#include <Precompiled.h>
#include "Component.h"
#include "GameObject.h"
#include "ObjectSpace.h"
#include "Game.h"

DefineType(Component, SinningZilch)
{
  BindConstructor();
  BindDestructor();
  BindMethod(Serialize);
  BindMethod(Create);
  BindMethod(Initialize);
  BindMethod(Uninitialize);
  BindMethod(Destroy);
}

void Component::Serialize(DataNode* node)
{
    
}
void Component::Create()
{
    
}
void Component::Initialize()
{

}
void Component::Uninitialize()
{

}
void Component::Destroy()
{

}