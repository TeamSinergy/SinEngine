#pragma once
#include "SinEntity.h"
#include "GraphicsSystem.h"
#include "DefaultGameSetup.h"
#include "TimeSystem.h"
#include "Camera.h"
#include "Transform.h"
#include "GraphicsManager.h"
#include "ForwardRenderer.h"
#include "CameraViewport.h"
#include "Sprite.h"
#include "AudioComponent.h"
#include "Microphone.h"
#include "SoundSpace.h"

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

    ComponentPointer(CameraViewport);
    ComponentPointer(GraphicsSpace); //Where all the graphics objects are stored;
    ComponentPointer(GraphicsSystem);
    ComponentPointer(WindowSystem);
    ComponentPointer(GraphicsManager);
    ComponentPointer(TimeSystem);
    ComponentPointer(DefaultGameSetup);
    ComponentPointer(Camera);
    ComponentPointer(Transform);
    ComponentPointer(VectorGraphic);
    ComponentPointer(ForwardRenderer);
	ComponentPointer(Sprite);
    ComponentPointer(AudioComponent);
    ComponentPointer(Microphone);
    ComponentPointer(SoundSpace);

    void Uninitialize() override;
    void Destroy() override;

    GameObject* GetParent() { return Parent; }
    const Array<Handle>& GetAllChildren() { return Children; }
    void Detach();
    void AttachTo(GameObject* parent);
    void AddChild(GameObject* child);
    void RemoveChild(GameObject* child);
    GameObject* FindChildByName(const String& name);
    void UpdateAllChildren(ObjectState stateToCall);
    void UpdateAllComponents(ObjectState stateToCall);
    

    void AddComponent(const Handle&, DataComponent* data);
    void AddComponent(const Handle&);
    void RemoveComponent(const Handle&);

    Component* FindComponentByName(const String& name);


    void Disconnect(const Handle& sender, const Handle& reciever, const String& EventName, const Handle& thisPointer);

    virtual ~GameObject();

    static Array<Type*>* SerializeFunction;
    static Array<Type*>* Default;
    static EventData* EmptyEventData;
    


//should be private
    Array<Handle> Components;
    Array<Handle> Children;
    GameObject* Parent;
    Space* Space;
    Game* GameSession;

    ObjectState LastState = ObjectState::Construct;
    ObjectState GetStateFromString(const String& stateName);
    String GetStringFromState(ObjectState state);
    
    
};