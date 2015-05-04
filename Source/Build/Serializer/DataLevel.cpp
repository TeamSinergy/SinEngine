#include <Precompiled.h>
#include "Serializer.h"
#include "DataLevel.h"
#include "DataObject.h"


DefineType(DataLevel, SinningZilch)    
{
    BindMethod(SetName);
}

DataLevel::DataLevel(DataFile* const parent) : Parent(parent)
{
}

DataLevel::DataLevel(DataFile* const parent, String* name, const ArrayRange<String*>& level) : Parent(parent)
{
    LoadLevel(name, level);
}


void DataLevel::LoadLevel(String* name, const ArrayRange<String*>& level)
{
    Name = name;
    FileData = level;
    SinWrite("Creating Level: ");
    SinWriteLineColor(GetName(), ConsoleColors::DarkMagenta);
}

//Always modifes the file.
DataObject* DataLevel::AddObject(const String& name)
{
    if (DataObjects.containsKey(name))
    {
        Error("Level %s already has an object with name %s", *Name, name);
        return nullptr;
    }

    FileData.insertAt(new String(String::Join("", DataSyntax::Padding, name, DataSyntax::EndName)), FileData.size());
    FileData.insertAt(new String(String::Join("", DataSyntax::Padding, DataSyntax::ObjectStart)), FileData.size());
    FileData.insertAt(new String(String::Join("", DataSyntax::Padding, DataSyntax::ObjectEnd)), FileData.size());
    MemCheck(FileData[FileData.size() - 2], "String in DataLevel.cpp");
    MemCheck(FileData[FileData.size() - 1], "String in DataLevel.cpp");
    MemCheck(FileData[FileData.size()], "String in DataLevel.cpp");

    DataObject* obj = new DataObject(this, FileData[FileData.size() - 3], ArrayRange<String*>(FileData, Unsigned2(FileData.range().y - 1, FileData.range().y - 1)));
    MemCheck(obj, name);
    DataObjects.insert(name, obj);
    

    return obj;
}

DataObject* DataLevel::AddObject(DataObject* Object, bool modifyFile)
{
    if (DataObjects.containsKey(Object->GetName()))
    {
        Error("Level %s already has an object with name %s", *Name, Object->GetName());
        return nullptr;
    }

    DataObjects.insert(Object->GetName(), Object);

    if (modifyFile)
    {
        FileData.insertAt(new String(String::Join("", DataSyntax::Padding, Object->GetName(), DataSyntax::EndName)), FileData.size());
        FileData.insertAt(new String(String::Join("", DataSyntax::Padding, DataSyntax::ObjectStart)), FileData.size());
        FileData.insertAt(new String(String::Join("", DataSyntax::Padding, DataSyntax::ObjectEnd)), FileData.size());
        MemCheck(FileData[FileData.size() - 2], "String in DataLevel.cpp");
        MemCheck(FileData[FileData.size() - 1], "String in DataLevel.cpp");
        MemCheck(FileData[FileData.size()], "String in DataLevel.cpp");
    
    }
    return Object;
}

bool DataLevel::RemoveObject(const String& name)
{
    if (DataObjects.containsKey(name))
    {
        DataObject* obj = FindObject(name);
        Unsigned2 range = obj->GetRange();
        unsigned size = range.y - range.x;

        while (size)
        {
            FileData.eraseAt(range.x - FileData.range().x);
            --size;
            //FileData.SetRangeY(FileData.range().y - 1);
        }
        FileData.eraseAt(range.x - FileData.range().x - 2);
        FileData.eraseAt(range.x - FileData.range().x - 2);
        FileData.eraseAt(range.x - FileData.range().x - 2);

        delete DataObjects[name];
        DataObjects.erase(name);
        return true;
    }
    Error("Could not find object %s in level %s", name.c_str(), String(GetName()).c_str());
    return false;
}

DataObject* DataLevel::FindObject(const String& name)
{
    if (HasObject(name))
    {
        return DataObjects[name];
    }
    Error("Could not find object %s in level %s", name.c_str(), String(GetName()).c_str());
    return nullptr;
}

bool DataLevel::HasObject(const String& name)
{
    return DataObjects.containsKey(name);
}

void DataLevel::SetName(const String& name)
{
    Parent->RemapChild(GetName(), name);
    unsigned start = (*Name).FindFirstNonWhitespaceCharIndex();
    (*Name) = String::Join("",(*Name).sub_string(0, start), name, DataSyntax::EndName);
}

const StringRange DataLevel::GetName()
{
    unsigned start = (*Name).FindFirstNonWhitespaceCharIndex();
    return (*Name).sub_string(start, (*Name).size() - DataSyntax::EndName.size() - start);
}

void DataLevel::RemapChild(const String& original, const String& newName)
{
    if (DataObjects.containsKey(original))
    {
        if (DataObjects.containsKey(newName))
        {
            Error("Level %s already has an object named %s", GetName(), newName);
        }
        else
        {
            DataObjects.insert(newName, DataObjects[original]);
            DataObjects.erase(original);
        }
    }
    else
    {
        Error("Level %s has no object called %s", GetName(), original);
    }
}

void DataLevel::PrintData()
{
    for (unsigned i = 0; i < FileData.size(); ++i)
    {
        SinWriteLine(*FileData[i]);
    }
}

Unsigned2 DataLevel::GetRange()
{
    return FileData.range();
}
//DO NOT USE
void DataLevel::SetRange(Unsigned2& range)
{
    unsigned newStart = FileData.FindFirstGlobalIndexOf(Name) + 2;
    Unsigned2 oldRange = FileData.range();
    oldRange.y = newStart + FileData.size();
    oldRange.x = newStart;
    FileData.SetRange(oldRange);
    FileData.SetRange(range);
}


DataLevel::~DataLevel()
{
    for (unsigned i = 0; i < DataObjects.values().size(); ++i)
    {
        delete DataObjects.values().front();
        DataObjects.values().front() = nullptr;
        DataObjects.values().popFront();
    }
    DataObjects.deallocate();
}