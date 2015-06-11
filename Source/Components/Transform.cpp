#include <Precompiled.h>
#include "Transform.h"

DefineType(Transform, SinningZilch)
{
    RegisterComponent(Transform);
    BindConstructor();
    BindDestructor();
    BindMethod(Create);
    BindMethod(Initialize);
    BindMethod(Uninitialize);
    BindMethod(Destroy);
}

void Transform::Serialize(DataNode* node)
{
    Owner->Transform = this;
    DataComponent* values = static_cast<DataComponent*>(node);
    SerializeValue(values, Translation);
    SerializeValue(values, Rotation);
    SerializeValue(values, Scale);
    SinWriteLine(Translation);
    SinWriteLine(Rotation);
    SinWriteLine(Scale);
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

Real3& Transform::GetWorldTranslation()
{
    if (!HasParent)
    {
        return GetTranslation();
    }
    return GetTranslation();
}
Quaternion& Transform::GetWorldRotation()
{
    if (!HasParent)
    {
        return GetRotation();
    }
    return GetRotation();
}
Real3& Transform::GetWorldScale()
{
    if (!HasParent)
    {
        return GetScale();
    }
    return GetScale();
}

void Transform::SetTranslation(const Real3& translation)
{
    Translation = translation;
}
void Transform::SetRotation(const Real3& rotation)
{
    Rotation = Math::ToQuaternion(rotation);
}
void Transform::SetScale(const Real3& scale)
{
    Scale = scale;
}

void Transform::SetWorldTranslation(const Real3& translation)
{
    if (!HasParent)
    {
        Translation = translation;
    }
}
void Transform::SetWorldRotation(const Real3& rotation)
{
    if (!HasParent)
    {
        Rotation = Math::ToQuaternion(rotation);
    }
}
void Transform::SetWorldScale(const Real3& scale)
{
    if (!HasParent)
    {
        Scale = scale;
    }
}