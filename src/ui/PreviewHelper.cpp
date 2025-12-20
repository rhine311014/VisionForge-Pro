/**
 * @file PreviewHelper.cpp
 * @brief 实时预览辅助类实现
 */

#include "ui/PreviewHelper.h"
#include "base/Logger.h"
#include "base/ConfigManager.h"

namespace VisionForge {
namespace UI {

PreviewHelper::PreviewHelper(QObject* parent, int debounceMs)
    : QObject(parent)
    , timer_(new QTimer(this))
    , debounceMs_(debounceMs)
    , autoPreviewEnabled_(true)
{
    // 设置为单次触发
    timer_->setSingleShot(true);
    connect(timer_, &QTimer::timeout, this, &PreviewHelper::onTimeout);

    // 从配置加载默认设置
    Base::ConfigManager& config = Base::ConfigManager::instance();
    autoPreviewEnabled_ = config.getValue("UI/AutoPreview", true).toBool();
    debounceMs_ = config.getValue("UI/PreviewDebounceMs", 150).toInt();
}

void PreviewHelper::setAutoPreviewEnabled(bool enabled)
{
    if (autoPreviewEnabled_ != enabled) {
        autoPreviewEnabled_ = enabled;

        // 保存到配置
        Base::ConfigManager& config = Base::ConfigManager::instance();
        config.setValue("UI/AutoPreview", enabled);

        LOG_DEBUG(QString("自动预览%1").arg(enabled ? "已启用" : "已禁用"));

        emit autoPreviewEnabledChanged(enabled);

        // 如果禁用，取消待处理的预览
        if (!enabled) {
            cancelPendingPreview();
        }
    }
}

void PreviewHelper::setDebounceTime(int ms)
{
    if (ms >= 0 && debounceMs_ != ms) {
        debounceMs_ = ms;

        // 保存到配置
        Base::ConfigManager& config = Base::ConfigManager::instance();
        config.setValue("UI/PreviewDebounceMs", ms);
    }
}

void PreviewHelper::requestPreview()
{
    if (!autoPreviewEnabled_) {
        return;
    }

    // 重新启动计时器（实现防抖）
    timer_->stop();
    timer_->start(debounceMs_);
}

void PreviewHelper::triggerPreviewNow()
{
    // 取消待处理的预览
    cancelPendingPreview();

    // 立即触发
    emit previewTriggered();
}

void PreviewHelper::cancelPendingPreview()
{
    if (timer_->isActive()) {
        timer_->stop();
    }
}

void PreviewHelper::onTimeout()
{
    emit previewTriggered();
}

} // namespace UI
} // namespace VisionForge
