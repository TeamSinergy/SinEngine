#pragma once
#include "Precompiled.h"
#include "Shlwapi.h"
#include <io.h>
#include <fcntl.h>
#include "SinSole.h"
#include "ZilchCompiledLib.h"

#define SINBUILD
#ifdef SINBUILD

bool FirstFile(const String& folder, String& path_found)            // placing path here if found
{

	//printf(TEXT("Target file is %s\n"), argv[1]);
	//hFind = FindFirstFile(folder.c_str(), &FindFileData);
	//ErrorIf(hFind == INVALID_HANDLE_VALUE, "Failed To Find Directory: %s\n", directory.c_str());
	std::vector<std::string> names;
	char search_path[200];

	sprintf(search_path, "%s", folder.c_str());

	//Stores information about the current File or Folder
	WIN32_FIND_DATA FileData;
	//A Handle to the current file or folder
	HANDLE hFind = ::FindFirstFile(search_path, &FileData);
	//If there are still files in the folder...
	if (hFind != INVALID_HANDLE_VALUE) 
	{
		do 
		{
			//if the current file is NOT a Folder
			if (!(FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				names.push_back(FileData.cFileName);
			}
		} while (::FindNextFile(hFind, &FileData));
		::FindClose(hFind);
	}
	
	FindClose(hFind);
	return true;
	//std::fstream fs;
	////fs.open(ScriptFileList);
	//if (!exists(dir_path.c_str())) return false;
	//directory_iterator end_itr; // default construction yields past-the-end
	//for (directory_iterator itr(dir_path);
	//	itr != end_itr;
	//	++itr)
	//{
	//	if (is_directory(itr->status()))
	//	{
	//		if (find_file(itr->path(), file_name, path_found)) return true;
	//	}
	//	else if (itr->leaf() == file_name) // see below
	//	{
	//		path_found = itr->path();
	//		return true;
	//	}
	//}
	//return false;
}

int main(void)
{
	SinSole::CreateConsole("SinSole");
	std::cout << "---- SinSole Initialized ----" << std::endl;

	//A 'static' initialization of the Zilch Project for the debugger.
	ZilchStartup(Debugging::UseZilchErrorHandler);
	ZilchCompiledLib ZilchLibrary = ZilchCompiledLib("ZilchCompiledLib");
	ZilchLibrary.Create();
	ZilchLibrary.CompiledProject = &Project(ZilchLibrary.Errors);
	ZilchLibrary.Initialize();
	
	String path;
	FirstFile("C:/Users/Josh/Desktop/SinEngine/Assets/*", path);
	

	system("pause");

	ZilchLibrary.Uninitialize();
	ZilchLibrary.Destroy();
	SinSole::DestroyConsole();
	return 0;
}

#endif