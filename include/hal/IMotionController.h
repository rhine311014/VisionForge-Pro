/**
 * @file IMotionController.h
 * @brief 运动控制器抽象接口
 * @details 定义统一的运动控制接口，支持XY台、转台等运动设备
 * @author VisionForge Team
 * @date 2025-12-20
 */

#pragma once

#include <QObject>
#include <QString>
#include <QPointF>
#include <QThread>
#include <QVector>

namespace VisionForge {
namespace HAL {

/**
 * @brief 运动模式
 */
enum class MoveMode {
    Absolute,       ///< 绝对运动
    Relative        ///< 相对运动
};

/**
 * @brief 运动状态
 */
enum class MotionState {
    Idle,           ///< 空闲
    Moving,         ///< 运动中
    Homing,         ///< 回原点中
    Error,          ///< 错误
    Disabled        ///< 禁用
};

/**
 * @brief 轴类型
 */
enum class AxisType {
    X,              ///< X轴
    Y,              ///< Y轴
    Z,              ///< Z轴
    Theta,          ///< 旋转轴
    Custom          ///< 自定义轴
};

/**
 * @brief 轴配置
 */
struct AxisConfig {
    QString name;               ///< 轴名称
    AxisType type;              ///< 轴类型
    double minPosition;         ///< 最小位置
    double maxPosition;         ///< 最大位置
    double maxVelocity;         ///< 最大速度
    double maxAcceleration;     ///< 最大加速度
    double homePosition;        ///< 原点位置
    double homingVelocity;      ///< 回原点速度
    bool invertDirection;       ///< 反向
    double unitsPerPulse;       ///< 每脉冲单位

    AxisConfig()
        : type(AxisType::X), minPosition(-1000), maxPosition(1000),
          maxVelocity(100), maxAcceleration(500), homePosition(0),
          homingVelocity(10), invertDirection(false), unitsPerPulse(0.001) {}
};

/**
 * @brief 运动控制器配置
 */
struct MotionControllerConfig {
    QString controllerType;     ///< 控制器类型
    QString connectionString;   ///< 连接字符串（COM口、IP地址等）
    int axisCount;              ///< 轴数量
    double defaultVelocity;     ///< 默认速度
    double defaultAcceleration; ///< 默认加速度
    int settleTimeMs;           ///< 稳定时间(毫秒)

    MotionControllerConfig()
        : axisCount(2), defaultVelocity(50), defaultAcceleration(200),
          settleTimeMs(100) {}
};

/**
 * @class IMotionController
 * @brief 运动控制器抽象接口
 *
 * 定义统一的运动控制接口，用于：
 * - XY精密运动台控制
 * - 旋转对位台控制
 * - 多轴协调运动
 */
class IMotionController : public QObject
{
    Q_OBJECT

public:
    explicit IMotionController(QObject* parent = nullptr) : QObject(parent) {}
    virtual ~IMotionController() = default;

    // ========== 连接管理 ==========

    /**
     * @brief 连接控制器
     * @param config 配置参数
     * @return 是否成功
     */
    virtual bool connect(const MotionControllerConfig& config) = 0;

    /**
     * @brief 断开连接
     */
    virtual void disconnect() = 0;

    /**
     * @brief 检查是否已连接
     */
    virtual bool isConnected() const = 0;

    // ========== 轴管理 ==========

    /**
     * @brief 获取轴数量
     */
    virtual int axisCount() const = 0;

    /**
     * @brief 配置轴
     * @param axisIndex 轴索引
     * @param config 轴配置
     * @return 是否成功
     */
    virtual bool configureAxis(int axisIndex, const AxisConfig& config) = 0;

    /**
     * @brief 获取轴配置
     */
    virtual AxisConfig getAxisConfig(int axisIndex) const = 0;

    /**
     * @brief 使能轴
     * @param axisIndex 轴索引
     * @param enable 是否使能
     */
    virtual bool enableAxis(int axisIndex, bool enable) = 0;

    /**
     * @brief 使能所有轴
     */
    virtual bool enableAllAxes(bool enable) = 0;

    // ========== 单轴运动 ==========

    /**
     * @brief 移动单轴
     * @param axisIndex 轴索引
     * @param position 目标位置
     * @param mode 运动模式
     * @param velocity 速度（0使用默认）
     * @return 是否成功开始运动
     */
    virtual bool moveAxis(int axisIndex, double position, MoveMode mode, double velocity = 0) = 0;

    /**
     * @brief 获取轴当前位置
     */
    virtual double getAxisPosition(int axisIndex) const = 0;

    /**
     * @brief 获取轴状态
     */
    virtual MotionState getAxisState(int axisIndex) const = 0;

    /**
     * @brief 轴回原点
     */
    virtual bool homeAxis(int axisIndex) = 0;

    /**
     * @brief 停止轴运动
     */
    virtual bool stopAxis(int axisIndex) = 0;

    // ========== XY运动（常用便捷接口）==========

    /**
     * @brief XY移动
     * @param x X坐标
     * @param y Y坐标
     * @param mode 运动模式
     * @return 是否成功开始运动
     */
    virtual bool moveXY(double x, double y, MoveMode mode) = 0;

    /**
     * @brief 旋转运动
     * @param angle 角度
     * @param mode 运动模式
     * @return 是否成功开始运动
     */
    virtual bool rotate(double angle, MoveMode mode) = 0;

    /**
     * @brief XYTheta联动
     * @param x X坐标
     * @param y Y坐标
     * @param theta 角度
     * @param mode 运动模式
     * @return 是否成功开始运动
     */
    virtual bool moveXYTheta(double x, double y, double theta, MoveMode mode) {
        if (!moveXY(x, y, mode)) return false;
        return rotate(theta, mode);
    }

    /**
     * @brief 获取当前位置
     * @param x 输出X坐标
     * @param y 输出Y坐标
     * @param theta 输出角度
     * @return 是否成功
     */
    virtual bool getCurrentPosition(double& x, double& y, double& theta) = 0;

    // ========== 等待和同步 ==========

    /**
     * @brief 等待运动完成
     * @param timeoutMs 超时时间
     * @return 是否运动完成
     */
    virtual bool waitMotionComplete(int timeoutMs = 5000) = 0;

    /**
     * @brief 等待单轴运动完成
     */
    virtual bool waitAxisComplete(int axisIndex, int timeoutMs = 5000) = 0;

    /**
     * @brief 检查是否正在运动
     */
    virtual bool isMoving() const = 0;

    /**
     * @brief 检查单轴是否正在运动
     */
    virtual bool isAxisMoving(int axisIndex) const = 0;

    /**
     * @brief 等待稳定
     * @param settleTimeMs 稳定时间
     */
    virtual void waitSettle(int settleTimeMs = 100) {
        QThread::msleep(settleTimeMs);
    }

    // ========== 控制命令 ==========

    /**
     * @brief 停止所有运动
     */
    virtual bool stopAll() = 0;

    /**
     * @brief 急停
     */
    virtual bool emergencyStop() = 0;

    /**
     * @brief 所有轴回原点
     */
    virtual bool homeAll() = 0;

    /**
     * @brief 清除错误
     */
    virtual bool clearError() = 0;

    // ========== 速度和参数设置 ==========

    /**
     * @brief 设置默认速度
     */
    virtual void setDefaultVelocity(double velocity) = 0;

    /**
     * @brief 获取默认速度
     */
    virtual double getDefaultVelocity() const = 0;

    /**
     * @brief 设置默认加速度
     */
    virtual void setDefaultAcceleration(double acceleration) = 0;

    /**
     * @brief 获取默认加速度
     */
    virtual double getDefaultAcceleration() const = 0;

signals:
    /**
     * @brief 连接状态改变
     */
    void connectionChanged(bool connected);

    /**
     * @brief 运动状态改变
     */
    void motionStateChanged(int axisIndex, MotionState state);

    /**
     * @brief 位置更新
     */
    void positionUpdated(int axisIndex, double position);

    /**
     * @brief 运动完成
     */
    void motionCompleted(int axisIndex);

    /**
     * @brief 原点已找到
     */
    void homeCompleted(int axisIndex);

    /**
     * @brief 错误发生
     */
    void errorOccurred(const QString& error);

    /**
     * @brief 限位触发
     */
    void limitTriggered(int axisIndex, bool isPositive);
};

/**
 * @class SimulatedMotionController
 * @brief 模拟运动控制器（用于测试）
 */
class SimulatedMotionController : public IMotionController
{
    Q_OBJECT

public:
    explicit SimulatedMotionController(QObject* parent = nullptr);
    ~SimulatedMotionController() override = default;

    // 实现所有接口
    bool connect(const MotionControllerConfig& config) override;
    void disconnect() override;
    bool isConnected() const override;

    int axisCount() const override;
    bool configureAxis(int axisIndex, const AxisConfig& config) override;
    AxisConfig getAxisConfig(int axisIndex) const override;
    bool enableAxis(int axisIndex, bool enable) override;
    bool enableAllAxes(bool enable) override;

    bool moveAxis(int axisIndex, double position, MoveMode mode, double velocity = 0) override;
    double getAxisPosition(int axisIndex) const override;
    MotionState getAxisState(int axisIndex) const override;
    bool homeAxis(int axisIndex) override;
    bool stopAxis(int axisIndex) override;

    bool moveXY(double x, double y, MoveMode mode) override;
    bool rotate(double angle, MoveMode mode) override;
    bool getCurrentPosition(double& x, double& y, double& theta) override;

    bool waitMotionComplete(int timeoutMs = 5000) override;
    bool waitAxisComplete(int axisIndex, int timeoutMs = 5000) override;
    bool isMoving() const override;
    bool isAxisMoving(int axisIndex) const override;

    bool stopAll() override;
    bool emergencyStop() override;
    bool homeAll() override;
    bool clearError() override;

    void setDefaultVelocity(double velocity) override;
    double getDefaultVelocity() const override;
    void setDefaultAcceleration(double acceleration) override;
    double getDefaultAcceleration() const override;

private:
    bool connected_;
    MotionControllerConfig config_;
    QVector<AxisConfig> axisConfigs_;
    QVector<double> positions_;
    QVector<MotionState> states_;
    QVector<bool> enabled_;
    double defaultVelocity_;
    double defaultAcceleration_;
};

} // namespace HAL
} // namespace VisionForge
