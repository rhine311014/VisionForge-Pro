/**
 * @file SimulatedCamera.cpp
 * @brief 模拟相机实现
 */

#include "hal/SimulatedCamera.h"
#include "base/Logger.h"
#include "base/ImageMemoryPool.h"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <QFileInfo>
#include <QThread>

namespace VisionForge {
namespace HAL {

SimulatedCamera::SimulatedCamera(QObject* parent)
    : ICamera(parent)
    , isOpen_(false)
    , isGrabbing_(false)
    , sourceType_(TestPattern)
    , currentIndex_(0)
    , testPatternType_(0)
    , frameRate_(30.0)
{
    grabTimer_ = new QTimer(this);
    connect(grabTimer_, &QTimer::timeout, this, &SimulatedCamera::onContinuousGrab);

    // 默认配置
    config_.width = 1920;
    config_.height = 1080;
    config_.triggerMode = Software;
}

SimulatedCamera::~SimulatedCamera()
{
    close();
}

// ========== 设备管理 ==========

bool SimulatedCamera::open()
{
    if (isOpen_) {
        return true;
    }

    LOG_INFO(QString("打开模拟相机: %1x%2")
        .arg(config_.width).arg(config_.height));

    isOpen_ = true;
    return true;
}

void SimulatedCamera::close()
{
    if (!isOpen_) {
        return;
    }

    stopGrabbing();
    isOpen_ = false;

    LOG_INFO("关闭模拟相机");
}

// ========== 采集控制 ==========

bool SimulatedCamera::startGrabbing()
{
    if (!isOpen_) {
        LOG_ERROR("相机未打开，无法开始采集");
        return false;
    }

    if (isGrabbing_) {
        return true;
    }

    isGrabbing_ = true;

    // 如果是连续模式，启动定时器
    if (config_.triggerMode == Continuous) {
        int interval = static_cast<int>(1000.0 / frameRate_);
        grabTimer_->start(interval);
        LOG_INFO(QString("开始连续采集，帧率: %1 FPS").arg(frameRate_));
    } else {
        LOG_INFO("开始采集（触发模式）");
    }

    return true;
}

void SimulatedCamera::stopGrabbing()
{
    if (!isGrabbing_) {
        return;
    }

    grabTimer_->stop();
    isGrabbing_ = false;

    LOG_INFO("停止采集");
}

Base::ImageData::Ptr SimulatedCamera::grabImage(int timeoutMs)
{
    Q_UNUSED(timeoutMs);

    if (!isOpen_) {
        LOG_ERROR("相机未打开");
        return nullptr;
    }

    Base::ImageData::Ptr image;

    switch (sourceType_) {
    case TestPattern:
        image = generateTestPattern(testPatternType_);
        break;

    case ImageFile:
        if (!singleImagePath_.isEmpty()) {
            cv::Mat mat = cv::imread(singleImagePath_.toStdString());
            if (!mat.empty()) {
                // 调整大小到配置的分辨率
                if (mat.cols != config_.width || mat.rows != config_.height) {
                    cv::resize(mat, mat, cv::Size(config_.width, config_.height));
                }
                // 应用增益和曝光
                mat = applyGainAndExposure(mat);
                image = std::make_shared<Base::ImageData>(mat);
            }
        }
        break;

    case ImageSequence:
        image = loadNextImage();
        break;

    case VideoFile:
        // TODO: 实现视频文件读取
        image = generateTestPattern(testPatternType_);
        break;
    }

    if (image) {
        image->setTimestamp(QDateTime::currentMSecsSinceEpoch());
        emit imageGrabbed(image);
    }

    return image;
}

bool SimulatedCamera::trigger()
{
    if (!isOpen_ || !isGrabbing_) {
        return false;
    }

    if (config_.triggerMode == Software) {
        // 软件触发：立即采集一帧
        grabImage();
        return true;
    }

    return false;
}

// ========== 参数配置 ==========

void SimulatedCamera::setConfig(const Config& config)
{
    config_ = config;
    cachedImage_.reset();  // 清除缓存，强制重新生成

    LOG_INFO(QString("更新相机配置: %1x%2, 曝光=%3μs, 增益=%4dB")
        .arg(config_.width).arg(config_.height)
        .arg(config_.exposure).arg(config_.gain));
}

void SimulatedCamera::setExposure(double exposure)
{
    config_.exposure = exposure;
    cachedImage_.reset();
}

void SimulatedCamera::setGain(double gain)
{
    config_.gain = gain;
    cachedImage_.reset();
}

void SimulatedCamera::setTriggerMode(TriggerMode mode)
{
    bool wasGrabbing = isGrabbing_;

    if (wasGrabbing) {
        stopGrabbing();
    }

    config_.triggerMode = mode;

    if (wasGrabbing) {
        startGrabbing();
    }
}

// ========== 模拟相机特有功能 ==========

void SimulatedCamera::setImageSource(const QString& imagePath)
{
    singleImagePath_ = imagePath;
    sourceType_ = ImageFile;
    cachedImage_.reset();

    LOG_INFO(QString("设置图像源: %1").arg(imagePath));
}

void SimulatedCamera::setImageSequence(const QStringList& imagePaths)
{
    imageSequence_ = imagePaths;
    sourceType_ = ImageSequence;
    currentIndex_ = 0;
    cachedImage_.reset();

    LOG_INFO(QString("设置图片序列: %1张图片").arg(imagePaths.size()));
}

void SimulatedCamera::setVideoSource(const QString& videoPath)
{
    videoPath_ = videoPath;
    sourceType_ = VideoFile;
    cachedImage_.reset();

    LOG_INFO(QString("设置视频源: %1").arg(videoPath));
}

void SimulatedCamera::useTestPattern(int patternType)
{
    testPatternType_ = patternType;
    sourceType_ = TestPattern;
    cachedImage_.reset();

    LOG_INFO(QString("使用测试图案: 类型%1").arg(patternType));
}

void SimulatedCamera::setFrameRate(double fps)
{
    frameRate_ = fps;

    if (isGrabbing_ && config_.triggerMode == Continuous) {
        int interval = static_cast<int>(1000.0 / frameRate_);
        grabTimer_->setInterval(interval);
    }
}

// ========== 私有方法 ==========

void SimulatedCamera::onContinuousGrab()
{
    grabImage();
}

Base::ImageData::Ptr SimulatedCamera::generateTestPattern(int patternType)
{
    // 使用内存池分配
    auto image = Base::ImageMemoryPool::instance().allocate(
        config_.width, config_.height, CV_8UC3);

    cv::Mat& mat = image->mat();

    switch (patternType) {
    case 0: {  // 渐变图案
        for (int y = 0; y < mat.rows; ++y) {
            for (int x = 0; x < mat.cols; ++x) {
                uchar r = static_cast<uchar>(255.0 * x / mat.cols);
                uchar g = static_cast<uchar>(255.0 * y / mat.rows);
                uchar b = static_cast<uchar>(128);
                mat.at<cv::Vec3b>(y, x) = cv::Vec3b(b, g, r);
            }
        }
        break;
    }

    case 1: {  // 棋盘图案
        int squareSize = 50;
        for (int y = 0; y < mat.rows; ++y) {
            for (int x = 0; x < mat.cols; ++x) {
                bool isWhite = ((x / squareSize) + (y / squareSize)) % 2 == 0;
                uchar color = isWhite ? 255 : 0;
                mat.at<cv::Vec3b>(y, x) = cv::Vec3b(color, color, color);
            }
        }
        break;
    }

    case 2: {  // 彩色条纹
        int stripeWidth = config_.width / 7;
        cv::Scalar colors[] = {
            cv::Scalar(255, 255, 255),  // 白
            cv::Scalar(0, 255, 255),    // 黄
            cv::Scalar(0, 255, 255),    // 青
            cv::Scalar(0, 255, 0),      // 绿
            cv::Scalar(255, 0, 255),    // 品红
            cv::Scalar(255, 0, 0),      // 红
            cv::Scalar(0, 0, 255)       // 蓝
        };

        for (int i = 0; i < 7; ++i) {
            cv::Rect rect(i * stripeWidth, 0, stripeWidth, mat.rows);
            cv::rectangle(mat, rect, colors[i], cv::FILLED);
        }
        break;
    }

    default:
        mat = cv::Scalar(128, 128, 128);  // 灰色
        break;
    }

    // 应用增益和曝光模拟
    mat = applyGainAndExposure(mat);

    return image;
}

Base::ImageData::Ptr SimulatedCamera::loadNextImage()
{
    if (imageSequence_.isEmpty()) {
        return generateTestPattern(testPatternType_);
    }

    QString imagePath = imageSequence_[currentIndex_];
    currentIndex_ = (currentIndex_ + 1) % imageSequence_.size();

    cv::Mat mat = cv::imread(imagePath.toStdString());
    if (mat.empty()) {
        LOG_WARNING(QString("无法加载图片: %1").arg(imagePath));
        return generateTestPattern(testPatternType_);
    }

    // 调整大小
    if (mat.cols != config_.width || mat.rows != config_.height) {
        cv::resize(mat, mat, cv::Size(config_.width, config_.height));
    }

    // 应用增益和曝光
    mat = applyGainAndExposure(mat);

    auto image = std::make_shared<Base::ImageData>(mat);
    return image;
}

cv::Mat SimulatedCamera::applyGainAndExposure(const cv::Mat& input)
{
    cv::Mat output = input.clone();

    // 模拟曝光（亮度调整）
    // 标准曝光10000μs，其他值按比例调整
    double exposureFactor = config_.exposure / 10000.0;
    exposureFactor = std::max(0.1, std::min(10.0, exposureFactor));

    // 模拟增益（对比度调整）
    // 增益范围通常0-24dB
    double gainFactor = 1.0 + config_.gain / 24.0;
    gainFactor = std::max(1.0, std::min(3.0, gainFactor));

    // 应用
    output.convertTo(output, -1, gainFactor * exposureFactor, 0);

    return output;
}

} // namespace HAL
} // namespace VisionForge
