/**
 * @file HalconDisplayWorker.cpp
 * @brief Halcon图像显示工作线程实现
 */

#include "ui/HalconDisplayWorker.h"
#include "base/Logger.h"
#include <QMutexLocker>

// 解决Halcon与OpenCV/STL的宏冲突
#ifdef high
#undef high
#endif
#ifdef low
#undef low
#endif
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

namespace VisionForge {
namespace UI {

HalconDisplayWorker::HalconDisplayWorker(QObject* parent)
    : QThread(parent)
    , pendingImage_(nullptr)
    , stopRequested_(false)
    , hasNewImage_(false)
    , clearRequested_(false)
    , hasXLDUpdate_(false)
    , windowHandleSet_(false)
{
}

HalconDisplayWorker::~HalconDisplayWorker()
{
    stop();
    wait();
}

void HalconDisplayWorker::setWindowHandle(const HTuple& windowHandle)
{
    QMutexLocker locker(&mutex_);
#ifdef _WIN32
    windowHandle_ = windowHandle;
    windowHandleSet_ = true;
    LOG_DEBUG("HalconDisplayWorker: 窗口句柄已设置");
#endif
}

void HalconDisplayWorker::requestDisplayImage(const Base::ImageData::Ptr& image)
{
    if (!image || image->isEmpty()) {
        LOG_WARNING("HalconDisplayWorker: 请求显示空图像");
        return;
    }

    QMutexLocker locker(&mutex_);
    pendingImage_ = image;
    hasNewImage_ = true;
    condition_.wakeOne();
}

void HalconDisplayWorker::requestClearWindow()
{
    QMutexLocker locker(&mutex_);
    clearRequested_ = true;
    condition_.wakeOne();
}

void HalconDisplayWorker::setXLDContours(const QList<HXLDCont>& contours,
                                         const QStringList& colors)
{
#ifdef _WIN32
    QMutexLocker locker(&mutex_);
    xldContours_.clear();
    xldColors_.clear();

    // 只添加有效的轮廓（CountObj > 0）
    int validCount = 0;
    for (int i = 0; i < contours.size(); i++) {
        try {
            // 检查轮廓是否有效
            if (contours[i].CountObj() > 0) {
                // 深拷贝轮廓以确保有效性
                HXLDCont contourCopy;
                CopyObj(contours[i], &contourCopy, 1, contours[i].CountObj());
                xldContours_.append(contourCopy);

                // 添加对应颜色
                if (i < colors.size()) {
                    xldColors_.append(colors[i]);
                } else {
                    xldColors_.append("green");
                }
                validCount++;
            } else {
                LOG_WARNING(QString("HalconDisplayWorker: 跳过空轮廓 [%1]").arg(i));
            }
        }
        catch (const HException& e) {
            LOG_WARNING(QString("HalconDisplayWorker: 跳过无效轮廓 [%1]: %2").arg(i).arg(e.ErrorMessage().Text()));
        }
    }

    hasXLDUpdate_ = true;
    condition_.wakeOne();
    LOG_DEBUG(QString("HalconDisplayWorker: 设置 %1 个有效XLD轮廓 (总共 %2 个)")
        .arg(validCount).arg(contours.size()));
#endif
}

void HalconDisplayWorker::clearXLDContours()
{
#ifdef _WIN32
    QMutexLocker locker(&mutex_);
    xldContours_.clear();
    xldColors_.clear();
    hasXLDUpdate_ = true;
    condition_.wakeOne();
    LOG_DEBUG("HalconDisplayWorker: 清除所有XLD轮廓");
#endif
}

void HalconDisplayWorker::stop()
{
    QMutexLocker locker(&mutex_);
    stopRequested_ = true;
    condition_.wakeOne();
}

void HalconDisplayWorker::run()
{
    LOG_INFO("HalconDisplayWorker: 显示线程已启动");

    while (true) {
        mutex_.lock();

        // 等待新任务或停止信号
        while (!stopRequested_ && !hasNewImage_ && !clearRequested_ && !hasXLDUpdate_) {
            condition_.wait(&mutex_);
        }

        if (stopRequested_) {
            mutex_.unlock();
            break;
        }

        // 处理清空请求
        if (clearRequested_) {
            clearRequested_ = false;
            mutex_.unlock();
            performClear();
            continue;
        }

        // 处理图像显示或XLD更新
        if (hasNewImage_ || hasXLDUpdate_) {
            Base::ImageData::Ptr image = pendingImage_;
            hasNewImage_ = false;
            hasXLDUpdate_ = false;
            mutex_.unlock();

            if (image && !image->isEmpty()) {
                performDisplay();
            } else if (currentHImage_.IsInitialized()) {
                // 只有XLD更新，重新显示当前图像
                performDisplay();
            }
            continue;
        }

        mutex_.unlock();
    }

    LOG_INFO("HalconDisplayWorker: 显示线程已停止");
}

void HalconDisplayWorker::performDisplay()
{
#ifdef _WIN32
    // 在锁外准备数据，减少锁持有时间
    HTuple windowHandle;
    Base::ImageData::Ptr imageCopy;

    // 快速获取数据副本
    {
        QMutexLocker locker(&mutex_);

        if (!windowHandleSet_ || windowHandle_.Length() == 0) {
            emit displayError("窗口句柄未设置");
            return;
        }

        if (!pendingImage_ || pendingImage_->isEmpty()) {
            return;
        }

        windowHandle = windowHandle_;
        imageCopy = pendingImage_;  // 共享指针拷贝（轻量）
    }
    // 锁已释放，后续操作不持有锁

    try {
        // 转换图像格式（耗时操作，在锁外执行）
        HImage hImg = imageDataToHImage(imageCopy);

        if (!hImg.IsInitialized() || hImg.CountObj() == 0) {
            emit displayError("图像转换失败");
            return;
        }

        // 显示图像（Halcon操作，在锁外执行）
        ClearWindow(windowHandle);
        DispObj(hImg, windowHandle);

        // 拷贝XLD轮廓数据（在锁内）
        QList<HXLDCont> xldCopy;
        QStringList colorsCopy;
        {
            QMutexLocker locker(&mutex_);
            currentHImage_ = hImg;
            xldCopy = xldContours_;
            colorsCopy = xldColors_;
        }
        // 锁已释放

        // 显示所有XLD轮廓（在锁外执行，避免死锁）
        for (int i = 0; i < xldCopy.size(); i++) {
            try {
                SetColor(windowHandle, colorsCopy[i].toStdString().c_str());
                SetLineWidth(windowHandle, 2);
                DispObj(xldCopy[i], windowHandle);
            }
            catch (const HException& e) {
                LOG_ERROR(QString("显示XLD轮廓失败 [%1]: %2")
                    .arg(i).arg(e.ErrorMessage().Text()));
            }
        }

        emit imageDisplayed();

        LOG_DEBUG(QString("HalconDisplayWorker: 图像已显示 (%1x%2)")
            .arg(imageCopy->width())
            .arg(imageCopy->height()));
    }
    catch (const HException& e) {
        QString errorMsg = QString("Halcon显示异常: %1").arg(e.ErrorMessage().Text());
        LOG_ERROR(errorMsg);
        emit displayError(errorMsg);
    }
#endif
}

void HalconDisplayWorker::performClear()
{
#ifdef _WIN32
    HTuple windowHandle;

    // 快速获取窗口句柄副本
    {
        QMutexLocker locker(&mutex_);

        if (!windowHandleSet_ || windowHandle_.Length() == 0) {
            return;
        }

        windowHandle = windowHandle_;
    }
    // 锁已释放

    try {
        ClearWindow(windowHandle);
        LOG_DEBUG("HalconDisplayWorker: 窗口已清空");
    }
    catch (const HException& e) {
        LOG_ERROR(QString("清空窗口失败: %1").arg(e.ErrorMessage().Text()));
    }
#endif
}

HImage HalconDisplayWorker::imageDataToHImage(const Base::ImageData::Ptr& image)
{
#ifdef _WIN32
    if (!image || image->isEmpty()) {
        return HImage();
    }

    try {
        cv::Mat mat = image->mat();
        int width = mat.cols;
        int height = mat.rows;
        int channels = mat.channels();

        HImage hImg;

        if (channels == 1) {
            // 灰度图像
            hImg.GenImage1(L"byte", width, height, (void*)mat.data);
        }
        else if (channels == 3) {
            // RGB图像 - OpenCV是BGR格式，需要转换
            cv::Mat rgb;
            cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);

            // 分离通道
            std::vector<cv::Mat> imageChannels;
            cv::split(rgb, imageChannels);

            hImg.GenImage3(L"byte", width, height,
                (void*)imageChannels[0].data,
                (void*)imageChannels[1].data,
                (void*)imageChannels[2].data);
        }
        else {
            LOG_ERROR(QString("不支持的图像通道数: %1").arg(channels));
            return HImage();
        }

        return hImg;
    }
    catch (HException& e) {
        LOG_ERROR(QString("ImageData转HImage失败: %1").arg(e.ErrorMessage().Text()));
        return HImage();
    }
#else
    return HImage();
#endif
}

} // namespace UI
} // namespace VisionForge
