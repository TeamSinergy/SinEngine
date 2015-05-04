#include <Precompiled.h>
#include "ResourceManager.h"

HashMap<String, DataFile*> ResourceManager::DataFiles = HashMap<String, DataFile*>();
HashMap<String, Icon*> ResourceManager::Icons = HashMap<String, Icon*>();

#define SettingsPath "../Settings.data"
#define IconsPath "../Assets/Sprites/Icons"

DefineType(ResourceManager, SinningZilch)
{
    BindStaticMethodOverload(RemapChild<DataFile>, void, const String& original, const String& newName);
}

void ResourceManager::Initialize()
{
    //loading files
    Array<String> filepaths;
    filepaths.push_back(SettingsPath);
    Serializer::FindAllFilesInFolder(DataFilePath, ".data", filepaths);
    for (unsigned i = 0; i < filepaths.size(); ++i)
    {
        DataFile* file = new DataFile(filepaths[i]);
        MemCheck(file, filepaths[i].c_str());
        file->Initialize();
        DataFiles.insert(filepaths[i], file);
    }
    filepaths.clear();

    Serializer::FindAllFilesInFolder(IconsPath, ".ico", filepaths);
    for (unsigned i = 0; i < filepaths.size(); ++i)
    {
        Icon* file = new Icon(filepaths[i]);
        MemCheck(file, filepaths[i].c_str());
        file->Initialize();
        Icons.insert(filepaths[i], file);
    }
    filepaths.clear();
    //loading icons
}

void ResourceManager::SerializeAllFiles()
{
    for (unsigned i = 0; i < DataFiles.values().size(); ++i)
    {
        DataFiles.values().front()->Serialize();
    }
}

void ResourceManager::Uninitialize()
{
    for (unsigned i = 0; i < DataFiles.values().size(); ++i)
    {
        delete DataFiles.values().front();
        DataFiles.values().front() = nullptr;
        DataFiles.values().popFront();
    }
}

HashMap<String, BoundType*>* ResourceManager::GetVector(Type* type)
{
    if (type == ZilchTypeId(DataFile))
    {
        return (HashMap<String, BoundType*>*)&DataFiles;
    }
    else if (type == ZilchTypeId(Icon))
    {
        return (HashMap<String, BoundType*>*)&Icons;
    }
    else
    {
        Error("Could not find a vector of %s's", type->ToString().c_str());
    }
    return nullptr;
}


DefineType(Icon, SinningZilch)
{

}

void Icon::Initialize()
{
    StoredIcon = (HICON)LoadImage(NULL, String::Join("/", IconsPath, Name).c_str(), IMAGE_ICON,
        0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
}