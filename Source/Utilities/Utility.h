#pragma once
#include <Precompiled.h>
#include "Component.h"
#include "ArrayRange.h"
#include "DirectXMath.h"

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

    static void Destroy();

    template <typename T>
    static void Swap(T& a, T& b)
    {
        T store = a;
        a = b;
        b = store;
    }

    template <typename T>
    static T& Max(T& a, T& b)
    {
        if (a >= b)
        {
            return a;
        }
        return b;
    }

    template <typename T>
    static T& Min(T& a, T& b)
    {
        if (a <= b)
        {
            return a;
        }
        return b;
    }
};

class Matrix
{
public:
    Matrix(unsigned rows = 0, unsigned columns = 0);
    Matrix(const DirectX::XMMATRIX& matrix);
    unsigned Dot(const Matrix& matrix) const;
    Matrix Cross(const Matrix& matrix) const;
    Matrix& Transpose();
    Matrix Transposed();
    unsigned Rows() const;
    unsigned Columns() const;
    Matrix& Resize(unsigned rows, unsigned columns);
    Matrix& Resize(Unsigned2 dimensions);
    Matrix& Indentity();
    Matrix& Fill(int number);
    Unsigned2 Dimensions() const;

    Matrix operator*(const Matrix& rhs) const;
    const Array<int>& operator[](unsigned rhs) const;
    Array<int>& operator[](unsigned rhs);
    void Print();

private:
    
    Array<Array<int>> data;
};

class Real4x4
{
public:
    Real4x4(){}
    Real4x4(const DirectX::XMMATRIX& inputMatrix) { matrix = inputMatrix; };

    const DirectX::XMMATRIX& GetXMMATRIX() const { return matrix; }
    DirectX::XMMATRIX& GetXMMATRIX() { return matrix; }
private:
    DirectX::XMMATRIX matrix;
};