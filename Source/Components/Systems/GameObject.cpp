#include <Precompiled.h>
#include "Space.h"
#include "DataLevel.h"
#include "Game.h"

DefineType(GameObject, SinningZilch)
{
  BindConstructor();
  BindDestructor();
  BindMethod(Create);
  BindMethod(Initialize);
  BindMethod(Uninitialize);
  BindMethod(Destroy);
  
}

Array<Type*>* GameObject::SerializeFunction = nullptr;
Array<Type*>* GameObject::Default = nullptr;
EventData* GameObject::EmptyEventData = nullptr;

void GameObject::Serialize(DataNode* node)
{
    LastState = ObjectState::Serialize;
    DataObject* objectData = static_cast<DataObject*>(node);
    const Array<DataComponent*>& ComponentsData = objectData->AllComponents();
    Name = objectData->GetName();

    for (unsigned i = 0; i < ComponentsData.size(); ++i)
    {
        String name = ComponentsData[i]->GetName();
        if (Utility::Components.containsKey(name))
        {
            Components.push_back(Utility::Components[name]->Create());
        }
        else
        {
            LibraryRef library = ZILCH->CompiledLib;
            //Bind the Zilch class
            BoundType* ZilchClass = library->BoundTypes.findValue(name, nullptr);
            ErrorIf(ZilchClass == nullptr, "Could not find a Zilch component with name %s.", name.c_str());

            ExecutableState* state = ZILCH->GetDependencies();
            Handle Component = state->AllocateDefaultConstructedHeapObject(ZilchClass, ZILCH->Report, Zilch::HeapFlags::ReferenceCounted);
            Components.push_back(Component);
        }
        ((Component*)Components.back().Dereference())->Name = name;
        ((Component*)Components.back().Dereference())->Owner = this;
        ((Component*)Components.back().Dereference())->Space = this->Space;
        ((Component*)Components.back().Dereference())->GameSession = this->GameSession;
        
        Function* ZilchSerialize = Components.back().Type->FindFunction("Serialize", *GameObject::SerializeFunction, ZilchTypeId(void), FindMemberOptions::None);
        ErrorIf(ZilchSerialize == nullptr, "Failed to find function 'Serialize' on Zilch type %s", name.c_str());
        {
            Zilch::Call call(ZilchSerialize, ZILCH->GetDependencies());
            call.SetHandle(Zilch::Call::This, Components.back());
            call.Set(0, ComponentsData[i]);
            call.Invoke(ZILCH->Report);
        }
        
    }
}
void GameObject::Create()
{
    if (Space)
    {
        EventSend(Space, "ObjectCreated", GameObject::EmptyEventData);
    }
    if (LastState >= ObjectState::Create || LastState == ObjectState::Destroy)
    {
        return;
    }
    LastState = ObjectState::Create;
    
    for (unsigned i = 0; i < Components.size(); ++i)
    {
        Function* ZilchSerialize = Components[i].Type->FindFunction("Create", *GameObject::Default, ZilchTypeId(void), FindMemberOptions::None);
        ErrorIf(ZilchSerialize == nullptr, "Failed to find function 'Create' on Zilch type %s", Components[i].Type->ToString().c_str());
        {
            Zilch::Call call(ZilchSerialize, ZILCH->GetDependencies());
            call.SetHandle(Zilch::Call::This, Components[i]);
            call.Invoke(ZILCH->Report);
        }
    }

    UpdateAllChildren(LastState);
}
void GameObject::Initialize()
{
    if (LastState >= ObjectState::Initialize)
    {
        return;
    }
    LastState = ObjectState::Initialize;
    
    for (unsigned i = 0; i < Components.size(); ++i)
    {
        Function* ZilchSerialize = Components[i].Type->FindFunction("Initialize", *GameObject::Default, ZilchTypeId(void), FindMemberOptions::None);
        ErrorIf(ZilchSerialize == nullptr, "Failed to find function 'Create' on Zilch type %s", Components[i].Type->ToString().c_str());
        {
            Zilch::Call call(ZilchSerialize, ZILCH->GetDependencies());
            call.SetHandle(Zilch::Call::This, Components[i]);
            call.Invoke(ZILCH->Report);
        }
    }

    UpdateAllChildren(LastState);
}

void GameObject::AttachTo(GameObject* parent)
{
    parent->AddChild(this);
}

void GameObject::AddChild(GameObject* child)
{
    child->Parent = this;
    Children.push_back(Handle(ZILCH->GetDependencies(), ZilchTypeId(GameObject), (byte*)child));
}

void GameObject::RemoveChild(GameObject* child)
{
    child->Parent = nullptr;
    Children.erase_value(Handle(ZILCH->GetDependencies(), ZilchTypeId(GameObject), (byte*)child));
}

GameObject* GameObject::FindChildByName(const String& name)
{
    GameObject* child = nullptr;
    for (unsigned i = 0; i < Children.size(); ++i)
    {
        child = ((GameObject*)Children[i].Dereference());
        if (child->Name == name)
        {
            return child;
        }
    }
    return nullptr;
}

void GameObject::Uninitialize()
{
    if (LastState >= ObjectState::Uninitialize)
    {
        return;
    }
    LastState = ObjectState::Uninitialize;
    UpdateAllChildren(LastState);
    
    for (unsigned i = 0; i < Components.size(); ++i)
    {
        Function* ZilchSerialize = Components[i].Type->FindFunction("Uninitialize", *GameObject::Default, ZilchTypeId(void), FindMemberOptions::None);
        ErrorIf(ZilchSerialize == nullptr, "Failed to find function 'Uninitialize' on Zilch type %s", Components[i].Type->ToString().c_str());
        {
            Zilch::Call call(ZilchSerialize, ZILCH->GetDependencies());
            call.SetHandle(Zilch::Call::This, Components[i]);
            call.Invoke(ZILCH->Report);
        }
    }
}
void GameObject::Destroy()
{
    if (Space)
    {
        EventSend(Space, "ObjectDestroyed", GameObject::EmptyEventData);
    }
    
    if (LastState >= ObjectState::Destroy)
    {
        return;
    }
    LastState = ObjectState::Destroy;
    UpdateAllChildren(LastState);

    for (unsigned i = 0; i < Components.size(); ++i)
    {
        Function* ZilchSerialize = Components[i].Type->FindFunction("Destroy", *GameObject::Default, ZilchTypeId(void), FindMemberOptions::None);
        ErrorIf(ZilchSerialize == nullptr, "Failed to find function 'Destroy' on Zilch type %s", Components[i].Type->ToString().c_str());
        {
            Zilch::Call call(ZilchSerialize, ZILCH->GetDependencies());
            call.SetHandle(Zilch::Call::This, Components[i]);
            call.Invoke(ZILCH->Report);
        }
    }
}

void GameObject::UpdateAllChildren(ObjectState stateToCall)
{
    String functionName;
    switch (stateToCall)
    {
        case ObjectState::Create:
        {
            functionName = "Create";
        }break;
        case ObjectState::Initialize:
        {
            functionName = "Initialize";
        }break;
        case ObjectState::Uninitialize:
        {
            functionName = "Uninitialize";
        }break;
        case ObjectState::Destroy:
        {
            functionName = "Destroy";
        }break;
        default:
        {
            Error("Invalid State");
        }
    }

    for (unsigned i = 0; i < Children.size(); ++i)
    {
        GameObject* child = (GameObject*)Children[i].Dereference();
        if (child->LastState < stateToCall)
        {
            Function* ZilchSerialize = Children[i].Type->FindFunction(functionName, *GameObject::Default, ZilchTypeId(void), FindMemberOptions::None);
            ErrorIf(ZilchSerialize == nullptr, "Failed to find function '%s' on Zilch type %s", functionName, Children[i].Type->ToString().c_str());
            {
                Zilch::Call call(ZilchSerialize, ZILCH->GetDependencies());
                call.SetHandle(Zilch::Call::This, Children[i]);
                call.Invoke(ZILCH->Report);
            }
        }
    }
    if (stateToCall == ObjectState::Destroy)
    {
        Children.clear();
    }
}