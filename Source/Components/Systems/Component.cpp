#include <Precompiled.h>
#include "Component.h"
#include "GameObject.h"
#include "Space.h"
#include "Game.h"

DefineType(Component, SinningZilch)
{
    //type->HandleManager = ZilchManagerId(Zilch::PointerManager);
    BindComponent(Component);
    BindFieldGet(Owner);
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

DefineType(Animal, SinningZilch)
{
    BindFieldGet(Name);
}

DefineType(Dog, SinningZilch)
{
    //BindFieldGet(Name);
}