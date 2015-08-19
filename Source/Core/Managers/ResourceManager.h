#pragma once
#include <Precompiled.h>
#include "DataLevel.h"
#include "DataObject.h"

class DataFile;

#define FreeResource(mapName) \
        { \
        auto comps = mapName.all(); \
                while (!comps.empty()) \
                        { \
        delete comps.front().second; \
        comps.popFront(); \
                        } \
        mapName.deallocate(); \
        } 

//template<typename T1, typename T2>
//using MapRange = Zero::HashedContainer<Zero::Pair<T1, T2>, Zero::PairHashAdapter<Zero::HashPolicy<T1>, T1, T2>, Zero::DefaultAllocator>::range;

class Icon
{
public:
    ZilchDeclareBaseType(Icon, TypeCopyMode::ReferenceType);
    Icon() {};
    Icon(String& name) : Name(name) {};
    void Initialize();
    HICON StoredIcon;

    String Name;
};

class FragmentShader
{
public:
    ZilchDeclareBaseType(FragmentShader, TypeCopyMode::ReferenceType);
    FragmentShader() {};
    FragmentShader(String& name) : Name(name) {};
    void Initialize();
    ID3DBlob* StoredShader;

    String Name;
    ~FragmentShader() { StoredShader->Release(); };
};

class VertexShader
{
public:
    ZilchDeclareBaseType(VertexShader, TypeCopyMode::ReferenceType);
    VertexShader() {};
    VertexShader(String& name) : Name(name) {};
    void Initialize();
    ID3DBlob* ShaderDesc;
	ID3D11VertexShader* StoredShader;
	DXInputLayout* InputLayout;

    String Name;
	~VertexShader() { ReleaseCOM(ShaderDesc); ReleaseCOM(StoredShader); ReleaseCOM(InputLayout); };
};

class PixelShader
{
public:
    ZilchDeclareBaseType(PixelShader, TypeCopyMode::ReferenceType);
    PixelShader() {};
    PixelShader(String& name) : Name(name) {};
    void Initialize();
	ID3DBlob* ShaderDesc;
	ID3D11PixelShader* StoredShader;

    String Name;
	~PixelShader() { ReleaseCOM(ShaderDesc); ReleaseCOM(StoredShader); };
};

typedef ID3D11ShaderResourceView DXTexture;

class Texture
{
public:
    ZilchDeclareBaseType(Texture, TypeCopyMode::ReferenceType);
    Texture() {};
    Texture(String& name);
    void LoadInTexture(ID3D11Device* device);
    void Initialize() {};
    Real4 Sample(Real2 uvCoordinates);
    DXTexture* StoredTexture = nullptr;

    String Name;
	~Texture() { ReleaseCOM(StoredTexture); };
};

class ResourceManager
{
public:
    ZilchDeclareBaseType(ResourceManager, TypeCopyMode::ReferenceType);
    static void Initialize();

    //Templates for object loading.
    template<typename T>
    static T* FindResourceType(const String& name)
    {
        HashMap<String, T*>* objects = nullptr;
        objects = (HashMap<String, T*>*) GetVector(ZilchTypeId(T));

        if (objects->containsKey(name))
        {
            return (*objects)[name];
        }
        Error("Vector of %s's has no resource named %s", ZilchTypeId(T)->ToString().c_str(), name.c_str());
        return nullptr;
    };
    template<typename T>
    static T* AddResource(const String& name)
    {
        HashMap<String, T*>* objects = nullptr;
        objects = (HashMap<String, T*>*) GetVector(ZilchTypeId(T));

        if (objects->containsKey(name))
        {
            Error("Vector of %s's already has a resource named %s", ZilchTypeId(T)->ToString().c_str(), name.c_str());
            return nullptr;
        }

        T* resource = new T(name);
        MemCheck(resource, ZilchTypeId(T)->ToString().c_str());
        resource->Initialize();
        objects->insert(name, resource);

        return resource;
    }
    //Deletes the file
    template<typename T>
    static void RemoveResource(const String& name)
    {
        HashMap<String, T*>* objects = nullptr;
        objects = (HashMap<String, T*>*) GetVector(ZilchTypeId(T));

        if (!objects->containsKey(name))
        {
            Error("Vector of %s's has no resource named %s", ZilchTypeId(T)->ToString().c_str(), name.c_str());
        }

        T* resource = (*objects)[name];
        delete resource;
        objects->erase(name);
    }
    static void SerializeAllFiles();
    
    static DataLevel* FindLevel(const String& levelName);

    static DataObject* ResourceManager::FindArchetype(const String& archName);

	//Very expensive to do this.
	template<typename T>
	static typename HashMap<String, T*>::range GetRange()
	{
		HashMap<String, T*>* objects = nullptr;

		objects = (HashMap<String, T*>*) GetVector(ZilchTypeId(T));

		return objects->all();
	}
    //Sounds

    //Fonts

    //Bitmaps

    template<typename T>
    static void RemapChild(const String& original, const String& newName)
    {
        HashMap<String, T*>* objects = nullptr;

        objects = (HashMap<String, T*>*) GetVector(ZilchTypeId(T));

        if (objects->containsKey(original))
        {
            if (objects->containsKey(newName))
            {
                Error("Vector of %s's already has a resource named %s", ZilchTypeId(T)->ToString().c_str(), newName.c_str());
            }
            else
            {
                objects->insert(newName, (*objects)[original]);
                objects->erase(original);
            }
        }
        else
        {
            Error("Vector of %s's nas no resource named %s", ZilchTypeId(T)->ToString().c_str(), original.c_str());
        }
    }

    static void Uninitialize();
private:
    static HashMap<String, BoundType*>* GetVector(Type* type);
    
    static HashMap<String, DataFile*> DataFiles;
    static HashMap<String, Icon*> Icons;
    static HashMap<String, FragmentShader*> FragmentShaders;
    static HashMap<String, VertexShader*> VertexShaders;
    static HashMap<String, PixelShader*> PixelShaders;
    static HashMap<String, Texture*> Textures;
};

