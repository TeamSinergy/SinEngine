#include <Precompiled.h>
#include "TimeSystem.h"

DefineType(TimeSystem, SinningZilch)
{
    BindConstructor();
    BindDestructor();
    RegisterComponent(TimeSystem);
}

void TimeSystem::Serialize(DataNode* node)
{
    Owner->TimeSystem = this;

    DataComponent* settings = static_cast<DataComponent*>(node);
    MinimumFPS = 30;
    SerializeValue(settings, MinimumFPS);
    MaximumFPS = 60;
    SerializeValue(settings, MaximumFPS);
    TimeScale = 1;
    SerializeValue(settings, TimeScale);
    Paused = false;
    SerializeValue(settings, Paused);
    StepCount = 1;
    SerializeValue(settings, StepCount);
    
    minDt = 1.0f / MaximumFPS;
    maxDt = 1.0f / MinimumFPS;
}
void TimeSystem::Create()
{

}
void TimeSystem::Initialize()
{
    EventConnect(GameSession, "EngineUpdate", &TimeSystem::Update, this);
}

void TimeSystem::Update(UpdateEvent* event)
{
    if (Paused)
    {
        return;
    }
    dt = event->Dt;
    dt *= TimeScale;
    dt /= StepCount;

    if (dt > maxDt)
    {
        dt = maxDt;
    }
    else if (dt < minDt)
    {
        dt = minDt;
    }
    event->Dt = dt;
    for (int i = 0; i < StepCount; ++i)
    {
        EventSend(Space, "LogicUpdate", event);
    }
    SinWriteLine(event->Dt);
}

void TimeSystem::Uninitialize()
{

}
void TimeSystem::Destroy()
{

}
