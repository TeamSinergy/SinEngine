#include <Precompiled.h>
#include "Serializer.h"
#include "dirent.h"

//These are all the types that can be serialized in.
HashMap<String, BoundType* > Serializer::Types = HashMap<String, BoundType* >();

void Serializer::Initialize()
{
    Types.insert("String", ZilchTypeId(String));
    Types.insert("Integer", ZilchTypeId(Integer));
    Types.insert("Integer2", ZilchTypeId(Integer2));
    Types.insert("Integer3", ZilchTypeId(Integer3));
    Types.insert("Integer4", ZilchTypeId(Integer4));
    Types.insert("Boolean", ZilchTypeId(Boolean));
    Types.insert("Boolean2", ZilchTypeId(Boolean2));
    Types.insert("Boolean3", ZilchTypeId(Boolean3));
    Types.insert("Boolean4", ZilchTypeId(Boolean4));
    Types.insert("Real", ZilchTypeId(Real));
    Types.insert("Real2", ZilchTypeId(Real2));
    Types.insert("Real3", ZilchTypeId(Real3));
    Types.insert("Real4", ZilchTypeId(Real4));
    Types.insert("Quaternion", ZilchTypeId(Quaternion));
}

String Serializer::FindFileInFolder(const String& folderPath, const String& filename, bool checkSubFolders)
{
    //This allows the serializer to skip the pointers to the current directory "." and the pointer to
    //the parent directory ".."
    #define SkippedFiles 2

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
            //                   ^
            else if (strcmp(ent->data.cFileName + strlen(ent->data.cFileName) - filetype.size(), filetype.c_str()) == 0)
            {
                container.push_back(ent->data.cFileName);

            }
        }
    }
    closedir(directory);
}

int Serializer::ToInt(const String& input, unsigned line)
{
    int value = 0;
    if ((input[0] == '\"' || input[0] == '\'') && (input.back() == '\"') || input.back() == ('\''))
    {
        String sub = input.sub_string(1, input.size() - 2);
        
        for (unsigned i = 0; i < sub.size(); ++i)
        {
            value += (unsigned)sub[i];
        }
        return value;
    }
    else
    {
        for (unsigned i = 0; i < input.size(); ++i)
        {
            if ((input[i] < '0' || input[i] > '9') && input[i] != '.')
            {
                Error("Integer value on line %i must not contain any characters.", line + 1);
            }
        }
        ToValue(input, value);
        return value;
    }
}
float Serializer::ToFloat(const String& input, unsigned line)
{
    float value;
    for (unsigned i = 0; i < input.size(); ++i)
    {
        if ((input[i] < '0' || input[i] > '9') && input[i] != '.')
        {
            if (tolower(input[i]) == 'f')
            {
                if (i != input.size() - 1)
                {
                    Error("Float value on line %i must not contain any characters.", line + 1);
                }
            }
        }
    }
    ToValue(input, value);
    return value;
}
bool Serializer::ToBool(String input, unsigned line)
{
    bool value;
    String lower = input.ToLower();
    if (lower.CompareTo("true") != 0 && lower.CompareTo("false") != 0)
    {
        Error("Boolean on line %i must either be |true| or |false|", line + 1);
    }
    ToValue(lower, value);
    return value;
}