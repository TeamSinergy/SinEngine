#pragma once
#include <Precompiled.h>

//Syntax for .data Files
namespace DataSyntax
{
    const String Padding = "    ";            //Padding that is added based on how deep we go.
    const String EndName = " = ";                //Everything between last line and this char defines the name.
    const String ObjectStart = "{";            //Defines the start of an object.
    const String ObjectEnd = "},";            //Defines the end of an object.
    const String PropertyEnd = ";";            //Defines the end of a property variable.
    const String GroupStart = "[";            //Defines the start of a group of values.
    const String GroupEnd = "]";            //Defines the end of a group of values.
    const String LineCommentStart = "//";    //Defines inline comment style.
    const String BlockCommentStart = "/*"; //Defines block comment style start.
    const String BlockCommentEnd = "*/";    //Defines block comment style end.
    
};

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