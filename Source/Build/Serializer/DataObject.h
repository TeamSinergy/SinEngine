#pragma once
#include <Precompiled.h>
#include "Engine.h"


class DataLevel;
class DataComponent;

class DataObject : public DataNode
{
public:
    ZilchDeclareDerivedType(DataObject, DataNode);
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

    const Array<DataComponent*>&  AllComponents() { return DataComponentArray; }

    ~DataObject();

private:
    DataLevel* const Parent;
    String* Name;
    Type* Type = ZilchTypeId(SinEntity);
    ArrayRange<String*> FileData; //Whole Level
    Array<DataComponent*> DataComponentArray;
    HashMap<StringRange, DataComponent*> DataComponents;
};