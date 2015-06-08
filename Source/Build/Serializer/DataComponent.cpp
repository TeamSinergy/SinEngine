#include <Precompiled.h>
#include "Serializer.h"
#include "DataComponent.h"
#include "DataProperty.h"

DefineType(DataComponent, SinningZilch)
{
    BindMethod(SetName);
}

DataComponent::DataComponent(DataObject* const parent) : Parent(parent), DataNode()
{
}

DataComponent::DataComponent(DataObject* const parent, String* name, const ArrayRange<String*>& level) : Parent(parent), DataNode()
{
    LoadLevel(name, level);
}


void DataComponent::LoadLevel(String* name, const ArrayRange<String*>& level)
{
    Name = name;
    FileData = level;
    SinWrite("Creating Component: ");
    SinWriteLineColor(GetName(), ConsoleColors::DarkYellow);
}

//Always modifes the file.
DataProperty* DataComponent::AddProperty(const String& type, const String& name, const String& value)
{
    if (DataProperties.containsKey(name))
    {
        Error("Object %s already has a Property with name %s", String(GetName()).c_str(), name.c_str());
        return nullptr;
    }
    String Padding = String::Join("", DataSyntax::Padding, DataSyntax::Padding, DataSyntax::Padding);
    String Content = String::Join("", type," ", name, DataSyntax::EndName);
    FileData.insertAt(new String(String::Join("", Padding, Content, value, DataSyntax::PropertyEnd)), FileData.size());

    DataProperty* obj = new DataProperty(this, FileData[FileData.size() - 1], FileData.range().y - 1);
    MemCheck(obj, obj->GetName());
    Unsigned2 newParentRange = Parent->GetRange();
    newParentRange.y += 1;
    Parent->SetRange(newParentRange);
    DataProperties.insert(obj->GetName(), obj);
    DataPropertyArray.push_back(obj);

    return nullptr;
}

DataProperty* DataComponent::AddProperty(DataProperty* Object, bool modifyFile)
{
    if (DataProperties.containsKey(Object->GetName()))
    {
        Error("Object %s already has a Property with name %s", *Name, Object->GetName());
        return nullptr;
    }

    DataProperties.insert(Object->GetName(), Object);
    DataPropertyArray.push_back(Object);
    if (modifyFile)
    {
        String Padding = String::Join("", DataSyntax::Padding, DataSyntax::Padding, DataSyntax::Padding);
        String Content = String::Join("", Object->GetName(), Object->GetType()->ToString(), DataSyntax::EndName, Object->GetValueString());
        FileData.insertAt(new String(String::Join("", Padding, Content, DataSyntax::PropertyEnd)), FileData.size());

        Unsigned2 newParentRange = Parent->GetRange();
        newParentRange.y += 1;
        Parent->SetRange(newParentRange);
    }
    return Object;
}

bool DataComponent::RemoveProperty(const String& name)
{
    if (DataProperties.containsKey(name))
    {
        DataProperty* obj = FindProperty(name);
        unsigned index = obj->GetIndex();
        DataPropertyArray.erase_value(DataProperties[name]);
        delete DataProperties[name];
        FileData.eraseAt(index - FileData.range().x);
        
        DataProperties.erase(name);
        return true;
    }
    Error("Could not find Property %s on object %s", name.c_str(), String(GetName()).c_str());
    return false;
}

DataProperty* DataComponent::FindProperty(const String& name)
{
    if (HasProperty(name))
    {
        return DataProperties[name];
    }
    Error("Could not find Property %s on object %s", name.c_str(), String(GetName()).c_str());
    return nullptr;
}

bool DataComponent::HasProperty(const String& name)
{
    return DataProperties.containsKey(name);
}

void DataComponent::SetName(const String& name)
{
    Parent->RemapChild(GetName(), name);
    unsigned start = (*Name).FindFirstNonWhitespaceCharIndex();
    (*Name) = String::Join("", (*Name).sub_string(0, start), name, DataSyntax::EndName);
}

const StringRange DataComponent::GetName()
{
    unsigned start = (*Name).FindFirstNonWhitespaceCharIndex();
    return (*Name).sub_string(start, (*Name).size() - DataSyntax::EndName.size() - start);
}

void DataComponent::RemapChild(const String& original, const String& newName)
{
    if (DataProperties.containsKey(original))
    {
        if (DataProperties.containsKey(newName))
        {
            Error("Object %s already has a Property named %s", GetName(), newName);
        }
        else
        {
            DataProperties.insert(newName, DataProperties[original]);
            DataProperties.erase(original);
        }
    }
    else
    {
        Error("Object %s has no Property called %s", GetName(), original);
    }
}

void DataComponent::PrintData()
{
    for (unsigned i = 0; i < FileData.size(); ++i)
    {
        SinWriteLine(*FileData[i]);
    }
}

Unsigned2 DataComponent::GetRange()
{
    unsigned newStart = FileData.FindFirstGlobalIndexOf(Name) + 2;
    Unsigned2 oldRange = FileData.range();
    oldRange.y = newStart + FileData.size();
    oldRange.x = newStart;
    FileData.SetRange(oldRange);
    return FileData.range();
}
//DO NOT USE
void DataComponent::SetRange(Unsigned2& range)
{
    Unsigned2 newRange = Parent->GetRange();
    newRange.y += range.y - GetRange().y;
    FileData.SetRange(range);
}

unsigned DataComponent::FindFirstGlobalIndexOfProperty(String* input)
{
    return FileData.FindFirstGlobalIndexOf(input);
}

DataComponent::~DataComponent()
{
    FreeHashMap(DataProperties);
}