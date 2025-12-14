/**
 * @file AlignedMemory.h
 * @brief 对齐内存分配器和辅助工具
 * @author VisionForge Team
 * @date 2025-12-14
 */

#pragma once

#include <cstddef>
#include <limits>
#include <new>
#include <memory>

namespace VisionForge {
namespace Base {

/**
 * @brief 对齐内存辅助类
 */
class AlignedMemoryHelper {
public:
    /**
     * @brief 分配对齐内存
     * @param size 字节数
     * @param alignment 对齐字节数（默认32字节）
     * @return 对齐的内存指针
     */
    static void* allocate(size_t size, size_t alignment = 32);

    /**
     * @brief 释放对齐内存
     * @param ptr 内存指针
     */
    static void deallocate(void* ptr);

    /**
     * @brief 检查内存是否对齐
     * @param ptr 内存指针
     * @param alignment 对齐字节数（默认32字节）
     * @return true如果对齐
     */
    static bool isAligned(const void* ptr, size_t alignment = 32);

    /**
     * @brief 获取对齐后的大小
     * @param size 原始大小
     * @param alignment 对齐字节数
     * @return 对齐后的大小
     */
    static size_t getAlignedSize(size_t size, size_t alignment = 32);

    /**
     * @brief 对齐指针
     * @param ptr 原始指针
     * @param alignment 对齐字节数
     * @return 对齐后的指针
     */
    static void* alignPointer(void* ptr, size_t alignment = 32);
};

/**
 * @brief STL兼容的对齐内存分配器
 * @tparam T 元素类型
 * @tparam Alignment 对齐字节数（默认32字节）
 */
template<typename T, size_t Alignment = 32>
class AlignedAllocator {
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

    template<typename U>
    struct rebind {
        using other = AlignedAllocator<U, Alignment>;
    };

    AlignedAllocator() noexcept = default;

    template<typename U>
    AlignedAllocator(const AlignedAllocator<U, Alignment>&) noexcept {}

    /**
     * @brief 分配对齐内存
     * @param n 元素数量
     * @return 对齐的内存指针
     */
    pointer allocate(size_type n)
    {
        if (n == 0) {
            return nullptr;
        }

        if (n > std::numeric_limits<size_type>::max() / sizeof(T)) {
            throw std::bad_alloc();
        }

        size_type size = n * sizeof(T);
        void* ptr = AlignedMemoryHelper::allocate(size, Alignment);

        if (!ptr) {
            throw std::bad_alloc();
        }

        return static_cast<pointer>(ptr);
    }

    /**
     * @brief 释放对齐内存
     * @param p 内存指针
     * @param n 元素数量（未使用）
     */
    void deallocate(pointer p, size_type) noexcept
    {
        AlignedMemoryHelper::deallocate(p);
    }

    /**
     * @brief 构造对象
     */
    template<typename U, typename... Args>
    void construct(U* p, Args&&... args)
    {
        ::new(static_cast<void*>(p)) U(std::forward<Args>(args)...);
    }

    /**
     * @brief 析构对象
     */
    template<typename U>
    void destroy(U* p)
    {
        p->~U();
    }

    /**
     * @brief 最大可分配元素数量
     */
    size_type max_size() const noexcept
    {
        return std::numeric_limits<size_type>::max() / sizeof(T);
    }
};

// 比较运算符
template<typename T1, size_t A1, typename T2, size_t A2>
bool operator==(const AlignedAllocator<T1, A1>&, const AlignedAllocator<T2, A2>&) noexcept
{
    return A1 == A2;
}

template<typename T1, size_t A1, typename T2, size_t A2>
bool operator!=(const AlignedAllocator<T1, A1>&, const AlignedAllocator<T2, A2>&) noexcept
{
    return A1 != A2;
}

} // namespace Base
} // namespace VisionForge
