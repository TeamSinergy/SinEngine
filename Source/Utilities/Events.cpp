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
    BindFieldGet(Button);
    BindFieldGet(IsButtonDown);
}