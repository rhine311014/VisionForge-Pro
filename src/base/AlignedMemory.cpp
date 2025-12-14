/**
 * @file AlignedMemory.cpp
 * @brief 对齐内存分配器实现
 */

#include "base/AlignedMemory.h"

#ifdef _WIN32
#include <malloc.h>
#else
#include <cstdlib>
#endif

namespace VisionForge {
namespace Base {

void* AlignedMemoryHelper::allocate(size_t size, size_t alignment)
{
    if (size == 0) {
        return nullptr;
    }

    void* ptr = nullptr;

#ifdef _WIN32
    ptr = _aligned_malloc(size, alignment);
#else
    if (posix_memalign(&ptr, alignment, size) != 0) {
        ptr = nullptr;
    }
#endif

    return ptr;
}

void AlignedMemoryHelper::deallocate(void* ptr)
{
    if (!ptr) {
        return;
    }

#ifdef _WIN32
    _aligned_free(ptr);
#else
    free(ptr);
#endif
}

bool AlignedMemoryHelper::isAligned(const void* ptr, size_t alignment)
{
    return (reinterpret_cast<uintptr_t>(ptr) % alignment) == 0;
}

size_t AlignedMemoryHelper::getAlignedSize(size_t size, size_t alignment)
{
    return ((size + alignment - 1) / alignment) * alignment;
}

void* AlignedMemoryHelper::alignPointer(void* ptr, size_t alignment)
{
    uintptr_t addr = reinterpret_cast<uintptr_t>(ptr);
    uintptr_t aligned = ((addr + alignment - 1) / alignment) * alignment;
    return reinterpret_cast<void*>(aligned);
}

} // namespace Base
} // namespace VisionForge
