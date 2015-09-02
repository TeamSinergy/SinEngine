#include <Precompiled.h>
#include "Camera.h"
#include "Game.h"

DefineType(Camera, SinningZilch)
{
    BindComponent(Camera);
	BindMethod(MouseToWorldZPlane);
	
}

void Camera::Serialize(DataNode* node)
{
    Owner->Camera = this;
	
    DataComponent* data = static_cast<DataComponent*>(node);
    FieldOfView = XM_PIDIV4;
    SerializeValue(data, FieldOfView);
    Dependancy(GameSession->WindowSystem);
	SerializeValue(data, ProjectionMode);
	SerializeValue(data, CameraSize);
    Integer2 dim = GameSession->WindowSystem->GetDimensions();
    AspectRatio = (float)dim.x / (float)dim.y;
    SerializeValue(data, AspectRatio);
    NearPlane = 0.01f;
    SerializeValue(data, NearPlane);
    FarPlane = 1000.0f;
    SerializeValue(data, FarPlane);

    //These and position for the World Matrix
    ForwardUpRight = {
        0, 0, 1, 0,   //Forward
        0, 1, 0, 0,   //Up
        1, 0, 0, 0,   //Right 
        0, 0, 0, 0,}; 

    viewMatrix.SetIdentity();
    projectionMatrix.SetIdentity();
}
void Camera::Create()
{
    Dependancy(Owner->Transform);
    transform = Owner->Transform;
    GameSession->GraphicsSystem->MainCamera = this;
}
void Camera::Initialize()
{
    UpdateProjectionMatrix();
    //UpdateOrthographicMatrix();
    UpdateViewMatrix();
    EventConnect(GameSession, "EngineUpdate", &Camera::Update, this);
}

Math::Matrix4 Camera::ViewProjectionMatrix() const
{
    return viewMatrix * projectionMatrix;
}

void Camera::UpdateViewMatrix()
{
    Math::Matrix4 rot = Math::RotationMatrixXYZ(Owner->Transform->GetWorldRotation() * Math::cPi / 180);
    rot = ForwardUpRight * rot;
    Real3 eyePosition = transform->GetWorldTranslation();

    Real3 store = rot.Cross3(0); //Forward
    viewMatrix.SetBasis(2, Real4(store.x, store.y, store.z, -Math::Dot(store, eyePosition)));

    store = rot.Cross3(1); //Up
    viewMatrix.SetBasis(1, Real4(store.x, store.y, store.z, -Math::Dot(store, eyePosition)));

    store = rot.Cross3(2); //Right
    viewMatrix.SetBasis(0, Real4(store.x, store.y, store.z, -Math::Dot(store, eyePosition)));
    
}
void Camera::UpdateProjectionMatrix()
{
	if (ProjectionMode == ProjectionMode::Orthographic)
	{
		UpdateOrthographicMatrix();
		return;
	}
    float fov = Math::DegToRad(FieldOfView);
    XMMATRIX proj = XMMatrixPerspectiveFovRH(fov, AspectRatio, NearPlane, FarPlane);
    XMStoreFloat4x4((XMFLOAT4X4*)&projectionMatrix.array, proj);
}

void Camera::UpdateOrthographicMatrix()
{
    Integer2 dim = GameSession->WindowSystem->GetDimensions();
    // Create an orthographic projection matrix for 2D rendering.
    XMMATRIX ortho = XMMatrixOrthographicRH((float)dim.x/CameraSize, (float)dim.y/CameraSize, NearPlane, FarPlane);
	XMStoreFloat4x4((XMFLOAT4X4*)&projectionMatrix.array, ortho);
}

Real2 Camera::MouseToWorldZPlane(Real2 mousePos) const
{
	Integer2 dim = GameSession->WindowSystem->GetDimensions();
	Integer2 pos = GameSession->WindowSystem->GetPosition();
	
	mousePos -= Real2((float)pos.x, (float)pos.y);
	mousePos.x = ((2.0f * (float)mousePos.x) / (float)dim.x) - 1.0f;
	mousePos.y = (((2.0f * (float)mousePos.y) / (float)dim.y) - 1.0f) * -1.0f;

	Math::Vec3 nearPlane = Math::Vec3(mousePos.x, mousePos.y, 0);
	//Math::Vec3 farPlane = Math::Vec3(mousePos.x, mousePos.y, 1);

    Math::Matrix4 viewProjInv = ViewProjectionMatrix().Inverted();;

    //FOR CASTING RAY
	Math::Vec3 origin = Math::TransformPointCol(viewProjInv, nearPlane);
	/*Math::Vec3 rayend = Math::TransformPointCol(viewProjInv, farPlane);
	Math::Vec3 raydir = rayend - origin;
	Math::Normalize(&raydir);*/

	

	return Real2(origin.x, origin.y);
}

void Camera::Reset()
{
    ForwardUpRight = {
        0, 0, 1, 0,   //Forward
        0, 1, 0, 0,   //Up
        1, 0, 0, 0,   //Right 
        0, 0, 0, 0, }; 

    UpdateViewMatrix();
}
void Camera::Update(UpdateEvent* event)
{
    //Owner->Transform->GetWorldRotation().z += 10.0f * event->Dt;
    UpdateViewMatrix();
    
}

void Camera::Uninitialize()
{
    EventDisconnect(GameSession, this, "EngineUpdate", this);
}
void Camera::Destroy()
{

}
