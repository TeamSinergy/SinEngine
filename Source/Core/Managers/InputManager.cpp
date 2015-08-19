#include <Precompiled.h>
#include "InputManager.h"



bool InputManager::InputStatesDown[MAX_KEYS] = { false };
bool InputManager::InputStatesWasDown[MAX_KEYS] = { false };
Real2 InputManager::MousePosition = Real2();

DefineType(InputManager, SinningZilch)
{
    BindStaticMethod(IsMouseDown);
    BindStaticMethod(IsMouseTriggered);
    BindStaticMethod(IsMouseReleased);
    BindStaticMethod(IsKeyDown);
    BindStaticMethod(IsKeyTriggered);
    BindStaticMethod(IsKeyReleased);
	BindStaticMethod(GetMousePosition);
}

//Mouse
bool InputManager::IsMouseDown(const unsigned& button)
{
    return IsKeyDown(button);
}

bool InputManager::IsMouseTriggered(const unsigned& button)
{
	return IsKeyTriggered(button);
}

bool InputManager::IsMouseReleased(const unsigned& button)
{
	return IsKeyReleased(button);
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

void InputManager::OnMouseMoveEvent(MouseEvent* event)
{
	MousePosition = event->MousePosition;
}
//Bool update
void InputManager::Update()
{

    for (unsigned i = 0; i < MAX_KEYS; ++i)
    {
        InputStatesWasDown[i] = InputStatesDown[i];
    }
    

}