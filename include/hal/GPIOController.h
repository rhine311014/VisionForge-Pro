/**
 * @file GPIOController.h
 * @brief GPIO控制器 - 用于硬件触发信号输出
 * @details 支持USB GPIO模块(FTDI、CH340等)和并口控制
 * @author VisionForge Team
 * @date 2025-12-20
 */

#pragma once

#include <QObject>
#include <QString>
#include <QMutex>
#include <QThread>
#include <QMap>
#include <QList>
#include <memory>

namespace VisionForge {
namespace HAL {

/**
 * @brief GPIO设备类型
 */
enum class GPIODeviceType {
    FTDI,           ///< FTDI USB转GPIO
    CH340,          ///< CH340 USB转GPIO
    ParallelPort,   ///< 并口GPIO
    ModbusDIO,      ///< Modbus数字IO
    Custom          ///< 自定义设备
};

/**
 * @brief GPIO引脚模式
 */
enum class GPIOPinMode {
    Input,          ///< 输入模式
    Output,         ///< 输出模式
    PullUp,         ///< 上拉输入
    PullDown        ///< 下拉输入
};

/**
 * @brief GPIO引脚电平
 */
enum class GPIOLevel {
    Low = 0,        ///< 低电平
    High = 1        ///< 高电平
};

/**
 * @brief GPIO设备信息
 */
struct GPIODeviceInfo {
    QString devicePath;         ///< 设备路径
    QString description;        ///< 设备描述
    GPIODeviceType type;        ///< 设备类型
    int pinCount;               ///< 可用引脚数量
    QString serialNumber;       ///< 序列号
    bool isConnected;           ///< 是否已连接

    GPIODeviceInfo()
        : type(GPIODeviceType::Custom), pinCount(0), isConnected(false) {}
};

/**
 * @brief GPIO脉冲参数
 */
struct GPIOPulseConfig {
    int pinNumber;              ///< 引脚号
    int pulseWidthUs;           ///< 脉冲宽度(微秒)
    bool activeHigh;            ///< 高电平有效
    int repeatCount;            ///< 重复次数(0=单次)
    int intervalUs;             ///< 重复间隔(微秒)

    GPIOPulseConfig()
        : pinNumber(0), pulseWidthUs(100), activeHigh(true),
          repeatCount(0), intervalUs(0) {}
};

/**
 * @class GPIOController
 * @brief GPIO控制器类
 *
 * 提供统一的GPIO控制接口，用于：
 * - 输出触发脉冲信号
 * - 读取外部触发输入
 * - 控制光源/气缸等设备
 */
class GPIOController : public QObject
{
    Q_OBJECT

public:
    explicit GPIOController(QObject* parent = nullptr);
    ~GPIOController() override;

    // ========== 静态方法 ==========

    /**
     * @brief 枚举所有GPIO设备
     */
    static QList<GPIODeviceInfo> enumerateDevices();

    /**
     * @brief 创建GPIO控制器实例
     * @param type 设备类型
     * @return 控制器实例
     */
    static std::shared_ptr<GPIOController> create(GPIODeviceType type);

    // ========== 设备管理 ==========

    /**
     * @brief 打开设备
     * @param devicePath 设备路径
     * @return 是否成功
     */
    virtual bool open(const QString& devicePath);

    /**
     * @brief 关闭设备
     */
    virtual void close();

    /**
     * @brief 检查设备是否打开
     */
    virtual bool isOpen() const;

    /**
     * @brief 获取设备信息
     */
    GPIODeviceInfo deviceInfo() const { return deviceInfo_; }

    // ========== 引脚配置 ==========

    /**
     * @brief 设置引脚模式
     * @param pin 引脚号
     * @param mode 模式
     * @return 是否成功
     */
    virtual bool setPinMode(int pin, GPIOPinMode mode);

    /**
     * @brief 获取引脚模式
     */
    virtual GPIOPinMode getPinMode(int pin) const;

    /**
     * @brief 设置多个引脚模式
     * @param pins 引脚列表
     * @param mode 模式
     */
    virtual bool setPinsMode(const QList<int>& pins, GPIOPinMode mode);

    // ========== 引脚操作 ==========

    /**
     * @brief 写入引脚电平
     * @param pin 引脚号
     * @param level 电平
     * @return 是否成功
     */
    virtual bool writePin(int pin, GPIOLevel level);

    /**
     * @brief 读取引脚电平
     * @param pin 引脚号
     * @return 电平
     */
    virtual GPIOLevel readPin(int pin);

    /**
     * @brief 写入多个引脚
     * @param pinValues 引脚值映射
     */
    virtual bool writePins(const QMap<int, GPIOLevel>& pinValues);

    /**
     * @brief 读取多个引脚
     * @param pins 引脚列表
     * @return 引脚值映射
     */
    virtual QMap<int, GPIOLevel> readPins(const QList<int>& pins);

    /**
     * @brief 切换引脚电平
     * @param pin 引脚号
     */
    virtual bool togglePin(int pin);

    // ========== 脉冲输出 ==========

    /**
     * @brief 输出单个脉冲
     * @param pin 引脚号
     * @param widthUs 脉冲宽度(微秒)
     * @param activeHigh 高电平有效
     * @return 是否成功
     */
    virtual bool outputPulse(int pin, int widthUs, bool activeHigh = true);

    /**
     * @brief 输出脉冲序列
     * @param config 脉冲配置
     * @return 是否成功
     */
    virtual bool outputPulseSequence(const GPIOPulseConfig& config);

    /**
     * @brief 异步输出脉冲（非阻塞）
     */
    virtual bool outputPulseAsync(int pin, int widthUs, bool activeHigh = true);

    // ========== 触发同步 ==========

    /**
     * @brief 发送同步触发信号
     * @param pins 触发引脚列表
     * @param widthUs 脉冲宽度
     * @return 是否成功
     */
    virtual bool sendSyncTrigger(const QList<int>& pins, int widthUs = 100);

    /**
     * @brief 等待外部触发
     * @param pin 输入引脚
     * @param edge 边沿触发(true=上升沿, false=下降沿)
     * @param timeoutMs 超时时间
     * @return 是否收到触发
     */
    virtual bool waitForTrigger(int pin, bool edge = true, int timeoutMs = 5000);

    // ========== 高精度延时 ==========

    /**
     * @brief 高精度微秒延时
     * @param us 延时微秒数
     */
    static void delayMicroseconds(int us);

signals:
    /**
     * @brief 设备连接状态改变
     */
    void connectionChanged(bool connected);

    /**
     * @brief 引脚电平改变（输入模式）
     */
    void pinChanged(int pin, GPIOLevel level);

    /**
     * @brief 外部触发接收
     */
    void triggerReceived(int pin);

    /**
     * @brief 错误发生
     */
    void errorOccurred(const QString& error);

protected:
    /**
     * @brief 微秒级精确延时（平台相关实现）
     */
    void preciseDelayUs(int us);

protected:
    GPIODeviceInfo deviceInfo_;     ///< 设备信息
    bool isOpen_;                   ///< 是否已打开
    mutable QMutex mutex_;          ///< 互斥锁
    QMap<int, GPIOPinMode> pinModes_;   ///< 引脚模式
    QMap<int, GPIOLevel> pinStates_;    ///< 引脚状态

    void* handle_;                  ///< 设备句柄（平台相关）
};

/**
 * @class FTDIGPIOController
 * @brief FTDI USB GPIO控制器
 */
class FTDIGPIOController : public GPIOController
{
    Q_OBJECT

public:
    explicit FTDIGPIOController(QObject* parent = nullptr);
    ~FTDIGPIOController() override;

    bool open(const QString& devicePath) override;
    void close() override;
    bool isOpen() const override;

    bool setPinMode(int pin, GPIOPinMode mode) override;
    bool writePin(int pin, GPIOLevel level) override;
    GPIOLevel readPin(int pin) override;
    bool outputPulse(int pin, int widthUs, bool activeHigh = true) override;

private:
    void* ftdiHandle_;
};

/**
 * @class CH340GPIOController
 * @brief CH340 USB GPIO控制器
 */
class CH340GPIOController : public GPIOController
{
    Q_OBJECT

public:
    explicit CH340GPIOController(QObject* parent = nullptr);
    ~CH340GPIOController() override;

    bool open(const QString& devicePath) override;
    void close() override;
    bool isOpen() const override;

    bool setPinMode(int pin, GPIOPinMode mode) override;
    bool writePin(int pin, GPIOLevel level) override;
    GPIOLevel readPin(int pin) override;
    bool outputPulse(int pin, int widthUs, bool activeHigh = true) override;

private:
    void* ch340Handle_;
};

/**
 * @class ParallelPortGPIO
 * @brief 并口GPIO控制器
 */
class ParallelPortGPIO : public GPIOController
{
    Q_OBJECT

public:
    explicit ParallelPortGPIO(QObject* parent = nullptr);
    ~ParallelPortGPIO() override;

    bool open(const QString& devicePath) override;
    void close() override;
    bool isOpen() const override;

    bool writePin(int pin, GPIOLevel level) override;
    GPIOLevel readPin(int pin) override;
    bool outputPulse(int pin, int widthUs, bool activeHigh = true) override;

    /**
     * @brief 直接写入并口数据
     * @param data 8位数据
     */
    bool writeData(uint8_t data);

    /**
     * @brief 读取并口数据
     */
    uint8_t readData();

private:
    int portHandle_;
    uint16_t portAddress_;
};

} // namespace HAL
} // namespace VisionForge
