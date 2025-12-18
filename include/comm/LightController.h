/**
 * @file LightController.h
 * @brief 光源控制器
 * @details 通用串口协议光源控制器，支持亮度控制、多通道、触发模式
 * @author VisionForge Team
 * @date 2025-12-18
 */

#pragma once

#include <QObject>
#include <QSerialPort>
#include <QString>
#include <QStringList>
#include <QMutex>
#include <memory>
#include <vector>

namespace VisionForge {
namespace Comm {

/**
 * @brief 光源通道状态
 */
struct LightChannel {
    int channelId = 0;          // 通道号 (0-based)
    int brightness = 0;         // 亮度值 (0-255)
    bool enabled = false;       // 是否启用

    LightChannel() = default;
    LightChannel(int id, int bri = 0, bool en = false)
        : channelId(id), brightness(bri), enabled(en) {}
};

/**
 * @brief 光源触发模式
 */
enum class LightTriggerMode {
    Continuous,     // 常亮模式
    Flash,          // 闪光模式（触发后短暂亮起）
    Strobe          // 频闪模式
};

/**
 * @brief 光源控制器配置
 */
struct LightControllerConfig {
    QString portName;           // 串口名
    int baudRate = 9600;        // 波特率
    int dataBits = 8;           // 数据位
    int stopBits = 1;           // 停止位
    int parity = 0;             // 校验 (0=无, 1=奇, 2=偶)
    int channelCount = 4;       // 通道数量
    int timeout = 1000;         // 超时时间(ms)
};

/**
 * @class LightController
 * @brief 通用光源控制器
 *
 * 支持的功能:
 * - 多通道亮度控制 (0-255)
 * - 触发模式切换 (常亮/闪光/频闪)
 * - 通道使能控制
 * - 可扩展的协议框架
 *
 * 默认协议格式 (可通过子类重写):
 * - 设置亮度: "CH<通道号>B<亮度>\r\n"
 * - 全局开: "ON\r\n"
 * - 全局关: "OFF\r\n"
 * - 读取状态: "STATUS\r\n"
 */
class LightController : public QObject
{
    Q_OBJECT

public:
    explicit LightController(QObject* parent = nullptr);
    virtual ~LightController();

    /**
     * @brief 连接光源控制器
     * @param config 控制器配置
     * @return 是否连接成功
     */
    virtual bool connect(const LightControllerConfig& config);

    /**
     * @brief 断开连接
     */
    virtual void disconnect();

    /**
     * @brief 是否已连接
     */
    bool isConnected() const;

    // ========== 亮度控制 ==========

    /**
     * @brief 设置单通道亮度
     * @param channel 通道号 (0-based)
     * @param brightness 亮度值 (0-255)
     * @return 是否成功
     */
    virtual bool setBrightness(int channel, int brightness);

    /**
     * @brief 获取单通道亮度
     * @param channel 通道号
     * @return 亮度值
     */
    int getBrightness(int channel) const;

    /**
     * @brief 设置所有通道亮度
     * @param brightness 亮度值 (0-255)
     * @return 是否成功
     */
    virtual bool setAllBrightness(int brightness);

    /**
     * @brief 设置多通道亮度
     * @param brightnesses 各通道亮度值
     * @return 是否成功
     */
    virtual bool setMultiBrightness(const std::vector<int>& brightnesses);

    // ========== 通道控制 ==========

    /**
     * @brief 启用/禁用单通道
     * @param channel 通道号
     * @param enabled 是否启用
     * @return 是否成功
     */
    virtual bool setChannelEnabled(int channel, bool enabled);

    /**
     * @brief 启用所有通道
     * @return 是否成功
     */
    virtual bool enableAll();

    /**
     * @brief 禁用所有通道
     * @return 是否成功
     */
    virtual bool disableAll();

    /**
     * @brief 获取通道状态
     * @param channel 通道号
     * @return 通道状态
     */
    LightChannel getChannelStatus(int channel) const;

    /**
     * @brief 获取所有通道状态
     * @return 通道状态列表
     */
    std::vector<LightChannel> getAllChannelStatus() const;

    // ========== 触发模式 ==========

    /**
     * @brief 设置触发模式
     * @param mode 触发模式
     * @return 是否成功
     */
    virtual bool setTriggerMode(LightTriggerMode mode);

    /**
     * @brief 获取当前触发模式
     */
    LightTriggerMode getTriggerMode() const { return triggerMode_; }

    /**
     * @brief 触发闪光
     * @param durationMs 闪光时间(ms)
     * @return 是否成功
     */
    virtual bool triggerFlash(int durationMs = 100);

    // ========== 配置与状态 ==========

    /**
     * @brief 获取通道数量
     */
    int getChannelCount() const { return channelCount_; }

    /**
     * @brief 获取最后错误信息
     */
    QString getLastError() const { return lastError_; }

    /**
     * @brief 获取可用串口列表
     */
    static QStringList availablePorts();

signals:
    /**
     * @brief 连接状态改变
     */
    void connectionChanged(bool connected);

    /**
     * @brief 亮度改变
     */
    void brightnessChanged(int channel, int brightness);

    /**
     * @brief 通道状态改变
     */
    void channelStatusChanged(int channel, bool enabled);

    /**
     * @brief 错误发生
     */
    void errorOccurred(const QString& error);

protected:
    /**
     * @brief 构建设置亮度命令 (可重写)
     */
    virtual QByteArray buildSetBrightnessCommand(int channel, int brightness);

    /**
     * @brief 构建全局开启命令 (可重写)
     */
    virtual QByteArray buildEnableAllCommand();

    /**
     * @brief 构建全局关闭命令 (可重写)
     */
    virtual QByteArray buildDisableAllCommand();

    /**
     * @brief 构建设置触发模式命令 (可重写)
     */
    virtual QByteArray buildSetTriggerModeCommand(LightTriggerMode mode);

    /**
     * @brief 构建触发闪光命令 (可重写)
     */
    virtual QByteArray buildTriggerFlashCommand(int durationMs);

    /**
     * @brief 发送命令并接收响应
     */
    QByteArray sendCommand(const QByteArray& command, int expectedResponseSize = 0);

    /**
     * @brief 解析响应 (可重写)
     */
    virtual bool parseResponse(const QByteArray& response);

protected:
    QSerialPort* serialPort_;
    LightControllerConfig config_;
    QString lastError_;
    int channelCount_;
    LightTriggerMode triggerMode_;
    std::vector<LightChannel> channels_;
    mutable QMutex mutex_;
};

// ========== 特定品牌光源控制器 ==========

/**
 * @class OPTLightController
 * @brief OPT光源控制器 (支持奥普特光源)
 *
 * OPT协议格式:
 * - 设置亮度: "#<通道><亮度>\r" (通道A-D, 亮度000-255)
 * - 读取亮度: "?<通道>\r"
 */
class OPTLightController : public LightController
{
    Q_OBJECT

public:
    explicit OPTLightController(QObject* parent = nullptr);

protected:
    QByteArray buildSetBrightnessCommand(int channel, int brightness) override;
    QByteArray buildEnableAllCommand() override;
    QByteArray buildDisableAllCommand() override;
};

/**
 * @class CCSLightController
 * @brief CCS光源控制器
 *
 * CCS协议格式 (示例):
 * - 设置亮度: "L<通道><亮度>\r\n"
 */
class CCSLightController : public LightController
{
    Q_OBJECT

public:
    explicit CCSLightController(QObject* parent = nullptr);

protected:
    QByteArray buildSetBrightnessCommand(int channel, int brightness) override;
};

} // namespace Comm
} // namespace VisionForge
