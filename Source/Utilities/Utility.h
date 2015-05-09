#pragma once
#include <Precompiled.h>
#include "Component.h"

#define MemCheck(pointer, name) ErrorIf(pointer == nullptr, String::Join("","Could not allocate enough memory for: ", name).c_str())


template <typename T>
Handle ZilchAllocate()
{
    ExecutableState* state = ZILCH->GetDependencies();
    return state->AllocateDefaultConstructedHeapObject(ZilchTypeId(T), ZILCH->Report, Zilch::HeapFlags::ReferenceCounted);
}



///Game Component Creator Interface.
///Used to create components in data driven method in the ObjectSystem.
class ComponentCreator
{
public:
    ComponentCreator(const String& componentName)
        :Name(componentName)
    {
    }

    const String Name;
    ///Create the component

    virtual Handle Create() { return Handle(); }
};

///Templated helper class used to make concrete component creators.
template<typename T>
class ComponentCreatorType : public ComponentCreator
{
public:
    ComponentCreatorType(const String& componentName)
        :ComponentCreator(componentName)
    {
    }

    virtual Handle Create() override
    {
        return ZilchAllocate<T>();
    }
};

//Register component macro
#define RegisterComponent(type) Utility::Components.insert( #type, new ComponentCreatorType<type>(#type) )

class Utility
{
public:
    static char CharMatchesAny(const char input, const char* match);
    static bool HasCharacters(const char* input, const char* match);
    static void Strcpy(String& buffer, const char* toCopy, const unsigned size = -1);
    static void CompileShader(const String& srcFile, const String& entryPoint, const String& profile, ID3DBlob** blob);

    static HashMap<String, ComponentCreator*> Components;

    static std::wstring ToWideString(const String& input);
};

