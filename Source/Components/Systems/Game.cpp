#include <Precompiled.h>
#include "Game.h"
#include "GameClock.h"
#include "InputManager.h"

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
    Handle eventHandle = ZilchAllocate<UpdateEvent>();
    UpdateEvent* eventData = (UpdateEvent*)eventHandle.Dereference();
    KeyboardEvent* keyboardEventData = new KeyboardEvent(0, 0);
    MemCheck(eventData, "UpdateEvent");
    // wait for the next message in the queue, store the result in 'msg'
    GameClock clock;
    bool Continue = true;

    EventConnect(this, Events::KeyDown, &InputManager::OnKeyEvent);
    EventConnect(this, Events::KeyUp, &InputManager::OnKeyEvent);

    while (Continue)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            // translate keystroke messages into the right format
            switch (msg.message)
            {
                //DISPATCH EVENT ON LOSE FOCUS, ELSE INPUT STICKS
                case WM_KEYDOWN:
                {
                    keyboardEventData->Key = msg.wParam;
                    keyboardEventData->IsKeyDown = true;
                    EventSend(this, Events::KeyDown, keyboardEventData);
                }break;
                case WM_KEYUP:
                {
                    keyboardEventData->Key = msg.wParam;
                    keyboardEventData->IsKeyDown = false;
                    EventSend(this, Events::KeyUp, keyboardEventData);
                }break;
                case WM_MBUTTONDOWN:
                {
                    //SinWriteLine((int)msg.wParam);
                }break;
                case WM_MOUSEMOVE:
                {

                }break;
                case WM_QUIT:
                {
                    Continue = false;
                }break;
            }
         
        }
        else
        {
            eventData->Dt = (float) clock.Update();
            EventSend(this, Events::EngineUpdate, eventData);
            InputManager::Update();
        }
    }

    EventDisconnect(this,this, Events::KeyDown, this);
    EventDisconnect(this, this, Events::KeyUp, this);
    delete keyboardEventData;
    eventHandle.Delete();
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
    space->LoadLevel(level);
    space->Initialize();
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

