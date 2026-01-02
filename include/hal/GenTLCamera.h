/**
 * @file GenTLCamera.h
 * @brief GenICam Transport Layer 通用相机接口
 * @details 基于GenTL标准实现的通用工业相机接口。
 *          GenTL(GenICam Transport Layer)是一种开放的相机接口标准，
 *          通过加载厂商提供的GenTL Producer (.cti文件)，可以访问任何
 *          符合GenICam标准的工业相机，无需针对每个品牌编写特定代码。
 *          支持约95%的市面工业相机产品。
 * @author VisionForge Team
 * @date 2025-12-22
 * @version 1.0
 *
 * @note 硬件通信协议说明：
 *       - GenTL是一个传输层抽象接口，实际通信由各厂商的CTI实现
 *       - 支持的传输层类型：
 *         - GigE Vision：基于以太网的工业相机协议
 *         - USB3 Vision：基于USB 3.0的工业相机协议
 *         - Camera Link：高速串行点对点接口
 *         - CoaXPress：基于同轴电缆的高速接口
 *       - 所有相机参数通过GenICam GenApi节点访问
 *
 * @note 编译依赖：
 *       - 需要GenICam运行时库（GenApi、GCBase等）
 *       - 编译时需定义宏 USE_GENTL
 *       - 需要相机厂商提供的CTI Producer文件
 *
 * @note CTI文件搜索路径：
 *       - 环境变量 GENICAM_GENTL64_PATH (64位)
 *       - 环境变量 GENICAM_GENTL32_PATH (32位)
 *       - 用户指定的额外搜索路径
 */

#pragma once

#include "hal/ICamera.h"
#include <QObject>
#include <QString>
#include <QStringList>
#include <QMutex>
#include <QMap>
#include <QVariant>
#include <memory>
#include <functional>

#ifdef USE_GENTL

namespace VisionForge {
namespace HAL {

// ============================================================
// GenTL 数据结构定义
// ============================================================

/**
 * @brief GenTL Producer信息结构体
 * @details 存储GenTL Producer(.cti文件)的详细信息。
 *          Producer是厂商提供的传输层实现库，负责实际的硬件通信。
 */
struct GenTLProducerInfo {
    QString filePath;           ///< CTI文件完整路径：如"C:/Program Files/Basler/pylon6/Runtime/x64/ProducerGEV.cti"
    QString vendorName;         ///< 厂商名称：如"Basler"、"MATRIX VISION"
    QString modelName;          ///< 模型名称：Producer的描述性名称
    QString version;            ///< 版本号：Producer版本信息
    QString tlType;             ///< 传输层类型：如"GEV"(GigE Vision)、"U3V"(USB3 Vision)、"CL"(Camera Link)
    bool isLoaded = false;      ///< 加载状态：是否已成功加载

    GenTLProducerInfo() = default;
};

/**
 * @brief GenTL设备信息结构体
 * @details 存储通过GenTL发现的相机设备信息。
 *          包含设备标识、厂商信息和网络配置（GigE相机）。
 */
struct GenTLDeviceInfo {
    QString deviceId;           ///< 设备唯一ID：GenTL分配的设备标识符
    QString vendorName;         ///< 厂商名称：如"Basler"、"FLIR"
    QString modelName;          ///< 型号名称：如"acA1920-40gm"
    QString serialNumber;       ///< 序列号：相机的唯一序列号
    QString userDefinedName;    ///< 用户定义名称：可通过配置软件自定义
    QString interfaceId;        ///< 接口ID：设备所属接口的标识符
    QString producerPath;       ///< Producer路径：发现此设备的CTI文件路径
    QString accessStatus;       ///< 访问状态："ReadWrite"(可读写)、"ReadOnly"(只读)、"NoAccess"(无法访问)
    QString tlType;             ///< 传输层类型："GEV"、"U3V"、"CL"等

    // GigE相机特有属性
    QString ipAddress;          ///< IP地址：GigE相机的当前IP地址
    QString subnetMask;         ///< 子网掩码：如"255.255.255.0"
    QString gateway;            ///< 网关地址：默认网关
    QString macAddress;         ///< MAC地址：相机的物理地址

    GenTLDeviceInfo() = default;
};

/**
 * @brief GenICam节点类型枚举
 * @details 定义GenICam标准中支持的节点类型。
 *          每种类型有不同的访问方式和值范围。
 */
enum class GenICamNodeType {
    Unknown,            ///< 未知类型
    Integer,            ///< 整数节点：如Width、Height、OffsetX等
    Float,              ///< 浮点节点：如ExposureTime、Gain等
    Boolean,            ///< 布尔节点：如ReverseX、AcquisitionFrameRateEnable等
    String,             ///< 字符串节点：如DeviceModelName、DeviceSerialNumber等
    Enumeration,        ///< 枚举节点：如TriggerMode、PixelFormat等
    Command,            ///< 命令节点：如AcquisitionStart、TriggerSoftware等
    Category,           ///< 类别节点：用于组织其他节点的容器
    Register            ///< 寄存器节点：直接访问硬件寄存器
};

/**
 * @brief GenICam节点信息结构体
 * @details 存储GenICam节点的完整元数据，包括名称、类型、范围和访问权限。
 *          用于动态生成参数界面和验证参数值。
 */
struct GenICamNodeInfo {
    QString name;               ///< 节点名称：GenICam标准名称，如"ExposureTime"
    QString displayName;        ///< 显示名称：用户友好的显示名称
    QString description;        ///< 描述：节点功能的详细描述
    QString tooltip;            ///< 提示信息：鼠标悬停时显示的简短提示
    GenICamNodeType type;       ///< 节点类型：决定如何读写此节点
    bool isReadable = true;     ///< 可读标志：是否可以读取当前值
    bool isWritable = true;     ///< 可写标志：是否可以修改值
    bool isImplemented = true;  ///< 实现标志：相机是否实现了此节点
    bool isAvailable = true;    ///< 可用标志：节点当前是否可访问
    QString visibility;         ///< 可见性："Beginner"(初级)、"Expert"(专家)、"Guru"(高级)、"Invisible"(隐藏)

    // 数值范围（Integer/Float节点适用）
    double minValue = 0;        ///< 最小值：允许设置的最小值
    double maxValue = 0;        ///< 最大值：允许设置的最大值
    double increment = 1;       ///< 增量：值的最小步进

    // 枚举选项（Enumeration节点适用）
    QStringList enumEntries;    ///< 枚举条目：所有可选值的列表
    QString currentEntry;       ///< 当前条目：当前选中的枚举值

    GenICamNodeInfo() : type(GenICamNodeType::Unknown) {}
};

/**
 * @brief 数据流统计信息结构体
 * @details 存储相机数据流的性能统计信息，用于诊断传输问题和优化性能。
 */
struct StreamStatistics {
    uint64_t deliveredFrameCount = 0;   ///< 已传输帧数：成功传输到主机的帧数
    uint64_t lostFrameCount = 0;        ///< 丢失帧数：传输过程中丢失的帧数
    uint64_t resendRequestCount = 0;    ///< 重传请求数：GigE相机发起的数据包重传请求次数
    uint64_t resendPacketCount = 0;     ///< 重传包数：实际重传的数据包数量
    uint64_t incompleteFrameCount = 0;  ///< 不完整帧数：接收不完整被丢弃的帧数
    double frameRate = 0.0;             ///< 当前帧率：实际采集帧率，单位fps
    double bandwidth = 0.0;             ///< 带宽使用：数据传输速率，单位MB/s
    uint64_t bufferUnderrunCount = 0;   ///< 缓冲区不足次数：接收缓冲区来不及处理的次数

    StreamStatistics() = default;
};

/**
 * @brief 缓冲区配置结构体
 * @details 配置相机数据流的缓冲区参数，影响采集性能和内存使用。
 *          合理配置可以减少帧丢失和提高系统稳定性。
 */
struct BufferConfig {
    int bufferCount = 10;           ///< 缓冲区数量：预分配的图像缓冲区个数，建议5-20
    size_t bufferSize = 0;          ///< 单个缓冲区大小：0表示自动根据PayloadSize计算
    bool useAnnounceBuffer = true;  ///< 使用预分配缓冲区：true可减少运行时内存分配开销
    int payloadSize = 0;            ///< 有效负载大小：单帧图像数据的字节数

    BufferConfig() = default;
};

// ============================================================
// GenTL Producer 管理器
// ============================================================

/**
 * @class GenTLProducerManager
 * @brief GenTL Producer管理器（单例模式）
 * @details 负责发现、加载和管理系统中的GenTL Producer (.cti文件)。
 *          Producer是厂商提供的传输层实现，一个Producer可能支持多个相机设备。
 *
 * @note 使用示例：
 * @code
 * auto& manager = GenTLProducerManager::instance();
 * manager.discoverProducers();  // 搜索系统中的CTI文件
 * auto producers = manager.getProducers();
 * for (const auto& p : producers) {
 *     manager.loadProducer(p.filePath);  // 加载Producer
 * }
 * auto devices = manager.enumerateDevices();  // 枚举所有设备
 * @endcode
 */
class GenTLProducerManager {
public:
    /**
     * @brief 获取单例实例
     * @return 管理器单例引用
     */
    static GenTLProducerManager& instance();

    /**
     * @brief 扫描系统中的GenTL Producer
     * @details 搜索以下位置的CTI文件：
     *          - GENICAM_GENTL64_PATH/GENICAM_GENTL32_PATH环境变量指定的路径
     *          - 用户指定的额外搜索路径
     * @param additionalPaths 额外的搜索路径列表
     * @return 发现的Producer数量
     */
    int discoverProducers(const QStringList& additionalPaths = QStringList());

    /**
     * @brief 获取所有发现的Producer信息
     * @return Producer信息列表
     */
    QList<GenTLProducerInfo> getProducers() const;

    /**
     * @brief 加载指定的Producer
     * @details 调用CTI的GCInitLib和TLOpen函数初始化传输层
     * @param ctiPath CTI文件的完整路径
     * @return true表示加载成功
     */
    bool loadProducer(const QString& ctiPath);

    /**
     * @brief 卸载指定的Producer
     * @details 调用TLClose和GCCloseLib释放传输层资源
     * @param ctiPath CTI文件的完整路径
     */
    void unloadProducer(const QString& ctiPath);

    /**
     * @brief 卸载所有已加载的Producer
     * @details 释放所有传输层资源，通常在程序退出时调用
     */
    void unloadAllProducers();

    /**
     * @brief 枚举所有已加载Producer的设备
     * @details 遍历所有已加载的Producer，调用各自的设备枚举功能
     * @return 设备信息列表
     */
    QList<GenTLDeviceInfo> enumerateDevices();

    /**
     * @brief 获取GenTL规范版本信息
     * @return 版本字符串，如"1.5.0"
     */
    QString getGenTLVersion() const;

    /**
     * @brief 设置环境变量搜索路径
     * @details 临时添加CTI搜索路径，不修改系统环境变量
     * @param path 要添加的搜索路径
     */
    void setEnvironmentPath(const QString& path);

private:
    GenTLProducerManager();
    ~GenTLProducerManager();
    GenTLProducerManager(const GenTLProducerManager&) = delete;
    GenTLProducerManager& operator=(const GenTLProducerManager&) = delete;

    class Impl;
    std::unique_ptr<Impl> impl_;  ///< PIMPL实现，隐藏GenTL头文件依赖
};

// ============================================================
// GenTL 通用相机类
// ============================================================

/**
 * @class GenTLCamera
 * @brief GenTL通用相机类
 * @details 基于GenICam Transport Layer标准实现的通用相机接口。
 *          支持所有符合GenICam标准的工业相机（约95%市面产品）。
 *
 *          主要特性：
 *          - 运行时自动发现相机设备
 *          - 通用GenICam参数访问接口
 *          - 支持GigE Vision、USB3 Vision、Camera Link等接口
 *          - Action Command多相机同步触发
 *          - 完整的错误处理和诊断功能
 *          - 数据流统计和性能监控
 *
 * @note 硬件通信协议：
 *       - 设备发现：通过GenTL的TLUpdateInterfaceList和IFUpdateDeviceList
 *       - 设备控制：通过GenApi NodeMap访问相机参数
 *       - 图像采集：通过DataStream获取图像缓冲区
 *
 * @note 使用示例：
 * @code
 * GenTLCamera camera;
 *
 * // 枚举设备
 * auto devices = GenTLCamera::enumerateDevices();
 * if (!devices.isEmpty()) {
 *     camera.selectDevice(devices.first().deviceId);
 *
 *     if (camera.open()) {
 *         // 通过GenICam节点名设置参数
 *         camera.setFeature("ExposureTime", 10000.0);
 *         camera.setFeature("Gain", 6.0);
 *
 *         camera.startGrabbing();
 *         auto image = camera.grabImage(1000);
 *         camera.stopGrabbing();
 *         camera.close();
 *     }
 * }
 * @endcode
 */
class GenTLCamera : public ICamera {
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent Qt父对象指针
     */
    explicit GenTLCamera(QObject* parent = nullptr);

    /**
     * @brief 析构函数
     * @details 自动关闭相机连接，释放所有资源
     */
    ~GenTLCamera() override;

    // ========== 静态方法 ==========

    /**
     * @brief 枚举所有可用的GenTL设备
     * @details 搜索所有已加载Producer管理的相机设备
     * @param refreshProducers 是否重新扫描Producer（首次调用或添加新设备时设为true）
     * @return 设备信息列表
     */
    static QList<GenTLDeviceInfo> enumerateDevices(bool refreshProducers = false);

    /**
     * @brief 获取GenTL SDK版本
     * @return 版本字符串
     */
    static QString sdkVersion();

    /**
     * @brief 获取支持的传输层类型
     * @return 类型列表，如["GigE Vision", "USB3 Vision", "Camera Link"]
     */
    static QStringList supportedTransportLayers();

    // ========== 设备选择方法 ==========

    /**
     * @brief 通过索引选择设备
     * @param index 设备在枚举列表中的索引
     */
    void selectDevice(int index);

    /**
     * @brief 通过设备ID选择设备
     * @param deviceId GenTL分配的设备唯一标识符
     * @return true表示找到并选择成功
     */
    bool selectByDeviceId(const QString& deviceId);

    /**
     * @brief 通过序列号选择设备
     * @param serialNumber 相机序列号
     * @return true表示找到并选择成功
     */
    bool selectBySerialNumber(const QString& serialNumber);

    /**
     * @brief 通过IP地址选择设备
     * @details 仅适用于GigE Vision相机
     * @param ipAddress 相机IP地址
     * @return true表示找到并选择成功
     */
    bool selectByIP(const QString& ipAddress);

    /**
     * @brief 通过用户定义名称选择设备
     * @param userName 用户定义的设备名称
     * @return true表示找到并选择成功
     */
    bool selectByUserName(const QString& userName);

    // ========== ICamera接口实现 ==========

    bool open() override;
    void close() override;
    bool isOpen() const override;
    QString deviceName() const override;
    QString serialNumber() const override;

    bool startGrabbing() override;
    void stopGrabbing() override;
    bool isGrabbing() const override;
    Base::ImageData::Ptr grabImage(int timeoutMs = 1000) override;
    bool trigger() override;

    void setConfig(const Config& config) override;
    Config getConfig() const override;
    void setExposure(double exposure) override;
    double getExposure() const override;
    void setGain(double gain) override;
    double getGain() const override;
    void setTriggerMode(TriggerMode mode) override;
    TriggerMode getTriggerMode() const override;

    // ========== 扩展触发模式 ==========

    /**
     * @brief 设置扩展触发模式
     * @param mode 扩展触发模式
     */
    void setTriggerModeEx(TriggerModeEx mode) override;

    /**
     * @brief 获取扩展触发模式
     * @return 当前扩展触发模式
     */
    TriggerModeEx getTriggerModeEx() const override;

    /**
     * @brief 配置Action Command参数
     * @details 配置GigE Vision Action Command多相机同步触发功能
     * @param config Action Command配置参数
     * @return true表示配置成功
     */
    bool configureActionCommand(const ActionCommandConfig& config) override;

    /**
     * @brief 获取Action Command配置
     * @return 当前Action Command配置
     */
    ActionCommandConfig getActionCommandConfig() const override;

    /**
     * @brief 检查是否支持Action Command
     * @return true表示支持（通常为GigE相机）
     */
    bool supportsActionCommand() const override;

    /**
     * @brief 配置硬件同步参数
     * @param config 硬件同步配置
     * @return true表示配置成功
     */
    bool configureHardwareSync(const HardwareSyncConfig& config) override;

    /**
     * @brief 获取硬件同步配置
     * @return 当前硬件同步配置
     */
    HardwareSyncConfig getHardwareSyncConfig() const override;

    /**
     * @brief 检查是否支持硬件同步
     * @return true表示支持
     */
    bool supportsHardwareSync() const override;

    /**
     * @brief 等待Action Command触发
     * @param timeoutMs 超时时间，单位毫秒
     * @return true表示成功接收到触发
     */
    bool waitForActionTrigger(int timeoutMs = 5000) override;

    // ========== GenICam通用参数接口 ==========

    /**
     * @brief 设置GenICam特性值
     * @details 通过GenICam节点名设置任意相机参数，支持自动类型转换。
     *          支持的节点类型：Integer、Float、Boolean、String、Enumeration
     * @param featureName GenICam节点名，如"ExposureTime"、"Gain"、"Width"
     * @param value 特性值（QVariant支持自动类型转换）
     * @return true表示设置成功
     * @note 示例：
     * @code
     * camera.setFeature("ExposureTime", 10000.0);  // 浮点参数
     * camera.setFeature("Width", 1920);             // 整数参数
     * camera.setFeature("TriggerMode", "On");       // 枚举参数
     * @endcode
     */
    bool setFeature(const QString& featureName, const QVariant& value);

    /**
     * @brief 获取GenICam特性值
     * @param featureName GenICam节点名
     * @return 特性值，失败返回无效QVariant（可用QVariant::isValid()检查）
     */
    QVariant getFeature(const QString& featureName) const;

    /**
     * @brief 获取特性节点详细信息
     * @details 获取节点的元数据，包括类型、范围、描述等
     * @param featureName GenICam节点名
     * @return 节点详细信息结构体
     */
    GenICamNodeInfo getFeatureInfo(const QString& featureName) const;

    /**
     * @brief 枚举所有可用的GenICam特性
     * @param category 类别过滤，空字符串表示返回全部
     * @return 特性名列表
     */
    QStringList enumerateFeatures(const QString& category = QString()) const;

    /**
     * @brief 执行GenICam命令
     * @details 执行命令类型节点，如软件触发、开始采集等
     * @param commandName 命令节点名，如"AcquisitionStart"、"TriggerSoftware"
     * @return true表示执行成功
     */
    bool executeCommand(const QString& commandName);

    /**
     * @brief 检查特性是否可用
     * @param featureName GenICam节点名
     * @return true表示节点存在且可访问
     */
    bool isFeatureAvailable(const QString& featureName) const;

    /**
     * @brief 检查特性是否可写
     * @param featureName GenICam节点名
     * @return true表示节点可写入
     */
    bool isFeatureWritable(const QString& featureName) const;

    // ========== 参数范围查询 ==========

    /**
     * @brief 获取曝光时间范围
     * @param[out] minVal 最小曝光时间，单位微秒
     * @param[out] maxVal 最大曝光时间，单位微秒
     */
    void getExposureRange(double& minVal, double& maxVal) const;

    /**
     * @brief 获取增益范围
     * @param[out] minVal 最小增益，单位dB
     * @param[out] maxVal 最大增益，单位dB
     */
    void getGainRange(double& minVal, double& maxVal) const;

    /**
     * @brief 获取图像宽度范围
     * @param[out] minVal 最小宽度，单位像素
     * @param[out] maxVal 最大宽度，单位像素
     */
    void getWidthRange(int& minVal, int& maxVal) const;

    /**
     * @brief 获取图像高度范围
     * @param[out] minVal 最小高度，单位像素
     * @param[out] maxVal 最大高度，单位像素
     */
    void getHeightRange(int& minVal, int& maxVal) const;

    /**
     * @brief 获取帧率范围
     * @param[out] minVal 最小帧率，单位fps
     * @param[out] maxVal 最大帧率，单位fps
     */
    void getFrameRateRange(double& minVal, double& maxVal) const;

    // ========== ROI设置 ==========

    /**
     * @brief 设置感兴趣区域
     * @details 设置ROI可以减少数据量，提高帧率
     * @param offsetX X方向偏移
     * @param offsetY Y方向偏移
     * @param width 图像宽度
     * @param height 图像高度
     * @return true表示设置成功
     */
    bool setROI(int offsetX, int offsetY, int width, int height);

    /**
     * @brief 重置ROI到最大范围
     * @details 将ROI设置为传感器全尺寸
     * @return true表示重置成功
     */
    bool resetROI();

    // ========== 数据流管理 ==========

    /**
     * @brief 配置数据流缓冲区
     * @details 调整缓冲区参数可以优化采集性能
     * @param config 缓冲区配置参数
     * @return true表示配置成功
     */
    bool configureBuffers(const BufferConfig& config);

    /**
     * @brief 获取数据流统计信息
     * @details 用于诊断传输问题和监控性能
     * @return 统计信息结构体
     */
    StreamStatistics getStreamStatistics() const;

    /**
     * @brief 重置数据流统计
     * @details 将所有统计计数器清零
     */
    void resetStreamStatistics();

    // ========== 设备信息 ==========

    /**
     * @brief 获取当前设备详细信息
     * @return 设备信息结构体
     */
    GenTLDeviceInfo getDeviceInfo() const;

    /**
     * @brief 获取传输层类型
     * @return 类型字符串，如"GEV"、"U3V"
     */
    QString getTransportLayerType() const;

    /**
     * @brief 检查是否为GigE相机
     * @return true表示是GigE Vision相机
     */
    bool isGigECamera() const;

    /**
     * @brief 检查是否为USB3相机
     * @return true表示是USB3 Vision相机
     */
    bool isUSB3Camera() const;

    // ========== GigE特有功能 ==========

    /**
     * @brief 配置GigE网络参数
     * @details 优化网络传输性能，减少丢包
     * @param packetSize 网络包大小（MTU），默认1500，巨型帧可设置为9000
     * @param interPacketDelay 包间延迟，单位时钟周期，增大可减少网卡负载
     * @return true表示配置成功
     */
    bool configureGigENetwork(int packetSize = 1500, int interPacketDelay = 0);

    /**
     * @brief 获取GigE网络统计
     * @details 获取网络传输相关的统计信息
     * @return 统计信息映射表
     */
    QMap<QString, QVariant> getGigEStatistics() const;

    /**
     * @brief 设置心跳超时
     * @details GigE相机需要定期心跳保持连接，超时会断开
     * @param timeoutMs 超时时间，单位毫秒，通常3000-60000
     * @return true表示设置成功
     */
    bool setHeartbeatTimeout(int timeoutMs);

    // ========== 参数持久化 ==========

    /**
     * @brief 保存当前参数到相机
     * @details 将当前参数保存到相机的用户设置集（UserSet）
     * @param setIndex 用户设置集索引，通常0-2
     * @return true表示保存成功
     */
    bool saveUserSet(int setIndex = 0);

    /**
     * @brief 从相机加载参数
     * @details 从相机的用户设置集加载保存的参数
     * @param setIndex 用户设置集索引
     * @return true表示加载成功
     */
    bool loadUserSet(int setIndex = 0);

    /**
     * @brief 设置默认启动配置
     * @details 设置相机上电时自动加载的用户设置集
     * @param setIndex 用户设置集索引
     * @return true表示设置成功
     */
    bool setDefaultUserSet(int setIndex);

    // ========== 错误处理 ==========

    /**
     * @brief 获取最后的错误信息
     * @return 错误描述字符串
     */
    QString getLastError() const;

    /**
     * @brief 获取最后的错误代码
     * @return 错误代码，0表示无错误
     */
    int getLastErrorCode() const;

signals:
    /**
     * @brief 设备断开连接信号
     * @details 当相机意外断开（如网线断开、USB拔出）时发出
     */
    void deviceDisconnected();

    /**
     * @brief 带宽警告信号
     * @details 当带宽使用率超过阈值时发出
     * @param usagePercent 带宽使用百分比
     */
    void bandwidthWarning(double usagePercent);

    /**
     * @brief 缓冲区不足警告
     * @details 当接收缓冲区不足导致丢帧风险时发出
     */
    void bufferUnderrun();

    /**
     * @brief 帧丢失信号
     * @details 当检测到帧丢失时发出
     * @param lostCount 本次丢失的帧数
     */
    void frameLost(int lostCount);

private:
    class Impl;
    std::unique_ptr<Impl> impl_;      ///< PIMPL实现

    mutable QMutex mutex_;            ///< 线程安全互斥锁
    Config config_;                   ///< 相机配置
    GenTLDeviceInfo deviceInfo_;      ///< 设备信息
    HardwareSyncConfig syncConfig_;   ///< 硬件同步配置
    ActionCommandConfig actionConfig_; ///< Action Command配置
    QString lastError_;               ///< 最后错误信息
    int lastErrorCode_ = 0;           ///< 最后错误代码

    bool isOpen_ = false;             ///< 打开状态标志
    bool isGrabbing_ = false;         ///< 采集状态标志
    int selectedDeviceIndex_ = -1;    ///< 选中的设备索引

    /**
     * @brief 获取标准节点名映射表
     * @details 将常用别名映射到GenICam标准节点名
     * @return 映射表引用
     */
    static const QMap<QString, QString>& getStandardNodeNames();

    /**
     * @brief 将别名映射到标准节点名
     * @param name 输入名称（可能是别名）
     * @return 标准GenICam节点名
     */
    QString mapToStandardName(const QString& name) const;

    // 内部辅助方法
    bool initializeGenTL();           ///< 初始化GenTL运行时
    void cleanupGenTL();              ///< 清理GenTL资源
    bool openDevice();                ///< 打开设备连接
    bool openDataStream();            ///< 打开数据流
    void closeDataStream();           ///< 关闭数据流
    bool allocateBuffers();           ///< 分配图像缓冲区
    void freeBuffers();               ///< 释放图像缓冲区
    Base::ImageData::Ptr convertToImageData(void* buffer, size_t size);  ///< 转换图像格式
    void updateConfig();              ///< 从相机同步配置
    void setLastError(const QString& error, int code = -1);  ///< 设置错误信息
};

} // namespace HAL
} // namespace VisionForge

#endif // USE_GENTL
