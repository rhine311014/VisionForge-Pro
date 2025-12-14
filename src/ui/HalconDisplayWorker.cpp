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
        while (!stopRequested_ && !hasNewImage_ && !clearRequested_) {
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

        // 处理图像显示
        if (hasNewImage_) {
            Base::ImageData::Ptr image = pendingImage_;
            hasNewImage_ = false;
            mutex_.unlock();

            if (image && !image->isEmpty()) {
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
    QMutexLocker locker(&mutex_);

    if (!windowHandleSet_ || windowHandle_.Length() == 0) {
        emit displayError("窗口句柄未设置");
        return;
    }

    if (!pendingImage_ || pendingImage_->isEmpty()) {
        return;
    }

    try {
        // 转换图像格式
        currentHImage_ = imageDataToHImage(pendingImage_);

        if (!currentHImage_.IsInitialized() || currentHImage_.CountObj() == 0) {
            emit displayError("图像转换失败");
            return;
        }

        // 显示图像
        ClearWindow(windowHandle_);
        DispObj(currentHImage_, windowHandle_);

        emit imageDisplayed();

        LOG_DEBUG(QString("HalconDisplayWorker: 图像已显示 (%1x%2)")
            .arg(pendingImage_->width())
            .arg(pendingImage_->height()));
    }
    catch (HException& e) {
        QString errorMsg = QString("Halcon显示异常: %1").arg(e.ErrorMessage().Text());
        LOG_ERROR(errorMsg);
        emit displayError(errorMsg);
    }
#endif
}

void HalconDisplayWorker::performClear()
{
#ifdef _WIN32
    QMutexLocker locker(&mutex_);

    if (!windowHandleSet_ || windowHandle_.Length() == 0) {
        return;
    }

    try {
        ClearWindow(windowHandle_);
        LOG_DEBUG("HalconDisplayWorker: 窗口已清空");
    }
    catch (HException& e) {
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
