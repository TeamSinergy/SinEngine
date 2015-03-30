#include <Precompiled.h>
#include "Serializer.h"
#include "dirent.h"

//This allows the serializer to skip the pointers to the current directory "." and the pointer to
//the parent directory ".."
#define SkippedFiles 2

String Serializer::FindFileInFolder(const String& folderPath, const String& filename, bool checkSubFolders)
{
	int skippedCount = SkippedFiles;

	DIR* directory;
	struct dirent* ent;

	String actualPath = String::Join("/", folderPath, "");


	if ((directory = opendir(actualPath.c_str())) != NULL)
	{
		while ((ent = readdir(directory)) != NULL) 
		{
			if (skippedCount)
			{
				--skippedCount;
				continue;
			}
			if (ent->data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (!checkSubFolders)
				{
					continue;
				}
				String subPath = actualPath.sub_string(0, actualPath.size());
				String subfile = FindFileInFolder(String::Join("", subPath, ent->data.cFileName), filename);
				if ((char*)subfile[0] != '\0')
				{
					return subfile;
				}
			}
			else if (filename.CompareTo(ent->data.cFileName) == 0)
			{
				return String::Join("/", folderPath, ent->data.cFileName);
			}
		}
	}
	closedir(directory);

	return "";
}


void Serializer::FindAllFilesInFolder(const String& folderPath, const String& filetype, Array<String>& container, bool checkSubFolders)
{
	int skippedCount = SkippedFiles;

	DIR* directory;
	struct dirent* ent;
	String actualPath = String::Join("/", folderPath, "");



	if ((directory = opendir(actualPath.c_str())) != NULL)
	{
		while ((ent = readdir(directory)) != NULL)
		{
			if (skippedCount)
			{
				--skippedCount;
				continue;
			}
			if (ent->data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (!checkSubFolders)
				{
					continue;
				}
				String subPath = actualPath.sub_string(0, actualPath.size());
				FindAllFilesInFolder(String::Join("", subPath, ent->data.cFileName), filetype, container);
			}
			//Moving back the size of the file size we are checking to see if the current file has the same extension.
			//Example: Settings.data == .data
			//				   ^
			else if (strcmp(ent->data.cFileName + strlen(ent->data.cFileName) - filetype.size(), filetype.c_str()) == 0)
			{
				container.push_back(ent->data.cFileName);
				
			}
		}
	}
	closedir(directory);
}

void Serializer::OpenFile(String filename)
{
	
}