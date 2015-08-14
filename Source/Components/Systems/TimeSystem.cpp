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
    
    if (MaximumFPS != 0)
    {
        minDt = 1.0f / MaximumFPS;
    }
    else
    {
        //Non-Zero to prevent divide by zero;
        minDt = 0.0000001f;
    }
    if (MinimumFPS != 0)
    {
        maxDt = 1.0f / MinimumFPS;
    }
    else
    {
        maxDt = FLT_MAX;
    }
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

    if (dt > maxDt)
    {
        dt = maxDt;
    }
    else if (dt < minDt)
    {
        dt = minDt;
    }
    
    dt /= StepCount;
    event->Dt = dt;
    for (int i = 0; i < StepCount; ++i)
    {
        EventSend(Space, "LogicUpdate", event);
    }
    //SinWriteLine(dt);
}

void TimeSystem::Uninitialize()
{

}
void TimeSystem::Destroy()
{

}
