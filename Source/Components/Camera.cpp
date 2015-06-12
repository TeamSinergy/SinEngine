#include <Precompiled.h>
#include "Camera.h"
#include "Game.h"

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
    Dependancy(GameSession->WindowSystem);
    Integer2 dim = GameSession->WindowSystem->GetDimensions();
    AspectRatio = (float)dim.y / (float)dim.x;
    SerializeValue(data, AspectRatio);
    NearPlane = 0.01f;
    SerializeValue(data, NearPlane);
    FarPlane = 1000.0f;
    SerializeValue(data, FarPlane);

    //These and position for the World Matrix
    Forward = Real3(0, 0, 0);
    Up = Real3(0, 1, 0);
    Right = Real3(1, 0, 0);
    
    /*SerializeValue(data, Up);
    SerializeValue(data, Right);*/
}
void Camera::Create()
{
    Dependancy(Owner->Transform);
    transform = Owner->Transform;
    Space->GraphicsSystem->MainCamera = this;
}
void Camera::Initialize()
{
    UpdateProjectionMatrix();
    UpdateOrthographicMatrix();
    UpdateViewMatrix();
    EventConnect(GameSession, "EngineUpdate", &Camera::Update, this);
}

Math::Matrix4 Camera::ViewProjectionMatrix() const
{
    return viewMatrix * projectionMatrix;
}

void Camera::UpdateViewMatrix()
{
    Real3 eyePosition = transform->GetWorldTranslation();
    XMMATRIX view = XMMatrixLookAtLH(*(XMVECTOR*)&eyePosition.array, *(XMVECTOR*)&Forward.array, *(XMVECTOR*)&Up.array);
    XMStoreFloat4x4((XMFLOAT4X4*)&viewMatrix.array, view);
}
void Camera::UpdateProjectionMatrix()
{
    XMMATRIX proj = XMMatrixPerspectiveFovLH(Math::DegToRad(FieldOfView), AspectRatio, NearPlane, FarPlane);
    XMStoreFloat4x4((XMFLOAT4X4*)&projectionMatrix.array, proj);   
}

void Camera::UpdateOrthographicMatrix()
{
    Integer2 dim = GameSession->WindowSystem->GetDimensions();
    // Create an orthographic projection matrix for 2D rendering.
    XMMATRIX ortho = XMMatrixOrthographicLH((float)dim.x, (float)dim.y, NearPlane, FarPlane);
    XMStoreFloat4x4((XMFLOAT4X4*)&orthographicMatrix.array, ortho);
}

void Camera::Reset()
{
    Forward = Real3(0, 0, -1);
    Up = Real3(0, 1, 0);
    Right = Real3(1, 0, 0);

    UpdateViewMatrix();
}
void Camera::Update(UpdateEvent* event)
{
    UpdateViewMatrix();
    
}

void Camera::Uninitialize()
{
    EventDisconnect(GameSession, this, "EngineUpdate", this);
}
void Camera::Destroy()
{

}
