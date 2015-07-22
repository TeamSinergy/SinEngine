#include <Precompiled.h>
#include "Game.h"
#include "Space.h"

DefineType(Space, SinningZilch)
{
    BindConstructor();
    BindDestructor();
    BindMethod(Create);
    BindMethod(Initialize);
    BindMethod(Uninitialize);
    BindMethod(Destroy);
}

void Space::InitializeSpace()
{
    EventConnect(this, "ObjectCreated", &Space::OnObjectCreated, this);
    EventConnect(this, "ObjectDestroyed", &Space::OnObjectCreated, this);
}

void Space::LoadLevel(DataLevel* level)
{
    UpdateAllChildren(ObjectState::Uninitialize);
    UpdateAllChildren(ObjectState::Destroy);

    const Array<DataObject*>& ObjectData = level->AllObjects();
    Name = level->GetName();

    for (unsigned i = 0; i < ObjectData.size(); ++i)
    {
        Handle handle = ZilchAllocate<GameObject>();
        GameObject* obj = (GameObject*)handle.Dereference();
        obj->Name = ObjectData[i]->GetName();
        obj->Space = this;
        obj->GameSession = GameSession;
        obj->AttachTo(this);
        obj->Serialize(ObjectData[i]);
        EventSend(obj, "AllComponentsInitialized", GameObject::EmptyEventData);
    }
    
    UpdateAllChildren(ObjectState::Create);
    if (LastState >= ObjectState::Initialize)
    {
        UpdateAllChildren(ObjectState::Initialize);
    }

    EventSend(this, "LevelLoaded", GameObject::EmptyEventData);
    EventSend(GameSession, "LevelLoaded", GameObject::EmptyEventData);
}

void Space::UninitializeSpace()
{
    EventDisconnect(this, this, "ObjectCreated", this);
    EventDisconnect(this, this, "ObjectDestroyed", this);
}

void Space::OnObjectCreated(EventData* data)
{
    ++ObjectCount;
}
void Space::OnObjectDestroyed(EventData* data)
{
    --ObjectCount;
}