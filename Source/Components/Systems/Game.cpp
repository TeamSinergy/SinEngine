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
	Handle scrollEventHandle = ZilchAllocate<ScrollEvent>();
    UpdateEvent* eventData = (UpdateEvent*)eventHandle.Dereference();
	ScrollEvent* keyboardEventData = (ScrollEvent*)scrollEventHandle.Dereference();
    MemCheck(eventData, "UpdateEvent");
    // wait for the next message in the queue, store the result in 'msg'
    GameClock clock;
    bool Continue = true;

    EventConnect(this, Events::KeyDown, &InputManager::OnKeyEvent);
    EventConnect(this, Events::KeyUp, &InputManager::OnKeyEvent);
	EventConnect(this, Events::MouseDown, &InputManager::OnKeyEvent);
	EventConnect(this, Events::MouseUp, &InputManager::OnKeyEvent);
	EventConnect(this, Events::MouseMove, &InputManager::OnMouseMoveEvent);
	EventSend(this, Events::MouseMove, keyboardEventData);
	

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
				case WM_LBUTTONDOWN:
				case WM_RBUTTONDOWN:
				{
					keyboardEventData->Key = msg.wParam;
					keyboardEventData->IsKeyDown = true;
					EventSend(this, Events::MouseDown, keyboardEventData);
				}break;
				case WM_XBUTTONDOWN:
				{
					if (msg.wParam == 1)
					{
						keyboardEventData->Key = 4;
					}
					else
					{
						keyboardEventData->Key = 5;
					}
					keyboardEventData->IsKeyDown = true;
					EventSend(this, Events::MouseDown, keyboardEventData);
				}break;
				//It registers Middle mouse as 16!?!?!?!
				case WM_MBUTTONDOWN:
				{
					keyboardEventData->Key = 3;
					keyboardEventData->IsKeyDown = true;
					EventSend(this, Events::MouseDown, keyboardEventData);
				}break;
				//It registers the proper key on mouse DOWN but not UP?????
				case WM_LBUTTONUP:
				{
					keyboardEventData->Key = 1;
					keyboardEventData->IsKeyDown = false;
					EventSend(this, Events::MouseUp, keyboardEventData);
				}break;
				case WM_RBUTTONUP:
				{
					keyboardEventData->Key = 2;
					keyboardEventData->IsKeyDown = false;
					EventSend(this, Events::MouseUp, keyboardEventData);
				}break;
				case WM_MBUTTONUP:
				{
					keyboardEventData->Key = 3;
					keyboardEventData->IsKeyDown = false;
					EventSend(this, Events::MouseUp, keyboardEventData);
				}break;
				case WM_XBUTTONUP:
				{
					if (msg.wParam == 1)
					{
						keyboardEventData->Key = 4;
					}
					else
					{
						keyboardEventData->Key = 5;
					}
					keyboardEventData->IsKeyDown = false;
					EventSend(this, Events::MouseUp, keyboardEventData);
				}break;
                case WM_MOUSEMOVE:
                {
					keyboardEventData->MousePosition = Real2(static_cast<float>(msg.pt.x), static_cast<float>(msg.pt.y));
					EventSend(this, Events::MouseMove, keyboardEventData);
                }break;
				case WM_MOUSEWHEEL:
				{
					keyboardEventData->WheelDelta = msg.wParam;
					SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &keyboardEventData->ScrollLines, 0);
					EventSend(this, Events::MouseScroll, keyboardEventData);
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
	scrollEventHandle.Delete();
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

