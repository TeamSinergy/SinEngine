#include <Precompiled.h>


void MemoryManager::Initialize(size_t heapSize)
{

}
void MemoryManager::Update()
{

}
byte* MemoryManager::Allocate(size_t size)
{
    return new byte[size];
}
void MemoryManager::Free(byte* start, size_t size)
{
    delete[] start;
}