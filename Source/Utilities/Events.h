#pragma once

class UpdateEvent : public EventData
{
public:
    ZilchDeclareDerivedType(UpdateEvent, EventData);
    UpdateEvent(float dt = 0.0f) : EventData(), Dt(dt) {}
    float Dt;
};

class KeyboardEvent : public EventData
{
public:
    ZilchDeclareDerivedType(KeyboardEvent, EventData);
    KeyboardEvent(int key, bool keyState) : EventData(), Key(key), IsKeyDown(keyState) {}
    int Key;
    bool IsKeyDown;
};

class MouseEvent : public EventData
{
public:
    ZilchDeclareDerivedType(MouseEvent, EventData);
    MouseEvent(int button, bool buttonState) : EventData(), Button(button), IsButtonDown(buttonState) {}
    int Button;
    bool IsButtonDown;
};

namespace Zilch
{
    namespace Events
    {
        //ZilchDeclareEvent(EngineUpdate, UpdateEvent);
        ZilchDefineEvent(EngineUpdate);
        //ZilchDeclareEvent(LogicUpdate, UpdateEvent);
        ZilchDefineEvent(LogicUpdate);
        //ZilchDeclareEvent(FrameUpdate, UpdateEvent);
        ZilchDefineEvent(FrameUpdate);
        ZilchDefineEvent(KeyUp);
        ZilchDefineEvent(KeyDown);
        ZilchDefineEvent(MouseUp);
        ZilchDefineEvent(MouseDown);
        ZilchDefineEvent(LeftMouseUp);
        ZilchDefineEvent(LeftMouseDown);
        ZilchDefineEvent(RightMouseUp);
        ZilchDefineEvent(RightMouseDown);
        ZilchDefineEvent(MiddleMouseUp);
        ZilchDefineEvent(MiddleMouseDown);
    };
};