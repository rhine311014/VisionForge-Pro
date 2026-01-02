/**
 * @file BaslerCamera.h
 * @brief Basler工业相机驱动封装
 * @details 基于Basler Pylon SDK实现的工业相机驱动类。
 *          支持GigE Vision和USB3 Vision接口的Basler相机，
 *          提供设备枚举、参数配置、图像采集等完整功能。
 *          遵循GenICam标准，可访问相机的所有标准和厂商特定参数。
 * @author VisionForge Team
 * @date 2025-12-17
 * @version 1.0
 *
 * @note 硬件通信协议说明：
 *       - GigE相机：使用GigE Vision协议（基于UDP），通过以太网传输
 *         - GVCP (GigE Vision Control Protocol)：设备控制，端口3956
 *         - GVSP (GigE Vision Streaming Protocol)：图像传输
 *       - USB3相机：使用USB3 Vision协议，通过USB 3.0传输
 *       - 所有相机遵循GenICam标准，使用GenApi访问参数节点
 *
 * @note 编译依赖：
 *       - 需要安装Basler Pylon SDK (推荐6.0或更高版本)
 *       - 编译时需定义宏 USE_BASLER_PYLON
 *       - 链接库：PylonBase_v6.x.lib, GenApi_MD_VC140_v3_x.lib等
 *
 * @note 此为预留框架，需要安装Basler Pylon SDK后完善实现
 */

#pragma once

#include "hal/ICamera.h"
#include <QMutex>

#ifdef USE_BASLER_PYLON

namespace VisionForge {
namespace HAL {

/**
 * @brief Basler相机设备信息结构体
 * @details 存储通过Pylon SDK枚举得到的Basler相机设备信息。
 *          用于设备选择和相机识别。
 */
struct BaslerDeviceInfo {
    QString modelName;        ///< 型号名称：如"acA1920-40gm"、"a2A1920-160ucBAS"
    QString serialNumber;     ///< 序列号：相机的唯一标识符，如"40282322"
    QString ipAddress;        ///< IP地址：仅GigE相机有效，如"192.168.1.100"
    QString interfaceType;    ///< 接口类型："GigE"(千兆以太网)或"USB3"(USB 3.0)
    QString vendorName;       ///< 厂商名称：通常为"Basler"

    /**
     * @brief 默认构造函数
     */
    BaslerDeviceInfo()
    {}
};

/**
 * @class BaslerCamera
 * @brief Basler工业相机驱动类
 * @details 继承自ICamera接口，封装Basler Pylon SDK实现完整的相机控制。
 *          支持以下功能：
 *          - 设备枚举：自动发现网络和USB连接的Basler相机
 *          - 设备选择：支持按索引、序列号、IP地址选择相机
 *          - 参数配置：曝光时间、增益、触发模式、ROI等
 *          - 图像采集：连续采集、软件触发、硬件触发
 *          - 格式转换：自动将各种像素格式转换为OpenCV Mat
 *
 * @note 硬件通信协议：
 *       相机参数通过GenApi节点访问，主要节点包括：
 *       - Width/Height/OffsetX/OffsetY：图像尺寸和ROI设置
 *       - ExposureTime：曝光时间（单位微秒）
 *       - Gain：增益（单位dB或倍数，取决于相机型号）
 *       - TriggerMode/TriggerSource：触发模式和触发源配置
 *       - TriggerSoftware：软件触发命令节点
 *
 * @note 线程安全：
 *       所有公共方法使用互斥锁保护，可安全在多线程环境使用
 *
 * @note 使用示例：
 * @code
 * // 枚举所有Basler相机
 * auto devices = BaslerCamera::enumerateDevices();
 * if (!devices.isEmpty()) {
 *     BaslerCamera camera;
 *     camera.selectBySerialNumber(devices.first().serialNumber);
 *     if (camera.open()) {
 *         camera.setExposure(5000);  // 5ms曝光
 *         camera.setTriggerMode(ICamera::Software);
 *         camera.startGrabbing();
 *         camera.trigger();
 *         auto image = camera.grabImage(1000);
 *         camera.stopGrabbing();
 *         camera.close();
 *     }
 * }
 * @endcode
 */
class BaslerCamera : public ICamera {
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @details 初始化Pylon运行时环境，准备相机操作。
     *          Pylon运行时在首次创建对象时自动初始化。
     * @param parent Qt父对象指针，用于对象生命周期管理
     */
    explicit BaslerCamera(QObject* parent = nullptr);

    /**
     * @brief 析构函数
     * @details 自动关闭相机连接，释放Pylon运行时资源。
     *          确保不会发生资源泄漏。
     */
    ~BaslerCamera() override;

    // ========== 静态方法 ==========

    /**
     * @brief 枚举所有在线Basler设备
     * @details 扫描系统中所有连接的Basler相机，包括：
     *          - GigE相机：通过网络广播发现（可能需要几秒钟）
     *          - USB3相机：通过USB总线枚举发现
     * @return 设备信息列表，包含所有发现的相机
     * @note 硬件通信：GigE相机通过GVCP广播包发现，端口3956
     */
    static QList<BaslerDeviceInfo> enumerateDevices();

    /**
     * @brief 获取Pylon SDK版本号
     * @details 返回当前安装的Pylon SDK版本信息。
     *          格式为"Pylon X.Y.Z"。
     * @return SDK版本字符串
     */
    static QString sdkVersion();

    // ========== 设备选择方法 ==========

    /**
     * @brief 通过设备索引选择相机
     * @details 根据枚举顺序选择相机。索引0表示第一个发现的设备。
     *          注意：设备枚举顺序可能因网络状态或USB顺序变化。
     * @param index 设备在枚举列表中的索引（从0开始）
     */
    void selectDevice(int index);

    /**
     * @brief 通过序列号选择相机
     * @details 根据相机唯一序列号选择设备。这是最可靠的设备选择方式，
     *          因为序列号不会改变且全球唯一。
     * @param serialNumber 相机序列号
     * @return true表示找到并选择成功，false表示未找到对应设备
     */
    bool selectBySerialNumber(const QString& serialNumber);

    /**
     * @brief 通过IP地址选择相机
     * @details 根据IP地址选择GigE相机。仅适用于GigE Vision接口的相机。
     *          USB相机没有IP地址，不能使用此方法。
     * @param ipAddress 相机IP地址，如"192.168.1.100"
     * @return true表示找到并选择成功，false表示未找到对应设备
     */
    bool selectByIP(const QString& ipAddress);

    // ========== ICamera接口实现 ==========

    /**
     * @brief 打开相机设备
     * @details 建立与Basler相机的连接。执行以下步骤：
     *          1. 根据选择的索引获取设备信息
     *          2. 创建Pylon CInstantCamera对象
     *          3. 打开相机连接
     *          4. 读取当前图像参数（宽度、高度等）
     * @return true表示成功打开，false表示打开失败
     * @note 硬件通信：GigE相机建立GVCP控制连接
     */
    bool open() override;

    /**
     * @brief 关闭相机设备
     * @details 断开与相机的连接，执行以下步骤：
     *          1. 如果正在采集，先停止采集
     *          2. 关闭Pylon相机对象
     *          3. 释放相关资源
     */
    void close() override;

    /**
     * @brief 检查相机是否已打开
     * @return true表示相机已打开且可用
     */
    bool isOpen() const override { return isOpen_; }

    /**
     * @brief 获取设备型号名称
     * @return 相机型号，如"acA1920-40gm"
     */
    QString deviceName() const override { return deviceInfo_.modelName; }

    /**
     * @brief 获取设备序列号
     * @return 相机唯一序列号
     */
    QString serialNumber() const override { return deviceInfo_.serialNumber; }

    /**
     * @brief 开始图像采集
     * @details 启动相机的图像采集流。使用LatestImageOnly策略，
     *          确保始终获取最新的图像帧。
     * @return true表示成功开始采集
     * @note 硬件通信：发送AcquisitionStart命令，开始GVSP图像流
     */
    bool startGrabbing() override;

    /**
     * @brief 停止图像采集
     * @details 停止相机的图像采集流，释放采集资源。
     * @note 硬件通信：发送AcquisitionStop命令
     */
    void stopGrabbing() override;

    /**
     * @brief 检查是否正在采集
     * @return true表示正在采集中
     */
    bool isGrabbing() const override { return isGrabbing_; }

    /**
     * @brief 采集一帧图像
     * @details 从相机获取一帧图像，支持多种像素格式的自动转换。
     *          转换流程：
     *          - Mono8/Mono16：保持灰度格式
     *          - BGR8：直接使用
     *          - BayerXX/其他：通过Pylon格式转换器转为BGR8
     * @param timeoutMs 超时时间，单位毫秒，默认1000ms
     * @return 图像数据智能指针，采集失败返回nullptr
     * @note 硬件通信：从GVSP流获取图像数据包
     */
    Base::ImageData::Ptr grabImage(int timeoutMs = 1000) override;

    /**
     * @brief 发送软件触发命令
     * @details 在软件触发模式下触发一帧采集。
     *          需要先设置触发模式为Software。
     * @return true表示触发成功
     * @note 硬件通信：执行TriggerSoftware命令节点
     */
    bool trigger() override;

    /**
     * @brief 设置相机配置
     * @details 批量设置相机参数，包括曝光、增益、触发模式、ROI等。
     *          如果相机已打开，参数会立即应用到硬件。
     * @param config 相机配置结构体
     */
    void setConfig(const Config& config) override;

    /**
     * @brief 获取当前相机配置
     * @return 当前配置结构体副本
     */
    Config getConfig() const override { return config_; }

    /**
     * @brief 设置曝光时间
     * @details 设置相机的曝光时间。如果相机已打开，
     *          会通过GenApi写入ExposureTime节点。
     * @param exposure 曝光时间，单位微秒(us)
     * @note 硬件通信：写入ExposureTime寄存器
     */
    void setExposure(double exposure) override;

    /**
     * @brief 获取当前曝光时间
     * @details 如果相机已打开，从硬件读取实际值；
     *          否则返回配置中的设定值。
     * @return 曝光时间，单位微秒(us)
     */
    double getExposure() const override;

    /**
     * @brief 设置增益
     * @details 设置相机的模拟增益。如果相机已打开，
     *          会通过GenApi写入Gain节点。
     * @param gain 增益值，单位分贝(dB)
     * @note 硬件通信：写入Gain寄存器
     */
    void setGain(double gain) override;

    /**
     * @brief 获取当前增益
     * @details 如果相机已打开，从硬件读取实际值；
     *          否则返回配置中的设定值。
     * @return 增益值，单位分贝(dB)
     */
    double getGain() const override;

    /**
     * @brief 设置触发模式
     * @details 配置相机的触发采集模式：
     *          - Continuous：关闭TriggerMode，自由运行
     *          - Software：开启TriggerMode，TriggerSource设为Software
     *          - Hardware：开启TriggerMode，TriggerSource设为Line1
     * @param mode 触发模式枚举值
     * @note 硬件通信：写入TriggerMode和TriggerSource寄存器
     */
    void setTriggerMode(TriggerMode mode) override;

    /**
     * @brief 获取当前触发模式
     * @return 触发模式枚举值
     */
    TriggerMode getTriggerMode() const override { return config_.triggerMode; }

    // ========== Basler特有功能 ==========

    /**
     * @brief 获取曝光时间范围
     * @details 从相机硬件读取曝光时间的允许范围。
     *          不同型号相机的范围不同。
     * @param[out] minVal 最小曝光时间，单位微秒
     * @param[out] maxVal 最大曝光时间，单位微秒
     */
    void getExposureRange(double& minVal, double& maxVal) const;

    /**
     * @brief 获取增益范围
     * @details 从相机硬件读取增益的允许范围。
     *          不同型号相机的范围不同。
     * @param[out] minVal 最小增益，单位dB
     * @param[out] maxVal 最大增益，单位dB
     */
    void getGainRange(double& minVal, double& maxVal) const;

    /**
     * @brief 获取图像宽度范围
     * @details 从相机硬件读取图像宽度的允许范围。
     *          通常最小值为16，最大值为传感器全宽度。
     * @param[out] minVal 最小宽度，单位像素
     * @param[out] maxVal 最大宽度，单位像素
     */
    void getWidthRange(int& minVal, int& maxVal) const;

    /**
     * @brief 获取图像高度范围
     * @details 从相机硬件读取图像高度的允许范围。
     *          通常最小值为16，最大值为传感器全高度。
     * @param[out] minVal 最小高度，单位像素
     * @param[out] maxVal 最大高度，单位像素
     */
    void getHeightRange(int& minVal, int& maxVal) const;

    /**
     * @brief 设置ROI区域
     * @details 设置感兴趣区域（Region of Interest）。
     *          ROI可以减少图像数据量，提高帧率。
     *          设置顺序：先重置偏移为0，再设置宽高，最后设置偏移。
     * @param offsetX X方向偏移，单位像素
     * @param offsetY Y方向偏移，单位像素
     * @param width 图像宽度，单位像素
     * @param height 图像高度，单位像素
     * @return true表示设置成功
     * @note 硬件通信：写入Width/Height/OffsetX/OffsetY寄存器
     */
    bool setROI(int offsetX, int offsetY, int width, int height);

    /**
     * @brief 获取当前设备信息
     * @return 设备信息结构体副本
     */
    BaslerDeviceInfo getDeviceInfo() const { return deviceInfo_; }

private:
    bool isOpen_;               ///< 相机打开状态标志
    bool isGrabbing_;           ///< 采集状态标志
    Config config_;             ///< 相机配置参数
    BaslerDeviceInfo deviceInfo_;  ///< 当前设备信息
    int selectedDeviceIndex_;   ///< 选中的设备索引，-1表示未选择

    mutable QMutex mutex_;      ///< 线程安全互斥锁

    /**
     * @brief Pylon相机对象指针
     * @details 使用void*避免头文件依赖，实际类型为Pylon::CInstantCamera*
     * @note 在.cpp中进行类型转换：static_cast<Pylon::CInstantCamera*>(camera_)
     */
    void* camera_;
};

} // namespace HAL
} // namespace VisionForge

#endif // USE_BASLER_PYLON
