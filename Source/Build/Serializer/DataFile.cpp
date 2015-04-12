#include <Precompiled.h>
#include "Serializer.h"
#include "DataFile.h"
#include "DataLevel.h"



DefineType(DataFile, SinningZilch)    
{

}

DataFile::DataFile(const String& fileName)
{
    LoadFile(fileName);
}

void DataFile::LoadFile(const String& fileName)
{
    Name = fileName;
    String path = String::Join("/", DataFilePath, Name);
    buffer.open(path.c_str());

    std::string lineString;

    if (buffer.is_open())
    {
        SinWrite("Opened File: ");
        SinWriteLineColor(Name, ConsoleColors::Yellow);
        while (getline(buffer, lineString))
        {
            //SinWriteLine(line.c_str());
            FileData.push_back(lineString.c_str());
        }
        if (FileData.size())
        {
            
            Array<Array<String*>> objectRanges;
            unsigned line = 0;
            Array<unsigned> objectStart;
            unsigned start = FileData[line].FindFirstNonWhitespaceCharIndex();
            unsigned end = FileData[line].FindFirstOf(DataSyntax::EndName); //Find first " = "
            
            SyntaxError((start == String::InvalidIndex || end == String::InvalidIndex) || (end + DataSyntax::EndName.size() < FileData[line].size()),
                line, Name, String::Join("", "Name", DataSyntax::EndName));
            StringRange name = FileData[line].sub_string(start, end); //Get Name
            ++line;
            
            start = FileData[line].FindFirstNonWhitespaceCharIndex();
            end = FileData[line].FindFirstOf(DataSyntax::ObjectStart);
            SyntaxError(FileData[line].sub_string(start, end + DataSyntax::ObjectStart.size()).CompareTo(DataSyntax::ObjectStart) != 0,
                        line, Name, DataSyntax::ObjectStart);
            ++line;
            objectStart.push_back(line);
            for (line; line < FileData.size(); ++line)
            {
                if (FileData[line].Contains(DataSyntax::ObjectStart))
                {
                    objectStart.push_back(line + 1);
                }
                else if (FileData[line].Contains(DataSyntax::ObjectEnd))
                {
                    objectRanges.push_back(Array<String*>());
                    for (unsigned i = objectStart.back(); i < line; ++i)
                    {
                        objectRanges.back().push_back(&FileData[i]);
                    }
                    objectStart.pop_back();
                    if (objectStart.empty())
                    {
                        break;
                    }
                }
            }
            //StringRange level = 
            //SinLevels.insert(name, new DataLevel(this, name, )

            SinWrite("Creating Level: ");
            SinWriteLineColor(name, ConsoleColors::DarkMagenta);
            for (unsigned k = 0; k < objectRanges.size(); ++k)
            {
                for (int j = 0; j < objectRanges[k].size(); ++j)
                {
                    SinWriteLine(*objectRanges[k][j]);
                }
                SinWriteLine("");
            }
            *objectRanges[2][1] = "URE A FAGET!!!";
            for (unsigned k = 0; k < FileData.size(); ++k)
            {
                SinWriteLine(FileData[k]);
            }
            
            
        }
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