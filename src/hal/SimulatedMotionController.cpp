/**
 * @file SimulatedMotionController.cpp
 * @brief 模拟运动控制器实现
 * @author VisionForge Team
 * @date 2025-12-20
 */

#include "hal/IMotionController.h"
#include <QThread>
#include <cmath>

namespace VisionForge {
namespace HAL {

SimulatedMotionController::SimulatedMotionController(QObject* parent)
    : IMotionController(parent)
    , connected_(false)
    , defaultVelocity_(50.0)
    , defaultAcceleration_(200.0)
{
}

bool SimulatedMotionController::connect(const MotionControllerConfig& config)
{
    config_ = config;

    // 初始化轴
    axisConfigs_.resize(config.axisCount);
    positions_.resize(config.axisCount);
    states_.resize(config.axisCount);
    enabled_.resize(config.axisCount);

    for (int i = 0; i < config.axisCount; ++i) {
        positions_[i] = 0.0;
        states_[i] = MotionState::Idle;
        enabled_[i] = false;
    }

    defaultVelocity_ = config.defaultVelocity;
    defaultAcceleration_ = config.defaultAcceleration;

    connected_ = true;
    emit connectionChanged(true);

    return true;
}

void SimulatedMotionController::disconnect()
{
    connected_ = false;
    emit connectionChanged(false);
}

bool SimulatedMotionController::isConnected() const
{
    return connected_;
}

int SimulatedMotionController::axisCount() const
{
    return config_.axisCount;
}

bool SimulatedMotionController::configureAxis(int axisIndex, const AxisConfig& config)
{
    if (axisIndex < 0 || axisIndex >= axisConfigs_.size()) {
        return false;
    }
    axisConfigs_[axisIndex] = config;
    return true;
}

AxisConfig SimulatedMotionController::getAxisConfig(int axisIndex) const
{
    if (axisIndex < 0 || axisIndex >= axisConfigs_.size()) {
        return AxisConfig();
    }
    return axisConfigs_[axisIndex];
}

bool SimulatedMotionController::enableAxis(int axisIndex, bool enable)
{
    if (axisIndex < 0 || axisIndex >= enabled_.size()) {
        return false;
    }
    enabled_[axisIndex] = enable;
    states_[axisIndex] = enable ? MotionState::Idle : MotionState::Disabled;
    emit motionStateChanged(axisIndex, states_[axisIndex]);
    return true;
}

bool SimulatedMotionController::enableAllAxes(bool enable)
{
    for (int i = 0; i < enabled_.size(); ++i) {
        enableAxis(i, enable);
    }
    return true;
}

bool SimulatedMotionController::moveAxis(int axisIndex, double position, MoveMode mode, double velocity)
{
    if (axisIndex < 0 || axisIndex >= positions_.size()) {
        return false;
    }

    if (!enabled_[axisIndex]) {
        return false;
    }

    double targetPos = position;
    if (mode == MoveMode::Relative) {
        targetPos = positions_[axisIndex] + position;
    }

    // 模拟运动
    states_[axisIndex] = MotionState::Moving;
    emit motionStateChanged(axisIndex, states_[axisIndex]);

    // 直接设置位置（模拟瞬时完成）
    positions_[axisIndex] = targetPos;
    emit positionUpdated(axisIndex, targetPos);

    states_[axisIndex] = MotionState::Idle;
    emit motionStateChanged(axisIndex, states_[axisIndex]);
    emit motionCompleted(axisIndex);

    return true;
}

double SimulatedMotionController::getAxisPosition(int axisIndex) const
{
    if (axisIndex < 0 || axisIndex >= positions_.size()) {
        return 0.0;
    }
    return positions_[axisIndex];
}

MotionState SimulatedMotionController::getAxisState(int axisIndex) const
{
    if (axisIndex < 0 || axisIndex >= states_.size()) {
        return MotionState::Error;
    }
    return states_[axisIndex];
}

bool SimulatedMotionController::homeAxis(int axisIndex)
{
    if (axisIndex < 0 || axisIndex >= positions_.size()) {
        return false;
    }

    states_[axisIndex] = MotionState::Homing;
    emit motionStateChanged(axisIndex, states_[axisIndex]);

    positions_[axisIndex] = axisConfigs_[axisIndex].homePosition;
    emit positionUpdated(axisIndex, positions_[axisIndex]);

    states_[axisIndex] = MotionState::Idle;
    emit motionStateChanged(axisIndex, states_[axisIndex]);
    emit homeCompleted(axisIndex);

    return true;
}

bool SimulatedMotionController::stopAxis(int axisIndex)
{
    if (axisIndex < 0 || axisIndex >= states_.size()) {
        return false;
    }
    states_[axisIndex] = MotionState::Idle;
    emit motionStateChanged(axisIndex, states_[axisIndex]);
    return true;
}

bool SimulatedMotionController::moveXY(double x, double y, MoveMode mode)
{
    if (positions_.size() < 2) {
        return false;
    }

    bool xOK = moveAxis(0, x, mode);
    bool yOK = moveAxis(1, y, mode);

    return xOK && yOK;
}

bool SimulatedMotionController::rotate(double angle, MoveMode mode)
{
    // 假设第三轴是旋转轴
    if (positions_.size() < 3) {
        return false;
    }
    return moveAxis(2, angle, mode);
}

bool SimulatedMotionController::getCurrentPosition(double& x, double& y, double& theta)
{
    x = positions_.size() > 0 ? positions_[0] : 0.0;
    y = positions_.size() > 1 ? positions_[1] : 0.0;
    theta = positions_.size() > 2 ? positions_[2] : 0.0;
    return true;
}

bool SimulatedMotionController::waitMotionComplete(int timeoutMs)
{
    Q_UNUSED(timeoutMs);
    // 模拟运动已经是瞬时完成的
    return true;
}

bool SimulatedMotionController::waitAxisComplete(int axisIndex, int timeoutMs)
{
    Q_UNUSED(axisIndex);
    Q_UNUSED(timeoutMs);
    return true;
}

bool SimulatedMotionController::isMoving() const
{
    for (const auto& state : states_) {
        if (state == MotionState::Moving || state == MotionState::Homing) {
            return true;
        }
    }
    return false;
}

bool SimulatedMotionController::isAxisMoving(int axisIndex) const
{
    if (axisIndex < 0 || axisIndex >= states_.size()) {
        return false;
    }
    return states_[axisIndex] == MotionState::Moving ||
           states_[axisIndex] == MotionState::Homing;
}

bool SimulatedMotionController::stopAll()
{
    for (int i = 0; i < states_.size(); ++i) {
        stopAxis(i);
    }
    return true;
}

bool SimulatedMotionController::emergencyStop()
{
    return stopAll();
}

bool SimulatedMotionController::homeAll()
{
    for (int i = 0; i < positions_.size(); ++i) {
        homeAxis(i);
    }
    return true;
}

bool SimulatedMotionController::clearError()
{
    for (int i = 0; i < states_.size(); ++i) {
        if (states_[i] == MotionState::Error) {
            states_[i] = MotionState::Idle;
            emit motionStateChanged(i, states_[i]);
        }
    }
    return true;
}

void SimulatedMotionController::setDefaultVelocity(double velocity)
{
    defaultVelocity_ = velocity;
}

double SimulatedMotionController::getDefaultVelocity() const
{
    return defaultVelocity_;
}

void SimulatedMotionController::setDefaultAcceleration(double acceleration)
{
    defaultAcceleration_ = acceleration;
}

double SimulatedMotionController::getDefaultAcceleration() const
{
    return defaultAcceleration_;
}

} // namespace HAL
} // namespace VisionForge
