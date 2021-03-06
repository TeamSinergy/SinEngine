#pragma once
#include <Precompiled.h>
#define MAX_KEYS 348

class InputManager
{
    ZilchDeclareBaseType(InputManager, Zilch::TypeCopyMode::ReferenceType);

public:
    static bool IsMouseDown(const unsigned& button);
    static bool IsMouseTriggered(const unsigned& button);
    static bool IsMouseReleased(const unsigned& button);
    static bool IsKeyDown(const unsigned& key);
    static bool IsKeyTriggered(const unsigned& key);
    static bool IsKeyReleased(const unsigned& key);

    static void OnKeyEvent(KeyboardEvent* event);
	static void OnMouseMoveEvent(MouseEvent* event);
    static void Update();

	static Real2 GetMousePosition() { return MousePosition; }
	//static void SetMousePosition(Real2*& pos) { MousePosition = pos; }
private:
    static bool InputStatesDown[MAX_KEYS];
    static bool InputStatesWasDown[MAX_KEYS];

	static Real2 MousePosition;
};

enum Keys
{
    BACKSPACE = 8,
    TAB = 9,
    CLEAR = 12,
    ENTER = 13,
    SHIFT = 16,
    CTRL = 17,
    ALT = 18,
    PAUSE = 19,
    CAPSLOCK = 20,
    ESCAPE = 27,
    SPACE = 32,
    PAGEUP = 33,
    PAGEDOWN = 34,
    END = 35,
    HOME = 36,
    LEFT = 37,
    UP = 38,
    RIGHT = 39,
    DOWN = 40,
    SELECT = 41,
    PRINT = 42,
    EXECUTE = 43,
    PRNTSCREEN = 44,
    INSERT = 45,
    DELETEKEY = 46,
    HELP = 47,
    ZERO = 48,
    ONE = 49,
    TWO = 50,
    THREE = 51,
    FOUR = 52,
    FIVE = 53,
    SIX = 54,
    SEVEN = 55,
    EIGHT = 56,
    NINE = 57,
    A = 65,
    B = 66,
    C = 67,
    D = 68,
    E = 69,
    F = 70,
    G = 71,
    H = 72,
    I = 73,
    J = 74,
    K = 75,
    L = 76,
    M = 77,
    N = 78,
    O = 79,
    P = 80,
    Q = 81,
    R = 82,
    S = 83,
    T = 84,
    U = 85,
    V = 86,
    W = 87,
    X = 88,
    Y = 89,
    Z = 90,
    LEFTSHIFT = 160,
    RIGHTSHIFT = 161

};

//enum Mouse
//{
//    LEFT = 1,
//    RIGHT = 2,
//    CANCEL = 3,
//    MIDDLE = 4,
//    BUTTONX1 = 5,
//    BUTTONX2 = 6
//};