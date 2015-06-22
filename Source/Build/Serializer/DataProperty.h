#pragma once
#include <Precompiled.h>
#include "Engine.h"
#include "DataComponent.h"

class DataProperty : public DataNode
{
public:
    ZilchDeclareDerivedType(DataProperty, DataNode);
    DataProperty(DataComponent* const parent);
    DataProperty(DataComponent* const parent, String* data, unsigned index);
    DataProperty(DataComponent* const parent, String* source, unsigned index, const String& name, const Handle& store);

    void LoadProperty(String* data);

    template<typename T>
    const T& GetValue()
    {
        if (ZilchTypeId(T)->CopyMode == TypeCopyMode::ReferenceType)
        {
            return **(T**)Value.Data;
        }

        return *(T*)Value.Data;
    }

    template<typename T>
    void SetValue(const T& value)
    {
        if (ZilchTypeId(T)->CopyMode == TypeCopyMode::ReferenceType)
        {
            *(T**)Value.Data = value;
        }
        else
        {
            *(T*)Value.Data = value;
        }
        SetType(ZilchTypeId(T));
    }

    String GetValueString();
    void SetValueString(const String& value);

    const Handle& GetHandle();
    void SetHandle(const Handle& Value);

    const String& GetName();
    void SetName(const String& Name);
    
    const BoundType* GetType() const;
    BoundType* GetType();
    void SetType(BoundType* Type);

    void PrintData();
    unsigned GetIndex();
    void SetIndex(unsigned& range);

    template<typename T>
    void FillArray(Array<T>& input, String& dataFile, unsigned line = 0, const String& type = "Undefined")
    {
        unsigned i = 0;
        for (i; i < input.size(); ++i)
        {
            input[i] = 0;
        }
        unsigned end = dataFile.size() - DataSyntax::GroupEnd.size();
        StringRange endString = dataFile.sub_string(end, DataSyntax::GroupEnd.size());
        //if it does not start with [ and end with ]
        if (dataFile.sub_string(0, DataSyntax::GroupEnd.size()).CompareTo(DataSyntax::GroupStart) != 0 || endString.CompareTo(DataSyntax::GroupEnd) != 0)
        {
            Error("%s on line %i must start with |%s| and end with |%s|.", type.c_str(), Index + 1, DataSyntax::GroupStart.c_str(), DataSyntax::GroupEnd.c_str());
        }
        endString = dataFile.sub_string(DataSyntax::GroupEnd.size(), dataFile.size() - 2 * DataSyntax::GroupEnd.size());
        Zero::StringSplitRange data = endString.Split(",");

        

        i = 0;
        while (!data.empty() && i < input.size())
        {
            
            Serializer::ToValueFromString(String(data.front().TrimStart().TrimEnd()), input[i], line);
            data.popFront();
            ++i;
        }
        if (!data.empty())
        {
            Error("%s on line %i cannot be more than %i elements long.", type.c_str(), Index + 1, input.size());
        }
        
    }

    ~DataProperty();

private:
    void ParseValue(String& data, BoundType* type);

    unsigned Index;
    DataComponent* const Parent;
    String Name;
    String TypeString;
    String ValueString;
    String* FileData; //Whole Line
    Handle Value;
};