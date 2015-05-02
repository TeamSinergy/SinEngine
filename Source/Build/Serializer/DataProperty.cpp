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
DataProperty::DataProperty(DataComponent* const parent, String* source, unsigned index, String& name, Type* type, byte* value) : Parent(parent), Name(name), rawValue(value), Index(index)
{

    SinWrite("Creating Property: ");
    SinWriteLineColor(name, ConsoleColors::Red);
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
    String typeString = typeName.sub_string(0, nameStart);
    
    unsigned valueStart = end + DataSyntax::EndName.size();
    String valueString = data->sub_string(valueStart, data->size() - valueStart - DataSyntax::PropertyEnd.size());
    
    SinWrite("Creating ");
    SinWriteColor(typeString, ConsoleColors::Cyan);
    SinWrite(" Property: ");
    SinWriteLineColor(Name, ConsoleColors::Red);

    if (!Serializer::Types.containsKey(typeString))
    {
        Error("Undefined type |%s| of property |%s| on line %i", typeString.c_str(), Name.c_str(), Index);
    }
    
    BoundType* type = Serializer::Types[typeString];
    ExecutableState* state = ZILCH->GetDependencies();
    
    //Zilch::StackManager::
    //Set the value to a handle to the type loaded in.
    Value = state->AllocateStackObject((byte*)&Value, type, ZILCH->Report);
    
    //Array<Type*> args;
    //args.push_back(type);
    
    //Value.Manager->Allocate(type, Value, 0);
    //a = (int)Value.Data;
    //Call call(Value.Type.Set, state);
    //a = (int*)Value.Dereference();
    //Most common types first. Wish I could of used a Switch :(
    if (type == ZilchTypeId(String))
    {
        if (valueString[0] != '\"' || valueString.back() != '\"')
        {
            Error("String on line %i must start and end with |\"\"|.\n\tExample: |\"\"Hello World\"\"|", Index + 1);
        }
        *(String**)Value.Data = new String(valueString.sub_string(1, valueString.size() - 2));
        SinWriteLine(**(String**)Value.Data);
    }
    else if (type == ZilchTypeId(Integer))
    {
        *(int*)Value.Data = Serializer::ToInt(valueString, Index);
        SinWriteLine(*(int*)Value.Data);
    }
    else if (type == ZilchTypeId(Boolean))
    {
        *(bool*)Value.Data = Serializer::ToBool(valueString, Index);
        SinWriteLine(*(bool*)Value.Data);
    }
    else if (type == ZilchTypeId(Real))
    {
        *(float*)Value.Data = Serializer::ToFloat(valueString, Index);
        SinWriteLine(*(float*)Value.Data);
    }
    else if (type == ZilchTypeId(Integer2))
    {
        unsigned end = valueString.size() - DataSyntax::GroupEnd.size();
        StringRange endString = valueString.sub_string(end, DataSyntax::GroupEnd.size());
        //if it does not start with [ and end with ]
        if (valueString.sub_string(0, DataSyntax::GroupEnd.size()).CompareTo(DataSyntax::GroupStart) != 0 || endString.CompareTo(DataSyntax::GroupEnd) != 0)
        {
            Error("Integer2 on line %i must start with |%s| and end with |%s|.", Index + 1, DataSyntax::GroupStart, DataSyntax::GroupEnd);
        }
        endString = valueString.sub_string(DataSyntax::GroupEnd.size(), valueString.size() - 2 * DataSyntax::GroupEnd.size());
        Zero::StringSplitRange data = endString.Split(",");

        Integer2 value = Integer2(0, 0);

        unsigned i = 0;
        while (!data.empty() && i < 2)
        {
            value[i] = Serializer::ToInt(data.front().TrimStart().TrimEnd());
            data.popFront();
            ++i;
        }
        if (!data.empty())
        {
            Error("Integer2 on line %i cannot be more than 2 elements long.", Index + 1);
        }
        *(Integer2*)Value.Data = value;
        SinWriteLine(*(Integer2*)Value.Data);
    }
    else if (type == ZilchTypeId(Integer3))
    {
        unsigned end = valueString.size() - DataSyntax::GroupEnd.size();
        StringRange endString = valueString.sub_string(end, DataSyntax::GroupEnd.size());
        //if it does not start with [ and end with ]
        if (valueString.sub_string(0, DataSyntax::GroupEnd.size()).CompareTo(DataSyntax::GroupStart) != 0 || endString.CompareTo(DataSyntax::GroupEnd) != 0)
        {
            Error("Integer3 on line %i must start with |%s| and end with |%s|.", Index + 1, DataSyntax::GroupStart, DataSyntax::GroupEnd);
        }
        endString = valueString.sub_string(DataSyntax::GroupEnd.size(), valueString.size() - 2 * DataSyntax::GroupEnd.size());
        Zero::StringSplitRange data = endString.Split(",");

        Integer3 value = Integer3(0, 0, 0);

        unsigned i = 0;
        while (!data.empty() && i < 3)
        {
            value[i] = Serializer::ToInt(data.front().TrimStart().TrimEnd());
            data.popFront();
            ++i;
        }
        if (!data.empty())
        {
            Error("Integer3 on line %i cannot be more than 3 elements long.", Index + 1);
        }
        *(Integer3*)Value.Data = value;
        SinWriteLine(*(Integer3*)Value.Data);
    }
    else if (type == ZilchTypeId(Integer4))
    {
        unsigned end = valueString.size() - DataSyntax::GroupEnd.size();
        StringRange endString = valueString.sub_string(end, DataSyntax::GroupEnd.size());
        //if it does not start with [ and end with ]
        if (valueString.sub_string(0, DataSyntax::GroupEnd.size()).CompareTo(DataSyntax::GroupStart) != 0 || endString.CompareTo(DataSyntax::GroupEnd) != 0)
        {
            Error("Integer4 on line %i must start with |%s| and end with |%s|.", Index + 1, DataSyntax::GroupStart, DataSyntax::GroupEnd);
        }
        endString = valueString.sub_string(DataSyntax::GroupEnd.size(), valueString.size() - 2 * DataSyntax::GroupEnd.size());
        Zero::StringSplitRange data = endString.Split(",");

        Integer4 value = Integer4(0, 0, 0, 0);

        unsigned i = 0;
        while (!data.empty() && i < 4)
        {
            value[i] = Serializer::ToInt(data.front().TrimStart().TrimEnd());
            data.popFront();
            ++i;
        }
        if (!data.empty())
        {
            Error("Integer4 on line %i cannot be more than 4 elements long.", Index + 1);
        }
        *(Integer4*)Value.Data = value;
        SinWriteLine(*(Integer4*)Value.Data);
    }
    else if (type == ZilchTypeId(Boolean2))
    {
        unsigned end = valueString.size() - DataSyntax::GroupEnd.size();
        StringRange endString = valueString.sub_string(end, DataSyntax::GroupEnd.size());
        //if it does not start with [ and end with ]
        if (valueString.sub_string(0, DataSyntax::GroupEnd.size()).CompareTo(DataSyntax::GroupStart) != 0 || endString.CompareTo(DataSyntax::GroupEnd) != 0)
        {
            Error("Boolean2 on line %i must start with |%s| and end with |%s|.", Index + 1, DataSyntax::GroupStart, DataSyntax::GroupEnd);
        }
        endString = valueString.sub_string(DataSyntax::GroupEnd.size(), valueString.size() - 2 * DataSyntax::GroupEnd.size());
        Zero::StringSplitRange data = endString.Split(",");

        Boolean2 value = Boolean2(0, 0);

        unsigned i = 0;
        while (!data.empty() && i < 2)
        {
            value[i] = Serializer::ToBool(data.front().TrimStart().TrimEnd());
            data.popFront();
            ++i;
        }
        if (!data.empty())
        {
            Error("Boolean2 on line %i cannot be more than 2 elements long.", Index + 1);
        }
        *(Boolean2*)Value.Data = value;
        SinWriteLine(*(Boolean2*)Value.Data);
    }
    else if (type == ZilchTypeId(Boolean3))
    {
        unsigned end = valueString.size() - DataSyntax::GroupEnd.size();
        StringRange endString = valueString.sub_string(end, DataSyntax::GroupEnd.size());
        //if it does not start with [ and end with ]
        if (valueString.sub_string(0, DataSyntax::GroupEnd.size()).CompareTo(DataSyntax::GroupStart) != 0 || endString.CompareTo(DataSyntax::GroupEnd) != 0)
        {
            Error("Boolean3 on line %i must start with |%s| and end with |%s|.", Index + 1, DataSyntax::GroupStart, DataSyntax::GroupEnd);
        }
        endString = valueString.sub_string(DataSyntax::GroupEnd.size(), valueString.size() - 2 * DataSyntax::GroupEnd.size());
        Zero::StringSplitRange data = endString.Split(",");

        Boolean3 value = Boolean3(0, 0, 0);

        unsigned i = 0;
        while (!data.empty() && i < 3)
        {
            value[i] = Serializer::ToBool(data.front().TrimStart().TrimEnd());
            data.popFront();
            ++i;
        }
        if (!data.empty())
        {
            Error("Boolean3 on line %i cannot be more than 3 elements long.", Index + 1);
        }
        *(Boolean3*)Value.Data = value;
        SinWriteLine(*(Boolean3*)Value.Data);
    }
    else if (type == ZilchTypeId(Boolean4))
    {
        unsigned end = valueString.size() - DataSyntax::GroupEnd.size();
        StringRange endString = valueString.sub_string(end, DataSyntax::GroupEnd.size());
        //if it does not start with [ and end with ]
        if (valueString.sub_string(0, DataSyntax::GroupEnd.size()).CompareTo(DataSyntax::GroupStart) != 0 || endString.CompareTo(DataSyntax::GroupEnd) != 0)
        {
            Error("Boolean4 on line %i must start with |%s| and end with |%s|.", Index + 1, DataSyntax::GroupStart, DataSyntax::GroupEnd);
        }
        endString = valueString.sub_string(DataSyntax::GroupEnd.size(), valueString.size() - 2 * DataSyntax::GroupEnd.size());
        Zero::StringSplitRange data = endString.Split(",");

        Boolean4 value = Boolean4(0, 0, 0, 0);

        unsigned i = 0;
        while (!data.empty() && i < 4)
        {
            value[i] = Serializer::ToBool(data.front().TrimStart().TrimEnd());
            data.popFront();
            ++i;
        }
        if (!data.empty())
        {
            Error("Boolean4 on line %i cannot be more than 4 elements long.", Index + 1);
        }
        *(Boolean4*)Value.Data = value;
        SinWriteLine(*(Boolean4*)Value.Data);
    }
    else if (type == ZilchTypeId(Real2))
    {
        unsigned end = valueString.size() - DataSyntax::GroupEnd.size();
        StringRange endString = valueString.sub_string(end, DataSyntax::GroupEnd.size());
        //if it does not start with [ and end with ]
        if (valueString.sub_string(0, DataSyntax::GroupEnd.size()).CompareTo(DataSyntax::GroupStart) != 0 || endString.CompareTo(DataSyntax::GroupEnd) != 0)
        {
            Error("Real2 on line %i must start with |%s| and end with |%s|.", Index + 1, DataSyntax::GroupStart, DataSyntax::GroupEnd);
        }
        endString = valueString.sub_string(DataSyntax::GroupEnd.size(), valueString.size() - 2 * DataSyntax::GroupEnd.size());
        Zero::StringSplitRange data = endString.Split(",");

        Real2 value = Real2(0, 0);

        unsigned i = 0;
        while (!data.empty() && i < 2)
        {
            value[i] = Serializer::ToFloat(data.front().TrimStart().TrimEnd());
            data.popFront();
            ++i;
        }
        if (!data.empty())
        {
            Error("Real2 on line %i cannot be more than 2 elements long.", Index + 1);
        }
        *(Real2*)Value.Data = value;
        SinWriteLine(*(Real2*)Value.Data);
    }
    else if (type == ZilchTypeId(Real3))
    {
        unsigned end = valueString.size() - DataSyntax::GroupEnd.size();
        StringRange endString = valueString.sub_string(end, DataSyntax::GroupEnd.size());
        //if it does not start with [ and end with ]
        if (valueString.sub_string(0, DataSyntax::GroupEnd.size()).CompareTo(DataSyntax::GroupStart) != 0 || endString.CompareTo(DataSyntax::GroupEnd) != 0)
        {
            Error("Real3 on line %i must start with |%s| and end with |%s|.", Index + 1, DataSyntax::GroupStart, DataSyntax::GroupEnd);
        }
        endString = valueString.sub_string(DataSyntax::GroupEnd.size(), valueString.size() - 2 * DataSyntax::GroupEnd.size());
        Zero::StringSplitRange data = endString.Split(",");

        Real3 value = Real3(0, 0, 0);

        unsigned i = 0;
        while (!data.empty() && i < 3)
        {
            value[i] = Serializer::ToFloat(data.front().TrimStart().TrimEnd());
            data.popFront();
            ++i;
        }
        if (!data.empty())
        {
            Error("Real3 on line %i cannot be more than 3 elements long.", Index + 1);
        }
        *(Real3*)Value.Data = value;
        SinWriteLine(*(Real3*)Value.Data);
    }
    else if (type == ZilchTypeId(Real4))
    {
        unsigned end = valueString.size() - DataSyntax::GroupEnd.size();
        StringRange endString = valueString.sub_string(end, DataSyntax::GroupEnd.size());
        //if it does not start with [ and end with ]
        if (valueString.sub_string(0, DataSyntax::GroupEnd.size()).CompareTo(DataSyntax::GroupStart) != 0 || endString.CompareTo(DataSyntax::GroupEnd) != 0)
        {
            Error("Real4 on line %i must start with |%s| and end with |%s|.", Index + 1, DataSyntax::GroupStart, DataSyntax::GroupEnd);
        }
        endString = valueString.sub_string(DataSyntax::GroupEnd.size(), valueString.size() - 2 * DataSyntax::GroupEnd.size());
        Zero::StringSplitRange data = endString.Split(",");

        Real4 value = Real4(0, 0, 0, 0);

        unsigned i = 0;
        while (!data.empty() && i < 4)
        {
            value[i] = Serializer::ToFloat(data.front().TrimStart().TrimEnd());
            data.popFront();
            ++i;
        }
        if (!data.empty())
        {
            Error("Real4 on line %i cannot be more than 4 elements long.", Index + 1);
        }
        *(Real4*)Value.Data = value;
        SinWriteLine(*(Real4*)Value.Data);
    }
    else if (type == ZilchTypeId(Quaternion))
    {
        unsigned end = valueString.size() - DataSyntax::GroupEnd.size();
        StringRange endString = valueString.sub_string(end, DataSyntax::GroupEnd.size());
        //if it does not start with [ and end with ]
        if (valueString.sub_string(0, DataSyntax::GroupEnd.size()).CompareTo(DataSyntax::GroupStart) != 0 || endString.CompareTo(DataSyntax::GroupEnd) != 0)
        {
            Error("Quaternion on line %i must start with |%s| and end with |%s|.", Index + 1, DataSyntax::GroupStart, DataSyntax::GroupEnd);
        }
        endString = valueString.sub_string(DataSyntax::GroupEnd.size(), valueString.size() - 2 * DataSyntax::GroupEnd.size());
        Zero::StringSplitRange data = endString.Split(",");

        Quaternion value = Quaternion(0, 0, 0, 0);

        unsigned i = 0;
        while (!data.empty() && i < 4)
        {
            value[i] = Serializer::ToFloat(data.front().TrimStart().TrimEnd());
            data.popFront();
            ++i;
        }
        if (!data.empty())
        {
            Error("Quaternion on line %i cannot be more than 4 elements long.", Index + 1);
        }
        *(Quaternion*)Value.Data = value;
        SinWriteLine(*(Quaternion*)Value.Data);
    }
}

String DataProperty::GetValueString()
{
    return String();
}

void DataProperty::SetValueString(const String& value, Type* type)
{

}

const byte* DataProperty::GetRawValue()
{
    return rawValue;
}
void DataProperty::SetRawValue(const byte* Value)
{

}

const Handle& DataProperty::GetHandle()
{
    return Value;
}
void DataProperty::SetHandle(const Handle& Value)
{

}

const String& DataProperty::GetName()
{
    return Name;
}
void DataProperty::SetName(const String& Name)
{

}

const Type* DataProperty::GetType()
{
    return ZilchTypeId(int);
}
void DataProperty::SetType(const Type& Type)
{

}

void DataProperty::PrintData()
{

}
unsigned DataProperty::GetIndex()
{
    return 0;
}
void DataProperty::SetIndex(unsigned& range)
{

}

DataProperty::~DataProperty()
{
}