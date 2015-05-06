#pragma once
#include <Precompiled.h>

#define MemCheck(pointer, name) ErrorIf(pointer == nullptr, String::Join("","Could not allocate enough memory for: ", name).c_str())



//constexpr unsigned const_hash(char const *input) {
//    return *input ?
//        static_cast<unsigned int>(*input) + 33 * const_hash(input + 1) :
//        5381;
//}
//
//// This don't take into account the nul char
//#define ConstantHash(x) const_hash(x)
//



class Utility
{
public:
    static char CharMatchesAny(const char input, const char* match);
    static bool HasCharacters(const char* input, const char* match);
    static void Strcpy(String& buffer, const char* toCopy, const unsigned size = -1);
};

