#pragma once
#include <Precompiled.h>
#include "Engine.h"
#include "DataObject.h"


class DataProperty;

class DataComponent
{
public:
    ZilchDeclareBaseType(DataComponent, TypeCopyMode::ReferenceType);
    DataComponent(DataObject* const parent);
    DataComponent(DataObject* const parent, String* name, const ArrayRange<String*>& level);

    void LoadLevel(String* name, const ArrayRange<String*>& level);

    DataProperty* AddProperty(const String& Name, const String& type, const String& value); //Always modifs the file.
    DataProperty* AddProperty(DataProperty* Property, bool modifyFile = true);
    bool RemoveProperty(const String& Name);

    DataProperty* FindProperty(const String& Name);

    void SetName(const String& Name);
    const StringRange GetName();

    void RemapChild(const String& original, const String& newName);

    bool HasProperty(const String& name);

    void PrintData();
    Unsigned2 GetRange();
    void SetRange(Unsigned2& range);
    ~DataComponent();

private:
    DataObject* const Parent;
    String* Name;
    Type* Type = ZilchTypeId(SinEntity);
    ArrayRange<String*> FileData; //Whole Level
    HashMap<StringRange, DataProperty*> DataProperties;
};