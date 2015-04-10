#pragma once
#include <Precompiled.h>
#include "Engine.h"

#define DataFilePath "../Assets/DataFiles"

class DataLevel
{

};

class DataFile
{
public:
    ZilchDeclareBaseType(DataFile, TypeCopyMode::ReferenceType);
    DataFile(const String& Name);

    const DataLevel* AddLevel(const String& Name);
    void RemoveLevel(const String& Name);
    void RemoveLevel(String Name);

    const DataLevel* FindLevel(const String& Name);

    void SetName(const String& Name);
    const String& GetName();

    void Serialize();

    ~DataFile();

private:

    String Name;
    Type* Type = ZilchTypeId(SinEngine);
    Array<String> FileData; //Whole File
    HashMap<String, DataLevel> SinObjects;

    std::ifstream buffer;
};