/**
 * @file VisionEngine.cpp
 * @brief 视觉引擎核心类实现
 */

#include "core/VisionEngine.h"
#include "base/Logger.h"
#include "base/ConfigManager.h"
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <opencv2/opencv.hpp>
#include <chrono>

namespace VisionForge {
namespace Core {

VisionEngine& VisionEngine::instance()
{
    static VisionEngine instance;
    return instance;
}

VisionEngine::VisionEngine()
    : QObject(nullptr)
    , camera_(nullptr)
    , liveTimer_(nullptr)
    , isLiveGrabbing_(false)
    , currentImage_(nullptr)
    , currentImageIndex_(-1)
{
    liveTimer_ = new QTimer(this);
    connect(liveTimer_, &QTimer::timeout, this, &VisionEngine::onLiveTimer);

    LOG_INFO("VisionEngine 初始化完成");
}

VisionEngine::~VisionEngine()
{
    stopLiveGrab();
    closeCamera();
    LOG_INFO("VisionEngine 已销毁");
}

// ============== 相机管理 ==============

void VisionEngine::setCamera(std::unique_ptr<HAL::ICamera> camera)
{
    // 先停止并关闭旧相机
    stopLiveGrab();
    if (camera_) {
        camera_->close();
    }

    camera_ = std::move(camera);

    bool connected = (camera_ != nullptr);
    emit cameraStatusChanged(connected);

    if (connected) {
        LOG_INFO("相机已设置");
        emit statusMessage("相机已设置");
    }
}

bool VisionEngine::isCameraReady() const
{
    return camera_ && camera_->isOpen();
}

bool VisionEngine::openCamera()
{
    if (!camera_) {
        emit errorOccurred("未设置相机");
        return false;
    }

    if (camera_->isOpen()) {
        return true;
    }

    if (!camera_->open()) {
        emit errorOccurred("打开相机失败");
        return false;
    }

    emit cameraStatusChanged(true);
    emit statusMessage("相机已打开");
    LOG_INFO("相机已打开");
    return true;
}

void VisionEngine::closeCamera()
{
    if (camera_) {
        stopLiveGrab();
        camera_->close();
        emit cameraStatusChanged(false);
        emit statusMessage("相机已关闭");
        LOG_INFO("相机已关闭");
    }
}

// ============== 图像采集 ==============

Base::ImageData::Ptr VisionEngine::grabSingle()
{
    if (!isCameraReady()) {
        emit errorOccurred("相机未就绪");
        return nullptr;
    }

    bool wasGrabbing = camera_->isGrabbing();

    // 如果相机未在采集，先启动
    if (!wasGrabbing) {
        if (!camera_->startGrabbing()) {
            emit errorOccurred("启动相机采集失败");
            return nullptr;
        }
    }

    // 触发采集（软触发模式）
    if (camera_->getConfig().triggerMode == HAL::ICamera::Software) {
        camera_->trigger();
    }

    // 获取图像
    Base::ImageData::Ptr image = camera_->grabImage(1000);

    // 如果之前未在连续采集，停止采集
    if (!wasGrabbing && !isLiveGrabbing_) {
        camera_->stopGrabbing();
    }

    if (image) {
        // 应用图像变换
        applyImageTransform(image);
        setCurrentImage(image);
        emit statusMessage("单帧采集成功");
        LOG_DEBUG("单帧采集成功");
    } else {
        emit errorOccurred("获取图像失败");
    }

    return image;
}

bool VisionEngine::startLiveGrab(int intervalMs)
{
    if (!isCameraReady()) {
        emit errorOccurred("相机未就绪，无法启动连续采集");
        return false;
    }

    if (isLiveGrabbing_) {
        return true;  // 已经在采集
    }

    // 启动相机采集
    if (!camera_->isGrabbing()) {
        if (!camera_->startGrabbing()) {
            emit errorOccurred("启动相机采集失败");
            return false;
        }
    }

    isLiveGrabbing_ = true;
    liveTimer_->start(intervalMs);

    emit statusMessage("连续采集已启动");
    LOG_INFO(QString("连续采集已启动，间隔: %1 ms").arg(intervalMs));
    return true;
}

void VisionEngine::stopLiveGrab()
{
    if (!isLiveGrabbing_) {
        return;
    }

    isLiveGrabbing_ = false;
    liveTimer_->stop();

    if (camera_ && camera_->isGrabbing()) {
        camera_->stopGrabbing();
    }

    emit statusMessage("连续采集已停止");
    LOG_INFO("连续采集已停止");
}

void VisionEngine::onLiveTimer()
{
    if (!isLiveGrabbing_ || !camera_) {
        return;
    }

    // 软触发模式
    if (camera_->getConfig().triggerMode == HAL::ICamera::Software) {
        camera_->trigger();
    }

    Base::ImageData::Ptr image = camera_->grabImage(100);
    if (image) {
        applyImageTransform(image);
        currentImage_ = image;
        emit liveFrameArrived(image);
    }
}

// ============== 图像管理 ==============

void VisionEngine::setCurrentImage(Base::ImageData::Ptr image)
{
    currentImage_ = image;
    emit imageUpdated(image);
}

bool VisionEngine::loadImage(const QString& filePath)
{
    // 使用Qt读取文件（支持中文路径）
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        emit errorOccurred(QString("无法打开图像文件: %1").arg(filePath));
        return false;
    }

    QByteArray fileData = file.readAll();
    file.close();

    // 使用OpenCV解码图像数据
    std::vector<uchar> buffer(fileData.begin(), fileData.end());
    cv::Mat mat = cv::imdecode(buffer, cv::IMREAD_COLOR);

    if (mat.empty()) {
        emit errorOccurred(QString("无法解码图像文件: %1").arg(filePath));
        return false;
    }

    currentImage_ = std::make_shared<Base::ImageData>(mat);
    emit imageUpdated(currentImage_);
    emit statusMessage(QString("已加载图像: %1").arg(QFileInfo(filePath).fileName()));
    LOG_INFO(QString("加载图像: %1").arg(filePath));

    return true;
}

bool VisionEngine::saveImage(const QString& filePath)
{
    if (!currentImage_) {
        emit errorOccurred("没有可保存的图像");
        return false;
    }

    if (cv::imwrite(filePath.toStdString(), currentImage_->mat())) {
        emit statusMessage(QString("图像已保存: %1").arg(filePath));
        LOG_INFO(QString("保存图像: %1").arg(filePath));
        return true;
    } else {
        emit errorOccurred("保存图像失败");
        return false;
    }
}

// ============== 图像序列 ==============

int VisionEngine::loadImageFolder(const QString& folderPath)
{
    QDir dir(folderPath);
    QStringList filters;
    filters << "*.png" << "*.jpg" << "*.jpeg" << "*.bmp" << "*.tiff" << "*.tif";
    QStringList files = dir.entryList(filters, QDir::Files, QDir::Name);

    if (files.isEmpty()) {
        emit errorOccurred("所选文件夹中没有找到图片文件");
        return 0;
    }

    // 构建完整路径列表
    imageSequence_.clear();
    for (const QString& file : files) {
        imageSequence_.append(dir.absoluteFilePath(file));
    }

    currentImageIndex_ = 0;
    loadImageAt(0);

    emit imageSequenceChanged(imageSequence_.size(), currentImageIndex_);
    emit statusMessage(QString("已加载图片文件夹: %1 (%2张图片)")
        .arg(folderPath).arg(imageSequence_.size()));
    LOG_INFO(QString("加载图片文件夹: %1, 共%2张图片")
        .arg(folderPath).arg(imageSequence_.size()));

    return imageSequence_.size();
}

bool VisionEngine::loadImageAt(int index)
{
    if (index < 0 || index >= imageSequence_.size()) {
        return false;
    }

    QString filePath = imageSequence_[index];
    if (loadImage(filePath)) {
        currentImageIndex_ = index;
        emit imageSequenceChanged(imageSequence_.size(), currentImageIndex_);

        QFileInfo fileInfo(filePath);
        emit statusMessage(QString("图片 %1/%2: %3")
            .arg(index + 1)
            .arg(imageSequence_.size())
            .arg(fileInfo.fileName()));
        return true;
    }
    return false;
}

bool VisionEngine::previousImage()
{
    if (imageSequence_.isEmpty() || currentImageIndex_ <= 0) {
        return false;
    }
    return loadImageAt(currentImageIndex_ - 1);
}

bool VisionEngine::nextImage()
{
    if (imageSequence_.isEmpty() || currentImageIndex_ >= imageSequence_.size() - 1) {
        return false;
    }
    return loadImageAt(currentImageIndex_ + 1);
}

void VisionEngine::clearImageSequence()
{
    imageSequence_.clear();
    currentImageIndex_ = -1;
    emit imageSequenceChanged(0, -1);
}

// ============== 工具链执行 ==============

ProcessResult VisionEngine::executeTool(Algorithm::VisionTool* tool)
{
    ProcessResult result;

    if (!tool) {
        result.errorMessage = "工具为空";
        return result;
    }

    if (!currentImage_) {
        result.errorMessage = "没有可处理的图像";
        return result;
    }

    if (!tool->isEnabled()) {
        result.errorMessage = "工具已禁用";
        return result;
    }

    auto startTime = std::chrono::high_resolution_clock::now();

    Algorithm::ToolResult toolResult;
    if (tool->process(currentImage_, toolResult)) {
        result.success = true;

        // 更新当前图像
        if (toolResult.outputImage) {
            currentImage_ = toolResult.outputImage;
            result.outputImage = currentImage_;
        }

        result.toolResults.append(toolResult);
    } else {
        result.errorMessage = toolResult.errorMessage;
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    result.totalTime = std::chrono::duration<double, std::milli>(endTime - startTime).count();

    if (result.success) {
        emit imageUpdated(currentImage_);
        emit statusMessage(QString("工具 \"%1\" 处理完成 (%.2f ms)")
            .arg(tool->displayName()).arg(result.totalTime));
    }

    emit processCompleted(result);
    return result;
}

ProcessResult VisionEngine::executeToolChain(const QList<Algorithm::VisionTool*>& tools)
{
    ProcessResult result;

    if (tools.isEmpty()) {
        result.errorMessage = "工具链为空";
        return result;
    }

    if (!currentImage_) {
        result.errorMessage = "没有可处理的图像";
        return result;
    }

    auto startTime = std::chrono::high_resolution_clock::now();

    // 保存原始图像用于恢复
    Base::ImageData::Ptr workingImage = currentImage_;

    result.success = true;
    for (Algorithm::VisionTool* tool : tools) {
        if (!tool || !tool->isEnabled()) {
            continue;
        }

        Algorithm::ToolResult toolResult;
        if (tool->process(workingImage, toolResult)) {
            result.toolResults.append(toolResult);

            // 更新工作图像
            if (toolResult.outputImage) {
                workingImage = toolResult.outputImage;
            }
        } else {
            result.success = false;
            result.errorMessage = QString("工具 \"%1\" 处理失败: %2")
                .arg(tool->displayName())
                .arg(toolResult.errorMessage);
            break;
        }
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    result.totalTime = std::chrono::duration<double, std::milli>(endTime - startTime).count();

    if (result.success) {
        currentImage_ = workingImage;
        result.outputImage = currentImage_;
        emit imageUpdated(currentImage_);
        emit statusMessage(QString("工具链处理完成 (%.2f ms)").arg(result.totalTime));
    }

    emit processCompleted(result);
    return result;
}

ProcessResult VisionEngine::processCurrentImage(const QList<Algorithm::VisionTool*>& tools)
{
    return executeToolChain(tools);
}

// ============== 图像变换 ==============

void VisionEngine::applyImageTransform(Base::ImageData::Ptr& image)
{
    if (!image) return;

    auto& config = Base::ConfigManager::instance();

    // 获取图像变换配置
    int rotation = config.getValue("image/rotation", 0).toInt();
    bool flipH = config.getValue("image/flipHorizontal", false).toBool();
    bool flipV = config.getValue("image/flipVertical", false).toBool();

    cv::Mat mat = image->mat();

    // 旋转
    if (rotation != 0) {
        cv::Mat rotated;
        if (rotation == 90) {
            cv::rotate(mat, rotated, cv::ROTATE_90_CLOCKWISE);
        } else if (rotation == 180) {
            cv::rotate(mat, rotated, cv::ROTATE_180);
        } else if (rotation == 270) {
            cv::rotate(mat, rotated, cv::ROTATE_90_COUNTERCLOCKWISE);
        }
        if (!rotated.empty()) {
            mat = rotated;
        }
    }

    // 镜像
    if (flipH && flipV) {
        cv::flip(mat, mat, -1);  // 同时水平和垂直翻转
    } else if (flipH) {
        cv::flip(mat, mat, 1);   // 水平翻转
    } else if (flipV) {
        cv::flip(mat, mat, 0);   // 垂直翻转
    }

    // 更新图像
    if (rotation != 0 || flipH || flipV) {
        image = std::make_shared<Base::ImageData>(mat);
    }
}

} // namespace Core
} // namespace VisionForge
