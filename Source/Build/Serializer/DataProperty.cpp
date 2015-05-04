#include <Precompiled.h>
#include "Serializer.h"
#include "DataProperty.h"


DefineType(DataProperty, SinningZilch)
{

}

DataProperty::DataProperty(DataComponent* const parent) : Parent(parent)
{

}
DataProperty::DataProperty(DataComponent* const parent, String* data, unsigned index) : Parent(parent), Index(index), FileData(data)
{
    LoadProperty(data);
}
DataProperty::DataProperty(DataComponent* const parent, String* source, unsigned index, const String& name, const Handle& store) : 
Parent(parent), 
Name(name), 
Index(index)
{
    SetHandle(store);
    /*SinWrite("Creating ");
    SinWriteColor(store.Type->ToString(), ConsoleColors::Cyan);
    SinWrite(" Property: ");
    SinWriteColor(Name, ConsoleColors::Red);*/
}

void DataProperty::LoadProperty(String* data)
{
    unsigned start = data->FindFirstNonWhitespaceCharIndex();
    unsigned end = data->FindFirstOf(DataSyntax::EndName);
    if (start == String::InvalidIndex || end == String::InvalidIndex)
    {
        Error("Syntax Error on line %i.\n\tExpected: |Type Name%sValue%s|", Index + 1, DataSyntax::EndName.c_str(), DataSyntax::PropertyEnd.c_str());
    }
    StringRange typeName = data->sub_string(start, end - start);
    unsigned nameStart = typeName.FindFirstOf(" ");
    
    Name = typeName.sub_string(nameStart + 1, end - start - nameStart);
    TypeString = typeName.sub_string(0, nameStart);
    
    unsigned valueStart = end + DataSyntax::EndName.size();
    ValueString = data->sub_string(valueStart, data->size() - valueStart - DataSyntax::PropertyEnd.size());
    
    /*SinWrite("Creating ");
    SinWriteColor(TypeString, ConsoleColors::Cyan);
    SinWrite(" Property: ");
    SinWriteColor(Name, ConsoleColors::Red);*/

    if (!Serializer::Types.containsKey(TypeString))
    {
        Error("Undefined type |%s| of property |%s| on line %i", TypeString.c_str(), Name.c_str(), Index);
    }
    
    BoundType* type = Serializer::Types[TypeString];
    ExecutableState* state = ZILCH->GetDependencies();
    
    //Set the value to a handle to the type loaded in.
    //Screw da police
    Value = state->AllocateStackObject((byte*)&Value, type, ZILCH->Report);

    ParseValue(ValueString, type);
    //PrintData();
}

void DataProperty::ParseValue(String& valueString, BoundType* type)
{
    //Most common types first. Wish I could of used a Switch :(
    if (type == ZilchTypeId(String))
    {
        if (valueString[0] != '\"' || valueString.back() != '\"')
        {
            Error("String on line %i must start and end with |\"\"|.\n\tExample: |\"\"Hello World\"\"|", Index + 1);
        }
        *(String**)Value.Data = new String(valueString.sub_string(1, valueString.size() - DataSyntax::GroupEnd.size() - 1));
        MemCheck(*(String**)Value.Data, "String in Propery.cpp");
        //SinWriteLine(**(String**)Value.Data);
    }
    else if (type == ZilchTypeId(Integer))
    {
        Serializer::ToValueFromString(valueString, *(int*)Value.Data, Index);
        //SinWriteLine(*(int*)Value.Data);
    }
    else if (type == ZilchTypeId(Boolean))
    {
        Serializer::ToValueFromString(valueString, *(bool*)Value.Data, Index);
        //SinWriteLine(*(bool*)Value.Data);
    }
    else if (type == ZilchTypeId(Real))
    {
        Serializer::ToValueFromString(valueString, *(float*)Value.Data, Index);
        //SinWriteLine(*(float*)Value.Data);
    }
    else if (type == ZilchTypeId(Integer2))
    {
        Array<int> store = Array<int>(2);
        FillArray<int>(store, valueString, Index, "Integer2");
        *(Integer2*)Value.Data = Integer2(store[0], store[1]);
        //SinWriteLine(*(Integer2*)Value.Data);
    }
    else if (type == ZilchTypeId(Integer3))
    {
        Array<int> store = Array<int>(3);
        FillArray<int>(store, valueString, Index, "Integer3");
        *(Integer3*)Value.Data = Integer3(store[0], store[1], store[2]);
        //SinWriteLine(*(Integer3*)Value.Data);
    }
    else if (type == ZilchTypeId(Integer4))
    {
        Array<int> store = Array<int>(4);
        FillArray<int>(store, valueString, Index, "Integer4");
        *(Integer4*)Value.Data = Integer4(store[0], store[1], store[2], store[3]);
        //SinWriteLine(*(Integer4*)Value.Data);
    }
    else if (type == ZilchTypeId(Boolean2))
    {
        Array<bool> store = Array<bool>(2);
        FillArray<bool>(store, valueString, Index, "Boolean2");
        *(Boolean2*)Value.Data = Boolean2(store[0], store[1]);
        //SinWriteLine(*(Boolean2*)Value.Data);
    }
    else if (type == ZilchTypeId(Boolean3))
    {
        Array<bool> store = Array<bool>(3);
        FillArray<bool>(store, valueString, Index, "Boolean3");
        *(Boolean3*)Value.Data = Boolean3(store[0], store[1], store[2]);
        //SinWriteLine(*(Boolean3*)Value.Data);
    }
    else if (type == ZilchTypeId(Boolean4))
    {
        Array<bool> store = Array<bool>(4);
        FillArray<bool>(store, valueString, Index, "Boolean4");
        *(Boolean4*)Value.Data = Boolean4(store[0], store[1], store[2], store[3]);
        //SinWriteLine(*(Boolean4*)Value.Data);
    }
    else if (type == ZilchTypeId(Real2))
    {
        Array<float> store = Array<float>(2);
        FillArray<float>(store, valueString, Index, "Real2");
        *(Real2*)Value.Data = Real2(store[0], store[1]);
        //SinWriteLine(*(Real2*)Value.Data);
    }
    else if (type == ZilchTypeId(Real3))
    {
        Array<float> store = Array<float>(3);
        FillArray<float>(store, valueString, Index, "Real3");
        *(Real3*)Value.Data = Real3(store[0], store[1], store[2]);
        //SinWriteLine(*(Real3*)Value.Data);
    }
    else if (type == ZilchTypeId(Real4))
    {
        Array<float> store = Array<float>(4);
        FillArray<float>(store, valueString, Index, "Real4");
        *(Real4*)Value.Data = Real4(store[0], store[1], store[2], store[3]);
        //SinWriteLine(*(Real4*)Value.Data);
    }
    else if (type == ZilchTypeId(Quaternion))
    {
        Array<float> store = Array<float>(4);
        FillArray<float>(store, valueString, Index, "Quaternion");
        *(Quaternion*)Value.Data = Quaternion(store[0], store[1], store[2], store[3]);
        //SinWriteLine(*(Quaternion*)Value.Data);
    }
}

String DataProperty::GetValueString()
{
    return ValueString;
}

void DataProperty::SetValueString(const String& value)
{
    *FileData = FileData->Replace(ValueString, value);
    ValueString = value;
    ParseValue(ValueString, Value.Type);
}

const Handle& DataProperty::GetHandle()
{
    return Value;
}
void DataProperty::SetHandle(const Handle& value)
{
    Value = value;
    TypeString = value.Type->ToString();
    Type* type = value.Type;

    if (type == ZilchTypeId(String))
    {
        FileData->Replace(ValueString, **(String**)Value.Data);
        ValueString = **(String**)Value.Data;
    }
    else if (type == ZilchTypeId(Integer))
    {
        String newString = std::to_string(**(Integer**)Value.Data).c_str();
        FileData->Replace(ValueString, newString);
        ValueString = newString;
    }
    else if (type == ZilchTypeId(Boolean))
    {
        String newString = std::to_string(**(Boolean**)Value.Data).c_str();
        FileData->Replace(ValueString, newString);
        ValueString = newString;
    }
    else if (type == ZilchTypeId(Real))
    {
        String newString = std::to_string(**(Real**)Value.Data).c_str();
        FileData->Replace(ValueString, newString);
        ValueString = newString;
    }
    else if (type == ZilchTypeId(Integer2))
    {
        String newString = String::Join("", DataSyntax::ObjectStart, std::to_string((**(Integer2**)Value.Data).x).c_str(), ", ");
        newString = String::Join("", newString, std::to_string((**(Integer2**)Value.Data).y).c_str(), DataSyntax::ObjectEnd);
        FileData->Replace(ValueString, newString);
        ValueString = newString;
    }
    else if (type == ZilchTypeId(Integer3))
    {
        String newString = String::Join("", DataSyntax::ObjectStart, std::to_string((**(Integer3**)Value.Data).x).c_str(), ", ");
        newString = String::Join("", newString, std::to_string((**(Integer3**)Value.Data).y).c_str(), ", ");
        newString = String::Join("", newString, std::to_string((**(Integer3**)Value.Data).z).c_str(), DataSyntax::ObjectEnd);
        FileData->Replace(ValueString, newString);
        ValueString = newString;
    }
    else if (type == ZilchTypeId(Integer4))
    {
        String newString = String::Join("", DataSyntax::ObjectStart, std::to_string((**(Integer4**)Value.Data).x).c_str(), ", ");
        newString = String::Join("", newString, std::to_string((**(Integer4**)Value.Data).y).c_str(), ", ");
        newString = String::Join("", newString, std::to_string((**(Integer4**)Value.Data).z).c_str(), ", ");
        newString = String::Join("", newString, std::to_string((**(Integer4**)Value.Data).w).c_str(), DataSyntax::ObjectEnd);
        FileData->Replace(ValueString, newString);
        ValueString = newString;
    }
    else if (type == ZilchTypeId(Boolean2))
    {
        String newString = String::Join("", DataSyntax::ObjectStart, std::to_string((**(Boolean2**)Value.Data).x).c_str(), ", ");
        newString = String::Join("", newString, std::to_string((**(Boolean2**)Value.Data).y).c_str(), DataSyntax::ObjectEnd);
        FileData->Replace(ValueString, newString);
        ValueString = newString;
    }
    else if (type == ZilchTypeId(Boolean3))
    {
        String newString = String::Join("", DataSyntax::ObjectStart, std::to_string((**(Boolean3**)Value.Data).x).c_str(), ", ");
        newString = String::Join("", newString, std::to_string((**(Boolean3**)Value.Data).y).c_str(), ", ");
        newString = String::Join("", newString, std::to_string((**(Boolean3**)Value.Data).z).c_str(), DataSyntax::ObjectEnd);
        FileData->Replace(ValueString, newString);
        ValueString = newString;
    }
    else if (type == ZilchTypeId(Boolean4))
    {
        String newString = String::Join("", DataSyntax::ObjectStart, std::to_string((**(Boolean4**)Value.Data).x).c_str(), ", ");
        newString = String::Join("", newString, std::to_string((**(Boolean4**)Value.Data).y).c_str(), ", ");
        newString = String::Join("", newString, std::to_string((**(Boolean4**)Value.Data).z).c_str(), ", ");
        newString = String::Join("", newString, std::to_string((**(Boolean4**)Value.Data).w).c_str(), DataSyntax::ObjectEnd);
        FileData->Replace(ValueString, newString);
        ValueString = newString;
    }
    else if (type == ZilchTypeId(Real2))
    {
        String newString = String::Join("", DataSyntax::ObjectStart, std::to_string((**(Real2**)Value.Data).x).c_str(), ", ");
        newString = String::Join("", newString, std::to_string((**(Real2**)Value.Data).y).c_str(), DataSyntax::ObjectEnd);
        FileData->Replace(ValueString, newString);
        ValueString = newString;
    }
    else if (type == ZilchTypeId(Real3))
    {
        String newString = String::Join("", DataSyntax::ObjectStart, std::to_string((**(Real3**)Value.Data).x).c_str(), ", ");
        newString = String::Join("", newString, std::to_string((**(Real3**)Value.Data).y).c_str(), ", ");
        newString = String::Join("", newString, std::to_string((**(Real3**)Value.Data).z).c_str(), DataSyntax::ObjectEnd);
        FileData->Replace(ValueString, newString);
        ValueString = newString;
    }
    else if (type == ZilchTypeId(Real4))
    {
        String newString = String::Join("", DataSyntax::ObjectStart, std::to_string((**(Real4**)Value.Data).x).c_str(), ", ");
        newString = String::Join("", newString, std::to_string((**(Real4**)Value.Data).y).c_str(), ", ");
        newString = String::Join("", newString, std::to_string((**(Real4**)Value.Data).z).c_str(), ", ");
        newString = String::Join("", newString, std::to_string((**(Real4**)Value.Data).w).c_str(), DataSyntax::ObjectEnd);
        FileData->Replace(ValueString, newString);
        ValueString = newString;
    }
    else if (type == ZilchTypeId(Quaternion))
    {
        String newString = String::Join("", DataSyntax::ObjectStart, std::to_string((**(Quaternion**)Value.Data).x).c_str(), ", ");
        newString = String::Join("", newString, std::to_string((**(Quaternion**)Value.Data).y).c_str(), ", ");
        newString = String::Join("", newString, std::to_string((**(Quaternion**)Value.Data).z).c_str(), ", ");
        newString = String::Join("", newString, std::to_string((**(Quaternion**)Value.Data).w).c_str(), DataSyntax::ObjectEnd);
        FileData->Replace(ValueString, newString);
        ValueString = newString;
    }
}

const String& DataProperty::GetName()
{
    return Name;
}
void DataProperty::SetName(const String& name)
{
    Parent->RemapChild(Name, name);
    *FileData = FileData->Replace(Name, name);
    Name = name;
    
}

const BoundType* DataProperty::GetType()
{
    return Value.Type;
}
void DataProperty::SetType(BoundType* type)
{
    if (Value.Type == ZilchTypeId(String))
    {
        delete *(String**)Value.Data;
        *(String**)Value.Data = nullptr;
    }
    *FileData = FileData->Replace(TypeString, type->ToString());
    Value.Type = type;
    ParseValue(ValueString, Value.Type);
}

void DataProperty::PrintData()
{
    SinWrite(" = ");
    SinSole::SetTextColor(ConsoleColors::DarkGray);
    BoundType* type = Value.Type;
    //Most common types first. Wish I could of used a Switch :(
    if (type == ZilchTypeId(String))
    {
        SinWriteLine(**(String**)Value.Data);
    }
    else if (type == ZilchTypeId(Integer))
    {
        SinWriteLine(*(int*)Value.Data);
    }
    else if (type == ZilchTypeId(Boolean))
    {
        SinWriteLine(*(bool*)Value.Data);
    }
    else if (type == ZilchTypeId(Real))
    {
        SinWriteLine(*(float*)Value.Data);
    }
    else if (type == ZilchTypeId(Integer2))
    {
        SinWriteLine(*(Integer2*)Value.Data);
    }
    else if (type == ZilchTypeId(Integer3))
    {
        SinWriteLine(*(Integer3*)Value.Data);
    }
    else if (type == ZilchTypeId(Integer4))
    {
        SinWriteLine(*(Integer4*)Value.Data);
    }
    else if (type == ZilchTypeId(Boolean2))
    {
        SinWriteLine(*(Boolean2*)Value.Data);
    }
    else if (type == ZilchTypeId(Boolean3))
    {
        SinWriteLine(*(Boolean3*)Value.Data);
    }
    else if (type == ZilchTypeId(Boolean4))
    {
        SinWriteLine(*(Boolean4*)Value.Data);
    }
    else if (type == ZilchTypeId(Real2))
    {
        SinWriteLine(*(Real2*)Value.Data);
    }
    else if (type == ZilchTypeId(Real3))
    {
        SinWriteLine(*(Real3*)Value.Data);
    }
    else if (type == ZilchTypeId(Real4))
    {
        SinWriteLine(*(Real4*)Value.Data);
    }
    else if (type == ZilchTypeId(Quaternion))
    {
        SinWriteLine(*(Quaternion*)Value.Data);
    }
    SinSole::SetTextColor(SinSole::LastTextColor);
}
unsigned DataProperty::GetIndex()
{
    Index = Parent->FindFirstGlobalIndexOfProperty(FileData);
    return Index;
}
void DataProperty::SetIndex(unsigned& range)
{
    Index = range;
}

DataProperty::~DataProperty()
{
    if (Value.Type == ZilchTypeId(String))
    {
        delete *(String**)Value.Data;
        *(String**)Value.Data = nullptr;
    }
    Value.Delete();
}