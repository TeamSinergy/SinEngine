#pragma once
#include "SinEntity.h"
#include "GraphicsSystem.h"
#include "DefaultGameSetup.h"
#include "TimeSystem.h"
#include "Camera.h"
#include "Transform.h"

class Game;
class Space;


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

    void Uninitialize() override;
    void Destroy() override;

    void AttachTo(GameObject* parent);
    void AddChild(GameObject* child);
    void RemoveChild(GameObject* child);
    GameObject* FindChildByName(const String& name);
    void UpdateAllChildren(ObjectState stateToCall);

    static Array<Type*>* SerializeFunction;
    static Array<Type*>* Default;
    static EventData* EmptyEventData;

    Array<Handle> Components;
    Array<Handle> Children;
    GameObject* Parent;
    Space* Space;
    Game* GameSession;

    ObjectState LastState = ObjectState::Construct;
protected:
    
    
};