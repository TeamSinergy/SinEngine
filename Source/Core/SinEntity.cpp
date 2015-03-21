#include <Precompiled.h>
#include "SinEntity.h"
#include "ZilchCompiledLib.h"

DefineType(SinEntity, SinningZilch)
{
	type->HandleManager = ZilchManagerId(Zilch::PointerManager);
	BindFieldGetSet(Name);

	BindConstructor();
	//BindMethod(Serialize); Not until serializer is done
	BindMethod(Create);
	BindMethod(Initialize);
	BindMethod(Uninitialize);
	BindMethod(Destroy);
	BindDestructor();
}