#pragma once
#include <Precompiled.h>
#include "Engine.h"
#include "DataLevel.h"


class DataComponent;

class DataObject : public DataNode
{
public:
    ZilchDeclareBaseType(DataObject, TypeCopyMode::ReferenceType);
    DataObject(DataLevel* const parent);
    DataObject(DataLevel* const parent, String* name, const ArrayRange<String*>& level);

    void LoadLevel(String* name, const ArrayRange<String*>& level);

    DataComponent* AddComponent(const String& Name); //Always modifs the file.
    DataComponent* AddComponent(DataComponent* Component, bool modifyFile = true);
    bool RemoveComponent(const String& Name);

    DataComponent* FindComponent(const String& Name);

    void SetName(const String& Name);
    const StringRange GetName();

    void RemapChild(const String& original, const String& newName);

    bool HasComponent(const String& name);

    void PrintData();
    Unsigned2 GetRange();
    void SetRange(Unsigned2& range);
    ~DataObject();

private:
    DataLevel* const Parent;
    String* Name;
    Type* Type = ZilchTypeId(SinEntity);
    ArrayRange<String*> FileData; //Whole Level
    HashMap<StringRange, DataComponent*> DataComponents;
};