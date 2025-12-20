/**
 * @file GPIOController.cpp
 * @brief GPIO控制器实现
 * @author VisionForge Team
 * @date 2025-12-20
 */

#include "hal/GPIOController.h"
#include "base/Logger.h"
#include <QElapsedTimer>
#include <QThread>
#include <chrono>
#include <thread>

#ifdef Q_OS_WIN
#include <windows.h>
#else
#include <unistd.h>
#include <sys/time.h>
#endif

namespace VisionForge {
namespace HAL {

// ============================================================
// GPIOController 基类实现
// ============================================================

GPIOController::GPIOController(QObject* parent)
    : QObject(parent)
    , isOpen_(false)
    , handle_(nullptr)
{
}

GPIOController::~GPIOController()
{
    if (isOpen_) {
        close();
    }
}

QList<GPIODeviceInfo> GPIOController::enumerateDevices()
{
    QList<GPIODeviceInfo> devices;

    // 枚举FTDI设备
    // TODO: 实现FTDI设备枚举

    // 枚举CH340设备
    // TODO: 实现CH340设备枚举

    // 枚举并口
#ifdef Q_OS_WIN
    // Windows: 检查LPT端口
    for (int i = 1; i <= 3; ++i) {
        GPIODeviceInfo info;
        info.devicePath = QString("LPT%1").arg(i);
        info.description = QString("Parallel Port %1").arg(i);
        info.type = GPIODeviceType::ParallelPort;
        info.pinCount = 8;
        info.isConnected = false;  // 需要尝试打开确认
        devices.append(info);
    }
#endif

    return devices;
}

std::shared_ptr<GPIOController> GPIOController::create(GPIODeviceType type)
{
    switch (type) {
    case GPIODeviceType::FTDI:
        return std::make_shared<FTDIGPIOController>();

    case GPIODeviceType::CH340:
        return std::make_shared<CH340GPIOController>();

    case GPIODeviceType::ParallelPort:
        return std::make_shared<ParallelPortGPIO>();

    default:
        return std::make_shared<GPIOController>();
    }
}

bool GPIOController::open(const QString& devicePath)
{
    Q_UNUSED(devicePath);
    LOG_WARNING("GPIOController::open - 基类方法，需要子类实现");
    return false;
}

void GPIOController::close()
{
    isOpen_ = false;
    handle_ = nullptr;
    emit connectionChanged(false);
}

bool GPIOController::isOpen() const
{
    return isOpen_;
}

bool GPIOController::setPinMode(int pin, GPIOPinMode mode)
{
    QMutexLocker locker(&mutex_);
    pinModes_[pin] = mode;
    return true;
}

GPIOPinMode GPIOController::getPinMode(int pin) const
{
    QMutexLocker locker(&mutex_);
    return pinModes_.value(pin, GPIOPinMode::Output);
}

bool GPIOController::setPinsMode(const QList<int>& pins, GPIOPinMode mode)
{
    for (int pin : pins) {
        if (!setPinMode(pin, mode)) {
            return false;
        }
    }
    return true;
}

bool GPIOController::writePin(int pin, GPIOLevel level)
{
    Q_UNUSED(pin);
    Q_UNUSED(level);
    LOG_WARNING("GPIOController::writePin - 基类方法，需要子类实现");
    return false;
}

GPIOLevel GPIOController::readPin(int pin)
{
    Q_UNUSED(pin);
    LOG_WARNING("GPIOController::readPin - 基类方法，需要子类实现");
    return GPIOLevel::Low;
}

bool GPIOController::writePins(const QMap<int, GPIOLevel>& pinValues)
{
    for (auto it = pinValues.begin(); it != pinValues.end(); ++it) {
        if (!writePin(it.key(), it.value())) {
            return false;
        }
    }
    return true;
}

QMap<int, GPIOLevel> GPIOController::readPins(const QList<int>& pins)
{
    QMap<int, GPIOLevel> result;
    for (int pin : pins) {
        result[pin] = readPin(pin);
    }
    return result;
}

bool GPIOController::togglePin(int pin)
{
    GPIOLevel current = readPin(pin);
    return writePin(pin, current == GPIOLevel::Low ? GPIOLevel::High : GPIOLevel::Low);
}

bool GPIOController::outputPulse(int pin, int widthUs, bool activeHigh)
{
    GPIOLevel active = activeHigh ? GPIOLevel::High : GPIOLevel::Low;
    GPIOLevel inactive = activeHigh ? GPIOLevel::Low : GPIOLevel::High;

    // 输出脉冲
    if (!writePin(pin, active)) {
        return false;
    }

    // 精确延时
    preciseDelayUs(widthUs);

    // 恢复
    return writePin(pin, inactive);
}

bool GPIOController::outputPulseSequence(const GPIOPulseConfig& config)
{
    int count = config.repeatCount > 0 ? config.repeatCount : 1;

    for (int i = 0; i < count; ++i) {
        if (!outputPulse(config.pinNumber, config.pulseWidthUs, config.activeHigh)) {
            return false;
        }

        if (i < count - 1 && config.intervalUs > 0) {
            preciseDelayUs(config.intervalUs);
        }
    }

    return true;
}

bool GPIOController::outputPulseAsync(int pin, int widthUs, bool activeHigh)
{
    // 在新线程中执行脉冲输出
    QThread* thread = QThread::create([this, pin, widthUs, activeHigh]() {
        outputPulse(pin, widthUs, activeHigh);
    });

    thread->start();
    return true;
}

bool GPIOController::sendSyncTrigger(const QList<int>& pins, int widthUs)
{
    if (pins.isEmpty()) {
        return false;
    }

    // 同时设置所有引脚为高
    QMap<int, GPIOLevel> highValues;
    for (int pin : pins) {
        highValues[pin] = GPIOLevel::High;
    }

    if (!writePins(highValues)) {
        return false;
    }

    // 精确延时
    preciseDelayUs(widthUs);

    // 同时设置所有引脚为低
    QMap<int, GPIOLevel> lowValues;
    for (int pin : pins) {
        lowValues[pin] = GPIOLevel::Low;
    }

    return writePins(lowValues);
}

bool GPIOController::waitForTrigger(int pin, bool edge, int timeoutMs)
{
    QElapsedTimer timer;
    timer.start();

    GPIOLevel targetLevel = edge ? GPIOLevel::High : GPIOLevel::Low;
    GPIOLevel lastLevel = readPin(pin);

    while (timer.elapsed() < timeoutMs) {
        GPIOLevel currentLevel = readPin(pin);

        // 检测边沿
        if (lastLevel != targetLevel && currentLevel == targetLevel) {
            emit triggerReceived(pin);
            return true;
        }

        lastLevel = currentLevel;

        // 短暂睡眠避免CPU占用过高
        QThread::usleep(10);
    }

    return false;
}

void GPIOController::delayMicroseconds(int us)
{
    if (us <= 0) return;

#ifdef Q_OS_WIN
    // Windows高精度延时
    LARGE_INTEGER frequency, start, end;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);

    double ticksPerUs = static_cast<double>(frequency.QuadPart) / 1000000.0;
    int64_t targetTicks = static_cast<int64_t>(us * ticksPerUs);

    do {
        QueryPerformanceCounter(&end);
    } while ((end.QuadPart - start.QuadPart) < targetTicks);
#else
    // Linux/Unix
    if (us >= 1000) {
        usleep(us);
    } else {
        // 对于小于1ms的延时，使用忙等待
        auto start = std::chrono::high_resolution_clock::now();
        auto end = start + std::chrono::microseconds(us);
        while (std::chrono::high_resolution_clock::now() < end) {
            // 忙等待
        }
    }
#endif
}

void GPIOController::preciseDelayUs(int us)
{
    delayMicroseconds(us);
}

// ============================================================
// FTDIGPIOController 实现
// ============================================================

FTDIGPIOController::FTDIGPIOController(QObject* parent)
    : GPIOController(parent)
    , ftdiHandle_(nullptr)
{
    deviceInfo_.type = GPIODeviceType::FTDI;
    deviceInfo_.pinCount = 8;
}

FTDIGPIOController::~FTDIGPIOController()
{
    close();
}

bool FTDIGPIOController::open(const QString& devicePath)
{
    Q_UNUSED(devicePath);
    // TODO: 实现FTDI设备打开
    // 需要链接FTDI D2XX库

    LOG_WARNING("FTDIGPIOController::open - FTDI支持需要D2XX库");
    return false;
}

void FTDIGPIOController::close()
{
    if (ftdiHandle_) {
        // TODO: 关闭FTDI设备
        ftdiHandle_ = nullptr;
    }
    isOpen_ = false;
    emit connectionChanged(false);
}

bool FTDIGPIOController::isOpen() const
{
    return ftdiHandle_ != nullptr;
}

bool FTDIGPIOController::setPinMode(int pin, GPIOPinMode mode)
{
    if (!isOpen()) return false;

    // TODO: 设置FTDI引脚方向
    GPIOController::setPinMode(pin, mode);
    return true;
}

bool FTDIGPIOController::writePin(int pin, GPIOLevel level)
{
    if (!isOpen()) return false;

    // TODO: 实现FTDI引脚写入
    Q_UNUSED(pin);
    Q_UNUSED(level);
    return false;
}

GPIOLevel FTDIGPIOController::readPin(int pin)
{
    if (!isOpen()) return GPIOLevel::Low;

    // TODO: 实现FTDI引脚读取
    Q_UNUSED(pin);
    return GPIOLevel::Low;
}

bool FTDIGPIOController::outputPulse(int pin, int widthUs, bool activeHigh)
{
    if (!isOpen()) return false;

    return GPIOController::outputPulse(pin, widthUs, activeHigh);
}

// ============================================================
// CH340GPIOController 实现
// ============================================================

CH340GPIOController::CH340GPIOController(QObject* parent)
    : GPIOController(parent)
    , ch340Handle_(nullptr)
{
    deviceInfo_.type = GPIODeviceType::CH340;
    deviceInfo_.pinCount = 4;
}

CH340GPIOController::~CH340GPIOController()
{
    close();
}

bool CH340GPIOController::open(const QString& devicePath)
{
    Q_UNUSED(devicePath);
    // TODO: 实现CH340设备打开
    // 需要链接CH340库

    LOG_WARNING("CH340GPIOController::open - CH340支持需要相应驱动库");
    return false;
}

void CH340GPIOController::close()
{
    if (ch340Handle_) {
        // TODO: 关闭CH340设备
        ch340Handle_ = nullptr;
    }
    isOpen_ = false;
    emit connectionChanged(false);
}

bool CH340GPIOController::isOpen() const
{
    return ch340Handle_ != nullptr;
}

bool CH340GPIOController::setPinMode(int pin, GPIOPinMode mode)
{
    if (!isOpen()) return false;

    GPIOController::setPinMode(pin, mode);
    return true;
}

bool CH340GPIOController::writePin(int pin, GPIOLevel level)
{
    if (!isOpen()) return false;

    Q_UNUSED(pin);
    Q_UNUSED(level);
    return false;
}

GPIOLevel CH340GPIOController::readPin(int pin)
{
    if (!isOpen()) return GPIOLevel::Low;

    Q_UNUSED(pin);
    return GPIOLevel::Low;
}

bool CH340GPIOController::outputPulse(int pin, int widthUs, bool activeHigh)
{
    if (!isOpen()) return false;

    return GPIOController::outputPulse(pin, widthUs, activeHigh);
}

// ============================================================
// ParallelPortGPIO 实现
// ============================================================

ParallelPortGPIO::ParallelPortGPIO(QObject* parent)
    : GPIOController(parent)
    , portHandle_(-1)
    , portAddress_(0x378)  // 默认LPT1地址
{
    deviceInfo_.type = GPIODeviceType::ParallelPort;
    deviceInfo_.pinCount = 8;
}

ParallelPortGPIO::~ParallelPortGPIO()
{
    close();
}

bool ParallelPortGPIO::open(const QString& devicePath)
{
#ifdef Q_OS_WIN
    // Windows: 使用inpout32.dll或WinIO
    if (devicePath == "LPT1") {
        portAddress_ = 0x378;
    } else if (devicePath == "LPT2") {
        portAddress_ = 0x278;
    } else if (devicePath == "LPT3") {
        portAddress_ = 0x3BC;
    }

    // TODO: 加载inpout32.dll并获取端口访问权限
    LOG_WARNING("ParallelPortGPIO::open - 并口支持需要inpout32.dll");

    deviceInfo_.devicePath = devicePath;
    isOpen_ = false;  // 暂时返回失败
    return false;
#else
    Q_UNUSED(devicePath);
    LOG_WARNING("ParallelPortGPIO: 仅支持Windows平台");
    return false;
#endif
}

void ParallelPortGPIO::close()
{
    portHandle_ = -1;
    isOpen_ = false;
    emit connectionChanged(false);
}

bool ParallelPortGPIO::isOpen() const
{
    return isOpen_;
}

bool ParallelPortGPIO::writePin(int pin, GPIOLevel level)
{
    if (!isOpen() || pin < 0 || pin >= 8) return false;

    QMutexLocker locker(&mutex_);

    // 读取当前状态
    uint8_t data = readData();

    // 设置或清除对应位
    if (level == GPIOLevel::High) {
        data |= (1 << pin);
    } else {
        data &= ~(1 << pin);
    }

    // 写入
    return writeData(data);
}

GPIOLevel ParallelPortGPIO::readPin(int pin)
{
    if (!isOpen() || pin < 0 || pin >= 8) return GPIOLevel::Low;

    QMutexLocker locker(&mutex_);

    uint8_t data = readData();
    return (data & (1 << pin)) ? GPIOLevel::High : GPIOLevel::Low;
}

bool ParallelPortGPIO::outputPulse(int pin, int widthUs, bool activeHigh)
{
    if (!isOpen()) return false;

    return GPIOController::outputPulse(pin, widthUs, activeHigh);
}

bool ParallelPortGPIO::writeData(uint8_t data)
{
#ifdef Q_OS_WIN
    // TODO: 使用inpout32.dll的Out32函数
    // Out32(portAddress_, data);
    Q_UNUSED(data);
    return false;
#else
    Q_UNUSED(data);
    return false;
#endif
}

uint8_t ParallelPortGPIO::readData()
{
#ifdef Q_OS_WIN
    // TODO: 使用inpout32.dll的Inp32函数
    // return Inp32(portAddress_);
    return 0;
#else
    return 0;
#endif
}

} // namespace HAL
} // namespace VisionForge
