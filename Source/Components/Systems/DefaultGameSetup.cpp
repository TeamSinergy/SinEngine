#include <Precompiled.h>
#include "DefaultGameSetup.h"

DefineType(DefaultGameSetup, SinningZilch)
{
    RegisterComponent(DefaultGameSetup);
    BindConstructor();
    BindDestructor();
}

void DefaultGameSetup::Serialize(DataNode* node)
{
    Owner->DefaultGameSetup = this;
    DataComponent* data = static_cast<DataComponent*>(node);
    SerializeValue(data, SpaceArchetype);
    SerializeValue(data, LevelName);
}
void DefaultGameSetup::Create()
{
    
    DataObject* spaceArchetype = ResourceManager::FindArchetype(SpaceArchetype);
    ErrorIf(spaceArchetype == nullptr, "Failed to find a space archetype with name %s.", SpaceArchetype.c_str());
    DataLevel* level = ResourceManager::FindLevel(LevelName);
    ErrorIf(level == nullptr, "Failed to find a level with name %s.", LevelName.c_str());
    GameSession->LoadSpace(spaceArchetype, level);
}
void DefaultGameSetup::Initialize()
{

}
void DefaultGameSetup::Uninitialize()
{

}
void DefaultGameSetup::Destroy()
{

}
