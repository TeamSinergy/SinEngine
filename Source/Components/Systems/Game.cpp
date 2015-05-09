#include <Precompiled.h>

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
    WindowSystem->InitializePipeline();
    WindowSystem->SetWindowAsViewport(WindowSystem);
    WindowSystem->DrawDebugTriangle();
    Update();
}

void Game::Update()
{
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
            WindowSystem->RenderFrame();
        }
    }
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