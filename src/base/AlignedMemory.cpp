/**
 * @file AlignedMemory.cpp
 * @brief 对齐内存分配器实现
 * @author VisionForge Team
 * @date 2025-12-14
 *
 * @details
 * 本文件实现AlignedMemoryHelper类的所有成员函数。
 *
 * ## 实现细节
 *
 * ### 平台特定实现
 * - Windows: 使用_aligned_malloc/_aligned_free (malloc.h)
 * - POSIX(Linux/macOS): 使用posix_memalign/free (cstdlib)
 *
 * ### 对齐计算
 * getAlignedSize()使用位运算优化：
 * aligned = ((size + alignment - 1) / alignment) * alignment
 * 等价于：aligned = (size + alignment - 1) & ~(alignment - 1)（当alignment为2的幂时）
 *
 * ### 指针对齐检查
 * isAligned()使用模运算检查：
 * (uintptr_t(ptr) % alignment) == 0
 *
 * ### 注意事项
 * - alignment必须是2的幂（系统要求）
 * - 分配失败时返回nullptr，不抛出异常
 * - 释放nullptr是安全的（函数内部检查）
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
