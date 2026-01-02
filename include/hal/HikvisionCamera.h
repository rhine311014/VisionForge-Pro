/**
 * @file HikvisionCamera.h
 * @brief 海康威视工业相机驱动封装
 * @details 基于海康威视MVS SDK实现的工业相机驱动类。
 *          支持海康威视全系列GigE Vision和USB3 Vision工业相机，
 *          提供设备枚举、参数配置、图像采集等完整功能。
 *          包括设备枚举缓存机制，提高重复枚举效率。
 * @author VisionForge Team
 * @date 2025-12-17
 * @version 1.0
 *
 * @note 硬件通信协议说明：
 *       - GigE相机：使用GigE Vision协议（基于UDP），端口3956
 *         - 支持巨型帧(Jumbo Frame)，可配置最佳包大小
 *         - 支持组播和动作命令(Action Command)
 *       - USB3相机：使用USB3 Vision协议，通过USB 3.0/3.1传输
 *       - 所有相机遵循GenICam标准，使用海康扩展的GenApi访问
 *
 * @note 编译依赖：
 *       - 需要安装海康威视MVS SDK（Machine Vision Software）
 *       - 编译时需定义宏 USE_HIKVISION_MVS
 *       - 链接库：MvCameraControl.lib
 *       - 运行时需要MVS运行环境和相机驱动
 *
 * @note 错误码说明：
 *       - 0x8000000C: MV_E_RESOURCE - 资源被占用（通常是MVS客户端已打开相机）
 *       - 0x80000000: MV_E_HANDLE - 句柄错误
 *       - 0x80000006: MV_E_NODATA - 无数据（软触发模式等待中）
 *       - 0x80000008: MV_E_CALLORDER - 调用顺序错误
 */

#pragma once

#include "hal/ICamera.h"
#include <QMutex>
#include <chrono>

#ifdef USE_HIKVISION_MVS

namespace VisionForge {
namespace HAL {

/**
 * @brief 海康相机设备信息结构体
 * @details 存储通过MVS SDK枚举得到的相机设备信息，
 *          包含GigE和USB3两种接口类型相机的公共信息。
 */
struct CameraDeviceInfo {
    QString manufacturer;     ///< 厂商名称：通常为"Hikvision"或"HikRobot"
    QString modelName;        ///< 型号名称：如"MV-CE050-10GM"、"MV-CA050-10GC"
    QString serialNumber;     ///< 序列号：相机的唯一标识符
    QString deviceVersion;    ///< 固件版本：相机固件的版本号
    QString userDefinedName;  ///< 用户定义名称：可通过MVS客户端自定义的名称
    QString ipAddress;        ///< IP地址：仅GigE相机有效，如"192.168.1.100"
    QString macAddress;       ///< MAC地址：相机的物理地址
    int deviceType;           ///< 设备类型：1=GigE, 2=USB3, 4=CameraLink

    /**
     * @brief 默认构造函数
     */
    CameraDeviceInfo()
        : deviceType(0)
    {}
};

/**
 * @class HikvisionCamera
 * @brief 海康威视工业相机驱动类
 * @details 继承自ICamera接口，封装海康威视MVS SDK实现完整的相机控制。
 *          支持以下功能：
 *          - 设备枚举：自动发现网络和USB连接的海康相机（带缓存优化）
 *          - 设备选择：支持按索引、序列号、IP地址选择相机
 *          - 参数配置：曝光时间、增益、触发模式、ROI等
 *          - 图像采集：连续采集、软件触发、硬件触发
 *          - 格式转换：支持Mono8、RGB、BGR、Bayer等格式自动转换
 *          - 参数持久化：支持保存/加载相机内部参数
 *
 * @note 硬件通信协议：
 *       相机参数通过MVS SDK的GenApi接口访问，主要节点包括：
 *       - Width/Height/OffsetX/OffsetY：图像尺寸和ROI设置
 *       - ExposureTime：曝光时间（单位微秒）
 *       - Gain：增益（单位dB）
 *       - TriggerMode/TriggerSource：触发模式和触发源配置
 *       - TriggerSoftware：软件触发命令节点
 *       - GevSCPSPacketSize：GigE相机的网络包大小
 *
 * @note 线程安全：
 *       所有公共方法使用互斥锁保护，可安全在多线程环境使用
 *
 * @note 使用示例：
 * @code
 * // 枚举所有海康相机
 * auto devices = HikvisionCamera::enumerateDevices();
 * if (!devices.isEmpty()) {
 *     HikvisionCamera camera;
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
class HikvisionCamera : public ICamera {
    Q_OBJECT

public:
    /**
     * @brief 设备类型枚举
     * @details 定义海康相机支持的接口类型
     */
    enum DeviceType {
        Unknown = 0,       ///< 未知类型
        GigE = 1,          ///< GigE Vision接口（千兆以太网）
        USB3 = 2,          ///< USB3 Vision接口（USB 3.0）
        CameraLink = 4     ///< Camera Link接口（高速串行）
    };

    /**
     * @brief 构造函数
     * @param parent Qt父对象指针，用于对象生命周期管理
     */
    explicit HikvisionCamera(QObject* parent = nullptr);

    /**
     * @brief 析构函数
     * @details 自动关闭相机连接，释放图像缓冲区
     */
    ~HikvisionCamera() override;

    // ========== 静态方法 ==========

    /**
     * @brief 枚举所有在线海康设备
     * @details 扫描系统中所有连接的海康威视相机，包括GigE和USB3相机。
     *          使用5秒缓存机制，避免频繁枚举导致的性能问题。
     * @return 设备信息列表，包含所有发现的相机
     * @note 硬件通信：
     *       - GigE相机通过网络广播发现
     *       - USB3相机通过USB总线枚举发现
     *       - 如果MVS客户端正在运行，可能导致枚举失败（错误码0x8000000C）
     */
    static QList<CameraDeviceInfo> enumerateDevices();

    /**
     * @brief 获取MVS SDK版本号
     * @details 返回当前安装的海康MVS SDK版本信息。
     *          格式为"MVS SDK X.Y.Z"。
     * @return SDK版本字符串
     */
    static QString sdkVersion();

    // ========== 设备选择方法 ==========

    /**
     * @brief 通过设备索引选择相机
     * @details 根据枚举顺序选择相机。索引对应enumerateDevices()返回列表的顺序。
     *          选择后会自动填充设备信息（型号、序列号、IP地址等）。
     * @param index 设备在枚举列表中的索引（从0开始）
     */
    void selectDevice(int index);

    /**
     * @brief 通过序列号选择相机
     * @details 根据相机唯一序列号选择设备。会自动触发设备重新枚举。
     *          这是最可靠的设备选择方式。
     * @param serialNumber 相机序列号
     * @return true表示找到并选择成功，false表示未找到对应设备
     */
    bool selectBySerialNumber(const QString& serialNumber);

    /**
     * @brief 通过IP地址选择相机
     * @details 根据IP地址选择GigE相机。会自动触发设备重新枚举。
     *          仅适用于GigE Vision接口的相机。
     * @param ipAddress 相机IP地址，如"192.168.1.100"
     * @return true表示找到并选择成功，false表示未找到对应设备
     */
    bool selectByIP(const QString& ipAddress);

    // ========== ICamera接口实现 ==========

    /**
     * @brief 打开相机设备
     * @details 建立与海康相机的连接。执行以下步骤：
     *          1. 如果未选择设备，自动选择第一个发现的相机
     *          2. 创建相机句柄（MV_CC_CreateHandle）
     *          3. 以独占模式打开设备（MV_CC_OpenDevice）
     *          4. 配置GigE相机的最佳包大小
     *          5. 读取当前图像参数并分配缓冲区
     *          6. 设置为连续采集模式
     * @return true表示成功打开，false表示打开失败
     * @note 硬件通信：使用独占模式(MV_ACCESS_Exclusive)打开设备
     */
    bool open() override;

    /**
     * @brief 关闭相机设备
     * @details 断开与相机的连接，执行以下步骤：
     *          1. 停止图像采集
     *          2. 关闭设备连接（MV_CC_CloseDevice）
     *          3. 销毁相机句柄（MV_CC_DestroyHandle）
     */
    void close() override;

    /**
     * @brief 检查相机是否已打开
     * @return true表示相机已打开且可用
     */
    bool isOpen() const override { return isOpen_; }

    /**
     * @brief 获取设备型号名称
     * @return 相机型号，如"MV-CE050-10GM"
     */
    QString deviceName() const override { return deviceInfo_.modelName; }

    /**
     * @brief 获取设备序列号
     * @return 相机唯一序列号
     */
    QString serialNumber() const override { return deviceInfo_.serialNumber; }

    /**
     * @brief 开始图像采集
     * @details 启动相机的图像采集流（MV_CC_StartGrabbing）。
     *          根据当前触发模式，相机进入连续采集或待触发状态。
     * @return true表示成功开始采集
     */
    bool startGrabbing() override;

    /**
     * @brief 停止图像采集
     * @details 停止相机的图像采集流（MV_CC_StopGrabbing）。
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
     *          如果未在采集状态，会自动启动采集。
     *          转换流程：
     *          - Mono8：保持灰度格式
     *          - RGB8_Packed：转换为BGR
     *          - BGR8_Packed：直接使用
     *          - BayerXX8：通过MVS SDK转换为BGR
     * @param timeoutMs 超时时间，单位毫秒，默认1000ms
     * @return 图像数据智能指针，采集失败返回nullptr
     * @note 硬件通信：使用MV_CC_GetImageBuffer获取图像
     */
    Base::ImageData::Ptr grabImage(int timeoutMs = 1000) override;

    /**
     * @brief 发送软件触发命令
     * @details 在软件触发模式下触发一帧采集。
     * @return true表示触发成功
     * @note 硬件通信：执行TriggerSoftware命令节点
     */
    bool trigger() override;

    /**
     * @brief 设置相机配置
     * @details 批量设置相机参数。如果正在采集，会自动暂停后设置再恢复。
     *          包括：ROI、曝光、增益、触发模式等。
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
     * @details 设置相机的曝光时间。某些海康相机型号需要停止采集才能设置参数，
     *          此方法会自动处理采集状态的暂停和恢复。
     * @param exposure 曝光时间，单位微秒(us)
     * @note 硬件通信：写入ExposureTime寄存器
     */
    void setExposure(double exposure) override;

    /**
     * @brief 获取当前曝光时间
     * @details 如果相机已打开，从硬件读取实际值；否则返回配置中的设定值。
     * @return 曝光时间，单位微秒(us)
     */
    double getExposure() const override;

    /**
     * @brief 设置增益
     * @details 设置相机的模拟增益。某些海康相机型号需要停止采集才能设置参数，
     *          此方法会自动处理采集状态的暂停和恢复。
     * @param gain 增益值，单位分贝(dB)
     * @note 硬件通信：写入Gain寄存器
     */
    void setGain(double gain) override;

    /**
     * @brief 获取当前增益
     * @details 如果相机已打开，从硬件读取实际值；否则返回配置中的设定值。
     * @return 增益值，单位分贝(dB)
     */
    double getGain() const override;

    /**
     * @brief 设置触发模式
     * @details 配置相机的触发采集模式：
     *          - Continuous：关闭TriggerMode
     *          - Software：开启TriggerMode，TriggerSource设为SOFTWARE
     *          - Hardware：开启TriggerMode，TriggerSource设为LINE0
     * @param mode 触发模式枚举值
     * @note 硬件通信：写入TriggerMode和TriggerSource寄存器
     */
    void setTriggerMode(TriggerMode mode) override;

    /**
     * @brief 获取当前触发模式
     * @return 触发模式枚举值
     */
    TriggerMode getTriggerMode() const override { return config_.triggerMode; }

    // ========== 海康特有功能 ==========

    /**
     * @brief 获取曝光时间范围
     * @details 从相机硬件读取曝光时间的允许范围。
     * @param[out] minVal 最小曝光时间，单位微秒
     * @param[out] maxVal 最大曝光时间，单位微秒
     */
    void getExposureRange(double& minVal, double& maxVal) const;

    /**
     * @brief 获取增益范围
     * @details 从相机硬件读取增益的允许范围。
     * @param[out] minVal 最小增益，单位dB
     * @param[out] maxVal 最大增益，单位dB
     */
    void getGainRange(double& minVal, double& maxVal) const;

    /**
     * @brief 获取图像宽度范围
     * @details 从相机硬件读取图像宽度的允许范围。
     * @param[out] minVal 最小宽度，单位像素
     * @param[out] maxVal 最大宽度，单位像素
     */
    void getWidthRange(int& minVal, int& maxVal) const;

    /**
     * @brief 获取图像高度范围
     * @details 从相机硬件读取图像高度的允许范围。
     * @param[out] minVal 最小高度，单位像素
     * @param[out] maxVal 最大高度，单位像素
     */
    void getHeightRange(int& minVal, int& maxVal) const;

    /**
     * @brief 设置ROI区域
     * @details 设置感兴趣区域（Region of Interest）。
     *          设置顺序：先重置偏移为0，再设置宽高，最后设置偏移。
     *          设置后会自动重新分配图像缓冲区。
     * @param offsetX X方向偏移，单位像素
     * @param offsetY Y方向偏移，单位像素
     * @param width 图像宽度，单位像素
     * @param height 图像高度，单位像素
     * @return true表示设置成功
     */
    bool setROI(int offsetX, int offsetY, int width, int height);

    /**
     * @brief 获取当前帧率
     * @details 读取相机当前的实际帧率（ResultingFrameRate）。
     * @return 帧率，单位fps
     */
    double getFrameRate() const;

    /**
     * @brief 获取当前设备信息
     * @return 设备信息结构体副本
     */
    CameraDeviceInfo getDeviceInfo() const { return deviceInfo_; }

    /**
     * @brief 保存参数到相机
     * @details 将当前参数保存到相机内部存储（UserSet）。
     *          下次开机时自动加载这些参数。
     * @return true表示保存成功
     * @note 硬件通信：执行UserSetSave命令
     */
    bool saveParameters();

    /**
     * @brief 从相机加载参数
     * @details 从相机内部存储（UserSet）加载保存的参数。
     * @return true表示加载成功
     * @note 硬件通信：执行UserSetLoad命令
     */
    bool loadParameters();

private:
    // ========== 内部辅助方法 ==========

    /**
     * @brief 创建相机句柄
     * @return true表示创建成功
     */
    bool createHandle();

    /**
     * @brief 销毁相机句柄
     */
    void destroyHandle();

    /**
     * @brief 设置整型参数
     * @param key 参数名称
     * @param value 参数值
     * @return true表示设置成功
     */
    bool setIntValue(const char* key, int64_t value);

    /**
     * @brief 设置浮点型参数
     * @param key 参数名称
     * @param value 参数值
     * @return true表示设置成功
     */
    bool setFloatValue(const char* key, double value);

    /**
     * @brief 设置枚举型参数
     * @param key 参数名称
     * @param value 枚举值
     * @return true表示设置成功
     */
    bool setEnumValue(const char* key, unsigned int value);

    /**
     * @brief 设置布尔型参数
     * @param key 参数名称
     * @param value 参数值
     * @return true表示设置成功
     */
    bool setBoolValue(const char* key, bool value);

    /**
     * @brief 获取整型参数
     * @param key 参数名称
     * @return 参数当前值
     */
    int64_t getIntValue(const char* key) const;

    /**
     * @brief 获取浮点型参数
     * @param key 参数名称
     * @return 参数当前值
     */
    double getFloatValue(const char* key) const;

    /**
     * @brief 获取枚举型参数
     * @param key 参数名称
     * @return 枚举当前值
     */
    unsigned int getEnumValue(const char* key) const;

    /**
     * @brief 获取布尔型参数
     * @param key 参数名称
     * @return 参数当前值
     */
    bool getBoolValue(const char* key) const;

    /**
     * @brief 获取整型参数范围
     * @param key 参数名称
     * @param[out] minVal 最小值
     * @param[out] maxVal 最大值
     */
    void getIntRange(const char* key, int64_t& minVal, int64_t& maxVal) const;

    /**
     * @brief 获取浮点型参数范围
     * @param key 参数名称
     * @param[out] minVal 最小值
     * @param[out] maxVal 最大值
     */
    void getFloatRange(const char* key, double& minVal, double& maxVal) const;

    /**
     * @brief 获取最后一次错误信息
     * @return 错误描述字符串
     */
    QString getLastError() const;

private:
    /**
     * @brief MVS相机句柄
     * @details 使用void*避免头文件依赖，实际类型为MV_CC_HANDLE
     */
    void* handle_;

    bool isOpen_;               ///< 相机打开状态标志
    bool isGrabbing_;           ///< 采集状态标志
    Config config_;             ///< 相机配置参数
    CameraDeviceInfo deviceInfo_;  ///< 当前设备信息
    int selectedDeviceIndex_;   ///< 选中的设备索引，-1表示未选择

    mutable QMutex mutex_;      ///< 线程安全互斥锁

    unsigned char* imageBuffer_;  ///< 图像转换缓冲区
    unsigned int bufferSize_;     ///< 缓冲区大小

    // ========== 设备枚举缓存（静态成员） ==========
    static QList<CameraDeviceInfo> deviceCache_;          ///< 设备列表缓存
    static std::chrono::steady_clock::time_point lastEnumTime_;  ///< 上次枚举时间
    static constexpr std::chrono::seconds CACHE_EXPIRY{5};  ///< 缓存过期时间：5秒
    static QMutex cacheMutex_;                            ///< 缓存访问互斥锁
};

} // namespace HAL
} // namespace VisionForge

#endif // USE_HIKVISION_MVS
