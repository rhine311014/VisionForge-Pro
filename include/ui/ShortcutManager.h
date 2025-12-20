/**
 * @file ShortcutManager.h
 * @brief 键盘快捷键管理器
 * @author VisionForge Team
 * @date 2025-12-19
 */

#pragma once

#include <QObject>
#include <QShortcut>
#include <QKeySequence>
#include <QMap>
#include <QVector>
#include <QJsonObject>
#include <functional>

class QWidget;

namespace VisionForge {
namespace UI {

/**
 * @brief 快捷键类别
 */
enum class ShortcutCategory {
    File,           ///< 文件操作
    Edit,           ///< 编辑操作
    View,           ///< 视图操作
    Tool,           ///< 工具操作
    Run,            ///< 运行操作
    Window,         ///< 窗口操作
    Help,           ///< 帮助
    Custom          ///< 自定义
};

/**
 * @brief 快捷键动作信息
 */
struct ShortcutAction {
    QString id;                     ///< 唯一标识符
    QString name;                   ///< 显示名称
    QString description;            ///< 描述
    ShortcutCategory category;      ///< 类别
    QKeySequence defaultKey;        ///< 默认快捷键
    QKeySequence currentKey;        ///< 当前快捷键
    bool enabled;                   ///< 是否启用
    std::function<void()> callback; ///< 回调函数

    ShortcutAction() : category(ShortcutCategory::Custom), enabled(true) {}
};

/**
 * @class ShortcutManager
 * @brief 键盘快捷键管理器（单例）
 *
 * 功能：
 * - 统一管理全局快捷键
 * - 支持自定义快捷键
 * - 快捷键冲突检测
 * - 配置保存/加载
 */
class ShortcutManager : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 获取单例实例
     */
    static ShortcutManager& instance();

    /**
     * @brief 初始化快捷键管理器
     * @param parent 父窗口
     */
    void initialize(QWidget* parent);

    /**
     * @brief 注册快捷键动作
     * @param action 快捷键动作
     * @return true如果注册成功
     */
    bool registerAction(const ShortcutAction& action);

    /**
     * @brief 注册快捷键动作（便捷方法）
     */
    bool registerAction(const QString& id,
                       const QString& name,
                       const QKeySequence& defaultKey,
                       ShortcutCategory category,
                       std::function<void()> callback,
                       const QString& description = QString());

    /**
     * @brief 注销快捷键动作
     * @param id 动作ID
     */
    void unregisterAction(const QString& id);

    /**
     * @brief 设置快捷键
     * @param id 动作ID
     * @param key 新快捷键
     * @return true如果设置成功
     */
    bool setShortcut(const QString& id, const QKeySequence& key);

    /**
     * @brief 重置为默认快捷键
     * @param id 动作ID
     */
    void resetToDefault(const QString& id);

    /**
     * @brief 重置所有快捷键为默认值
     */
    void resetAllToDefaults();

    /**
     * @brief 获取快捷键动作
     * @param id 动作ID
     * @return 快捷键动作（如果不存在返回空）
     */
    ShortcutAction getAction(const QString& id) const;

    /**
     * @brief 获取指定类别的所有动作
     * @param category 类别
     */
    QVector<ShortcutAction> getActionsByCategory(ShortcutCategory category) const;

    /**
     * @brief 获取所有动作
     */
    QVector<ShortcutAction> getAllActions() const;

    /**
     * @brief 检查快捷键冲突
     * @param key 快捷键
     * @param excludeId 排除的动作ID
     * @return 冲突的动作ID列表
     */
    QStringList checkConflict(const QKeySequence& key, const QString& excludeId = QString()) const;

    /**
     * @brief 启用/禁用快捷键
     * @param id 动作ID
     * @param enabled 是否启用
     */
    void setEnabled(const QString& id, bool enabled);

    /**
     * @brief 启用/禁用所有快捷键
     */
    void setAllEnabled(bool enabled);

    /**
     * @brief 获取类别名称
     */
    static QString categoryName(ShortcutCategory category);

    /**
     * @brief 保存配置
     */
    void saveConfig();

    /**
     * @brief 加载配置
     */
    void loadConfig();

    /**
     * @brief 导出快捷键配置
     * @return JSON对象
     */
    QJsonObject exportConfig() const;

    /**
     * @brief 导入快捷键配置
     * @param config JSON对象
     * @return true如果导入成功
     */
    bool importConfig(const QJsonObject& config);

    /**
     * @brief 注册默认快捷键
     */
    void registerDefaultShortcuts();

signals:
    /**
     * @brief 快捷键变更信号
     */
    void shortcutChanged(const QString& id, const QKeySequence& newKey);

    /**
     * @brief 动作触发信号
     */
    void actionTriggered(const QString& id);

private:
    ShortcutManager();
    ~ShortcutManager() = default;
    ShortcutManager(const ShortcutManager&) = delete;
    ShortcutManager& operator=(const ShortcutManager&) = delete;

    void createShortcut(const QString& id);
    void updateShortcut(const QString& id);
    void removeShortcut(const QString& id);

private:
    QWidget* parentWidget_;
    QMap<QString, ShortcutAction> actions_;
    QMap<QString, QShortcut*> shortcuts_;
    bool initialized_;
};

} // namespace UI
} // namespace VisionForge

// 便捷宏
#define SHORTCUT_MGR VisionForge::UI::ShortcutManager::instance()
