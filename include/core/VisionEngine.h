/**
 * @file VisionEngine.h
 * @brief 视觉引擎核心类
 * @details 负责相机管理、图像采集、工具链执行等核心业务逻辑
 * @author VisionForge Team
 * @date 2025-12-25
 */

#pragma once

#include <QObject>
#include <QTimer>
#include <QStringList>
#include <memory>
#include <vector>

#include "base/ImageData.h"
#include "hal/ICamera.h"
#include "algorithm/VisionTool.h"

namespace VisionForge {
namespace Core {

/**
 * @brief 图像处理结果
 */
struct ProcessResult {
    bool success = false;
    QString errorMessage;
    Base::ImageData::Ptr outputImage;
    QList<Algorithm::ToolResult> toolResults;
    double totalTime = 0.0;  // 总处理时间(ms)
};

/**
 * @class VisionEngine
 * @brief 视觉引擎核心类
 *
 * 职责：
 * - 相机生命周期管理
 * - 图像采集（单帧/连续）
 * - 工具链执行
 * - 图像序列管理
 *
 * 将业务逻辑从 MainWindow 中剥离，实现关注点分离
 */
class VisionEngine : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 获取单例实例
     */
    static VisionEngine& instance();

    /**
     * @brief 析构函数
     */
    ~VisionEngine() override;

    // ============== 相机管理 ==============

    /**
     * @brief 设置当前相机
     * @param camera 相机指针（转移所有权）
     */
    void setCamera(std::unique_ptr<HAL::ICamera> camera);

    /**
     * @brief 获取当前相机
     */
    HAL::ICamera* camera() const { return camera_.get(); }

    /**
     * @brief 检查相机是否就绪
     */
    bool isCameraReady() const;

    /**
     * @brief 打开相机
     */
    bool openCamera();

    /**
     * @brief 关闭相机
     */
    void closeCamera();

    // ============== 图像采集 ==============

    /**
     * @brief 单帧采集
     * @return 采集的图像，失败返回nullptr
     */
    Base::ImageData::Ptr grabSingle();

    /**
     * @brief 启动连续采集
     * @param intervalMs 采集间隔(毫秒)
     */
    bool startLiveGrab(int intervalMs = 100);

    /**
     * @brief 停止连续采集
     */
    void stopLiveGrab();

    /**
     * @brief 是否正在连续采集
     */
    bool isLiveGrabbing() const { return isLiveGrabbing_; }

    // ============== 图像管理 ==============

    /**
     * @brief 设置当前图像
     */
    void setCurrentImage(Base::ImageData::Ptr image);

    /**
     * @brief 获取当前图像
     */
    Base::ImageData::Ptr currentImage() const { return currentImage_; }

    /**
     * @brief 从文件加载图像
     * @param filePath 文件路径
     * @return 是否成功
     */
    bool loadImage(const QString& filePath);

    /**
     * @brief 保存当前图像
     * @param filePath 文件路径
     * @return 是否成功
     */
    bool saveImage(const QString& filePath);

    // ============== 图像序列 ==============

    /**
     * @brief 加载图像文件夹
     * @param folderPath 文件夹路径
     * @return 加载的图像数量
     */
    int loadImageFolder(const QString& folderPath);

    /**
     * @brief 加载序列中指定索引的图像
     */
    bool loadImageAt(int index);

    /**
     * @brief 上一张图像
     */
    bool previousImage();

    /**
     * @brief 下一张图像
     */
    bool nextImage();

    /**
     * @brief 获取图像序列大小
     */
    int imageSequenceSize() const { return imageSequence_.size(); }

    /**
     * @brief 获取图像序列数量（别名，兼容旧接口）
     */
    int getImageSequenceCount() const { return imageSequence_.size(); }

    /**
     * @brief 获取当前图像索引
     */
    int currentImageIndex() const { return currentImageIndex_; }

    /**
     * @brief 获取当前图像索引（别名，兼容旧接口）
     */
    int getCurrentImageIndex() const { return currentImageIndex_; }

    /**
     * @brief 获取图像序列列表
     */
    const QStringList& imageSequence() const { return imageSequence_; }

    /**
     * @brief 清空图像序列
     */
    void clearImageSequence();

    // ============== 工具链执行 ==============

    /**
     * @brief 执行单个工具
     * @param tool 工具指针
     * @return 处理结果
     */
    ProcessResult executeTool(Algorithm::VisionTool* tool);

    /**
     * @brief 执行工具链
     * @param tools 工具列表
     * @return 处理结果
     */
    ProcessResult executeToolChain(const QList<Algorithm::VisionTool*>& tools);

    /**
     * @brief 对当前图像执行工具链
     */
    ProcessResult processCurrentImage(const QList<Algorithm::VisionTool*>& tools);

    /**
     * @brief 异步执行单个工具
     * @param tool 工具指针
     */
    void executeToolAsync(Algorithm::VisionTool* tool);

    /**
     * @brief 异步执行工具链
     * @param tools 工具列表
     */
    void executeToolChainAsync(const QList<Algorithm::VisionTool*>& tools);

    // ============== 图像变换 ==============

    /**
     * @brief 应用图像变换（旋转、镜像等）
     * @param image 输入输出图像
     */
    void applyImageTransform(Base::ImageData::Ptr& image);

signals:
    /**
     * @brief 图像更新信号
     */
    void imageUpdated(Base::ImageData::Ptr image);

    /**
     * @brief 连续采集帧到达信号
     */
    void liveFrameArrived(Base::ImageData::Ptr image);

    /**
     * @brief 处理完成信号
     */
    void processCompleted(const ProcessResult& result);

    /**
     * @brief 相机状态变化信号
     */
    void cameraStatusChanged(bool connected);

    /**
     * @brief 图像序列变化信号
     */
    void imageSequenceChanged(int count, int currentIndex);

    /**
     * @brief 错误信号
     */
    void errorOccurred(const QString& message);

    /**
     * @brief 状态消息信号
     */
    void statusMessage(const QString& message);

private slots:
    void onLiveTimer();

private:
    VisionEngine();
    VisionEngine(const VisionEngine&) = delete;
    VisionEngine& operator=(const VisionEngine&) = delete;

    // 相机
    std::unique_ptr<HAL::ICamera> camera_;
    QTimer* liveTimer_;
    bool isLiveGrabbing_;

    // 当前图像
    Base::ImageData::Ptr currentImage_;

    // 图像序列
    QStringList imageSequence_;
    int currentImageIndex_;
};

} // namespace Core
} // namespace VisionForge
