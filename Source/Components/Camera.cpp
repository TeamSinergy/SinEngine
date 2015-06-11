#include <Precompiled.h>
#include "Camera.h"

DefineType(Camera, SinningZilch)
{
    BindComponent(Camera);
}

void Camera::Serialize(DataNode* node)
{
    Owner->Camera = this;

    DataComponent* data = static_cast<DataComponent*>(node);
    FieldOfView = XM_PIDIV4;
    SerializeValue(data, FieldOfView);
    AspectRatio = 16 / 9;
    SerializeValue(data, AspectRatio);
    NearPlane = 0.01f;
    SerializeValue(data, NearPlane);
    FarPlane = 1000.0f;
    SerializeValue(data, FarPlane);
    Forward = Real3(0, 0, -1);
    Up = Real3(0, 1, 0);
    Right = Real3(1, 0, 0);
    
    /*SerializeValue(data, Up);
    SerializeValue(data, Right);*/
}
void Camera::Create()
{
    
    Dependancy(Owner->Transform);
    transform = Owner->Transform;
}
void Camera::Initialize()
{
    UpdateViewMatrix();
    UpdateProjectionMatrix();
    Reset();
}

Math::Matrix4 Camera::ViewProjectionMatrix() const
{
    return viewMatrix * projectionMatrix;
}

void Camera::UpdateViewMatrix()
{
    Real3 eyePosition = transform->GetTranslation();
    viewMatrix = Math::ToMatrix4(Math::ToQuaternion(Forward, Up));
    return;
}
void Camera::UpdateProjectionMatrix()
{

}


void Camera::Reset()
{

}
void Camera::Update(UpdateEvent* event)
{
    UpdateViewMatrix();
}

void Camera::Uninitialize()
{

}
void Camera::Destroy()
{

}
