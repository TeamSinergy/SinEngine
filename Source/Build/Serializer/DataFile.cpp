#include <Precompiled.h>
#include "Serializer.h"
#include "DataFile.h"
#include "DataLevel.h"
#include "DataObject.h"
#include "DataComponent.h"
#include "DataProperty.h"



DefineType(DataFile, SinningZilch)    
{
    BindConstructor();
    BindDestructor();
}

DataFile::DataFile(const String& fileName) : Name(fileName)
{
    
}

void DataFile::Initialize()
{
    String path = String::Join("/", DataFilePath, Name);
    std::ifstream buffer;
    buffer.open(path.c_str());

    std::string lineString;

    if (buffer.is_open())
    {
        SinWrite("Opened File: ");
        SinWriteLineColor(Name, ConsoleColors::Yellow);
        while (getline(buffer, lineString))
        {
            //SinWriteLine(line.c_str());
            FileData.push_back(new String(lineString.c_str()));
            MemCheck(FileData.back(), "String in DataFile.cpp");
        }
        if (FileData.size())
        {
            
            //The Levels, then the Objects, then the Components, then the properties. 
            unsigned depth = Depth::Level;

            
            Array<DataRange> levels;
            Array<DataRange> objects;
            Array<DataRange> components;
            
            bool lookingForName = true;
            
            Array<unsigned> objectStart;
            
            //I'm sorry this is so complicated...
            for (unsigned line = 0; line < FileData.size(); ++line) //For each line in the file, starting just after the first level name.
            {
                unsigned start = FileData[line]->FindFirstNonWhitespaceCharIndex();
                if (FileData[line]->sub_string(start, FileData[line]->size()).FindFirstOf(DataSyntax::LineCommentStart) == 0) //the line is a comment
                {
                    continue;
                }
                else if (start == String::InvalidIndex)
                {
                    continue;
                }
                if (lookingForName)
                {
                    
                    unsigned end = FileData[line]->sub_string(start, FileData[line]->size()).FindFirstOf(DataSyntax::ObjectEnd);
                    if (end != String::InvalidIndex)
                    {
                        //We close the object and continue
                        //We ARE still looking for a name.
                        --depth;
                        switch (depth)
                        {
                        case Depth::Object:
                        {
                            ++levels.back().Children;
                            objects.back().StringRange.SetRangeY(line);
                        }break;
                        case Depth::Level:
                        {
                            levels.back().StringRange.SetRangeY(line);
                        }break;
                        }
                        
                        continue;
                    }
                    end = FileData[line]->sub_string(start, FileData[line]->size()).FindFirstOf(DataSyntax::EndName);
                    if (depth == 0 && end == String::InvalidIndex)
                    {
                        break;
                    }
                    //If the name is not formatted properly, we error.
                    SyntaxError((start == String::InvalidIndex || end == String::InvalidIndex),
                        line, Name, String::Join("", "Name", DataSyntax::EndName));
                    //Otherwise, we check what depth we are at.
                    DataRange range = DataRange(FileData[line], &FileData, Unsigned2(0, FileData.size()));
                    switch (depth)
                    {
                    case Depth::DepthComponent:
                    {
                        range.StringRange.SetRangeX(line + 2); //safe because it won't compile otherwise
                        components.push_back(range);
                        ++depth;
                        ++objects.back().Children;
                        lookingForName = false;
                    }break;
                    case Depth::Object:
                    {
                        range.StringRange.SetRangeX(line + 2);
                        objects.push_back(range);
                        ++depth;
                        lookingForName = false;
                    }break;
                    case Depth::Level:
                    {
                        range.StringRange.SetRangeX(line + 2);
                        levels.push_back(range);
                        ++depth;
                        lookingForName = false;
                    }break;
                    }
                }
                else
                {
                    unsigned start = FileData[line]->FindFirstNonWhitespaceCharIndex();

                    if (FileData[line]->sub_string(start, FileData[line]->size()).CompareTo(DataSyntax::ObjectStart) == 0) //we are looking for a {
                    {
                        if (depth != Depth::Property)
                        {
                            lookingForName = true;
                        }
                        else
                        {
                            ++depth;
                        }
                        
                    }
                    else if (depth-1 == Depth::Property) //if we are looking for a property
                    {
                        StringRange end = FileData[line]->sub_string(start, FileData[line]->size());
                        if (end.CompareTo(DataSyntax::ObjectEnd) == 0)
                        {
                            components.back().StringRange.SetRangeY(line);
                            depth-=2;
                            lookingForName = true;
                        }
                        else if (end.sub_string(end.size() - DataSyntax::PropertyEnd.size(), end.size()).CompareTo(DataSyntax::PropertyEnd) == 0)
                        {
                            ++components.back().Children;
                        }
                        else
                        {
                            if (end[0] == DataSyntax::ObjectEnd[0] || end.back() == DataSyntax::ObjectEnd.back())
                            {
                                SyntaxError(true,
                                    line, Name, String::Join("", "", DataSyntax::ObjectEnd));
                            }
                            else
                            {
                                SyntaxError(true,
                                    line, Name, String::Join("", "", DataSyntax::PropertyEnd));
                            }
                        }
                    }
                    else 
                    {
                        SyntaxError(true,
                            line, Name, String::Join("", "", DataSyntax::ObjectStart));
                    }
                }
            }
            
            //Remove excess whitespace from the file
            FileData.resize(levels.back().StringRange.range().y + 1);

            unsigned currentObject = 0;
            unsigned currentComponent = 0;
            for (unsigned i = 0; i < levels.size(); ++i)
            {
                DataLevel* dataLevel = new DataLevel(this, levels[i].Name, levels[i].StringRange);
                DataLevelArray.push_back(dataLevel);
                //Create that level's objects
                for (unsigned k = currentObject; k < levels[i].Children + currentObject; ++k)
                {
                    DataObject* dataObject = new DataObject(dataLevel, objects[k].Name, objects[k].StringRange);
                    MemCheck(dataObject, "DataObject in DataFile.cpp");
                    dataLevel->AddObject(dataObject, false);
                    //Create that object's components
                    for (unsigned j = currentComponent; j < objects[k].Children + currentComponent; ++j)
                    {
                        DataComponent* dataComponent = new DataComponent(dataObject, components[j].Name, components[j].StringRange);
                        MemCheck(dataComponent, "DataComponent in DataFile.cpp");
                        dataObject->AddComponent(dataComponent, false);
                        //Create that component's properties
                        Unsigned2 range = components[j].StringRange.range();
                        for (unsigned w = 0; w < components[j].Children; ++w)
                        {
                            String* line = FileData[range.x + w];
                            //If it's a comment we continue
                            unsigned start = line->FindFirstNonWhitespaceCharIndex();
                            if (line->sub_string(start, line->size() - start).FindFirstOf(DataSyntax::LineCommentStart) == 0)
                            {
                                ++components[j].Children;
                                continue;
                            }
                            DataProperty* dataProperty = new DataProperty(dataComponent, FileData[range.x + w], range.x + w);
                            MemCheck(dataProperty, "DataProperty in DataFile.cpp");
                            dataComponent->AddProperty(dataProperty, false);
                        }
                    }
                    currentComponent += objects[k].Children;
                }
                currentObject += levels[i].Children;

                DataLevels.insert(dataLevel->GetName(), dataLevel);
            }
            
            buffer.close();
        }
    }
    else
    {
        SinWriteColor("Unable to open file: ", ConsoleColors::Red);
        SinWriteLineColor(path, ConsoleColors::Yellow);
    }
}

DataLevel* DataFile::AddLevel(const String& name)
{
    if (DataLevels.containsKey(name))
    {
        Error("File %s already has a level with name %s", Name, name);
        return nullptr;
    }

    FileData.push_back(new String(String::Join("", name, DataSyntax::EndName)));
    FileData.push_back(new String(DataSyntax::ObjectStart));
    FileData.push_back(new String(DataSyntax::ObjectEnd));
    MemCheck(FileData[FileData.size() - 3], "String in DataFile.cpp");
    MemCheck(FileData[FileData.size() - 2], "String in DataFile.cpp");
    MemCheck(FileData[FileData.size() - 1], "String in DataFile.cpp");
    DataLevel* obj = new DataLevel(this, FileData[FileData.size() - 3], ArrayRange<String*>(&FileData, Unsigned2(FileData.size() - 3, FileData.size()-1)));
    MemCheck(obj, name);
    DataLevels.insert(name, obj);
    DataLevelArray.push_back(obj);

    return obj;
}

void DataFile::RemoveLevel(const String& name)
{
    if (!DataLevels.containsKey(name))
    {
        Error("File %s has no level with name %s", Name, name);
        return;
    }

    DataLevel* lvl = FindLevel(name);
    DataLevelArray.erase_value(lvl);
    Unsigned2 range = lvl->GetRange();
    unsigned size = range.y - range.x;

    while (size)
    {
        FileData.eraseAt(range.x);
        --size;
    }
    FileData.eraseAt(range.x - 2);
    FileData.eraseAt(range.x - 2);
    FileData.eraseAt(range.x - 2);
    DataLevels.erase(name);
    
}

DataLevel* DataFile::FindLevel(const String& name)
{
    if (DataLevels.containsKey(name))
    {
        return DataLevels[name];
    }
    return nullptr;
}

void DataFile::SetName(const String& name)
{
    ResourceManager::RemapChild < DataFile >(Name, name);
    Name = name;
}
const String& DataFile::GetName()
{
    return Name;
}

void DataFile::RemapChild(const String& original, const String& newName)
{
    if (DataLevels.containsKey(original))
    {
        if (DataLevels.containsKey(newName))
        {
            Error("File %s already has a level named %s", GetName().c_str(), newName.c_str());
        }
        else
        {
            DataLevels.insert(newName, DataLevels[original]);
            DataLevels.erase(original);
        }
    }
    else
    {
        Error("File %s has no level called %s", GetName().c_str(), original.c_str());
    }
}

void DataFile::PrintData()
{
    for (unsigned i = 0; i < FileData.size(); ++i)
    {
        SinWriteLine(*FileData[i]);
    }
}

void DataFile::Serialize()
{
    String path = String::Join("/", DataFilePath, Name);
    std::ofstream buffer;
    buffer.open(path.c_str());
    if (buffer.is_open())
    {
        SinWrite("Writing to file: ");
        SinWriteLineColor(path, ConsoleColors::Yellow);
        for (unsigned i = 0; i < FileData.size(); ++i)
        {
            String output = String::Join("", *FileData[i], "\n");
            buffer.write(output.c_str(), output.size());
        }
        buffer.close();
    }
    else
    {
        Error("Failed to write to file: %s", path.c_str());
    }
    SinWriteLine("Done.");
}

bool DataFile::HasLevel(const String& name)
{
    return DataLevels.containsKey(name);
}

const Array<DataLevel*>& DataFile::AllLevels()
{
    return DataLevelArray;
}

DataFile::~DataFile()
{
    for (auto i : FileData)
    {
        delete i;
        i = nullptr;
    }

    FreeHashMap(DataLevels);
}