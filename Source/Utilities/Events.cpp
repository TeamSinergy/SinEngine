#include <Precompiled.h>
#include "Events.h"

DefineType(UpdateEvent, SinningZilch)
{
    BindConstructor();
    BindDestructor();
    BindFieldGet(Dt);
}

DefineType(KeyboardEvent, SinningZilch)
{
    BindFieldGet(Key);
    BindFieldGet(IsKeyDown);
}

DefineType(MouseEvent, SinningZilch)
{
	BindConstructor();
	BindDestructor();
    BindFieldGetAs(Key, "Button");
    BindFieldGetAs(IsKeyDown, "IsButtonDown");
	BindFieldGet(MousePosition);
}

DefineType(ScrollEvent, SinningZilch)
{
	BindConstructor();
	BindDestructor();
	BindFieldGet(ScrollLines);
	BindFieldGet(WheelDelta);
}

DefineType(FocusEvent, SinningZilch)
{
    BindConstructor();
    BindDestructor();
    BindFieldGet(InFocus);
}