#pragma once
#include <Precompiled.h>
#include "Engine.h"
#include "DataComponent.h"

class DataProperty
{
public:
    ZilchDeclareBaseType(DataProperty, TypeCopyMode::ReferenceType);
    DataProperty(DataComponent* const parent);
    DataProperty(DataComponent* const parent, String* data, unsigned index);
    DataProperty(DataComponent* const parent, String* source, unsigned index, String& name, Type* type, byte* value);

    void LoadProperty(String* data);

    String GetValueString();
    void SetValueString(const String& value, Type* type);

    const byte* GetRawValue();
    void SetRawValue(const byte* Value);

    const Handle& GetHandle();
    void SetHandle(const Handle& Value);

    const String& GetName();
    void SetName(const String& Name);
    
    const Type* GetType();
    void SetType(const Type& Type);

    void PrintData();
    unsigned GetIndex();
    void SetIndex(unsigned& range);

    ~DataProperty();

private:
    unsigned Index;
    DataComponent* const Parent;
    String Name;
    String* FileData; //Whole Line
    Handle Value;
    byte* rawValue;
};