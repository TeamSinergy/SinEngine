#pragma once
#include <Precompiled.h>
#include "DataComponent.h"

#define SerializeVarFromType(dataComponent, type, variable, name) if(dataComponent->HasProperty(name)) \
                                                      { variable = dataComponent->FindProperty(name)->GetValue<type>();}
#define SerializeValue(dataComponent, variable) Serializer::SerializeVariable(dataComponent, variable, #variable)
#define SerializeValueName(dataComponent, variable, name) Serializer::SerializeVariable(dataComponent, variable, name)
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

    static void ToValueFromString(const String& input, int&, unsigned line = 0);
    static void ToValueFromString(const String& input, float&, unsigned line = 0);
    static void ToValueFromString(const String& input, bool&, unsigned line = 0);

    template<typename T>
    static void SerializeVariable(DataComponent* comp, T& store, const String& name)
    {
        SerializeVarFromType(comp, T, store, name);
    }
    
    //static void Serialize(DataComponent* comp, Integer2& store);
    //static void Serialize(DataComponent* comp, Integer2& store);
    //template<typename TYPE>
    //static void ToValueFromString(String& input, TYPE&, unsigned line = 0) { Error("Cannot convert string to this type."); }


    static void Initialize();
    static HashMap<String, BoundType* > Types;
};

//extern template void Serializer::Serialize < Integer >(DataComponent* comp, Integer& store);
//extern template void Serializer::Serialize < Real >(DataComponent* comp, Real& store);
