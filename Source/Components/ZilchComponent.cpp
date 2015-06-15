#include <Precompiled.h>
#include "ZilchComponent.h"

DefineType(ZilchComponent, SinningZilch)
{
    BindComponent(ZilchComponent);
    BindFieldGet(Owner);
    BindFieldGet(Space);
    BindFieldGet(GameSession);
}

void ZilchComponent::Serialize(DataNode* node)
{
    
}
void ZilchComponent::Create()
{
    
}
void ZilchComponent::Initialize()
{

}
void ZilchComponent::Uninitialize()
{

}
void ZilchComponent::Destroy()
{

}
