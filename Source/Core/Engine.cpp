#include <Precompiled.h>

DefineType(Engine, SinningZilch)
{
    BindConstructor();
    BindDestructor();
}

void Engine::Serialize(DataNode* node)
{
    DataLevel* fileData = static_cast<DataLevel*>(node);
    Name = fileData->GetName();
    //Actually number of GameSessions.
    const Array<DataObject*>& levels = fileData->AllObjects();

    for (unsigned i = 0; i < levels.size(); ++i)
    {
        GameSessions.push_back(ZilchAllocate<Game>());
        ((Game*)GameSessions.back().Dereference())->Serialize(levels[i]);
    }
}
void Engine::Create()
{
    for (unsigned i = 0; i < GameSessions.size(); ++i)
    {
        ((Game*)GameSessions[i].Dereference())->Create();
    }
}
void Engine::Initialize()
{
    for (unsigned i = 0; i < GameSessions.size(); ++i)
    {
        ((Game*)GameSessions[i].Dereference())->Initialize();
    }
}
void Engine::Uninitialize()
{
    for (unsigned i = 0; i < GameSessions.size(); ++i)
    {
        ((Game*)GameSessions[i].Dereference())->Uninitialize();
    }
}
void Engine::Destroy()
{
    for (unsigned i = 0; i < GameSessions.size(); ++i)
    {
        ((Game*)GameSessions[i].Dereference())->Destroy();
        GameSessions[i].Delete();
    }
}