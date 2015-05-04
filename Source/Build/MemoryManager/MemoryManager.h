#pragma once
#include <Precompiled.h>

#define DefaultHeapSize 500

class MemoryManager
{
public:
    static void Initialize(size_t heapSize = DefaultHeapSize);
    static void Update();
    static byte* Allocate(size_t size);
    static void Free(byte* start, size_t size);

private:
    Array<byte> Heap;
};
