#pragma once
#include "SinEntity.h"
#include "GraphicsSystem.h"
#include "DefaultGameSetup.h"
#include "TimeSystem.h"
#include "Camera.h"
#include "Transform.h"


class Game;
class Space;
class DataComponent;

enum ObjectState
{
    Construct,
    Serialize,
    Create,
    Initialize,
    Uninitialize,
    Destroy,
};

class GameObject : public SinEntity
{
public:
    GameObject() : SinEntity(){};
    GameObject(const String& name) : SinEntity(name){}

    ZilchDeclareDerivedType(GameObject, SinEntity);

    void Serialize(DataNode* node) override; //DataLevel
    void Create() override;
    void Initialize() override;

    ComponentPointer(WindowSystem);
    ComponentPointer(GraphicsSystem);
    ComponentPointer(TimeSystem);
    ComponentPointer(DefaultGameSetup);
    ComponentPointer(Camera);
    ComponentPointer(Transform);
    ComponentPointer(VectorGraphic);

    void Uninitialize() override;
    void Destroy() override;

    void AttachTo(GameObject* parent);
    void AddChild(GameObject* child);
    void RemoveChild(GameObject* child);
    GameObject* FindChildByName(const String& name);
    void UpdateAllChildren(ObjectState stateToCall);
    void UpdateAllComponents(ObjectState stateToCall);
    

    void AddComponent(Handle comp, DataComponent* data);
    void AddComponent(Handle comp);
    void RemoveComponent(Handle comp);

    Component* FindComponentByName(const String& name);

    

    Array<Handle> Components;
    Array<Handle> Children;
    GameObject* Parent;
    Space* Space;
    Game* GameSession;

    ObjectState LastState = ObjectState::Construct;
    ObjectState GetStateFromString(const String& stateName);
    String GetStringFromState(ObjectState state);
    
    static Array<Type*>* SerializeFunction;
    static Array<Type*>* Default;
    static EventData* EmptyEventData;
};