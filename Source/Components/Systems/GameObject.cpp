#include <Precompiled.h>
#include "Space.h"
#include "DataLevel.h"
#include "Game.h"
#include "DataComponent.h"


//#define MacroTest(x) ##x

DefineType(GameObject, SinningZilch)
{
    
    BindComponent(GameObject);
    BindMethod(FindComponentByName);
    BindVirtualConstructor(const String&);
    BindMethod(GetParent);
    BindMethod(AttachTo);
    ZilchBindProperty(builder, type, &GameObject::GetParent, &GameObject::AttachTo, "Parent");
    BindMethod(FindChildByName);
    BindMethod(AddChild);
    BindMethod(RemoveChild);
    BindMethodOverload(AddComponent, void, const Handle&);
    BindMethodOverload(AddComponent, void, const Handle&, DataComponent*);
    BindFieldGet(Space);
    BindFieldGet(GameSession);
    BindFieldGet(Transform);
	BindFieldGet(Sprite);
	BindFieldGet(Camera);
    BindMethod(Disconnect);
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

    //auto a = MacroTest(objectData);

    Handle currentComp;
    for (unsigned i = 0; i < ComponentsData.size(); ++i)
    {
        String name = ComponentsData[i]->GetName();
        if (Utility::Components.containsKey(name))
        {
            currentComp = Utility::Components[name]->Create();
        }
        else
        {
            LibraryRef library = ZILCH->CompiledLib;
            //Bind the Zilch class
            BoundType* ZilchClass = library->BoundTypes.findValue(name, nullptr);
            ErrorIf(ZilchClass == nullptr, "Could not find a Zilch component with name %s.", name.c_str());

            ExecutableState* state = ZILCH->GetDependencies();
            currentComp = state->AllocateDefaultConstructedHeapObject(ZilchClass, ZILCH->Report, Zilch::HeapFlags::ReferenceCounted);
            
        }

        ((Component*)currentComp.Dereference())->Name = name;
        AddComponent(currentComp, ComponentsData[i]);
        
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
    
    UpdateAllComponents(LastState);
    UpdateAllChildren(LastState);
}
void GameObject::Initialize()
{
    if (LastState >= ObjectState::Initialize)
    {
        return;
    }
    LastState = ObjectState::Initialize;
    
    UpdateAllComponents(LastState);
    UpdateAllChildren(LastState);
}

void GameObject::Detach()
{
    if (Parent)
    {
        Parent->RemoveChild(this);
    }
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

Component* GameObject::FindComponentByName(const String& name)
{
    for (unsigned i = 0; i < Components.size(); ++i)
    {
        if (Components[i].Type->ToString().CompareTo(name) == 0)
        {
            return (Component*)Components[i].Dereference();
        }
    }

    return nullptr;
}

void GameObject::AddComponent(const Handle& comp, DataComponent* data)
{
    ((Component*)comp.Dereference())->Name = comp.Type->ToString();
    ((Component*)comp.Dereference())->Owner = this;
    ((Component*)comp.Dereference())->Space = this->Space;
    ((Component*)comp.Dereference())->GameSession = this->GameSession;
    Components.push_back(comp);
    if (data != nullptr)
    {
        Function* ZilchSerialize = comp.Type->FindFunction("Serialize", *GameObject::SerializeFunction, ZilchTypeId(void), FindMemberOptions::None);
        ErrorIf(ZilchSerialize == nullptr, "Failed to find function 'Serialize' on Zilch type %s", ((Component*)comp.Dereference())->Name.c_str());
        {
            Zilch::Call call(ZilchSerialize, ZILCH->GetDependencies());
            call.SetHandle(Zilch::Call::This, Components.back());
            call.Set(0, data);
            call.Invoke(ZILCH->Report);
        }
    }
    
    for (int i = ObjectState::Create; i <= LastState; ++i)
    {
        String functionName = GetStringFromState(ObjectState(i));
        CallFunctionOnObject(comp, functionName);
    }

}
void GameObject::RemoveComponent(const Handle& comp)
{
    if (LastState < ObjectState::Destroy)
    {
        for (int i = LastState; i <= ObjectState::Destroy; ++i)
        {
            String functionName = GetStringFromState(ObjectState(i));
            CallFunctionOnObject(comp, functionName);
        }
    }
    Components.erase_value(comp);
    //comp.Delete();
}


void GameObject::Uninitialize()
{
    if (LastState >= ObjectState::Uninitialize)
    {
        return;
    }
    LastState = ObjectState::Uninitialize;
    UpdateAllChildren(LastState);
    UpdateAllComponents(LastState);
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
    UpdateAllComponents(LastState);
    
    
}

ObjectState GameObject::GetStateFromString(const String& stateName)
{
    if (stateName == "Create")
    {
        return ObjectState::Create;
    }
    else if (stateName == "Initialize")
    {
        return ObjectState::Initialize;
    }
    else if (stateName == "Uninitialize")
    {
        return ObjectState::Uninitialize;
    }
    else if (stateName == "Destroy")
    {
        return ObjectState::Destroy;
    }
    else
    {
        Error("Invalid State Name %s.", stateName.c_str());
    }
    return ObjectState::Construct;
}

String GameObject::GetStringFromState(ObjectState state)
{
    switch (state)
    {
        case ObjectState::Create:
        {
            return "Create";
        }break;
        case ObjectState::Initialize:
        {
            return "Initialize";
        }break;
        case ObjectState::Uninitialize:
        {
            return "Uninitialize";
        }break;
        case ObjectState::Destroy:
        {
            return "Destroy";
        }break;
        default:
        {
            Error("Invalid State");
        }
    }
    return "";
}

void GameObject::UpdateAllChildren(ObjectState stateToCall)
{
    String functionName = GetStringFromState(stateToCall);
    

    for (unsigned i = 0; i < Children.size(); ++i)
    {
        GameObject* child = (GameObject*)Children[i].Dereference();
        if (child->LastState < stateToCall)
        {
            CallFunctionOnObject(Children[i], functionName);
        }
    }
}

void GameObject::UpdateAllComponents(ObjectState stateToCall)
{
    String functionName = GetStringFromState(stateToCall);

    unsigned Count = Components.size();
    for (unsigned i = 0; i < Count; ++i)
    {
        
        if (LastState <= stateToCall)
        {
            CallFunctionOnObject(Components[i], functionName);
        }
    }
}

void GameObject::AddComponent(const Handle& comp)
{ 
    AddComponent(comp, nullptr); 
}

void GameObject::Disconnect(const Handle& sender, const Handle& reciever, const String& EventName, const Handle& thisPointer)
{
    EventHandler* Sender = (EventHandler*)sender.Dereference();
    EventHandler* Reciever = (EventHandler*)sender.Dereference();
    EventHandler* ThisPointer = (EventHandler*)thisPointer.Dereference();

    EventDisconnect(Sender, Reciever, EventName, ThisPointer);
}

GameObject::~GameObject()
{
    for (unsigned i = 0; i < Children.size(); ++i)
    {
        Children[i].Delete();
    }
    Children.clear();

    for (unsigned i = 0; i < Components.size(); ++i)
    {
        Components[i].Delete();
    }
    Components.clear();
}