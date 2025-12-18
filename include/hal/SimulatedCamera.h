/**
 * @file SimulatedCamera.h
 * @brief 模拟相机（用于测试和演示）
 * @author VisionForge Team
 * @date 2025-12-14
 */

#pragma once

#include "hal/ICamera.h"
#include <QStringList>
#include <QTimer>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

namespace VisionForge {
namespace HAL {

/**
 * @brief 模拟相机类
 *
 * 用于测试和演示的虚拟相机，可以：
 * - 加载图片序列模拟采集
 * - 生成测试图案
 * - 支持所有标准相机接口
 */
class SimulatedCamera : public ICamera {
    Q_OBJECT

public:
    /**
     * @brief 图像源类型
     */
    enum SourceType {
        TestPattern,    // 测试图案
        ImageFile,      // 单张图片
        ImageSequence,  // 图片序列
        VideoFile       // 视频文件
    };

    explicit SimulatedCamera(QObject* parent = nullptr);
    ~SimulatedCamera() override;

    // ========== ICamera接口实现 ==========

    bool open() override;
    void close() override;
    bool isOpen() const override { return isOpen_; }

    QString deviceName() const override { return "SimulatedCamera"; }
    QString serialNumber() const override { return "SIM-000001"; }

    bool startGrabbing() override;
    void stopGrabbing() override;
    bool isGrabbing() const override { return isGrabbing_; }

    Base::ImageData::Ptr grabImage(int timeoutMs = 1000) override;
    bool trigger() override;

    void setConfig(const Config& config) override;
    Config getConfig() const override { return config_; }

    void setExposure(double exposure) override;
    double getExposure() const override { return config_.exposure; }

    void setGain(double gain) override;
    double getGain() const override { return config_.gain; }

    void setTriggerMode(TriggerMode mode) override;
    TriggerMode getTriggerMode() const override { return config_.triggerMode; }

    // ========== 模拟相机特有功能 ==========

    /**
     * @brief 设置图像源
     * @param imagePath 图片文件路径
     */
    void setImageSource(const QString& imagePath);

    /**
     * @brief 设置图片序列
     * @param imagePaths 图片文件路径列表
     */
    void setImageSequence(const QStringList& imagePaths);

    /**
     * @brief 设置视频源
     * @param videoPath 视频文件路径
     */
    void setVideoSource(const QString& videoPath);

    /**
     * @brief 使用测试图案
     * @param patternType 0=渐变, 1=棋盘, 2=彩色条纹
     */
    void useTestPattern(int patternType = 0);

    /**
     * @brief 设置帧率（仅连续模式）
     * @param fps 帧率
     */
    void setFrameRate(double fps);

    /**
     * @brief 设置视频循环播放
     * @param enabled 是否循环
     */
    void setVideoLoopEnabled(bool enabled) { videoLoopEnabled_ = enabled; }

    /**
     * @brief 获取视频循环播放状态
     */
    bool isVideoLoopEnabled() const { return videoLoopEnabled_; }

private slots:
    void onContinuousGrab();

private:
    Base::ImageData::Ptr generateTestPattern(int patternType);
    Base::ImageData::Ptr loadNextImage();
    Base::ImageData::Ptr loadNextVideoFrame();
    cv::Mat applyGainAndExposure(const cv::Mat& input);
    bool openVideoCapture();
    void closeVideoCapture();

private:
    bool isOpen_;
    bool isGrabbing_;
    Config config_;

    // 图像源
    SourceType sourceType_;
    QString singleImagePath_;
    QStringList imageSequence_;
    QString videoPath_;
    int currentIndex_;
    int testPatternType_;

    // 视频捕获
    cv::VideoCapture videoCapture_;
    bool videoLoopEnabled_;         // 视频循环播放

    // 连续采集定时器
    QTimer* grabTimer_;
    double frameRate_;

    // 缓存的图像
    Base::ImageData::Ptr cachedImage_;
};

} // namespace HAL
} // namespace VisionForge
