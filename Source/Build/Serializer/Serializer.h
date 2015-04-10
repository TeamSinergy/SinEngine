#pragma once
#include <Precompiled.h>

class Serializer
{
public:
    static String FindFileInFolder(const String& folderPath, const String& filename, bool checkSubFolders = false);
    static void FindAllFilesInFolder(const String& folderPath, const String& filetype, Array<String>& container, bool checkSubFolders = false);
    static void Serializer::DelimitDataFile(const std::string& buffer, Array<String>& store);
};

enum DATATYPE
{
    TYPE_OBJECT,
    TYPE_INT,
    TYPE_LONG,
    TYPE_FLOAT,
    TYPE_DOUBLE,
    TYPE_BOOL,
    TYPE_STRING,
    TYPE_REAL2,
    TYPE_REAL3,
    TYPE_REAL4,
    TYPE_INT2,
    TYPE_INT3,
    TYPE_INT4,
    TYPE_BOOL2,
    TYPE_BOOL3,
    TYPE_BOOL4,
    TYPE_QUAT,
    TYPE_ARRAY,
    TYPE_ENUM,
    TYPE_CUSTOM
};