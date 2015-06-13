#include <Precompiled.h>
#include "SinEntity.h"
#include "ZilchCompiledLib.h"

DefineType(SinEntity, SinningZilch)
{
    BindFieldGetSet(Name);
    BindConstructor();
    BindVirtualConstructor(const String&);
    BindMethod(Create);
    BindMethod(Initialize);
    BindMethod(Uninitialize);
    BindMethod(Destroy);
    BindDestructor();
}

DefineType(DataNode, SinningZilch)
{
    type->HandleManager = ZilchManagerId(Zilch::PointerManager);
    BindConstructor();
    BindDestructor();
}