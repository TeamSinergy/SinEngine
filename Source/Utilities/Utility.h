#pragma once
#include <Precompiled.h>

#define MemCheck(pointer, name) ErrorIf(pointer == nullptr, String::Join("","Could not allocate enough memory for: ", name).c_str())

class Utility
{
public:
    static char CharMatchesAny(const char input, const char* match);
    static bool HasCharacters(const char* input, const char* match);
    static void Strcpy(String& buffer, const char* toCopy, const unsigned size = -1);
};

