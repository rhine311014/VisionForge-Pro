/**
 * @file AlignedMemory.h
 * @brief 对齐内存分配器和辅助工具
 * @author VisionForge Team
 * @date 2025-12-14
 *
 * @details
 * 本文件提供内存对齐分配功能，用于优化SIMD指令集性能。
 *
 * ## 设计模式
 * - **工具类模式**：AlignedMemoryHelper提供静态方法
 * - **分配器模式 (Allocator)**：AlignedAllocator兼容STL容器
 *
 * ## 为什么需要内存对齐
 *
 * ### SIMD指令集要求
 * - SSE: 16字节对齐
 * - AVX/AVX2: 32字节对齐
 * - AVX-512: 64字节对齐
 *
 * 未对齐的内存访问可能导致：
 * - 性能下降（需要多次内存访问）
 * - 某些指令直接崩溃（如_mm256_load_ps）
 *
 * ### 缓存行优化
 * - 现代CPU缓存行通常为64字节
 * - 对齐分配可减少缓存行分裂
 * - 提高缓存利用率
 *
 * ## 核心组件
 *
 * ### AlignedMemoryHelper
 * 提供基础的对齐内存操作：
 * - allocate(): 分配对齐内存
 * - deallocate(): 释放对齐内存
 * - isAligned(): 检查指针是否对齐
 * - getAlignedSize(): 计算对齐后的大小
 * - alignPointer(): 将指针对齐到边界
 *
 * ### AlignedAllocator
 * STL兼容的分配器模板，可用于std::vector等容器：
 * @code
 * std::vector<float, AlignedAllocator<float, 32>> alignedVector;
 * @endcode
 *
 * ## 线程安全
 * - 所有函数都是线程安全的
 * - 底层使用系统调用，无共享状态
 *
 * ## 内存管理
 * - 必须使用deallocate()释放allocate()分配的内存
 * - 不能混用标准free()和对齐释放函数
 *
 * ## 使用示例
 * @code
 * // 分配32字节对齐的内存
 * void* ptr = AlignedMemoryHelper::allocate(1024, 32);
 *
 * // 检查对齐
 * if (AlignedMemoryHelper::isAligned(ptr, 32)) {
 *     // 可以安全使用AVX指令
 * }
 *
 * // 释放内存
 * AlignedMemoryHelper::deallocate(ptr);
 *
 * // 使用STL容器
 * std::vector<float, AlignedAllocator<float, 32>> data(1000);
 * @endcode
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
