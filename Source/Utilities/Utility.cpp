#include <Precompiled.h>
#include "Utility.h"

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


