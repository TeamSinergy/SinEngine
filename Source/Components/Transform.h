#pragma once
#include "GameObject.h"

class UpdateEvent;

class Transform : public Component
{
public:
    //Inherited
    ZilchDeclareDerivedType(Transform, Component);
    void Serialize(DataNode* node) override; //Serialize this through zilch
    void Create() override;
    void Initialize() override;
    void Uninitialize() override;
    void Destroy() override;

    //Getters
    const Real3& GetTranslation() const { return Translation; }
    Real3& GetTranslation() { return Translation; }
    const Real3& GetRotation() const { return Rotation; }
    Real3& GetRotation() { return Rotation; }
    const Real3& GetScale() const { return Scale; }
    Real3& GetScale() { return Scale; }

    const Real3& GetWorldTranslation() const { return WorldTranslation; }
    Real3& GetWorldTranslation() { return WorldTranslation; }
    const Real3& GetWorldRotation() const { return WorldRotation; }
    Real3& GetWorldRotation() { return WorldRotation; }
    const Real3& GetWorldScale() const { return WorldScale; }
    Real3& GetWorldScale() { return WorldScale; }

    void Update(UpdateEvent* event);

    //Setters
    void SetTranslation(const Real3& translation);
    void SetRotation(const Real3& rotation);
    void SetScale(const Real3& scale);
    void SetWorldTranslation(const Real3& translation);
    void SetWorldRotation(const Real3& rotation);
    void SetWorldScale(const Real3& scale);

    Math::Matrix4 GetWorldMatrix() const;
private:
    Real3 Translation = Real3();
    Real3 Rotation = Real3();
    Real3 Scale = Real3();
    
    Real3 WorldTranslation = Real3();
    Real3 WorldRotation = Real3();
    Real3 WorldScale = Real3();
};