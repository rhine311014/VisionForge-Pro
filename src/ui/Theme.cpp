/**
 * @file Theme.cpp
 * @brief 主题管理器实现
 */

#include "ui/Theme.h"
#include <QApplication>
#include <QPalette>
#include <QFile>

namespace VisionForge {
namespace UI {

void Theme::applyTheme(ThemeType type)
{
    QString styleSheet = getStyleSheet(type);
    qApp->setStyleSheet(styleSheet);

    // 设置调色板
    if (type == IndustrialDark || type == ClassicDark) {
        QPalette darkPalette;
        darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
        darkPalette.setColor(QPalette::WindowText, Qt::white);
        darkPalette.setColor(QPalette::Base, QColor(35, 35, 35));
        darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
        darkPalette.setColor(QPalette::ToolTipBase, QColor(25, 25, 25));
        darkPalette.setColor(QPalette::ToolTipText, Qt::white);
        darkPalette.setColor(QPalette::Text, Qt::white);
        darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
        darkPalette.setColor(QPalette::ButtonText, Qt::white);
        darkPalette.setColor(QPalette::BrightText, Qt::red);
        darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
        darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
        darkPalette.setColor(QPalette::HighlightedText, Qt::black);

        darkPalette.setColor(QPalette::Disabled, QPalette::WindowText, QColor(127, 127, 127));
        darkPalette.setColor(QPalette::Disabled, QPalette::Text, QColor(127, 127, 127));
        darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(127, 127, 127));

        qApp->setPalette(darkPalette);
    }
}

QString Theme::getStyleSheet(ThemeType type)
{
    switch (type) {
    case IndustrialDark:
        return getIndustrialDarkStyleSheet();
    case IndustrialLight:
        return getIndustrialLightStyleSheet();
    case ClassicDark:
        return getClassicDarkStyleSheet();
    case ClassicLight:
        return getClassicLightStyleSheet();
    default:
        return getIndustrialDarkStyleSheet();
    }
}

QIcon Theme::getIcon(const QString& name)
{
    // 先尝试从资源文件加载
    QString resourcePath = QString(":/icons/%1.png").arg(name);
    if (QFile::exists(resourcePath)) {
        return QIcon(resourcePath);
    }

    // 如果资源不存在，使用Qt标准图标作为后备
    // 这里可以根据name映射到对应的StandardPixmap
    if (name == Icons::FILE_OPEN) {
        return getStandardIcon(QStyle::SP_DialogOpenButton);
    } else if (name == Icons::FILE_SAVE) {
        return getStandardIcon(QStyle::SP_DialogSaveButton);
    } else if (name == Icons::EXIT) {
        return getStandardIcon(QStyle::SP_DialogCloseButton);
    } else if (name == Icons::TOOL_ADD) {
        return getStandardIcon(QStyle::SP_FileDialogNewFolder);
    } else if (name == Icons::TOOL_REMOVE) {
        return getStandardIcon(QStyle::SP_TrashIcon);
    } else if (name == Icons::TOOL_RUN) {
        return getStandardIcon(QStyle::SP_MediaPlay);
    } else if (name == Icons::TOOL_STOP) {
        return getStandardIcon(QStyle::SP_MediaStop);
    } else if (name == Icons::APP_HELP) {
        return getStandardIcon(QStyle::SP_MessageBoxQuestion);
    }

    return QIcon();
}

QIcon Theme::getStandardIcon(QStyle::StandardPixmap icon)
{
    return qApp->style()->standardIcon(icon);
}

// ========== 工业深色主题 ==========

QString Theme::getIndustrialDarkStyleSheet()
{
    return R"(
/* ========== 全局样式 ========== */
QWidget {
    background-color: #2b2b2b;
    color: #d4d4d4;
    font-family: "Segoe UI", "Microsoft YaHei";
    font-size: 9pt;
}

QMainWindow {
    background-color: #1e1e1e;
}

/* ========== 菜单栏 ========== */
QMenuBar {
    background-color: #2d2d30;
    color: #d4d4d4;
    border-bottom: 1px solid #3f3f46;
    padding: 2px;
}

QMenuBar::item {
    background-color: transparent;
    padding: 4px 12px;
    margin: 2px 0px;
}

QMenuBar::item:selected {
    background-color: #3f3f46;
    border-radius: 2px;
}

QMenuBar::item:pressed {
    background-color: #007acc;
    color: #ffffff;
}

/* ========== 菜单 ========== */
QMenu {
    background-color: #2d2d30;
    color: #d4d4d4;
    border: 1px solid #3f3f46;
    padding: 4px;
}

QMenu::item {
    background-color: transparent;
    padding: 6px 30px 6px 30px;
    margin: 2px;
    border-radius: 2px;
}

QMenu::item:selected {
    background-color: #3f3f46;
}

QMenu::item:pressed {
    background-color: #007acc;
    color: #ffffff;
}

QMenu::separator {
    height: 1px;
    background-color: #3f3f46;
    margin: 4px 8px;
}

/* ========== 工具栏 ========== */
QToolBar {
    background-color: #2d2d30;
    border: none;
    border-bottom: 1px solid #3f3f46;
    spacing: 4px;
    padding: 4px;
}

QToolBar::separator {
    background-color: #3f3f46;
    width: 1px;
    margin: 4px 8px;
}

QToolButton {
    background-color: transparent;
    color: #d4d4d4;
    border: 1px solid transparent;
    border-radius: 3px;
    padding: 6px;
    margin: 2px;
}

QToolButton:hover {
    background-color: #3f3f46;
    border: 1px solid #007acc;
}

QToolButton:pressed {
    background-color: #007acc;
    color: #ffffff;
}

QToolButton:checked {
    background-color: #094771;
    border: 1px solid #007acc;
}

/* ========== 状态栏 ========== */
QStatusBar {
    background-color: #007acc;
    color: #ffffff;
    border-top: 1px solid #005a9e;
}

QStatusBar::item {
    border: none;
}

QStatusBar QLabel {
    background-color: transparent;
    color: #ffffff;
    padding: 2px 8px;
}

/* ========== 停靠窗口 ========== */
QDockWidget {
    background-color: #2b2b2b;
    titlebar-close-icon: url(close.png);
    titlebar-normal-icon: url(undock.png);
}

QDockWidget::title {
    background-color: #2d2d30;
    color: #d4d4d4;
    padding: 6px;
    border: 1px solid #3f3f46;
    text-align: left;
    font-weight: bold;
}

QDockWidget::close-button, QDockWidget::float-button {
    background-color: transparent;
    border: 1px solid transparent;
    border-radius: 2px;
    padding: 2px;
}

QDockWidget::close-button:hover, QDockWidget::float-button:hover {
    background-color: #3f3f46;
}

/* ========== 按钮 ========== */
QPushButton {
    background-color: #0e639c;
    color: #ffffff;
    border: 1px solid #007acc;
    border-radius: 3px;
    padding: 6px 16px;
    min-width: 80px;
}

QPushButton:hover {
    background-color: #1177bb;
    border: 1px solid #007acc;
}

QPushButton:pressed {
    background-color: #094771;
}

QPushButton:disabled {
    background-color: #3f3f46;
    color: #808080;
    border: 1px solid #3f3f46;
}

/* ========== 列表控件 ========== */
QListWidget {
    background-color: #252526;
    color: #d4d4d4;
    border: 1px solid #3f3f46;
    outline: none;
    padding: 2px;
}

QListWidget::item {
    background-color: transparent;
    color: #d4d4d4;
    padding: 6px;
    border-radius: 2px;
    margin: 1px;
}

QListWidget::item:selected {
    background-color: #094771;
    color: #ffffff;
}

QListWidget::item:hover {
    background-color: #2a2d2e;
}

/* ========== 组合框 ========== */
QComboBox {
    background-color: #3f3f46;
    color: #d4d4d4;
    border: 1px solid #3f3f46;
    border-radius: 3px;
    padding: 4px 8px;
    min-width: 100px;
}

QComboBox:hover {
    border: 1px solid #007acc;
}

QComboBox:on {
    background-color: #094771;
    border: 1px solid #007acc;
}

QComboBox::drop-down {
    border: none;
    width: 20px;
}

QComboBox::down-arrow {
    image: url(down_arrow.png);
    width: 12px;
    height: 12px;
}

QComboBox QAbstractItemView {
    background-color: #2d2d30;
    color: #d4d4d4;
    border: 1px solid #3f3f46;
    selection-background-color: #094771;
    outline: none;
}

/* ========== 复选框 ========== */
QCheckBox {
    color: #d4d4d4;
    spacing: 6px;
}

QCheckBox::indicator {
    width: 16px;
    height: 16px;
    border: 1px solid #3f3f46;
    border-radius: 2px;
    background-color: #2b2b2b;
}

QCheckBox::indicator:hover {
    border: 1px solid #007acc;
}

QCheckBox::indicator:checked {
    background-color: #007acc;
    border: 1px solid #007acc;
    image: url(check.png);
}

/* ========== 文本输入框 ========== */
QLineEdit {
    background-color: #3f3f46;
    color: #d4d4d4;
    border: 1px solid #3f3f46;
    border-radius: 3px;
    padding: 4px 8px;
    selection-background-color: #094771;
}

QLineEdit:hover {
    border: 1px solid #007acc;
}

QLineEdit:focus {
    border: 1px solid #007acc;
    background-color: #252526;
}

/* ========== 数字输入框 ========== */
QSpinBox, QDoubleSpinBox {
    background-color: #3f3f46;
    color: #d4d4d4;
    border: 1px solid #3f3f46;
    border-radius: 3px;
    padding: 4px;
    selection-background-color: #094771;
}

QSpinBox:hover, QDoubleSpinBox:hover {
    border: 1px solid #007acc;
}

QSpinBox:focus, QDoubleSpinBox:focus {
    border: 1px solid #007acc;
}

QSpinBox::up-button, QDoubleSpinBox::up-button {
    background-color: #2d2d30;
    border-left: 1px solid #3f3f46;
    border-top-right-radius: 3px;
}

QSpinBox::down-button, QDoubleSpinBox::down-button {
    background-color: #2d2d30;
    border-left: 1px solid #3f3f46;
    border-bottom-right-radius: 3px;
}

/* ========== 滚动条 ========== */
QScrollBar:vertical {
    background-color: #2b2b2b;
    width: 14px;
    margin: 0px;
}

QScrollBar::handle:vertical {
    background-color: #686868;
    min-height: 20px;
    border-radius: 7px;
    margin: 2px;
}

QScrollBar::handle:vertical:hover {
    background-color: #9e9e9e;
}

QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
    height: 0px;
}

QScrollBar:horizontal {
    background-color: #2b2b2b;
    height: 14px;
    margin: 0px;
}

QScrollBar::handle:horizontal {
    background-color: #686868;
    min-width: 20px;
    border-radius: 7px;
    margin: 2px;
}

QScrollBar::handle:horizontal:hover {
    background-color: #9e9e9e;
}

QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {
    width: 0px;
}

/* ========== 分组框 ========== */
QGroupBox {
    background-color: #2b2b2b;
    border: 1px solid #3f3f46;
    border-radius: 4px;
    margin-top: 12px;
    padding-top: 8px;
    font-weight: bold;
}

QGroupBox::title {
    subcontrol-origin: margin;
    subcontrol-position: top left;
    left: 12px;
    padding: 0 6px;
    color: #007acc;
}

/* ========== 标签页 ========== */
QTabWidget::pane {
    background-color: #2b2b2b;
    border: 1px solid #3f3f46;
    border-top: none;
}

QTabBar::tab {
    background-color: #2d2d30;
    color: #d4d4d4;
    border: 1px solid #3f3f46;
    border-bottom: none;
    padding: 6px 16px;
    margin-right: 2px;
}

QTabBar::tab:selected {
    background-color: #2b2b2b;
    color: #007acc;
    border-bottom: 2px solid #007acc;
}

QTabBar::tab:hover {
    background-color: #3f3f46;
}

/* ========== 工具提示 ========== */
QToolTip {
    background-color: #2d2d30;
    color: #d4d4d4;
    border: 1px solid #007acc;
    padding: 4px;
}

/* ========== 进度条 ========== */
QProgressBar {
    background-color: #3f3f46;
    border: 1px solid #3f3f46;
    border-radius: 3px;
    text-align: center;
    color: #d4d4d4;
}

QProgressBar::chunk {
    background-color: #007acc;
    border-radius: 2px;
}
)";
}

QString Theme::getIndustrialLightStyleSheet()
{
    // TODO: 实现浅色主题
    return "";
}

QString Theme::getClassicDarkStyleSheet()
{
    // TODO: 实现经典深色主题
    return "";
}

QString Theme::getClassicLightStyleSheet()
{
    // TODO: 实现经典浅色主题
    return "";
}

} // namespace UI
} // namespace VisionForge
