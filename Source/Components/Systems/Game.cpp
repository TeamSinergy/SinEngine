#include <Precompiled.h>

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

void Game::Serialize(DataNode* node)
{
    DataObject* objectData = static_cast<DataObject*>(node);
    const Array<DataComponent*>& ComponentsData = objectData->AllComponents();
    Name = objectData->GetName();
    
    for (unsigned i = 0; i < ComponentsData.size(); ++i)
    {
        if (ComponentsData[i]->GetName() == StartingSpaceComp)
        {
            String fileName = DefaultLevelName;
            SerializeValueName(ComponentsData[i], fileName, FilePropertyName);
            DataFile* fileData = ResourceManager::FindResourceType<DataFile>(fileName);

            String levelName = DefaultLevelName;
            SerializeValueName(ComponentsData[i], levelName, LevelPropertyName);
            DataLevel* levelData = fileData->FindLevel(levelName);

            if (levelData)
            {
                LoadLevel(levelData);
            }
            continue;
        }
        Components.push_back(Utility::Components[ComponentsData[i]->GetName()]->Create());
        ((Component*)Components.back().Dereference())->Owner = this;
        ((Component*)Components.back().Dereference())->Serialize(ComponentsData[i]);
    }
}
void Game::Create()
{
    for (unsigned i = 0; i < Components.size(); ++i)
    {
        ((Component*)Components[i].Dereference())->Create();
    }
    
}

void Game::Initialize()
{
    for (unsigned i = 0; i < Components.size(); ++i)
    {
        ((Component*)Components[i].Dereference())->Initialize();
    }
    //WindowSystem->InitializePipeline();
    //WindowSystem->SetWindowAsViewport(WindowSystem);
    //WindowSystem->DrawDebugTriangle();
    Update();
}

void Game::Update()
{
    // this struct holds Windows event messages
    MSG msg;
    UpdateEvent* eventData = new UpdateEvent();
    MemCheck(eventData, "UpdateEvent");
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
            WindowSystem->RenderFrame();
        }
        //CALCULATE DELTA TIME PROPERLY!
        eventData->Dt = 0.016666666;
        EventSend(this, "GameUpdate", eventData);
    }
}

void Game::LoadLevel(DataLevel* level)
{
    Space* space = new Space(level->GetName());
    space->GameSession = this;
    space->Serialize(level);
    space->Create();
    space->Initialize();
    Spaces.insert(space->Name, space);
    
}

void Game::UnloadLevel(const String& levelName)
{
    Space* space = Spaces[levelName];
    space->Uninitialize();
    space->Destroy();
    delete space;

    Spaces.erase(space->Name);

}

void Game::Uninitialize()
{
    for (unsigned i = 0; i < Components.size(); ++i)
    {
        ((Component*)Components[i].Dereference())->Uninitialize();
    }
}
void Game::Destroy()
{
    for (unsigned i = 0; i < Components.size(); ++i)
    {
        ((Component*)Components[i].Dereference())->Destroy();
    }
}

Game::~Game()
{
    auto comps = Spaces.all(); 
    while (!comps.empty()) 
    { 
        Space* space = comps.front().second;
        space->Uninitialize();
        space->Destroy();
        delete comps.front().second; 
        comps.popFront(); 
    } 
    Spaces.deallocate();
}

