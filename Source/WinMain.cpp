#pragma once
#include "Precompiled.h"
#include "Serializer.h"
#include "SinSole.h"
#include "ZilchCompiledLib.h"

#include "DataFile.h"

#define SINBUILD
#ifdef SINBUILD

int main(void)
{
	//onexit(OnExit); //Not really needed
	SinSole::CreateConsole("SinSole");
	std::cout << "---- SinSole Initialized ----" << std::endl;

	//A 'static' initialization of the Zilch Project for the debugger.
	ZilchStartup(Debugging::UseZilchErrorHandler);
	ZilchCompiledLib* ZilchLibrary = new ZilchCompiledLib("ZilchCompiledLib");
	ZilchLibrary->Create();
	ZilchLibrary->CompiledProject = &Project(ZilchLibrary->Errors);
	ZilchLibrary->Initialize();
	
	ExecutableState* state = ZILCH->GetDependencies();
	Handle EngineHandle = state->AllocateDefaultConstructedHeapObject(ZilchTypeId(SinEntity), ZILCH->Report, Zilch::HeapFlags::NonReferenceCounted);
	SinEntity* Engine = (SinEntity*)EngineHandle.Dereference();
	
	DataFile* file = new DataFile("../Settings.data");

	system("pause");

	ZilchLibrary->Uninitialize();
	ZilchLibrary->Destroy();
	delete ZilchLibrary;
	SinSole::DestroyConsole();
	return 0;
}

#endif