/**
 * @file ShortcutManager.cpp
 * @brief 键盘快捷键管理器实现
 */

#include "ui/ShortcutManager.h"
#include "base/ConfigManager.h"
#include "base/Logger.h"

#include <QWidget>
#include <QJsonArray>
#include <QJsonDocument>

namespace VisionForge {
namespace UI {

ShortcutManager& ShortcutManager::instance()
{
    static ShortcutManager instance;
    return instance;
}

ShortcutManager::ShortcutManager()
    : QObject(nullptr)
    , parentWidget_(nullptr)
    , initialized_(false)
{
}

void ShortcutManager::initialize(QWidget* parent)
{
    if (initialized_) {
        return;
    }

    parentWidget_ = parent;
    initialized_ = true;

    // 注册默认快捷键
    registerDefaultShortcuts();

    // 加载用户配置
    loadConfig();

    LOG_INFO("快捷键管理器初始化完成");
}

bool ShortcutManager::registerAction(const ShortcutAction& action)
{
    if (action.id.isEmpty()) {
        LOG_WARNING("注册快捷键失败: 动作ID为空");
        return false;
    }

    if (actions_.contains(action.id)) {
        LOG_WARNING(QString("注册快捷键失败: 动作ID已存在 - %1").arg(action.id));
        return false;
    }

    actions_[action.id] = action;
    actions_[action.id].currentKey = action.defaultKey;

    if (initialized_ && parentWidget_) {
        createShortcut(action.id);
    }

    LOG_DEBUG(QString("注册快捷键: %1 -> %2")
             .arg(action.id)
             .arg(action.defaultKey.toString()));

    return true;
}

bool ShortcutManager::registerAction(const QString& id,
                                     const QString& name,
                                     const QKeySequence& defaultKey,
                                     ShortcutCategory category,
                                     std::function<void()> callback,
                                     const QString& description)
{
    ShortcutAction action;
    action.id = id;
    action.name = name;
    action.description = description;
    action.category = category;
    action.defaultKey = defaultKey;
    action.currentKey = defaultKey;
    action.enabled = true;
    action.callback = callback;

    return registerAction(action);
}

void ShortcutManager::unregisterAction(const QString& id)
{
    if (!actions_.contains(id)) {
        return;
    }

    removeShortcut(id);
    actions_.remove(id);

    LOG_DEBUG(QString("注销快捷键: %1").arg(id));
}

bool ShortcutManager::setShortcut(const QString& id, const QKeySequence& key)
{
    if (!actions_.contains(id)) {
        LOG_WARNING(QString("设置快捷键失败: 动作不存在 - %1").arg(id));
        return false;
    }

    // 检查冲突
    QStringList conflicts = checkConflict(key, id);
    if (!conflicts.isEmpty()) {
        LOG_WARNING(QString("快捷键冲突: %1 与 %2")
                   .arg(key.toString())
                   .arg(conflicts.join(", ")));
        return false;
    }

    actions_[id].currentKey = key;
    updateShortcut(id);

    emit shortcutChanged(id, key);
    saveConfig();

    LOG_INFO(QString("设置快捷键: %1 -> %2").arg(id).arg(key.toString()));

    return true;
}

void ShortcutManager::resetToDefault(const QString& id)
{
    if (!actions_.contains(id)) {
        return;
    }

    actions_[id].currentKey = actions_[id].defaultKey;
    updateShortcut(id);
    emit shortcutChanged(id, actions_[id].currentKey);
    saveConfig();
}

void ShortcutManager::resetAllToDefaults()
{
    for (auto it = actions_.begin(); it != actions_.end(); ++it) {
        it->currentKey = it->defaultKey;
        updateShortcut(it.key());
        emit shortcutChanged(it.key(), it->currentKey);
    }
    saveConfig();
    LOG_INFO("所有快捷键已重置为默认值");
}

ShortcutAction ShortcutManager::getAction(const QString& id) const
{
    return actions_.value(id);
}

QVector<ShortcutAction> ShortcutManager::getActionsByCategory(ShortcutCategory category) const
{
    QVector<ShortcutAction> result;
    for (const auto& action : actions_) {
        if (action.category == category) {
            result.append(action);
        }
    }
    return result;
}

QVector<ShortcutAction> ShortcutManager::getAllActions() const
{
    QVector<ShortcutAction> result;
    for (const auto& action : actions_) {
        result.append(action);
    }
    return result;
}

QStringList ShortcutManager::checkConflict(const QKeySequence& key, const QString& excludeId) const
{
    QStringList conflicts;

    if (key.isEmpty()) {
        return conflicts;
    }

    for (auto it = actions_.begin(); it != actions_.end(); ++it) {
        if (it.key() != excludeId && it->currentKey == key && it->enabled) {
            conflicts.append(it->name);
        }
    }

    return conflicts;
}

void ShortcutManager::setEnabled(const QString& id, bool enabled)
{
    if (!actions_.contains(id)) {
        return;
    }

    actions_[id].enabled = enabled;

    if (shortcuts_.contains(id)) {
        shortcuts_[id]->setEnabled(enabled);
    }
}

void ShortcutManager::setAllEnabled(bool enabled)
{
    for (auto it = actions_.begin(); it != actions_.end(); ++it) {
        it->enabled = enabled;
    }

    for (auto shortcut : shortcuts_) {
        shortcut->setEnabled(enabled);
    }
}

QString ShortcutManager::categoryName(ShortcutCategory category)
{
    switch (category) {
        case ShortcutCategory::File: return QObject::tr("文件");
        case ShortcutCategory::Edit: return QObject::tr("编辑");
        case ShortcutCategory::View: return QObject::tr("视图");
        case ShortcutCategory::Tool: return QObject::tr("工具");
        case ShortcutCategory::Run: return QObject::tr("运行");
        case ShortcutCategory::Window: return QObject::tr("窗口");
        case ShortcutCategory::Help: return QObject::tr("帮助");
        case ShortcutCategory::Custom: return QObject::tr("自定义");
        default: return QObject::tr("未知");
    }
}

void ShortcutManager::saveConfig()
{
    Base::ConfigManager& config = Base::ConfigManager::instance();

    QJsonObject shortcuts;
    for (auto it = actions_.begin(); it != actions_.end(); ++it) {
        if (it->currentKey != it->defaultKey) {
            shortcuts[it.key()] = it->currentKey.toString();
        }
    }

    config.setValue("Shortcuts/Custom", QJsonDocument(shortcuts).toJson(QJsonDocument::Compact));
}

void ShortcutManager::loadConfig()
{
    Base::ConfigManager& config = Base::ConfigManager::instance();

    QString jsonStr = config.getValue("Shortcuts/Custom", "{}").toString();
    QJsonDocument doc = QJsonDocument::fromJson(jsonStr.toUtf8());
    QJsonObject shortcuts = doc.object();

    for (auto it = shortcuts.begin(); it != shortcuts.end(); ++it) {
        QString id = it.key();
        QString keyStr = it.value().toString();

        if (actions_.contains(id)) {
            QKeySequence key(keyStr);
            if (!key.isEmpty()) {
                actions_[id].currentKey = key;
                updateShortcut(id);
            }
        }
    }
}

QJsonObject ShortcutManager::exportConfig() const
{
    QJsonObject config;
    QJsonArray actionsArray;

    for (const auto& action : actions_) {
        QJsonObject obj;
        obj["id"] = action.id;
        obj["key"] = action.currentKey.toString();
        actionsArray.append(obj);
    }

    config["shortcuts"] = actionsArray;
    config["version"] = "1.0";

    return config;
}

bool ShortcutManager::importConfig(const QJsonObject& config)
{
    if (!config.contains("shortcuts")) {
        return false;
    }

    QJsonArray actionsArray = config["shortcuts"].toArray();
    for (const auto& item : actionsArray) {
        QJsonObject obj = item.toObject();
        QString id = obj["id"].toString();
        QString keyStr = obj["key"].toString();

        if (actions_.contains(id)) {
            QKeySequence key(keyStr);
            actions_[id].currentKey = key;
            updateShortcut(id);
        }
    }

    saveConfig();
    return true;
}

void ShortcutManager::registerDefaultShortcuts()
{
    // ========== 文件操作 ==========
    registerAction("file.open", tr("打开图像"),
                  QKeySequence(Qt::CTRL | Qt::Key_O),
                  ShortcutCategory::File, nullptr, tr("打开图像文件"));

    registerAction("file.save", tr("保存方案"),
                  QKeySequence(Qt::CTRL | Qt::Key_S),
                  ShortcutCategory::File, nullptr, tr("保存当前方案"));

    registerAction("file.saveAs", tr("另存方案"),
                  QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_S),
                  ShortcutCategory::File, nullptr, tr("方案另存为"));

    registerAction("file.export", tr("导出结果"),
                  QKeySequence(Qt::CTRL | Qt::Key_E),
                  ShortcutCategory::File, nullptr, tr("导出检测结果"));

    // ========== 编辑操作 ==========
    registerAction("edit.undo", tr("撤销"),
                  QKeySequence(Qt::CTRL | Qt::Key_Z),
                  ShortcutCategory::Edit, nullptr, tr("撤销上一步操作"));

    registerAction("edit.redo", tr("重做"),
                  QKeySequence(Qt::CTRL | Qt::Key_Y),
                  ShortcutCategory::Edit, nullptr, tr("重做上一步操作"));

    registerAction("edit.copy", tr("复制工具"),
                  QKeySequence(Qt::CTRL | Qt::Key_C),
                  ShortcutCategory::Edit, nullptr, tr("复制选中的工具"));

    registerAction("edit.paste", tr("粘贴工具"),
                  QKeySequence(Qt::CTRL | Qt::Key_V),
                  ShortcutCategory::Edit, nullptr, tr("粘贴工具"));

    registerAction("edit.delete", tr("删除工具"),
                  QKeySequence(Qt::Key_Delete),
                  ShortcutCategory::Edit, nullptr, tr("删除选中的工具"));

    // ========== 视图操作 ==========
    registerAction("view.zoomIn", tr("放大"),
                  QKeySequence(Qt::CTRL | Qt::Key_Plus),
                  ShortcutCategory::View, nullptr, tr("放大图像"));

    registerAction("view.zoomOut", tr("缩小"),
                  QKeySequence(Qt::CTRL | Qt::Key_Minus),
                  ShortcutCategory::View, nullptr, tr("缩小图像"));

    registerAction("view.fitWindow", tr("适应窗口"),
                  QKeySequence(Qt::CTRL | Qt::Key_0),
                  ShortcutCategory::View, nullptr, tr("图像适应窗口大小"));

    registerAction("view.actualSize", tr("实际大小"),
                  QKeySequence(Qt::CTRL | Qt::Key_1),
                  ShortcutCategory::View, nullptr, tr("显示图像实际大小"));

    registerAction("view.fullscreen", tr("全屏"),
                  QKeySequence(Qt::Key_F11),
                  ShortcutCategory::View, nullptr, tr("切换全屏显示"));

    registerAction("view.toggleMode", tr("切换模式"),
                  QKeySequence(Qt::CTRL | Qt::Key_M),
                  ShortcutCategory::View, nullptr, tr("切换简单/专业模式"));

    // ========== 工具操作 ==========
    registerAction("tool.addGray", tr("添加灰度转换"),
                  QKeySequence(Qt::ALT | Qt::Key_G),
                  ShortcutCategory::Tool, nullptr, tr("添加灰度转换工具"));

    registerAction("tool.addBlur", tr("添加滤波"),
                  QKeySequence(Qt::ALT | Qt::Key_B),
                  ShortcutCategory::Tool, nullptr, tr("添加滤波工具"));

    registerAction("tool.addThreshold", tr("添加二值化"),
                  QKeySequence(Qt::ALT | Qt::Key_T),
                  ShortcutCategory::Tool, nullptr, tr("添加二值化工具"));

    registerAction("tool.addTemplate", tr("添加模板匹配"),
                  QKeySequence(Qt::ALT | Qt::Key_M),
                  ShortcutCategory::Tool, nullptr, tr("添加模板匹配工具"));

    registerAction("tool.addCircle", tr("添加圆检测"),
                  QKeySequence(Qt::ALT | Qt::Key_C),
                  ShortcutCategory::Tool, nullptr, tr("添加圆检测工具"));

    registerAction("tool.addLine", tr("添加直线检测"),
                  QKeySequence(Qt::ALT | Qt::Key_L),
                  ShortcutCategory::Tool, nullptr, tr("添加直线检测工具"));

    registerAction("tool.addBlob", tr("添加Blob分析"),
                  QKeySequence(Qt::ALT | Qt::Key_A),
                  ShortcutCategory::Tool, nullptr, tr("添加Blob分析工具"));

    registerAction("tool.selectTool", tr("选择工具"),
                  QKeySequence(Qt::Key_V),
                  ShortcutCategory::Tool, nullptr, tr("选择工具模式"));

    registerAction("tool.drawROI", tr("绘制ROI"),
                  QKeySequence(Qt::Key_R),
                  ShortcutCategory::Tool, nullptr, tr("绘制感兴趣区域"));

    // ========== 运行操作 ==========
    registerAction("run.execute", tr("执行检测"),
                  QKeySequence(Qt::Key_F5),
                  ShortcutCategory::Run, nullptr, tr("执行一次检测"));

    registerAction("run.runOnce", tr("单步执行"),
                  QKeySequence(Qt::Key_F6),
                  ShortcutCategory::Run, nullptr, tr("单步执行当前工具"));

    registerAction("run.continuous", tr("连续运行"),
                  QKeySequence(Qt::Key_F7),
                  ShortcutCategory::Run, nullptr, tr("启动/停止连续运行"));

    registerAction("run.stop", tr("停止"),
                  QKeySequence(Qt::Key_Escape),
                  ShortcutCategory::Run, nullptr, tr("停止当前操作"));

    registerAction("run.startCamera", tr("启动相机"),
                  QKeySequence(Qt::Key_F8),
                  ShortcutCategory::Run, nullptr, tr("启动相机采集"));

    registerAction("run.stopCamera", tr("停止相机"),
                  QKeySequence(Qt::Key_F9),
                  ShortcutCategory::Run, nullptr, tr("停止相机采集"));

    // ========== 窗口操作 ==========
    registerAction("window.settings", tr("系统设置"),
                  QKeySequence(Qt::CTRL | Qt::Key_Comma),
                  ShortcutCategory::Window, nullptr, tr("打开系统设置"));

    registerAction("window.toolchain", tr("工具链面板"),
                  QKeySequence(Qt::CTRL | Qt::Key_T),
                  ShortcutCategory::Window, nullptr, tr("显示/隐藏工具链面板"));

    registerAction("window.result", tr("结果面板"),
                  QKeySequence(Qt::CTRL | Qt::Key_R),
                  ShortcutCategory::Window, nullptr, tr("显示/隐藏结果面板"));

    registerAction("window.log", tr("日志面板"),
                  QKeySequence(Qt::CTRL | Qt::Key_L),
                  ShortcutCategory::Window, nullptr, tr("显示/隐藏日志面板"));

    // ========== 帮助 ==========
    registerAction("help.about", tr("关于"),
                  QKeySequence(Qt::Key_F1),
                  ShortcutCategory::Help, nullptr, tr("显示关于对话框"));

    registerAction("help.shortcuts", tr("快捷键列表"),
                  QKeySequence(Qt::CTRL | Qt::Key_K),
                  ShortcutCategory::Help, nullptr, tr("显示快捷键列表"));
}

void ShortcutManager::createShortcut(const QString& id)
{
    if (!parentWidget_ || !actions_.contains(id)) {
        return;
    }

    const ShortcutAction& action = actions_[id];

    QShortcut* shortcut = new QShortcut(action.currentKey, parentWidget_);
    shortcut->setContext(Qt::ApplicationShortcut);
    shortcut->setEnabled(action.enabled);

    connect(shortcut, &QShortcut::activated, this, [this, id]() {
        if (actions_.contains(id) && actions_[id].callback) {
            actions_[id].callback();
        }
        emit actionTriggered(id);
    });

    shortcuts_[id] = shortcut;
}

void ShortcutManager::updateShortcut(const QString& id)
{
    if (!shortcuts_.contains(id)) {
        if (initialized_ && parentWidget_) {
            createShortcut(id);
        }
        return;
    }

    if (actions_.contains(id)) {
        shortcuts_[id]->setKey(actions_[id].currentKey);
        shortcuts_[id]->setEnabled(actions_[id].enabled);
    }
}

void ShortcutManager::removeShortcut(const QString& id)
{
    if (shortcuts_.contains(id)) {
        delete shortcuts_[id];
        shortcuts_.remove(id);
    }
}

} // namespace UI
} // namespace VisionForge
