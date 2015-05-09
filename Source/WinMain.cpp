#pragma once
#include "Precompiled.h"
#include "Serializer.h"
#include "SinSole.h"
#include "ZilchCompiledLib.h"
#include "GraphicsManager.h"
#include "WindowSystem.h"
#include "DataFile.h"

#define SINBUILD
#ifdef SINBUILD

#define SettingsPath "../Settings.data"

typedef HINSTANCE EngineInstance;

int OnExit(void)
{
    SinSole::DestroyConsole();
    //_CrtDumpMemoryLeaks();
    return 1;
}

int main(void)
{
    //EngineInstance program = GetModuleHandle(0);

    onexit(OnExit); //Not really needed
    SinSole::CreateConsole("SinSole");
    std::cout << "---- SinSole Initialized ----" << std::endl;

    //A static initialization of the Zilch Project for the debugger.
    ZilchStartup(StartupFlags::None);
    ZilchCompiledLib* ZilchLibrary = new ZilchCompiledLib("ZilchCompiledLib");
    ZilchLibrary->Create();
    ZilchLibrary->Initialize();
    
    //Static Initializations
    Serializer::Initialize();
    ResourceManager::Initialize();

    DataFile* settingsFile = ResourceManager::FindResourceType<DataFile>(SettingsPath);
	

    ExecutableState* state = ZILCH->GetDependencies();
    Handle EngineHandle = state->AllocateDefaultConstructedHeapObject(ZilchTypeId(Engine), ZILCH->Report, Zilch::HeapFlags::NonReferenceCounted);
    Engine* SinEngine = (Engine*)EngineHandle.Dereference();
    SinEngine->Serialize(settingsFile->FindLevel("SinEngine"));
    SinEngine->Create();
    SinEngine->Initialize();
    SinEngine->Uninitialize();
    SinEngine->Destroy();
    EngineHandle.Delete();
    
    //delete window;
    //system("pause");
    //delete settings;

    ZilchLibrary->Uninitialize();
    ZilchLibrary->Destroy();
    delete ZilchLibrary;
    
    return 0;
}


#endif