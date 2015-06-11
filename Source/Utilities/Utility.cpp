#include <Precompiled.h>
#include "Utility.h"
#include <iomanip>

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

void Utility::Destroy()
{
    auto comps = Components.all();
    while (!comps.empty())
    {
        delete comps.front().second;
        comps.popFront();
    }
    Components.deallocate();
}

Matrix::Matrix(unsigned rows, unsigned columns)
{
    data.resize(rows);
    for (unsigned i = 0; i < rows; ++i)
    {
        data[i].resize(columns);
    }
    Fill(0);
}

Matrix::Matrix(const XMMATRIX& matrix)
{
    
}

Matrix Matrix::operator*(const Matrix& rhs) const
{
    const Matrix& lhs = *this;
    Unsigned2 dim1 = Dimensions();
    Unsigned2 dim2 = rhs.Dimensions();
    if (dim1.y != dim2.x)
    {
        Error("Matrices are not able to be multiplied");
    }

    Matrix matrix = Matrix(dim1.x, dim2.y);

    for (unsigned k = 0; k < dim1.x; ++k) //0
    {
        for (unsigned l = 0; l < dim2.y; ++l) //0, 1
        {
            for (unsigned i = 0; i < dim1.y; ++i) //0, 1, 2
            {
                matrix[k][l] += lhs[k][i] * rhs[i][l];
            }
        }
    }
    return matrix;
}

Unsigned2 Matrix::Dimensions() const
{
    return Unsigned2(Rows(), Columns());
}

const Array<int>& Matrix::operator[](unsigned rhs) const
{
    return data[rhs];
}

Array<int>& Matrix::operator[](unsigned rhs)
{
    return data[rhs];
}

unsigned Matrix::Dot(const Matrix& matrix) const
{
    Unsigned2 dim = Dimensions();
    if (dim != matrix.Dimensions())
    {
        Error("Dimensions must be the same for a dot product.");
    }
    unsigned dot = 0;
    for (unsigned i = 0; i < dim.x; ++i)
    {
        for (unsigned j = 0; j < dim.y; ++j)
        {
            dot += (*this)[i][j] * matrix[i][j];
        }
    }
    return dot;
}
Matrix Matrix::Cross(const Matrix& matrix) const
{
    Error("NOT IMPLIMENTED");
    return Matrix();
}
Matrix& Matrix::Transpose()
{
    Unsigned2 dim = Dimensions();
    unsigned max = Utility::Max(dim.x, dim.y);
    unsigned min = Utility::Min(dim.x, dim.y);
    Resize(max, max);

    unsigned padding = 0;
    for (unsigned i = padding; i < max; ++i)
    {
        for (unsigned j = padding; j < max; ++j)
        {
            if (i == j)
            {
                continue;
            }
            Utility::Swap(data[i][j], data[j][i]);
        }
        ++padding;
        if (padding == min)
        {
            break;
        }
    }
    Resize(dim.y, dim.x);

    return *this;
}

Matrix Matrix::Transposed()
{
    Matrix copy = *this;
    return copy.Transpose();
}

unsigned Matrix::Rows() const
{
    return data.size();
}
unsigned Matrix::Columns() const
{
    if (Rows() == 0)
    {
        return 0;
    }
    return data[0].size();
}
Matrix& Matrix::Resize(unsigned rows, unsigned columns)
{
    return Resize(Unsigned2(rows, columns));
}
Matrix& Matrix::Resize(Unsigned2 dimensions)
{
    Unsigned2 dim = Dimensions();
    data.resize(dimensions.x);
    for (unsigned i = 0; i < dimensions.x; ++i)
    {
        data[i].resize(dimensions.y);
        if (i < dim.x)
        {
            for (unsigned j = dim.y; j < dimensions.y; ++j)
            {
                data[i][j] = 0;
            }
        }
        else
        {
            for (unsigned j = 0; j < dimensions.y; ++j)
            {
                data[i][j] = 0;
            }
        }
    }
    return *this;
}
Matrix& Matrix::Indentity()
{
    Fill(0);
    for (unsigned i = 0; i < Rows(); ++i)
    {
        (*this)[i][i] = 1;
    }
    return *this;
}

void Matrix::Print()
{
    unsigned rows = Rows();
    unsigned columns = Columns();

    Matrix matrix = Matrix(rows, columns);
    SinWrite("Matrix: ");
    SinWrite((int)rows);
    SinWrite("x");
    SinWriteLine((int)columns);
    for (unsigned i = 0; i < rows; ++i)
    {
        SinWrite("[");
        for (unsigned j = 0; j < columns; ++j)
        {
            SinWrite(" ");
            std::cout << std::setw(4) << std::setprecision(4) << (*this)[i][j];
            SinWrite(" ");
        }
        SinWriteLine("]");
    }
}

Matrix& Matrix::Fill(int number)
{
    unsigned rows = Rows();
    unsigned columns = Columns();
    for (unsigned i = 0; i < rows; ++i)
    {
        for (unsigned j = 0; j < columns; ++j)
        {
            (*this)[i][j] = number;
        }
    }
    return *this;
}