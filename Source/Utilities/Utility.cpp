#include <Precompiled.h>
#include "Utility.h"

HashMap<String, ComponentCreator*> Utility::Components = HashMap<String, ComponentCreator*>();

char Utility::CharMatchesAny(const char input, const char* match)
{
    while (*match != NULL)
    {
        if (input == *match)
        {
            return *match;
        }
        ++match;
    }
    return false;
}

bool Utility::HasCharacters(const char* input, const char* match)
{
    unsigned i = 0;
    while (match[i] != NULL)
    {
        if (input[i] != match[i])
        {
            return false;
        }
        ++i;
    }
    return true;
}

void Utility::Strcpy(String& buffer, const char* toCopy, const unsigned size)
{
    std::string temp;
    unsigned i = 0;
    while ((i < size) && (toCopy[i] != NULL))
    {
        temp.push_back(toCopy[i]);
    }
    buffer = temp.c_str();
}

std::wstring Utility::ToWideString(const String& input)
{
    wchar_t* buf = new wchar_t[input.size() * 2 + 2];
    swprintf(buf, L"%S", input.c_str());
    std::wstring rval = buf;
    delete[] buf;
    return rval;
}

void Utility::CompileShader(const String& srcFile, const String& entryPoint, const String& profile, ID3DBlob** blob)
{

    *blob = nullptr;

    UINT flags = 0;//D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    flags |= D3DCOMPILE_DEBUG;
#endif

    const D3D_SHADER_MACRO defines[] =
    {
        "EXAMPLE_DEFINE", "1",
        NULL, NULL
    };

    ID3DBlob* shaderBlob = nullptr;
    ID3DBlob* errorBlob = nullptr;
    std::wstring path = Utility::ToWideString(srcFile);
    HRESULT hr = D3DCompileFromFile(LPWSTR(path.data()), defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
        LPCSTR(entryPoint.c_str()), LPSTR(profile.c_str()),
        flags, 0, &shaderBlob, &errorBlob);

    if (FAILED(hr))
    {
        if (errorBlob)
        {
            SinWriteLineColor((char*)errorBlob->GetBufferPointer(), ConsoleColors::Red);
            Error("Error on shader %s: %s", srcFile.c_str(), (char*)errorBlob->GetBufferPointer());
            errorBlob->Release();
        }
        else
        {
            SinWriteLineColor((char*)errorBlob->GetBufferPointer(), ConsoleColors::Red);
            Error("Unknown error on shader %s.", srcFile.c_str());
        }
        if (shaderBlob)
            shaderBlob->Release();
    }

    *blob = shaderBlob;
}