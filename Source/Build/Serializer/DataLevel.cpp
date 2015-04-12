#include <Precompiled.h>
#include "Serializer.h"
#include "DataLevel.h"
#include "DataFile.h"


DefineType(DataLevel, SinningZilch)    
{

}

DataLevel::DataLevel(const DataFile* parent) : Parent(parent)
{
}

DataLevel::DataLevel(const DataFile* parent, const StringRange& name, const StringRange& level) : Parent(parent)
{
    LoadLevel(name, level);
}

void DataLevel::LoadLevel(const StringRange& name, const StringRange& level)
{
    SinWrite("Creating Level: ");
    SinWriteLineColor(name, ConsoleColors::DarkMagenta);
    Name = name;
    //String path = String::Join("/", DataLevelPath, Name);
    //buffer.open(path.c_str());

    //std::string line;

    //if (buffer.is_open())
    //{
    //    SinWriteLineColor(Name, ConsoleColors::Yellow);
    //    while (getline(buffer, line))
    //    {
    //        //SinWriteLine(line.c_str());
    //        LevelData.push_back(line.c_str());
    //    }
    //    if (LevelData.size())
    //    {
    //        unsigned start = LevelData[0].FindFirstOf(DataSyntax::EndName);
    //        ErrorIf(start == String::InvalidIndex, "Syntax Error on line |%i| of |%s|.\n\tExpected |Name%s|.\n", 
    //                                                                    0, Name.c_str(), DataSyntax::EndName);
    //        SinWrite("Creating Level: ");
    //        SinWriteLineColor(LevelData[0].sub_string(0, start), ConsoleColors::DarkMagenta);

    //    }
    //}
    //else
    //{
    //    SinWriteColor("Unable to open file: ", ConsoleColors::Red);
    //    SinWriteLineColor(path, ConsoleColors::Yellow);
    //}
}

const DataLevel* DataLevel::AddObject(const String& name)
{
    return nullptr;
}

void DataLevel::RemoveObject(const String& name)
{
}

const DataLevel* DataLevel::FindObject(const String& name)
{
    return nullptr;
}

void DataLevel::SetName(const String& name)
{
}
const StringRange& DataLevel::GetName()
{
    return Name;
}


DataLevel::~DataLevel()
{
}