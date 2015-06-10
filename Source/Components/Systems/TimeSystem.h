#pragma once
#include "SinEntity.h"

class TimeSystem : public Component
{
public:
    ZilchDeclareDerivedType(TimeSystem, Component);
    void Serialize(DataNode* node) override; //Serialize this through zilch
    void Create() override;
    void Initialize() override;
    void Uninitialize() override;
    void Destroy() override;

    void Update(UpdateEvent* event);

    int MinimumFPS;
    int MaximumFPS;
    float TimeScale;
    bool Paused;
    int StepCount;
private:
    float minDt;
    float maxDt;
    float dt = 0.0f;
};