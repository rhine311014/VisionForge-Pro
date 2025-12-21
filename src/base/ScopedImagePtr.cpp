/**
 * @file ScopedImagePtr.cpp
 * @brief RAII图像指针实现
 * @author VisionForge Team
 * @date 2025-12-20
 */

#include "base/ScopedImagePtr.h"
#include "base/ImageMemoryPool.h"
#include "base/Logger.h"

namespace VisionForge {
namespace Base {

ScopedImagePtr::ScopedImagePtr(int width, int height, int type)
{
    try {
        // 从内存池分配
        image_ = ImageMemoryPool::instance().allocate(width, height, type);

        if (!image_) {
            LOG_ERROR(QString("ScopedImagePtr: Failed to allocate image (%1x%2, type=%3)")
                      .arg(width).arg(height).arg(type));
        }
    }
    catch (const std::exception& e) {
        LOG_ERROR(QString("ScopedImagePtr: Exception during allocation: %1")
                  .arg(e.what()));
        image_ = nullptr;
    }
}

ScopedImagePtr::~ScopedImagePtr()
{
    reset();  // 归还图像到池中
}

ScopedImagePtr& ScopedImagePtr::operator=(ScopedImagePtr&& other) noexcept
{
    if (this != &other) {
        // 归还当前图像
        reset();

        // 移动新图像
        image_ = std::move(other.image_);
        other.image_ = nullptr;
    }

    return *this;
}

void ScopedImagePtr::reset()
{
    if (image_) {
        try {
            // 归还图像到池中
            ImageMemoryPool::instance().release(image_);
        }
        catch (const std::exception& e) {
            LOG_ERROR(QString("ScopedImagePtr: Exception during release: %1")
                      .arg(e.what()));
        }

        image_ = nullptr;
    }
}

void ScopedImagePtr::reset(ImageData::Ptr image)
{
    // 先归还当前图像
    reset();

    // 接管新图像
    image_ = std::move(image);
}

void ScopedImagePtr::reset(int width, int height, int type)
{
    // 先归还当前图像
    reset();

    // 从池中分配新图像
    try {
        image_ = ImageMemoryPool::instance().allocate(width, height, type);

        if (!image_) {
            LOG_ERROR(QString("ScopedImagePtr::reset: Failed to allocate image (%1x%2, type=%3)")
                      .arg(width).arg(height).arg(type));
        }
    }
    catch (const std::exception& e) {
        LOG_ERROR(QString("ScopedImagePtr::reset: Exception during allocation: %1")
                  .arg(e.what()));
        image_ = nullptr;
    }
}

} // namespace Base
} // namespace VisionForge
