/**
 * @file CameraManager.h
 * @brief 多相机管理器
 * @details 管理多相机的创建、连接、分组和采集
 * @author VisionForge Team
 * @date 2025-12-19
 */

#pragma once

#include "hal/ICamera.h"
#include "hal/CameraFactory.h"
#include "base/ImageData.h"
#include <QObject>
#include <QMap>
#include <QMutex>
#include <QTimer>
#include <memory>
#include <functional>

namespace VisionForge {
namespace HAL {

/**
 * @brief 相机采集模式
 */
enum class CaptureMode {
    Simultaneous,    ///< 同时采集：所有相机同时触发
    Sequential,      ///< 顺序采集：按序依次采集
    Independent      ///< 独立采集：各相机独立工作
};

/**
 * @brief 相机信息结构
 */
struct CameraInfo {
    QString id;                         ///< 相机唯一ID
    QString name;                       ///< 显示名称
    QString groupId;                    ///< 所属分组ID
    CameraFactory::CameraType type;     ///< 相机类型
    GenericDeviceInfo deviceInfo;       ///< 设备信息
    ICamera::Config config;             ///< 相机配置
    bool enabled = true;                ///< 是否启用

    CameraInfo() : type(CameraFactory::Simulated), enabled(true) {}
};

/**
 * @brief 相机组结构
 */
struct CameraGroup {
    QString id;                         ///< 组ID
    QString name;                       ///< 组名称
    QStringList cameraIds;              ///< 组内相机ID列表
    CaptureMode captureMode;            ///< 采集模式
    int captureDelayMs = 0;             ///< 采集延迟(顺序模式用)

    CameraGroup() : captureMode(CaptureMode::Simultaneous), captureDelayMs(0) {}
};

/**
 * @brief 单相机采集结果
 */
struct CaptureResult {
    QString cameraId;                   ///< 相机ID
    Base::ImageData::Ptr image;         ///< 采集的图像
    bool success = false;               ///< 是否成功
    QString errorMessage;               ///< 错误信息
    qint64 timestamp;                   ///< 时间戳
    double captureTime = 0.0;           ///< 采集耗时(ms)

    CaptureResult() : success(false), timestamp(0), captureTime(0.0) {}
};

/**
 * @brief 批量采集结果
 */
struct BatchCaptureResult {
    QMap<QString, CaptureResult> results;  ///< 各相机采集结果
    bool allSuccess = false;               ///< 是否全部成功
    int successCount = 0;                  ///< 成功数量
    int failCount = 0;                     ///< 失败数量
    double totalTime = 0.0;                ///< 总耗时(ms)

    BatchCaptureResult() : allSuccess(false), successCount(0), failCount(0), totalTime(0.0) {}
};

/**
 * @class CameraManager
 * @brief 多相机管理器（单例）
 *
 * 功能：
 * - 管理多个相机的生命周期
 * - 支持相机分组
 * - 支持同时/顺序/独立采集模式
 * - 提供统一的采集接口
 */
class CameraManager : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 获取单例实例
     */
    static CameraManager& instance();

    // ========== 相机管理 ==========

    /**
     * @brief 添加相机
     * @param info 相机信息
     * @return 是否成功
     */
    bool addCamera(const CameraInfo& info);

    /**
     * @brief 移除相机
     * @param cameraId 相机ID
     * @return 是否成功
     */
    bool removeCamera(const QString& cameraId);

    /**
     * @brief 获取相机实例
     * @param cameraId 相机ID
     * @return 相机指针，不存在返回nullptr
     */
    ICamera* getCamera(const QString& cameraId);

    /**
     * @brief 获取相机信息
     * @param cameraId 相机ID
     * @return 相机信息
     */
    CameraInfo getCameraInfo(const QString& cameraId) const;

    /**
     * @brief 更新相机信息
     * @param cameraId 相机ID
     * @param info 新的相机信息
     * @return 是否成功
     */
    bool updateCameraInfo(const QString& cameraId, const CameraInfo& info);

    /**
     * @brief 获取所有相机ID列表
     */
    QStringList getAllCameraIds() const;

    /**
     * @brief 获取相机数量
     */
    int cameraCount() const;

    /**
     * @brief 检查相机是否存在
     */
    bool hasCamera(const QString& cameraId) const;

    // ========== 相机分组 ==========

    /**
     * @brief 创建相机组
     * @param group 组信息
     * @return 是否成功
     */
    bool createGroup(const CameraGroup& group);

    /**
     * @brief 删除相机组
     * @param groupId 组ID
     * @return 是否成功
     */
    bool deleteGroup(const QString& groupId);

    /**
     * @brief 获取相机组
     * @param groupId 组ID
     * @return 组信息
     */
    CameraGroup getGroup(const QString& groupId) const;

    /**
     * @brief 更新相机组
     * @param groupId 组ID
     * @param group 新的组信息
     * @return 是否成功
     */
    bool updateGroup(const QString& groupId, const CameraGroup& group);

    /**
     * @brief 获取所有分组
     */
    QList<CameraGroup> getAllGroups() const;

    /**
     * @brief 获取所有分组ID
     */
    QStringList getAllGroupIds() const;

    /**
     * @brief 将相机添加到组
     * @param cameraId 相机ID
     * @param groupId 组ID
     * @return 是否成功
     */
    bool addCameraToGroup(const QString& cameraId, const QString& groupId);

    /**
     * @brief 从组中移除相机
     * @param cameraId 相机ID
     * @param groupId 组ID
     * @return 是否成功
     */
    bool removeCameraFromGroup(const QString& cameraId, const QString& groupId);

    /**
     * @brief 检查组是否存在
     */
    bool hasGroup(const QString& groupId) const;

    // ========== 连接管理 ==========

    /**
     * @brief 连接指定相机
     * @param cameraId 相机ID
     * @return 是否成功
     */
    bool connectCamera(const QString& cameraId);

    /**
     * @brief 断开指定相机
     * @param cameraId 相机ID
     */
    void disconnectCamera(const QString& cameraId);

    /**
     * @brief 连接组内所有相机
     * @param groupId 组ID
     * @return 是否全部成功
     */
    bool connectGroup(const QString& groupId);

    /**
     * @brief 断开组内所有相机
     * @param groupId 组ID
     */
    void disconnectGroup(const QString& groupId);

    /**
     * @brief 连接所有相机
     */
    void connectAll();

    /**
     * @brief 断开所有相机
     */
    void disconnectAll();

    /**
     * @brief 检查相机连接状态
     * @param cameraId 相机ID
     * @return 是否已连接
     */
    bool isCameraConnected(const QString& cameraId) const;

    /**
     * @brief 获取已连接的相机数量
     */
    int connectedCameraCount() const;

    // ========== 采集控制 ==========

    /**
     * @brief 单相机采集
     * @param cameraId 相机ID
     * @param timeoutMs 超时时间
     * @return 采集结果
     */
    CaptureResult capture(const QString& cameraId, int timeoutMs = 1000);

    /**
     * @brief 组采集（根据组的采集模式）
     * @param groupId 组ID
     * @param timeoutMs 超时时间
     * @return 批量采集结果
     */
    BatchCaptureResult captureGroup(const QString& groupId, int timeoutMs = 3000);

    /**
     * @brief 同时采集多个相机
     * @param cameraIds 相机ID列表
     * @param timeoutMs 超时时间
     * @return 批量采集结果
     */
    BatchCaptureResult captureSimultaneous(const QStringList& cameraIds, int timeoutMs = 3000);

    /**
     * @brief 顺序采集多个相机
     * @param cameraIds 相机ID列表
     * @param delayMs 每次采集间延迟
     * @param timeoutMs 单次超时时间
     * @return 批量采集结果
     */
    BatchCaptureResult captureSequential(const QStringList& cameraIds,
                                         int delayMs = 0,
                                         int timeoutMs = 1000);

    /**
     * @brief 采集所有相机
     * @param mode 采集模式
     * @param timeoutMs 超时时间
     * @return 批量采集结果
     */
    BatchCaptureResult captureAll(CaptureMode mode = CaptureMode::Simultaneous,
                                  int timeoutMs = 3000);

    // ========== 连续采集 ==========

    /**
     * @brief 开始连续采集（组）
     * @param groupId 组ID
     * @param fps 帧率
     * @return 是否成功
     */
    bool startContinuousCapture(const QString& groupId, double fps = 10.0);

    /**
     * @brief 停止连续采集
     * @param groupId 组ID
     */
    void stopContinuousCapture(const QString& groupId);

    /**
     * @brief 检查是否正在连续采集
     * @param groupId 组ID
     */
    bool isContinuousCapturing(const QString& groupId) const;

    // ========== 配置管理 ==========

    /**
     * @brief 保存配置到文件
     * @param filePath 文件路径
     * @return 是否成功
     */
    bool saveConfig(const QString& filePath);

    /**
     * @brief 从文件加载配置
     * @param filePath 文件路径
     * @return 是否成功
     */
    bool loadConfig(const QString& filePath);

    /**
     * @brief 清空所有相机和分组
     */
    void clear();

signals:
    // 相机事件
    void cameraAdded(const QString& cameraId);
    void cameraRemoved(const QString& cameraId);
    void cameraConnected(const QString& cameraId);
    void cameraDisconnected(const QString& cameraId);
    void cameraError(const QString& cameraId, const QString& error);
    void cameraInfoChanged(const QString& cameraId);

    // 采集事件
    void captureStarted(const QString& cameraId);
    void captureCompleted(const QString& cameraId, Base::ImageData::Ptr image);
    void captureFailed(const QString& cameraId, const QString& error);
    void batchCaptureCompleted(const BatchCaptureResult& result);

    // 组事件
    void groupCreated(const QString& groupId);
    void groupDeleted(const QString& groupId);
    void groupChanged(const QString& groupId);

    // 连续采集事件
    void continuousCaptureStarted(const QString& groupId);
    void continuousCaptureStopped(const QString& groupId);

private:
    CameraManager();
    ~CameraManager();
    CameraManager(const CameraManager&) = delete;
    CameraManager& operator=(const CameraManager&) = delete;

    QString generateCameraId();
    QString generateGroupId();
    ICamera* createCameraInstance(const CameraInfo& info);

private slots:
    void onContinuousCaptureTimer();

private:
    QMap<QString, std::unique_ptr<ICamera>> cameras_;       ///< 相机实例
    QMap<QString, CameraInfo> cameraInfos_;                 ///< 相机信息
    QMap<QString, CameraGroup> groups_;                     ///< 相机组
    QMap<QString, QTimer*> continuousTimers_;               ///< 连续采集定时器
    mutable QMutex mutex_;
    int cameraIdCounter_ = 0;
    int groupIdCounter_ = 0;
};

} // namespace HAL
} // namespace VisionForge
