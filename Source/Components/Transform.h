#pragma once
#include "GameObject.h"

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
    const Real3& GetEulerAngles() const { return Real3(Math::QuatToEulerDegrees(Rotation)); }
    const Quaternion& GetRotation() const { return Rotation; }
    Quaternion& GetRotation() { return Rotation; }
    const Real3& GetScale() const { return Scale; }
    Real3& GetScale() { return Scale; }

    const Real3& GetWorldTranslation() const { return GetWorldTranslation(); }
    Real3& GetWorldTranslation();
    const Quaternion& GetWorldRotation() const { return GetWorldRotation(); }
    Quaternion& GetWorldRotation();
    const Real3& GetWorldScale() const { return GetWorldScale(); }
    Real3& GetWorldScale();

    //Setters
    void SetTranslation(const Real3& translation);
    void SetRotation(const Real3& rotation);
    void SetScale(const Real3& scale);
    void SetWorldTranslation(const Real3& translation);
    void SetWorldRotation(const Real3& rotation);
    void SetWorldScale(const Real3& scale);
private:
    Real3 Translation = Real3();
    Quaternion Rotation = Math::ToQuaternion(Real3());
    Real3 Scale = Real3();
    bool HasParent = false;
};