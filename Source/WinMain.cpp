#pragma once
#include "Precompiled.h"
#include "Serializer.h"
#include "SinSole.h"
#include "ZilchCompiledLib.h"
#include "GraphicsManager.h"

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
    EngineInstance program = GetModuleHandle(0);

    onexit(OnExit); //Not really needed
    SinSole::CreateConsole("SinSole");
    std::cout << "---- SinSole Initialized ----" << std::endl;

    //A static initialization of the Zilch Project for the debugger.
    ZilchStartup(Debugging::UseZilchErrorHandler);
    ZilchCompiledLib* ZilchLibrary = new ZilchCompiledLib("ZilchCompiledLib");
    ZilchLibrary->Create();
    ZilchLibrary->CompiledProject = &Project(ZilchLibrary->Errors);
    ZilchLibrary->Initialize();
    
    //Static Initializations
    Serializer::Initialize();
    ResourceManager::Initialize();

    DataComponent* settings = ResourceManager::FindResourceType<DataFile>(SettingsPath)->FindLevel("Game")->FindObject("SinEngine")->FindComponent("Settings");
    
    Window* window = GraphicsManager::CreateGameWindow(program, settings, WindowStyles::BorderedWindowStyle);

    ExecutableState* state = ZILCH->GetDependencies();
    Handle EngineHandle = state->AllocateDefaultConstructedHeapObject(ZilchTypeId(SinEntity), ZILCH->Report, Zilch::HeapFlags::NonReferenceCounted);
    SinEntity* Engine = (SinEntity*)EngineHandle.Dereference();
    

    // this struct holds Windows event messages
    MSG msg;

    // wait for the next message in the queue, store the result in 'msg'
    while (true)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            // translate keystroke messages into the right format
            TranslateMessage(&msg);

            // send the message to the WindowProc function
            DispatchMessage(&msg);

            // check to see if it's time to quit
            if (msg.message == WM_QUIT)
                break;
        }
        else
        {
            //Update
        }
    }
    delete window;
    //system("pause");
    //delete settings;
    EngineHandle.Delete();

    ZilchLibrary->Uninitialize();
    ZilchLibrary->Destroy();
    delete ZilchLibrary;
    
    return 0;
}


#endif