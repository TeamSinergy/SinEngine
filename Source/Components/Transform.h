#pragma once
#include "GameObject.h"

class Transform : public Component
{
public:
    const Real3& GetTranslation();
    const Real3& GetRotation();
    const Real3& GetScale();
    void SetTranslation();
    void SetRotation();
    void SetScale();
private:
    Real3 Translation;
    Real3 Rotation;
    Real3 Scale;
};