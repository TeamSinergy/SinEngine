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
    Utility::Destroy();
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
    //settingsFile->AddLevel("Level6")->AddObject("Object10")->AddComponent("_Movement")->AddProperty("Integer", "Speed", "10");
    //settingsFile->RemoveLevel("Level6");
    //settingsFile->Serialize();

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
    ResourceManager::Uninitialize();
    ZilchLibrary->Uninitialize();
    ZilchLibrary->Destroy();
    delete ZilchLibrary;
    ZilchShutdown();
    
    
    /*Matrix m = Matrix(4, 2);
    m[0][0] = 7; m[0][1] = 3;
    m[1][0] = 2; m[1][1] = 5;
    m[2][0] = 6; m[2][1] = 8;
    m[3][0] = 9; m[3][1] = 0;
    m.Print();
    Matrix s = Matrix(2, 3);
    s[0][0] = 7; s[0][1] = 4; s[0][2] = 9;
    s[1][0] = 8; s[1][1] = 1; s[1][2] = 5;
    s.Print();

    Matrix g = m * s;
    g.Print();
    g.Transpose();
    g.Print();*/
    

    //system("pause");

    return 0;
}


#endif