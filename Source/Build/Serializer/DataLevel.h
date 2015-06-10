#pragma once
#include <Precompiled.h>
#include "Engine.h"
#include "DataFile.h"



class DataObject;

class DataLevel : public DataNode
{
public:
    ZilchDeclareDerivedType(DataLevel, DataNode);
    DataLevel(DataFile* const parent);
    DataLevel(DataFile* const parent, String* name, const ArrayRange<String*>& level);

    void LoadLevel(String* name, const ArrayRange<String*>& level);

    DataObject* AddObject(const String& Name); //Always modifies the file.
    DataObject* AddObject(DataObject* Object, bool modifyFile = true);
    bool RemoveObject(const String& Name);

    DataObject* FindObject(const String& Name);

    void SetName(const String& Name);
    const StringRange GetName();

    void RemapChild(const String& original, const String& newName);
    bool HasObject(const String& name);

    void PrintData();
    Unsigned2 GetRange();
    void SetRange(Unsigned2& range);

    const Array<DataObject*>&  AllObjects() { return DataObjectArray; }

    ~DataLevel();

private:
    DataFile* const Parent;
    String* Name;
    Type* Type = ZilchTypeId(SinEntity);
    ArrayRange<String*> FileData; //Whole Level
    Array<DataObject*> DataObjectArray;
    HashMap<StringRange, DataObject*> DataObjects;

};