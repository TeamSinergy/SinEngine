#pragma once
#include <Precompiled.h>
#include "Engine.h"
#include "DataObject.h"


class DataProperty;

class DataComponent : public DataNode
{
public:
    ZilchDeclareDerivedType(DataComponent, DataNode);
    DataComponent(DataObject* const parent);
    DataComponent(DataObject* const parent, String* name, const ArrayRange<String*>& level);

    void LoadLevel(String* name, const ArrayRange<String*>& level);

    DataProperty* AddProperty(const String& type, const String& Name, const String& value); //Always modifs the file.
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

    unsigned FindFirstGlobalIndexOfProperty(String* input);
    const Array<DataProperty*>&  AllProperties() { return DataPropertyArray; }

    ~DataComponent();

private:
    DataObject* const Parent;
    String* Name;
    Type* Type = ZilchTypeId(SinEntity);
    ArrayRange<String*> FileData; //Whole Level
    Array<DataProperty*> DataPropertyArray;
    HashMap<StringRange, DataProperty*> DataProperties;
};