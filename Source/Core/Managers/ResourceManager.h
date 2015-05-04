#pragma once
#include "DataFile.h" 

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
        Error("Vector of %s's already has no resource named %s", ZilchTypeId(T)->ToString().c_str(), name.c_str());
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
};

