#pragma once
#include <Precompiled.h>
#include "Engine.h"
#include "DataFile.h"

class DataObject
{

};

class DataFile;

class DataLevel
{
public:
    ZilchDeclareBaseType(DataLevel, TypeCopyMode::ReferenceType);
    DataLevel(const DataFile* parent);
    DataLevel(const DataFile* parent, const StringRange& name, const StringRange& level);

    void LoadLevel(const StringRange& name, const StringRange& level);

    const DataLevel* AddObject(const String& Name);
    void RemoveObject(const String& Name);

    const DataLevel* FindObject(const String& Name);

    void SetName(const String& Name);
    const StringRange& GetName();

    ~DataLevel();

private:
    const DataFile* const Parent;
    StringRange Name;
    Type* Type = ZilchTypeId(SinEntity);
    Array<String> FileData; //Whole File
    HashMap<StringRange, DataObject*> SinObjects;

};