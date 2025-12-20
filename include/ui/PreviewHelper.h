/**
 * @file PreviewHelper.h
 * @brief 实时预览辅助类 - 提供防抖功能的预览触发器
 * @author VisionForge Team
 * @date 2025-12-19
 */

#pragma once

#include <QObject>
#include <QTimer>

namespace VisionForge {
namespace UI {

/**
 * @class PreviewHelper
 * @brief 实时预览辅助类
 *
 * 功能：
 * - 防抖机制：避免频繁触发预览
 * - 可配置延迟时间
 * - 自动预览开关控制
 * - 立即预览选项
 */
class PreviewHelper : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父对象
     * @param debounceMs 防抖延迟毫秒数（默认150ms）
     */
    explicit PreviewHelper(QObject* parent = nullptr, int debounceMs = 150);
    ~PreviewHelper() override = default;

    /**
     * @brief 是否启用自动预览
     */
    bool isAutoPreviewEnabled() const { return autoPreviewEnabled_; }

    /**
     * @brief 设置自动预览开关
     * @param enabled 是否启用
     */
    void setAutoPreviewEnabled(bool enabled);

    /**
     * @brief 获取防抖延迟时间
     */
    int debounceTime() const { return debounceMs_; }

    /**
     * @brief 设置防抖延迟时间
     * @param ms 延迟毫秒数
     */
    void setDebounceTime(int ms);

    /**
     * @brief 请求预览（带防抖）
     *
     * 如果自动预览启用，会在防抖延迟后触发previewTriggered信号
     */
    void requestPreview();

    /**
     * @brief 立即触发预览（跳过防抖）
     */
    void triggerPreviewNow();

    /**
     * @brief 取消待处理的预览请求
     */
    void cancelPendingPreview();

    /**
     * @brief 检查是否有待处理的预览
     */
    bool hasPendingPreview() const { return timer_->isActive(); }

signals:
    /**
     * @brief 预览触发信号
     *
     * 当防抖延迟结束或立即预览时发出
     */
    void previewTriggered();

    /**
     * @brief 自动预览开关变更信号
     */
    void autoPreviewEnabledChanged(bool enabled);

private slots:
    void onTimeout();

private:
    QTimer* timer_;
    int debounceMs_;
    bool autoPreviewEnabled_;
};

/**
 * @class PreviewCheckBox
 * @brief 自动预览复选框组件
 *
 * 封装了PreviewHelper和QCheckBox的组合
 */
class PreviewCheckBox;  // 前向声明，在需要时可以扩展

} // namespace UI
} // namespace VisionForge
