#include <Precompiled.h>
#include "Serializer.h"
#include "DataFile.h"



DefineType(DataFile, SinningZilch)
{

}

DataFile::DataFile(const String& fileName)
{
	Name = fileName;
	String path = String::Join("/", DataFilePath, Name);
	buffer.open(path.c_str());

	unsigned lineNumber = 0;
	std::string line;

	if (buffer.is_open())
	{
		SinWrite("Opened File: ");
		SinWriteLineColor(Name, ConsoleColors::Yellow);
		while (getline(buffer, line))
		{
			SinWriteLine(line.c_str());
			File.push_back(line.c_str());
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