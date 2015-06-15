#include <Precompiled.h>
#include "Transform.h"
#include "Game.h"

DefineType(Transform, SinningZilch)
{
    BindComponent(Transform);
    BindMethodOverload(GetTranslation, Real3&);
    BindProperty(GetTranslation, SetTranslation, "Translation");
    BindMethod(SetTranslation);
    BindMethod(GetTranslation);
    BindProperty(GetScale, SetScale, "Scale");
    BindMethod(SetScale);
    BindMethod(GetScale);
    BindProperty(GetRotation, SetRotation, "Rotation");
    BindMethod(SetRotation);
    BindMethod(GetRotation);
}

void Transform::Serialize(DataNode* node)
{
    Owner->Transform = this;
    DataComponent* values = static_cast<DataComponent*>(node);
    Real3 euler;
    SerializeValue(values, Translation);
    SerializeValueName(values, euler, "Rotation");
    SerializeValue(values, Scale);

    SetTranslation(Translation);
    SetRotation(euler);
    SetScale(Scale);
}
void Transform::Create()
{
    
}
void Transform::Initialize()
{
    
}
void Transform::Uninitialize()
{
    
}
void Transform::Destroy()
{

}

void Transform::SetTranslation(const Real3& translation)
{
    
    Translation = translation;
    WorldTranslation = Translation;
}
void Transform::SetRotation(const Real3& rotation)
{
    
    Rotation = rotation;
    WorldRotation = Rotation;
}
void Transform::SetScale(const Real3& scale)
{
    
    Scale = scale;
    WorldScale = Scale;
}

void Transform::SetWorldTranslation(const Real3& translation)
{
    Translation = translation;
    WorldTranslation = Translation;
}
void Transform::SetWorldRotation(const Real3& rotation)
{
    Rotation = rotation;
    WorldRotation = Rotation;
}
void Transform::SetWorldScale(const Real3& scale)
{
    Scale = scale;
    WorldScale = Scale;
}

Math::Matrix4 Transform::GetWorldMatrix() const
{

    Math::Matrix4 matRotate = Math::RotationMatrixXYZ(Math::DegToRad(WorldRotation.x), Math::DegToRad(WorldRotation.y), Math::DegToRad(-WorldRotation.z));
    
    Math::Matrix4 matScale;    // a matrix to store the scaling information
    matScale.Scale(WorldScale);
    Math::Matrix4 matTranslate;
    matTranslate.Translate(WorldTranslation);
    Math::Matrix4 matWorld;
    matWorld = matRotate * matScale;
    matWorld.SetCross(3, WorldTranslation, 1);
    
    return matWorld;
}

