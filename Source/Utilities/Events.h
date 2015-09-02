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

class MouseEvent : public KeyboardEvent
{
public:
    ZilchDeclareDerivedType(MouseEvent, KeyboardEvent);
	MouseEvent(int button = 0, bool buttonState = false) : KeyboardEvent(button, buttonState){}
    int& Button = Key;
    bool& IsButtonDown = IsKeyDown;
	Real2 MousePosition;
};

class ScrollEvent : public MouseEvent
{
public:
	ZilchDeclareDerivedType(ScrollEvent, MouseEvent);
	ScrollEvent() : MouseEvent() {}
	unsigned ScrollLines = 0;
	int WheelDelta = 0;
};

class FocusEvent : public EventData
{
public:
    ZilchDeclareDerivedType(FocusEvent, EventData);
    FocusEvent() : EventData() {}
    bool InFocus;
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
		ZilchDefineEvent(MouseMove);
		ZilchDefineEvent(MouseScroll);
        ZilchDefineEvent(LevelLoaded);
        ZilchDefineEvent(FocusEvent);
		
    };
};