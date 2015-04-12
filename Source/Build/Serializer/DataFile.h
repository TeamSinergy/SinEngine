#pragma once
#include <Precompiled.h>
#include "Engine.h"

#define DataFilePath "../Assets/DataFiles"

class DataLevel;

#define SyntaxError(condition, line, fileName, expected) ErrorIf(condition, \
    "Syntax Error on line %i of %s.\n\tExpected: |%s|\n", line, fileName.c_str(), expected.c_str())

class DataFile
{
public:
    ZilchDeclareBaseType(DataFile, TypeCopyMode::ReferenceType);
    DataFile() = default;
    DataFile(const String& Name);

    void LoadFile(const String& Name);

    const DataLevel* AddLevel(const String& Name);
    void RemoveLevel(const String& Name);

    const DataLevel* FindLevel(const String& Name);

    void SetName(const String& Name);
    const String& GetName();

    void Serialize();

    ~DataFile();

private:

    String Name;
    Type* Type = ZilchTypeId(DataFile);
    Array<String> FileData; //Whole File
    HashMap<String, DataLevel*> SinLevels;

    std::ifstream buffer;
};