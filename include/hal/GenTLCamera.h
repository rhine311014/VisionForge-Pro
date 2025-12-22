/**
 * @file GenTLCamera.h
 * @brief GenICam Transport Layer 通用相机接口
 * @author VisionForge Team
 * @date 2025-12-22
 *
 * 基于GenTL标准实现的通用相机接口，支持所有符合GenICam标准的工业相机。
 * 通过加载厂商提供的GenTL Producer (.cti文件)，无需针对每个品牌编写特定代码。
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
 * @brief GenTL Producer 信息
 */
struct GenTLProducerInfo {
    QString filePath;           ///< .cti 文件路径
    QString vendorName;         ///< 厂商名称
    QString modelName;          ///< 模型名称
    QString version;            ///< 版本号
    QString tlType;             ///< 传输层类型 (GigE, USB3, CameraLink等)
    bool isLoaded = false;      ///< 是否已加载

    GenTLProducerInfo() = default;
};

/**
 * @brief GenTL 设备信息
 */
struct GenTLDeviceInfo {
    QString deviceId;           ///< 设备唯一ID
    QString vendorName;         ///< 厂商名称
    QString modelName;          ///< 型号名称
    QString serialNumber;       ///< 序列号
    QString userDefinedName;    ///< 用户定义名称
    QString interfaceId;        ///< 接口ID
    QString producerPath;       ///< 所属Producer路径
    QString accessStatus;       ///< 访问状态 (ReadWrite/ReadOnly/NoAccess)
    QString tlType;             ///< 传输层类型

    // GigE 特有属性
    QString ipAddress;          ///< IP地址（GigE相机）
    QString subnetMask;         ///< 子网掩码
    QString gateway;            ///< 网关
    QString macAddress;         ///< MAC地址

    GenTLDeviceInfo() = default;
};

/**
 * @brief GenICam 节点类型
 */
enum class GenICamNodeType {
    Unknown,
    Integer,            ///< 整数节点
    Float,              ///< 浮点节点
    Boolean,            ///< 布尔节点
    String,             ///< 字符串节点
    Enumeration,        ///< 枚举节点
    Command,            ///< 命令节点
    Category,           ///< 类别节点
    Register            ///< 寄存器节点
};

/**
 * @brief GenICam 节点信息
 */
struct GenICamNodeInfo {
    QString name;               ///< 节点名称
    QString displayName;        ///< 显示名称
    QString description;        ///< 描述
    QString tooltip;            ///< 提示信息
    GenICamNodeType type;       ///< 节点类型
    bool isReadable = true;     ///< 是否可读
    bool isWritable = true;     ///< 是否可写
    bool isImplemented = true;  ///< 是否已实现
    bool isAvailable = true;    ///< 是否可用
    QString visibility;         ///< 可见性 (Beginner/Expert/Guru/Invisible)

    // 数值范围（Integer/Float节点）
    double minValue = 0;
    double maxValue = 0;
    double increment = 1;

    // 枚举选项（Enumeration节点）
    QStringList enumEntries;
    QString currentEntry;

    GenICamNodeInfo() : type(GenICamNodeType::Unknown) {}
};

/**
 * @brief 数据流统计信息
 */
struct StreamStatistics {
    uint64_t deliveredFrameCount = 0;   ///< 已传输帧数
    uint64_t lostFrameCount = 0;        ///< 丢失帧数
    uint64_t resendRequestCount = 0;    ///< 重传请求数（GigE）
    uint64_t resendPacketCount = 0;     ///< 重传包数
    uint64_t incompleteFrameCount = 0;  ///< 不完整帧数
    double frameRate = 0.0;             ///< 当前帧率
    double bandwidth = 0.0;             ///< 带宽使用 (MB/s)
    uint64_t bufferUnderrunCount = 0;   ///< 缓冲区不足次数

    StreamStatistics() = default;
};

/**
 * @brief GenTL 缓冲区配置
 */
struct BufferConfig {
    int bufferCount = 10;           ///< 缓冲区数量
    size_t bufferSize = 0;          ///< 单个缓冲区大小（0=自动）
    bool useAnnounceBuffer = true;  ///< 使用预分配缓冲区
    int payloadSize = 0;            ///< 有效负载大小

    BufferConfig() = default;
};

// ============================================================
// GenTL Producer 管理器
// ============================================================

/**
 * @brief GenTL Producer 管理器
 *
 * 单例模式，负责发现和管理系统中的GenTL Producer (.cti文件)
 */
class GenTLProducerManager {
public:
    /**
     * @brief 获取单例实例
     */
    static GenTLProducerManager& instance();

    /**
     * @brief 扫描系统中的GenTL Producer
     * @param additionalPaths 额外的搜索路径
     * @return 发现的Producer数量
     */
    int discoverProducers(const QStringList& additionalPaths = QStringList());

    /**
     * @brief 获取所有发现的Producer信息
     */
    QList<GenTLProducerInfo> getProducers() const;

    /**
     * @brief 加载指定的Producer
     * @param ctiPath .cti文件路径
     * @return true如果加载成功
     */
    bool loadProducer(const QString& ctiPath);

    /**
     * @brief 卸载指定的Producer
     * @param ctiPath .cti文件路径
     */
    void unloadProducer(const QString& ctiPath);

    /**
     * @brief 卸载所有Producer
     */
    void unloadAllProducers();

    /**
     * @brief 枚举所有已加载Producer的设备
     * @return 设备信息列表
     */
    QList<GenTLDeviceInfo> enumerateDevices();

    /**
     * @brief 获取GenTL版本信息
     */
    QString getGenTLVersion() const;

    /**
     * @brief 设置环境变量搜索路径
     */
    void setEnvironmentPath(const QString& path);

private:
    GenTLProducerManager();
    ~GenTLProducerManager();
    GenTLProducerManager(const GenTLProducerManager&) = delete;
    GenTLProducerManager& operator=(const GenTLProducerManager&) = delete;

    class Impl;
    std::unique_ptr<Impl> impl_;
};

// ============================================================
// GenTL 通用相机类
// ============================================================

/**
 * @brief GenTL 通用相机类
 *
 * 基于GenICam Transport Layer标准实现的通用相机接口。
 * 支持所有符合GenICam标准的工业相机（约95%市面产品）。
 *
 * 特性：
 * - 运行时发现相机设备
 * - 通用GenICam参数访问
 * - 支持GigE Vision、USB3 Vision、Camera Link等接口
 * - Action Command多相机同步
 * - 完整的错误处理和诊断
 *
 * @code
 * // 使用示例
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
    explicit GenTLCamera(QObject* parent = nullptr);
    ~GenTLCamera() override;

    // ========== 静态方法 ==========

    /**
     * @brief 枚举所有可用的GenTL设备
     * @param refreshProducers 是否重新扫描Producer
     * @return 设备信息列表
     */
    static QList<GenTLDeviceInfo> enumerateDevices(bool refreshProducers = false);

    /**
     * @brief 获取GenTL SDK版本
     */
    static QString sdkVersion();

    /**
     * @brief 获取支持的传输层类型
     */
    static QStringList supportedTransportLayers();

    // ========== 设备选择 ==========

    /**
     * @brief 通过索引选择设备
     * @param index 设备索引
     */
    void selectDevice(int index);

    /**
     * @brief 通过设备ID选择
     * @param deviceId GenTL设备ID
     * @return true如果找到设备
     */
    bool selectByDeviceId(const QString& deviceId);

    /**
     * @brief 通过序列号选择
     * @param serialNumber 设备序列号
     * @return true如果找到设备
     */
    bool selectBySerialNumber(const QString& serialNumber);

    /**
     * @brief 通过IP地址选择（GigE相机）
     * @param ipAddress IP地址
     * @return true如果找到设备
     */
    bool selectByIP(const QString& ipAddress);

    /**
     * @brief 通过用户定义名称选择
     * @param userName 用户定义的设备名称
     * @return true如果找到设备
     */
    bool selectByUserName(const QString& userName);

    // ========== ICamera 接口实现 ==========

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

    void setTriggerModeEx(TriggerModeEx mode) override;
    TriggerModeEx getTriggerModeEx() const override;
    bool configureActionCommand(const ActionCommandConfig& config) override;
    ActionCommandConfig getActionCommandConfig() const override;
    bool supportsActionCommand() const override;
    bool configureHardwareSync(const HardwareSyncConfig& config) override;
    HardwareSyncConfig getHardwareSyncConfig() const override;
    bool supportsHardwareSync() const override;
    bool waitForActionTrigger(int timeoutMs = 5000) override;

    // ========== GenICam 通用参数接口 ==========

    /**
     * @brief 设置GenICam特性值
     * @param featureName GenICam节点名（如 "ExposureTime", "Gain", "Width"）
     * @param value 特性值（自动类型转换）
     * @return true如果设置成功
     */
    bool setFeature(const QString& featureName, const QVariant& value);

    /**
     * @brief 获取GenICam特性值
     * @param featureName GenICam节点名
     * @return 特性值，失败返回无效QVariant
     */
    QVariant getFeature(const QString& featureName) const;

    /**
     * @brief 获取特性节点信息
     * @param featureName GenICam节点名
     * @return 节点详细信息
     */
    GenICamNodeInfo getFeatureInfo(const QString& featureName) const;

    /**
     * @brief 枚举所有可用的GenICam特性
     * @param category 类别过滤（空=全部）
     * @return 特性名列表
     */
    QStringList enumerateFeatures(const QString& category = QString()) const;

    /**
     * @brief 执行GenICam命令
     * @param commandName 命令节点名（如 "AcquisitionStart", "TriggerSoftware"）
     * @return true如果执行成功
     */
    bool executeCommand(const QString& commandName);

    /**
     * @brief 检查特性是否可用
     */
    bool isFeatureAvailable(const QString& featureName) const;

    /**
     * @brief 检查特性是否可写
     */
    bool isFeatureWritable(const QString& featureName) const;

    // ========== 参数范围查询 ==========

    /**
     * @brief 获取曝光范围
     */
    void getExposureRange(double& minVal, double& maxVal) const;

    /**
     * @brief 获取增益范围
     */
    void getGainRange(double& minVal, double& maxVal) const;

    /**
     * @brief 获取宽度范围
     */
    void getWidthRange(int& minVal, int& maxVal) const;

    /**
     * @brief 获取高度范围
     */
    void getHeightRange(int& minVal, int& maxVal) const;

    /**
     * @brief 获取帧率范围
     */
    void getFrameRateRange(double& minVal, double& maxVal) const;

    // ========== ROI 设置 ==========

    /**
     * @brief 设置感兴趣区域
     * @return true如果设置成功
     */
    bool setROI(int offsetX, int offsetY, int width, int height);

    /**
     * @brief 重置ROI到最大范围
     */
    bool resetROI();

    // ========== 数据流管理 ==========

    /**
     * @brief 配置数据流缓冲区
     */
    bool configureBuffers(const BufferConfig& config);

    /**
     * @brief 获取数据流统计信息
     */
    StreamStatistics getStreamStatistics() const;

    /**
     * @brief 重置数据流统计
     */
    void resetStreamStatistics();

    // ========== 设备信息 ==========

    /**
     * @brief 获取当前设备详细信息
     */
    GenTLDeviceInfo getDeviceInfo() const;

    /**
     * @brief 获取传输层类型
     */
    QString getTransportLayerType() const;

    /**
     * @brief 检查是否为GigE相机
     */
    bool isGigECamera() const;

    /**
     * @brief 检查是否为USB3相机
     */
    bool isUSB3Camera() const;

    // ========== GigE 特有功能 ==========

    /**
     * @brief 配置GigE网络参数
     * @param packetSize 网络包大小（MTU）
     * @param interPacketDelay 包间延迟
     * @return true如果配置成功
     */
    bool configureGigENetwork(int packetSize = 1500, int interPacketDelay = 0);

    /**
     * @brief 获取GigE网络统计
     */
    QMap<QString, QVariant> getGigEStatistics() const;

    /**
     * @brief 设置心跳超时
     */
    bool setHeartbeatTimeout(int timeoutMs);

    // ========== 参数持久化 ==========

    /**
     * @brief 保存当前参数到相机
     * @param setIndex 用户设置集索引（0-2）
     * @return true如果保存成功
     */
    bool saveUserSet(int setIndex = 0);

    /**
     * @brief 从相机加载参数
     * @param setIndex 用户设置集索引（0-2）
     * @return true如果加载成功
     */
    bool loadUserSet(int setIndex = 0);

    /**
     * @brief 设置默认启动配置
     * @param setIndex 用户设置集索引（0-2）
     */
    bool setDefaultUserSet(int setIndex);

    // ========== 错误处理 ==========

    /**
     * @brief 获取最后的错误信息
     */
    QString getLastError() const;

    /**
     * @brief 获取错误代码
     */
    int getLastErrorCode() const;

signals:
    /**
     * @brief 设备断开连接信号
     */
    void deviceDisconnected();

    /**
     * @brief 带宽警告信号
     * @param usagePercent 带宽使用百分比
     */
    void bandwidthWarning(double usagePercent);

    /**
     * @brief 缓冲区不足警告
     */
    void bufferUnderrun();

    /**
     * @brief 帧丢失信号
     * @param lostCount 丢失帧数
     */
    void frameLost(int lostCount);

private:
    class Impl;
    std::unique_ptr<Impl> impl_;

    mutable QMutex mutex_;
    Config config_;
    GenTLDeviceInfo deviceInfo_;
    HardwareSyncConfig syncConfig_;
    ActionCommandConfig actionConfig_;
    QString lastError_;
    int lastErrorCode_ = 0;

    bool isOpen_ = false;
    bool isGrabbing_ = false;
    int selectedDeviceIndex_ = -1;

    // GenICam 标准节点名映射
    static const QMap<QString, QString>& getStandardNodeNames();
    QString mapToStandardName(const QString& name) const;

    // 内部辅助方法
    bool initializeGenTL();
    void cleanupGenTL();
    bool openDevice();
    bool openDataStream();
    void closeDataStream();
    bool allocateBuffers();
    void freeBuffers();
    Base::ImageData::Ptr convertToImageData(void* buffer, size_t size);
    void updateConfig();
    void setLastError(const QString& error, int code = -1);
};

} // namespace HAL
} // namespace VisionForge

#endif // USE_GENTL
