#include <Precompiled.h>
#include "Game.h"
#include "GameClock.h"

#define StartingSpaceComp "StartingLevel"
#define FilePropertyName "DataFile"
#define LevelPropertyName "LevelName"

#define DefaultLevelName "DefaultLevel"


DefineType(Game, SinningZilch)
{
    RegisterComponent(Game);
    BindConstructor();
    BindDestructor();
}

void Game::Update()
{
    // this struct holds Windows event messages
    MSG msg;
    UpdateEvent* eventData = new UpdateEvent();
    MemCheck(eventData, "UpdateEvent");
    // wait for the next message in the queue, store the result in 'msg'
    GameClock clock;

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
            eventData->Dt = (float) clock.Update();
            EventSend(this, "EngineUpdate", eventData);
        }
    }

    delete eventData;
}

void Game::LoadSpace(DataObject* spaceArchetype, DataLevel* level)
{
    Handle handle = ZilchAllocate<ObjectSpace>();
    ObjectSpace* space = (ObjectSpace*)handle.Dereference();
    ((GameObject*)space)->Space = space;
    space->Name = level->GetName();
    space->GameSession = this;
    

    space->Serialize(spaceArchetype);
    space->InitializeSpace();
    space->Create();
    space->Initialize();
    space->LoadLevel(level);
    AddChild(space);
    
}

void Game::UnloadSpace(const String& levelName)
{
    ObjectSpace* space = (ObjectSpace*)FindChildByName(levelName);
    ErrorIf(space == nullptr, "Could not find a child with the name %s.", levelName.c_str());
    space->Uninitialize();
    space->Destroy();
    space->UninitializeSpace();
}


Game::~Game()
{
    
}

