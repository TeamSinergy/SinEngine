#include <Precompiled.h>
#include "Transform.h"
#include "Game.h"

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
    EventConnect(GameSession, "EngineUpdate", &Transform::Update, this);
}
void Transform::Uninitialize()
{
    EventDisconnect(GameSession, this, "EngineUpdate", this);
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
    XMMATRIX matRotateX;    // a matrix to store the rotation information
    // build a matrix to rotate the model 3.14 radians
    matRotateX = XMMatrixRotationX(Math::DegToRad(WorldRotation.x));
    XMMATRIX matRotateY;
    matRotateY = XMMatrixRotationY(Math::DegToRad(WorldRotation.y));
    XMMATRIX matRotateZ;
    matRotateZ = XMMatrixRotationZ(Math::DegToRad(WorldRotation.z));

    XMMATRIX matScale;    // a matrix to store the scaling information
    
    // build a matrix to double the size of the model and store it to matScale
    matScale = XMMatrixScaling(WorldScale.x, WorldScale.y, WorldScale.z);

    XMMATRIX matTranslate;
    matTranslate = XMMatrixTranslation(WorldTranslation.x, WorldTranslation.y, WorldTranslation.z);

    XMMATRIX matWorld;
    matWorld = matRotateX * matRotateY * matRotateZ * matScale * matTranslate;
    Math::Matrix4 viewMatrix;
    XMStoreFloat4x4((XMFLOAT4X4*)viewMatrix.array, matWorld);
    
    return viewMatrix;
}

void Transform::Update(UpdateEvent* event)
{
    WorldRotation.y += 360 *event->Dt;

    //WorldTranslation.z += 30.0f * event->Dt;
}