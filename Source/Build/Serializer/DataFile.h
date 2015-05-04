#pragma once
#include <Precompiled.h>
#include "Engine.h"
#include "ArrayRange.h"

#define DataFilePath "../Assets/DataFiles"

class DataLevel;

#define SyntaxError(condition, line, fileName, expected) ErrorIf(condition, \
    "Syntax Error on line %i of %s.\n\tExpected: |%s|\n", line + 1, fileName.c_str(), expected.c_str())

struct DataRange
{
    DataRange() : Name(nullptr), StringRange(ArrayRange<String*>()){}
    DataRange(String* name, Array<String*>* refrence, Unsigned2 range) : Name(name), StringRange(ArrayRange<String*>(refrence, range)){}
    DataRange(String* name, Array<String*>* refrence) : Name(name), StringRange(ArrayRange<String*>(refrence, Unsigned2(0, 0))){}
    String* Name;
    unsigned Children = 0;
    ArrayRange<String*> StringRange;
};

enum Depth
{
    Level = 0,
    Object = 1,
    Component = 2,
    Property = 3
};

class DataFile : public DataNode
{
public:
    ZilchDeclareBaseType(DataFile, TypeCopyMode::ReferenceType);
    DataFile() = default;
    DataFile(const String& Name);

    void Initialize();

    DataLevel* AddLevel(const String& Name);
    void RemoveLevel(const String& Name);

    DataLevel* FindLevel(const String& Name);
    bool HasLevel(const String& name);

    void SetName(const String& Name);
    const String& GetName();

    void RemapChild(const String& original, const String& newName);

    void PrintData();

    void Serialize();

    ~DataFile();

private:

    String Name;
    Type* Type = ZilchTypeId(DataFile);
    Array<String*> FileData; //Whole File
    HashMap<String, DataLevel*> DataLevels;

    
};