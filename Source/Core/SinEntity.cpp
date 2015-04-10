#include <Precompiled.h>
#include "SinEntity.h"
#include "ZilchCompiledLib.h"

DefineType(SinEntity, SinningZilch)
{
    type->HandleManager = ZilchManagerId(Zilch::PointerManager);
    BindFieldGetSet(Name);

    BindConstructor();
    BindMethod(Create);
    BindMethod(Initialize);
    BindMethod(Uninitialize);
    BindMethod(Destroy);
    BindDestructor();
}