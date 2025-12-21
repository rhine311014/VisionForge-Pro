/**
 * @file ScopedImagePtr.h
 * @brief RAII图像指针包装类 - 自动管理内存池申请和释放
 * @details 解决7x24小时运行内存泄漏问题
 * @author VisionForge Team
 * @date 2025-12-20
 */

#pragma once

#include "base/ImageData.h"
#include <opencv2/core.hpp>
#include <utility>

namespace VisionForge {
namespace Base {

// 前向声明
class ImageMemoryPool;

/**
 * @class ScopedImagePtr
 * @brief RAII图像智能指针 - 自动从内存池申请和释放图像
 *
 * **问题**:
 * - 原有shared_ptr方式虽然自动释放内存,但未归还内存池
 * - 7x24小时运行后内存池命中率下降,最终耗尽系统内存
 *
 * **解决方案**:
 * - 构造时从池中分配图像
 * - 析构时自动归还图像到池中
 * - 支持移动语义,禁止拷贝
 * - RAII保证异常安全
 *
 * **使用示例**:
 * @code
 * // 旧方式（有问题）
 * {
 *     auto img = ImageMemoryPool::instance().allocate(1920, 1080, CV_8UC3);
 *     // 使用图像...
 * } // shared_ptr析构,内存释放但未归还池 ❌
 *
 * // 新方式（RAII）
 * {
 *     ScopedImagePtr img(1920, 1080, CV_8UC3);
 *     // 使用图像...
 * } // 自动归还池中，确保无泄漏 ✅
 *
 * // 移动语义
 * ScopedImagePtr createImage() {
 *     ScopedImagePtr img(1920, 1080, CV_8UC3);
 *     // 处理...
 *     return img;  // 自动移动，无拷贝
 * }
 * @endcode
 */
class ScopedImagePtr {
public:
    // ========== 构造/析构 ==========

    /**
     * @brief 默认构造（空指针）
     */
    ScopedImagePtr() noexcept
        : image_(nullptr)
    {}

    /**
     * @brief 从内存池分配图像
     * @param width 图像宽度
     * @param height 图像高度
     * @param type OpenCV图像类型（如CV_8UC3）
     *
     * @code
     * ScopedImagePtr img(1920, 1080, CV_8UC3);
     * @endcode
     */
    ScopedImagePtr(int width, int height, int type);

    /**
     * @brief 从已有ImageData构造（接管所有权）
     * @param image 图像指针
     * @note 通常不直接使用,由内部调用
     */
    explicit ScopedImagePtr(ImageData::Ptr image) noexcept
        : image_(std::move(image))
    {}

    /**
     * @brief 析构 - 自动归还图像到池中
     */
    ~ScopedImagePtr();

    // ========== 拷贝控制（禁止拷贝，支持移动）==========

    /**
     * @brief 禁止拷贝构造
     * @note 避免多次释放同一对象
     */
    ScopedImagePtr(const ScopedImagePtr&) = delete;

    /**
     * @brief 禁止拷贝赋值
     */
    ScopedImagePtr& operator=(const ScopedImagePtr&) = delete;

    /**
     * @brief 移动构造
     * @param other 被移动对象
     *
     * @code
     * ScopedImagePtr img2(std::move(img1));  // img1变为空
     * @endcode
     */
    ScopedImagePtr(ScopedImagePtr&& other) noexcept
        : image_(std::move(other.image_))
    {
        other.image_ = nullptr;
    }

    /**
     * @brief 移动赋值
     * @param other 被移动对象
     * @return 自身引用
     *
     * @code
     * img2 = std::move(img1);  // img1变为空
     * @endcode
     */
    ScopedImagePtr& operator=(ScopedImagePtr&& other) noexcept;

    // ========== 访问接口 ==========

    /**
     * @brief 获取底层shared_ptr
     * @return ImageData智能指针
     *
     * @code
     * ScopedImagePtr img(1920, 1080, CV_8UC3);
     * ImageData::Ptr ptr = img.get();  // 获取shared_ptr
     * @endcode
     */
    ImageData::Ptr get() const noexcept {
        return image_;
    }

    /**
     * @brief 成员访问运算符
     * @return ImageData指针
     *
     * @code
     * ScopedImagePtr img(1920, 1080, CV_8UC3);
     * cv::Mat mat = img->mat();  // 访问成员函数
     * @endcode
     */
    ImageData* operator->() const noexcept {
        return image_.get();
    }

    /**
     * @brief 解引用运算符
     * @return ImageData引用
     *
     * @code
     * ScopedImagePtr img(1920, 1080, CV_8UC3);
     * ImageData& data = *img;
     * @endcode
     */
    ImageData& operator*() const noexcept {
        return *image_;
    }

    /**
     * @brief 检查是否为空
     * @return true表示空指针
     *
     * @code
     * if (!img) {
     *     // 处理空指针情况
     * }
     * @endcode
     */
    bool isNull() const noexcept {
        return image_ == nullptr;
    }

    /**
     * @brief bool转换运算符
     * @return true表示非空
     *
     * @code
     * if (img) {
     *     // 图像有效
     * }
     * @endcode
     */
    explicit operator bool() const noexcept {
        return image_ != nullptr;
    }

    /**
     * @brief 释放所有权（不归还池中）
     * @return 释放的ImageData指针
     * @warning 调用后本对象变为空,图像不会归还池中
     *
     * @code
     * ScopedImagePtr img(1920, 1080, CV_8UC3);
     * auto ptr = img.release();  // 手动管理生命周期
     * @endcode
     */
    ImageData::Ptr release() noexcept {
        ImageData::Ptr temp = std::move(image_);
        image_ = nullptr;
        return temp;
    }

    /**
     * @brief 重置为空（归还当前图像到池中）
     *
     * @code
     * img.reset();  // 提前归还图像
     * @endcode
     */
    void reset();

    /**
     * @brief 重置为新图像（归还当前图像，接管新图像）
     * @param image 新图像
     */
    void reset(ImageData::Ptr image);

    /**
     * @brief 重新从池中分配图像（归还当前图像）
     * @param width 图像宽度
     * @param height 图像高度
     * @param type OpenCV图像类型
     *
     * @code
     * img.reset(1920, 1080, CV_8UC3);  // 归还旧图像，分配新图像
     * @endcode
     */
    void reset(int width, int height, int type);

    // ========== 工具函数 ==========

    /**
     * @brief 交换两个ScopedImagePtr
     * @param other 另一个对象
     */
    void swap(ScopedImagePtr& other) noexcept {
        std::swap(image_, other.image_);
    }

private:
    ImageData::Ptr image_;  ///< 底层图像指针
};

/**
 * @brief 交换函数（ADL）
 */
inline void swap(ScopedImagePtr& lhs, ScopedImagePtr& rhs) noexcept {
    lhs.swap(rhs);
}

} // namespace Base
} // namespace VisionForge
