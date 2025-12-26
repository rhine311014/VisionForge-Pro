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
#include <QThread>
#include <QElapsedTimer>
#include <QMutexLocker>
#include <opencv2/opencv.hpp>
#include <QtConcurrent>
#include <chrono>
#include <algorithm>

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
    , asyncCancelRequested_(false)
{
    liveTimer_ = new QTimer(this);
    connect(liveTimer_, &QTimer::timeout, this, &VisionEngine::onLiveTimer);

    LOG_INFO("VisionEngine 初始化完成");
}

VisionEngine::~VisionEngine()
{
    // 取消并等待所有异步任务完成
    cancelAsyncTasks();
    waitForAsyncTasks(5000);  // 最多等待5秒

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

void VisionEngine::executeToolAsync(Algorithm::VisionTool* tool)
{
    if (!tool || !currentImage_) {
        emit errorOccurred("无法执行工具：工具为空或无图像");
        return;
    }

    // 清理已完成的任务
    cleanupFinishedTasks();

    // 重置取消标志
    asyncCancelRequested_.store(false);

    // 捕获当前图像和工具指针
    // 注意：ImageData是线程安全的（只要不修改），VisionTool需要确保在执行期间不被修改
    Base::ImageData::Ptr inputImage = currentImage_;
    QString toolName = tool->displayName();  // 提前保存工具名称

    QFuture<void> future = QtConcurrent::run([this, tool, inputImage, toolName]() {
        ProcessResult result;

        // 检查是否已取消
        if (asyncCancelRequested_.load()) {
            result.errorMessage = "任务已取消";
            return;
        }

        // 模拟executeTool的逻辑，但使用捕获的图像
        if (!tool->isEnabled()) {
            result.errorMessage = "工具已禁用";
        } else {
            auto startTime = std::chrono::high_resolution_clock::now();
            Algorithm::ToolResult toolResult;

            try {
                if (tool->process(inputImage, toolResult)) {
                    result.success = true;
                    result.outputImage = toolResult.outputImage;
                    result.toolResults.append(toolResult);
                } else {
                    result.errorMessage = toolResult.errorMessage;
                }
            } catch (const std::exception& e) {
                result.errorMessage = QString("工具执行异常: %1").arg(e.what());
            } catch (...) {
                result.errorMessage = "工具执行发生未知异常";
            }

            auto endTime = std::chrono::high_resolution_clock::now();
            result.totalTime = std::chrono::duration<double, std::milli>(endTime - startTime).count();
        }

        // 再次检查是否取消（任务完成后）
        if (asyncCancelRequested_.load()) {
            return;  // 取消时不发送结果
        }

        // 回到主线程更新状态
        QMetaObject::invokeMethod(this, [this, result, toolName]() {
            if (result.success) {
                if (result.outputImage) {
                    setCurrentImage(result.outputImage);
                }
                emit statusMessage(QString("工具 \"%1\" 异步处理完成 (%.2f ms)")
                    .arg(toolName).arg(result.totalTime));
            } else {
                emit errorOccurred(QString("工具 \"%1\" 处理失败: %2")
                    .arg(toolName).arg(result.errorMessage));
            }
            emit processCompleted(result);
        });
    });

    // 记录任务
    {
        QMutexLocker locker(&asyncMutex_);
        asyncTasks_.append(future);
    }
}

void VisionEngine::executeToolChainAsync(const QList<Algorithm::VisionTool*>& tools)
{
    if (tools.isEmpty() || !currentImage_) {
        emit errorOccurred("无法执行工具链：工具链为空或无图像");
        return;
    }

    // 清理已完成的任务
    cleanupFinishedTasks();

    // 重置取消标志
    asyncCancelRequested_.store(false);

    Base::ImageData::Ptr inputImage = currentImage_;

    QFuture<void> future = QtConcurrent::run([this, tools, inputImage]() {
        ProcessResult result;
        result.success = true;

        // 检查是否已取消
        if (asyncCancelRequested_.load()) {
            return;
        }

        auto startTime = std::chrono::high_resolution_clock::now();
        Base::ImageData::Ptr workingImage = inputImage;

        for (Algorithm::VisionTool* tool : tools) {
            // 在每个工具执行前检查取消标志
            if (asyncCancelRequested_.load()) {
                result.success = false;
                result.errorMessage = "任务已取消";
                break;
            }

            if (!tool || !tool->isEnabled()) continue;

            Algorithm::ToolResult toolResult;
            try {
                if (tool->process(workingImage, toolResult)) {
                    result.toolResults.append(toolResult);
                    if (toolResult.outputImage) {
                        workingImage = toolResult.outputImage;
                    }
                } else {
                    result.success = false;
                    result.errorMessage = QString("工具 \"%1\" 处理失败: %2")
                        .arg(tool->displayName()).arg(toolResult.errorMessage);
                    break;
                }
            } catch (const std::exception& e) {
                result.success = false;
                result.errorMessage = QString("工具 \"%1\" 执行异常: %2")
                    .arg(tool->displayName()).arg(e.what());
                break;
            }
        }

        auto endTime = std::chrono::high_resolution_clock::now();
        result.totalTime = std::chrono::duration<double, std::milli>(endTime - startTime).count();

        if (result.success) {
            result.outputImage = workingImage;
        }

        // 取消时不发送结果
        if (asyncCancelRequested_.load()) {
            return;
        }

        QMetaObject::invokeMethod(this, [this, result]() {
            if (result.success) {
                setCurrentImage(result.outputImage);
                emit statusMessage(QString("工具链异步处理完成 (%.2f ms)").arg(result.totalTime));
            } else {
                emit errorOccurred(result.errorMessage);
            }
            emit processCompleted(result);
        });
    });

    // 记录任务
    {
        QMutexLocker locker(&asyncMutex_);
        asyncTasks_.append(future);
    }
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

// ============== 异步任务管理 ==============

void VisionEngine::cancelAsyncTasks()
{
    asyncCancelRequested_.store(true);
    LOG_DEBUG("请求取消所有异步任务");
}

bool VisionEngine::waitForAsyncTasks(int timeoutMs)
{
    QMutexLocker locker(&asyncMutex_);

    for (QFuture<void>& future : asyncTasks_) {
        if (!future.isFinished()) {
            if (timeoutMs < 0) {
                future.waitForFinished();
            } else {
                // Qt 6 中 QFuture 没有带超时的 wait，使用轮询
                QElapsedTimer timer;
                timer.start();
                while (!future.isFinished() && timer.elapsed() < timeoutMs) {
                    locker.unlock();
                    QThread::msleep(10);
                    locker.relock();
                }
                if (!future.isFinished()) {
                    LOG_WARNING("等待异步任务超时");
                    return false;
                }
            }
        }
    }

    asyncTasks_.clear();
    return true;
}

bool VisionEngine::hasRunningAsyncTasks() const
{
    QMutexLocker locker(&asyncMutex_);
    for (const QFuture<void>& future : asyncTasks_) {
        if (!future.isFinished()) {
            return true;
        }
    }
    return false;
}

void VisionEngine::cleanupFinishedTasks()
{
    QMutexLocker locker(&asyncMutex_);
    asyncTasks_.erase(
        std::remove_if(asyncTasks_.begin(), asyncTasks_.end(),
            [](const QFuture<void>& f) { return f.isFinished(); }),
        asyncTasks_.end()
    );
}

} // namespace Core
} // namespace VisionForge
