#include <Precompiled.h>
#include "Serializer.h"
#include "DataComponent.h"

DefineType(DataObject, SinningZilch)
{
    BindMethod(SetName);
}

DataObject::DataObject(DataLevel* const parent) : Parent(parent)
{
}

DataObject::DataObject(DataLevel* const parent, String* name, const ArrayRange<String*>& level) : Parent(parent)
{
    LoadLevel(name, level);
}


void DataObject::LoadLevel(String* name, const ArrayRange<String*>& level)
{
    Name = name;
    FileData = level;
    SinWrite("Creating Object: ");
    SinWriteLineColor(GetName(), ConsoleColors::Blue);
}

//Always modifes the file.
DataComponent* DataObject::AddComponent(const String& name)
{
    if (DataComponents.containsKey(name))
    {
        Error("Object %s already has a component with name %s", String(GetName()).c_str(), name.c_str());
        return nullptr;
    }

    FileData.insertAt(new String(String::Join("", DataSyntax::Padding, DataSyntax::Padding, name, DataSyntax::EndName)), FileData.size());
    FileData.insertAt(new String(String::Join("", DataSyntax::Padding, DataSyntax::Padding, DataSyntax::ObjectStart)), FileData.size());
    FileData.insertAt(new String(String::Join("", DataSyntax::Padding, DataSyntax::Padding, DataSyntax::ObjectEnd)), FileData.size());
    MemCheck(FileData[FileData.size() - 3], "String in DataObject.cpp");
    MemCheck(FileData[FileData.size() - 2], "String in DataObject.cpp");
    MemCheck(FileData[FileData.size() - 1], "String in DataObject.cpp");

    DataComponent* obj = new DataComponent(this, FileData[FileData.size() - 3], ArrayRange<String*>(FileData, Unsigned2(FileData.range().y - 1, FileData.range().y - 1)));
    MemCheck(obj, obj->GetName());
    Unsigned2 newParentRange = Parent->GetRange();
    newParentRange.y += 3;
    Parent->SetRange(newParentRange);
    DataComponents.insert(obj->GetName(), obj);




    return obj;
}

DataComponent* DataObject::AddComponent(DataComponent* Object, bool modifyFile)
{
    if (DataComponents.containsKey(Object->GetName()))
    {
        Error("Object %s already has a component with name %s", *Name, Object->GetName());
        return nullptr;
    }

    DataComponents.insert(Object->GetName(), Object);
    if (modifyFile)
    {
        FileData.insertAt(new String(String::Join("", DataSyntax::Padding, DataSyntax::Padding, Object->GetName(), DataSyntax::EndName)), FileData.size() - 1);
        FileData.insertAt(new String(String::Join("", DataSyntax::Padding, DataSyntax::Padding, DataSyntax::ObjectStart)), FileData.size() - 1);
        FileData.insertAt(new String(String::Join("", DataSyntax::Padding, DataSyntax::Padding, DataSyntax::ObjectEnd)), FileData.size() - 1);
        MemCheck(FileData[FileData.size() - 3], "String in DataObject.cpp");
        MemCheck(FileData[FileData.size() - 2], "String in DataObject.cpp");
        MemCheck(FileData[FileData.size() - 1], "String in DataObject.cpp");

        Unsigned2 newParentRange = Parent->GetRange();
        newParentRange.y += 1;
        Parent->SetRange(newParentRange);
    }
    return Object;
}

bool DataObject::RemoveComponent(const String& name)
{
    if (DataComponents.containsKey(name))
    {
        DataComponent* obj = FindComponent(name);
        Unsigned2 range = obj->GetRange();
        unsigned size = range.y - range.x;

        Unsigned2 newParentRange = Parent->GetRange();
        newParentRange.y -= size + 3;
        Parent->SetRange(newParentRange);

        while (size)
        {
            FileData.eraseAt(range.x - FileData.range().x);
            --size;
        }
        FileData.eraseAt(range.x - FileData.range().x - 2);
        FileData.eraseAt(range.x - FileData.range().x - 2);
        FileData.eraseAt(range.x - FileData.range().x - 2);
        
        

        delete DataComponents[name];
        DataComponents.erase(name);
        return true;
    }
    Error("Could not find component %s on object %s", name.c_str(), String(GetName()).c_str());
    return false;
}

DataComponent* DataObject::FindComponent(const String& name)
{
    if (HasComponent(name))
    {
        return DataComponents[name];
    }
    Error("Could not find component %s on object %s", name.c_str(), String(GetName()).c_str());
    return nullptr;
}

bool DataObject::HasComponent(const String& name)
{
    return DataComponents.containsKey(name);
}

void DataObject::SetName(const String& name)
{
    Parent->RemapChild(GetName(), name);
    unsigned start = (*Name).FindFirstNonWhitespaceCharIndex();
    (*Name) = String::Join("", (*Name).sub_string(0, start), name, DataSyntax::EndName);
}

const StringRange DataObject::GetName()
{
    unsigned start = (*Name).FindFirstNonWhitespaceCharIndex();
    return (*Name).sub_string(start, (*Name).size() - DataSyntax::EndName.size() - start);
}

void DataObject::RemapChild(const String& original, const String& newName)
{
    if (DataComponents.containsKey(original))
    {
        if (DataComponents.containsKey(newName))
        {
            Error("Object %s already has a component named %s", GetName(), newName);
        }
        else
        {
            DataComponents.insert(newName, DataComponents[original]);
            DataComponents.erase(original);
        }
    }
    else
    {
        Error("Object %s has no component called %s", GetName(), original);
    }
}

void DataObject::PrintData()
{
    for (unsigned i = 0; i < FileData.size(); ++i)
    {
        SinWriteLine(*FileData[i]);
    }
}

Unsigned2 DataObject::GetRange()
{
    unsigned newStart = FileData.FindFirstGlobalIndexOf(Name) + 2;
    Unsigned2 oldRange = FileData.range();
    oldRange.y = newStart + FileData.size();
    oldRange.x = newStart;
    FileData.SetRange(oldRange);
    return FileData.range();
}
//DO NOT USE
void DataObject::SetRange(Unsigned2& range)
{
    Unsigned2 newRange = Parent->GetRange();
    newRange.y += range.y - GetRange().y;
    FileData.SetRange(range);
}

DataObject::~DataObject()
{
    for (unsigned i = 0; i < DataComponents.values().size(); ++i)
    {
        delete DataComponents.values().front();
        DataComponents.values().front() = nullptr;
        DataComponents.values().popFront();
    }
    DataComponents.deallocate();
}