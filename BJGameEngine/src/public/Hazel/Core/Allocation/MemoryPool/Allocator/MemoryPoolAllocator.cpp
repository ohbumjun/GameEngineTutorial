#include "hzpch.h"
#include "MemoryPoolAllocator.h"

MemoryPoolAllocator::MemoryPoolAllocator(size_t TotalSize) 
    : m_TotalSize(TotalSize)
{
}

MemoryPoolAllocator::~MemoryPoolAllocator()
{
    m_TotalSize = 0;
}

void* MemoryPoolAllocator::Allocate(const size_t allocationSize, const size_t alignment)
{
    return nullptr;
}