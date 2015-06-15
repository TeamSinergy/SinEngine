#include <Precompiled.h>
#include "InputManager.h"



bool InputManager::InputStatesDown[MAX_KEYS] = { false };
bool InputManager::InputStatesWasDown[MAX_KEYS] = { false };

DefineType(InputManager, SinningZilch)
{
    BindStaticMethod(IsMouseDown);
    BindStaticMethod(IsMouseTriggered);
    BindStaticMethod(IsMouseReleased);
    BindStaticMethod(IsKeyDown);
    BindStaticMethod(IsKeyTriggered);
    BindStaticMethod(IsKeyReleased);
}

//Mouse
bool InputManager::IsMouseDown(const unsigned& button)
{
    return InputStatesWasDown[button] && InputStatesDown[button];
}

bool InputManager::IsMouseTriggered(const unsigned& button)
{
    return !InputStatesWasDown[button] && InputStatesDown[button];
}

bool InputManager::IsMouseReleased(const unsigned& button)
{
    return !InputStatesDown[button] && InputStatesWasDown[button];
}

//Keyboard
bool InputManager::IsKeyDown(const unsigned& key)
{
    return InputStatesWasDown[key] && InputStatesDown[key];
}

bool InputManager::IsKeyTriggered(const unsigned& key)
{
    return InputStatesDown[key] && !InputStatesWasDown[key];
}

bool InputManager::IsKeyReleased(const unsigned& key)
{
    return InputStatesWasDown[key] && !InputStatesDown[key];
}

void InputManager::OnKeyEvent(KeyboardEvent* event)
{
        // Set the Key's State      
    InputStatesDown[event->Key] = static_cast<bool>(event->IsKeyDown);
}

void InputManager::OnMouseButtonEvent(MouseEvent* event)
{
        // Set the Mouse's State
    InputStatesDown[event->Button] = static_cast<bool>(event->IsButtonDown);
}

void InputManager::OnMouseMoveEvent(const Real2& screenPosition)
{

}
//Bool update
void InputManager::Update()
{

    for (unsigned i = 0; i < MAX_KEYS; ++i)
    {
        InputStatesWasDown[i] = InputStatesDown[i];
    }
    

}