#pragma once
#include <Precompiled.h>
#include "GameObject.h"
#include "Transform.h"

class UpdateEvent;

class Camera : public Component
{
public:
    ZilchDeclareDerivedType(Camera, Component);
    void Serialize(DataNode* node) override; //Serialize this through zilch
    void Create() override;
    void Initialize() override;

    Math::Matrix4 WorldMatrix() const;
    const Math::Matrix4& ViewMatrix() const { return viewMatrix; }
    const Math::Matrix4& ProjectionMatrix() const { return projectionMatrix; };
    Math::Matrix4 ViewProjectionMatrix() const;

    void UpdateViewMatrix();
    void UpdateProjectionMatrix();
    void UpdateOrthographicMatrix();

    void Reset();
    void Update(UpdateEvent* event);

    void Uninitialize() override;
    void Destroy() override;
private:

    Transform* transform;

    float FieldOfView;
    float AspectRatio;
    float NearPlane;
    float FarPlane;

    Math::Matrix4 ForwardUpRight;

    Math::Matrix4 viewMatrix;
    Math::Matrix4 projectionMatrix;
    Math::Matrix4 orthographicMatrix;
};