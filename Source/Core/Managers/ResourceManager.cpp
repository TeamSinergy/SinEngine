#include <Precompiled.h>
#include "ResourceManager.h"


HashMap<String, DataFile*> ResourceManager::DataFiles = HashMap<String, DataFile*>();
HashMap<String, Icon*> ResourceManager::Icons = HashMap<String, Icon*>();
HashMap<String, VertexShader*> ResourceManager::VertexShaders = HashMap<String, VertexShader*>();
HashMap<String, FragmentShader*> ResourceManager::FragmentShaders = HashMap<String, FragmentShader*>();
HashMap<String, PixelShader*> ResourceManager::PixelShaders = HashMap<String, PixelShader*>();

#define SettingsPath "../Settings.data"
#define IconsPath "../Assets/Sprites/Icons"
#define VertexShaderPath "../Assets/Shaders/Vertex"
#define FragmentShaderPath "../Assets/Shaders/Fragment"
#define PixelShaderPath "../Assets/Shaders/Pixel"

#define LoadIn(type, dir, path, vector)                                 \
    Serializer::FindAllFilesInFolder(dir, path, filepaths);             \
    for (unsigned i = 0; i < filepaths.size(); ++i)                     \
    {                                                                   \
        type* file = new type(filepaths[i]);                            \
        MemCheck(file, filepaths[i].c_str());                           \
        file->Initialize();                                             \
        vector.insert(filepaths[i], file);                              \
    }                                                                   \
    filepaths.clear();                                                  \

DefineType(ResourceManager, SinningZilch)
{
    BindStaticMethodOverload(RemapChild<DataFile>, void, const String& original, const String& newName);
}

void ResourceManager::Initialize()
{
    //loading files
    Array<String> filepaths;
    filepaths.push_back(SettingsPath);
    LoadIn(DataFile, DataFilePath, ".data", DataFiles);

    //loading icons
    LoadIn(Icon, IconsPath, ".ico", Icons);

    LoadIn(VertexShader, VertexShaderPath, ".vert", VertexShaders);
    LoadIn(FragmentShader, FragmentShaderPath, ".frag", FragmentShaders);
    LoadIn(PixelShader, PixelShaderPath, ".pix", PixelShaders);
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
    else if (type == ZilchTypeId(VertexShader))
    {
        return (HashMap<String, BoundType*>*)&VertexShaders;
    }
    else if (type == ZilchTypeId(FragmentShader))
    {
        return (HashMap<String, BoundType*>*)&FragmentShaders;
    }
    else if (type == ZilchTypeId(PixelShader))
    {
        return (HashMap<String, BoundType*>*)&PixelShaders;
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

DefineType(FragmentShader, SinningZilch)
{
}

void FragmentShader::Initialize()
{
    Utility::CompileShader(String::Join("/", FragmentShaderPath, Name), "FSMain", "fs_4_0_level_9_1", &StoredShader);
}

DefineType(PixelShader, SinningZilch)
{
}

void PixelShader::Initialize()
{
    Utility::CompileShader(String::Join("/", PixelShaderPath, Name), "PSMain", "ps_4_0_level_9_1", &StoredShader);
}

DefineType(VertexShader, SinningZilch)
{
}

void VertexShader::Initialize()
{
    Utility::CompileShader(String::Join("/", VertexShaderPath, Name).c_str(), "VSMain", "vs_4_0_level_9_1", &StoredShader);
    //Utility::CompileShader("VertexTest.V", "VSMain", "vs_4_0_level_9_1", &StoredShader);
}