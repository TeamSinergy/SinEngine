#include <Precompiled.h>
#include "Serializer.h"
#include "DataFile.h"



DefineType(DataFile, SinningZilch)    
{

}

DataFile::DataFile(const String& fileName) : Name(fileName)
{
    String path = String::Join("/", DataFilePath, Name);
    buffer.open(path.c_str());

    unsigned lineNumber = 0;
    std::string fileString;

    if (buffer.is_open())
    {
        SinWrite("Opened File: ");
        SinWriteLineColor(Name, ConsoleColors::Yellow);
        buffer.seekg(0, std::ios::end); //Go to the end of the file.
        fileString.reserve(static_cast<unsigned>(buffer.tellg())); //Allocate the proper amount of memory.
        buffer.seekg(0, std::ios::beg); //Go back to the beginning of the file.
        fileString.assign((std::istreambuf_iterator<char>(buffer)), std::istreambuf_iterator<char>());
        Serializer::DelimitDataFile(fileString, FileData);
    }
    else
    {
        SinWriteColor("Unable to open file: ", ConsoleColors::Red);
        SinWriteLineColor(path, ConsoleColors::Yellow);
    }
}


const DataLevel* DataFile::AddLevel(const String& name)
{
    return nullptr;
}

void DataFile::RemoveLevel(const String& name)
{
}
void DataFile::RemoveLevel(String name)
{
}

const DataLevel* DataFile::FindLevel(const String& name)
{
    return nullptr;
}

void DataFile::SetName(const String& name)
{
}
const String& DataFile::GetName()
{
    return Name;
}

void DataFile::Serialize()
{
}

DataFile::~DataFile()
{
}