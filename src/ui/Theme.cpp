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
        // 深色主题调色板
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

        // 根据主题类型设置强调色
        if (type == IndustrialDark) {
            darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));       // 蓝色
            darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
        } else {
            darkPalette.setColor(QPalette::Link, QColor(76, 175, 80));        // 绿色
            darkPalette.setColor(QPalette::Highlight, QColor(76, 175, 80));
        }
        darkPalette.setColor(QPalette::HighlightedText, Qt::white);

        darkPalette.setColor(QPalette::Disabled, QPalette::WindowText, QColor(127, 127, 127));
        darkPalette.setColor(QPalette::Disabled, QPalette::Text, QColor(127, 127, 127));
        darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(127, 127, 127));

        qApp->setPalette(darkPalette);
    } else {
        // 浅色主题调色板
        QPalette lightPalette;
        lightPalette.setColor(QPalette::Window, QColor(245, 245, 245));
        lightPalette.setColor(QPalette::WindowText, QColor(30, 30, 30));
        lightPalette.setColor(QPalette::Base, QColor(255, 255, 255));
        lightPalette.setColor(QPalette::AlternateBase, QColor(240, 240, 240));
        lightPalette.setColor(QPalette::ToolTipBase, QColor(255, 255, 255));
        lightPalette.setColor(QPalette::ToolTipText, QColor(30, 30, 30));
        lightPalette.setColor(QPalette::Text, QColor(30, 30, 30));
        lightPalette.setColor(QPalette::Button, QColor(240, 240, 240));
        lightPalette.setColor(QPalette::ButtonText, QColor(30, 30, 30));
        lightPalette.setColor(QPalette::BrightText, Qt::red);

        // 根据主题类型设置强调色
        if (type == IndustrialLight) {
            lightPalette.setColor(QPalette::Link, QColor(0, 122, 204));       // 蓝色
            lightPalette.setColor(QPalette::Highlight, QColor(204, 229, 255));
        } else {
            lightPalette.setColor(QPalette::Link, QColor(46, 125, 50));       // 绿色
            lightPalette.setColor(QPalette::Highlight, QColor(200, 230, 201));
        }
        lightPalette.setColor(QPalette::HighlightedText, QColor(30, 30, 30));

        lightPalette.setColor(QPalette::Disabled, QPalette::WindowText, QColor(160, 160, 160));
        lightPalette.setColor(QPalette::Disabled, QPalette::Text, QColor(160, 160, 160));
        lightPalette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(160, 160, 160));

        qApp->setPalette(lightPalette);
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
    return R"(
/* ========== 工业浅色主题 ========== */
QWidget {
    background-color: #f5f5f5;
    color: #1e1e1e;
    font-family: "Segoe UI", "Microsoft YaHei";
    font-size: 9pt;
}

QMainWindow {
    background-color: #ffffff;
}

/* ========== 菜单栏 ========== */
QMenuBar {
    background-color: #e8e8e8;
    color: #1e1e1e;
    border-bottom: 1px solid #d0d0d0;
    padding: 2px;
}

QMenuBar::item {
    background-color: transparent;
    padding: 4px 12px;
    margin: 2px 0px;
}

QMenuBar::item:selected {
    background-color: #d0d0d0;
    border-radius: 2px;
}

QMenuBar::item:pressed {
    background-color: #007acc;
    color: #ffffff;
}

/* ========== 菜单 ========== */
QMenu {
    background-color: #ffffff;
    color: #1e1e1e;
    border: 1px solid #d0d0d0;
    padding: 4px;
}

QMenu::item {
    background-color: transparent;
    padding: 6px 30px 6px 30px;
    margin: 2px;
    border-radius: 2px;
}

QMenu::item:selected {
    background-color: #e5f3ff;
    color: #007acc;
}

QMenu::item:pressed {
    background-color: #007acc;
    color: #ffffff;
}

QMenu::separator {
    height: 1px;
    background-color: #d0d0d0;
    margin: 4px 8px;
}

/* ========== 工具栏 ========== */
QToolBar {
    background-color: #e8e8e8;
    border: none;
    border-bottom: 1px solid #d0d0d0;
    spacing: 4px;
    padding: 4px;
}

QToolBar::separator {
    background-color: #d0d0d0;
    width: 1px;
    margin: 4px 8px;
}

QToolButton {
    background-color: transparent;
    color: #1e1e1e;
    border: 1px solid transparent;
    border-radius: 3px;
    padding: 6px;
    margin: 2px;
}

QToolButton:hover {
    background-color: #d0d0d0;
    border: 1px solid #007acc;
}

QToolButton:pressed {
    background-color: #007acc;
    color: #ffffff;
}

QToolButton:checked {
    background-color: #cce5ff;
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
    background-color: #f5f5f5;
}

QDockWidget::title {
    background-color: #e8e8e8;
    color: #1e1e1e;
    padding: 6px;
    border: 1px solid #d0d0d0;
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
    background-color: #d0d0d0;
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
    background-color: #cccccc;
    color: #808080;
    border: 1px solid #b0b0b0;
}

/* ========== 列表控件 ========== */
QListWidget {
    background-color: #ffffff;
    color: #1e1e1e;
    border: 1px solid #d0d0d0;
    outline: none;
    padding: 2px;
}

QListWidget::item {
    background-color: transparent;
    color: #1e1e1e;
    padding: 6px;
    border-radius: 2px;
    margin: 1px;
}

QListWidget::item:selected {
    background-color: #cce5ff;
    color: #007acc;
}

QListWidget::item:hover {
    background-color: #e5f3ff;
}

/* ========== 组合框 ========== */
QComboBox {
    background-color: #ffffff;
    color: #1e1e1e;
    border: 1px solid #d0d0d0;
    border-radius: 3px;
    padding: 4px 8px;
    min-width: 100px;
}

QComboBox:hover {
    border: 1px solid #007acc;
}

QComboBox:on {
    background-color: #e5f3ff;
    border: 1px solid #007acc;
}

QComboBox::drop-down {
    border: none;
    width: 20px;
}

QComboBox QAbstractItemView {
    background-color: #ffffff;
    color: #1e1e1e;
    border: 1px solid #d0d0d0;
    selection-background-color: #cce5ff;
    outline: none;
}

/* ========== 复选框 ========== */
QCheckBox {
    color: #1e1e1e;
    spacing: 6px;
}

QCheckBox::indicator {
    width: 16px;
    height: 16px;
    border: 1px solid #d0d0d0;
    border-radius: 2px;
    background-color: #ffffff;
}

QCheckBox::indicator:hover {
    border: 1px solid #007acc;
}

QCheckBox::indicator:checked {
    background-color: #007acc;
    border: 1px solid #007acc;
}

/* ========== 文本输入框 ========== */
QLineEdit {
    background-color: #ffffff;
    color: #1e1e1e;
    border: 1px solid #d0d0d0;
    border-radius: 3px;
    padding: 4px 8px;
    selection-background-color: #cce5ff;
}

QLineEdit:hover {
    border: 1px solid #007acc;
}

QLineEdit:focus {
    border: 1px solid #007acc;
    background-color: #ffffff;
}

/* ========== 数字输入框 ========== */
QSpinBox, QDoubleSpinBox {
    background-color: #ffffff;
    color: #1e1e1e;
    border: 1px solid #d0d0d0;
    border-radius: 3px;
    padding: 4px;
    selection-background-color: #cce5ff;
}

QSpinBox:hover, QDoubleSpinBox:hover {
    border: 1px solid #007acc;
}

QSpinBox:focus, QDoubleSpinBox:focus {
    border: 1px solid #007acc;
}

QSpinBox::up-button, QDoubleSpinBox::up-button {
    background-color: #e8e8e8;
    border-left: 1px solid #d0d0d0;
    border-top-right-radius: 3px;
}

QSpinBox::down-button, QDoubleSpinBox::down-button {
    background-color: #e8e8e8;
    border-left: 1px solid #d0d0d0;
    border-bottom-right-radius: 3px;
}

/* ========== 滚动条 ========== */
QScrollBar:vertical {
    background-color: #f5f5f5;
    width: 14px;
    margin: 0px;
}

QScrollBar::handle:vertical {
    background-color: #c0c0c0;
    min-height: 20px;
    border-radius: 7px;
    margin: 2px;
}

QScrollBar::handle:vertical:hover {
    background-color: #a0a0a0;
}

QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
    height: 0px;
}

QScrollBar:horizontal {
    background-color: #f5f5f5;
    height: 14px;
    margin: 0px;
}

QScrollBar::handle:horizontal {
    background-color: #c0c0c0;
    min-width: 20px;
    border-radius: 7px;
    margin: 2px;
}

QScrollBar::handle:horizontal:hover {
    background-color: #a0a0a0;
}

QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {
    width: 0px;
}

/* ========== 分组框 ========== */
QGroupBox {
    background-color: #f5f5f5;
    border: 1px solid #d0d0d0;
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
    background-color: #f5f5f5;
    border: 1px solid #d0d0d0;
    border-top: none;
}

QTabBar::tab {
    background-color: #e8e8e8;
    color: #1e1e1e;
    border: 1px solid #d0d0d0;
    border-bottom: none;
    padding: 6px 16px;
    margin-right: 2px;
}

QTabBar::tab:selected {
    background-color: #f5f5f5;
    color: #007acc;
    border-bottom: 2px solid #007acc;
}

QTabBar::tab:hover {
    background-color: #d0d0d0;
}

/* ========== 表格控件 ========== */
QTableWidget, QTableView {
    background-color: #ffffff;
    color: #1e1e1e;
    border: 1px solid #d0d0d0;
    gridline-color: #e0e0e0;
    selection-background-color: #cce5ff;
}

QTableWidget::item, QTableView::item {
    padding: 4px;
}

QTableWidget::item:selected, QTableView::item:selected {
    background-color: #cce5ff;
    color: #007acc;
}

QHeaderView::section {
    background-color: #e8e8e8;
    color: #1e1e1e;
    padding: 6px;
    border: none;
    border-right: 1px solid #d0d0d0;
    border-bottom: 1px solid #d0d0d0;
    font-weight: bold;
}

/* ========== 工具提示 ========== */
QToolTip {
    background-color: #ffffff;
    color: #1e1e1e;
    border: 1px solid #007acc;
    padding: 4px;
}

/* ========== 进度条 ========== */
QProgressBar {
    background-color: #e0e0e0;
    border: 1px solid #d0d0d0;
    border-radius: 3px;
    text-align: center;
    color: #1e1e1e;
}

QProgressBar::chunk {
    background-color: #007acc;
    border-radius: 2px;
}

/* ========== 滑块 ========== */
QSlider::groove:horizontal {
    background-color: #e0e0e0;
    height: 6px;
    border-radius: 3px;
}

QSlider::handle:horizontal {
    background-color: #007acc;
    width: 16px;
    height: 16px;
    margin: -5px 0;
    border-radius: 8px;
}

QSlider::handle:horizontal:hover {
    background-color: #1177bb;
}
)";
}

QString Theme::getClassicDarkStyleSheet()
{
    // 经典深色主题 - 使用绿色作为强调色，更传统的配色
    return R"(
/* ========== 经典深色主题 ========== */
QWidget {
    background-color: #2d2d2d;
    color: #e0e0e0;
    font-family: "Segoe UI", "Microsoft YaHei";
    font-size: 9pt;
}

QMainWindow {
    background-color: #252525;
}

/* ========== 菜单栏 ========== */
QMenuBar {
    background-color: #333333;
    color: #e0e0e0;
    border-bottom: 1px solid #444444;
    padding: 2px;
}

QMenuBar::item {
    background-color: transparent;
    padding: 4px 12px;
    margin: 2px 0px;
}

QMenuBar::item:selected {
    background-color: #444444;
    border-radius: 2px;
}

QMenuBar::item:pressed {
    background-color: #4caf50;
    color: #ffffff;
}

/* ========== 菜单 ========== */
QMenu {
    background-color: #333333;
    color: #e0e0e0;
    border: 1px solid #444444;
    padding: 4px;
}

QMenu::item {
    background-color: transparent;
    padding: 6px 30px 6px 30px;
    margin: 2px;
    border-radius: 2px;
}

QMenu::item:selected {
    background-color: #444444;
}

QMenu::item:pressed {
    background-color: #4caf50;
    color: #ffffff;
}

QMenu::separator {
    height: 1px;
    background-color: #444444;
    margin: 4px 8px;
}

/* ========== 工具栏 ========== */
QToolBar {
    background-color: #333333;
    border: none;
    border-bottom: 1px solid #444444;
    spacing: 4px;
    padding: 4px;
}

QToolButton {
    background-color: transparent;
    color: #e0e0e0;
    border: 1px solid transparent;
    border-radius: 3px;
    padding: 6px;
    margin: 2px;
}

QToolButton:hover {
    background-color: #444444;
    border: 1px solid #4caf50;
}

QToolButton:pressed {
    background-color: #4caf50;
    color: #ffffff;
}

QToolButton:checked {
    background-color: #2e7d32;
    border: 1px solid #4caf50;
}

/* ========== 状态栏 ========== */
QStatusBar {
    background-color: #4caf50;
    color: #ffffff;
    border-top: 1px solid #388e3c;
}

QStatusBar QLabel {
    background-color: transparent;
    color: #ffffff;
    padding: 2px 8px;
}

/* ========== 按钮 ========== */
QPushButton {
    background-color: #388e3c;
    color: #ffffff;
    border: 1px solid #4caf50;
    border-radius: 3px;
    padding: 6px 16px;
    min-width: 80px;
}

QPushButton:hover {
    background-color: #4caf50;
}

QPushButton:pressed {
    background-color: #2e7d32;
}

QPushButton:disabled {
    background-color: #444444;
    color: #808080;
    border: 1px solid #444444;
}

/* ========== 列表控件 ========== */
QListWidget {
    background-color: #2a2a2a;
    color: #e0e0e0;
    border: 1px solid #444444;
    outline: none;
}

QListWidget::item:selected {
    background-color: #2e7d32;
    color: #ffffff;
}

QListWidget::item:hover {
    background-color: #383838;
}

/* ========== 组合框 ========== */
QComboBox {
    background-color: #404040;
    color: #e0e0e0;
    border: 1px solid #444444;
    border-radius: 3px;
    padding: 4px 8px;
}

QComboBox:hover {
    border: 1px solid #4caf50;
}

QComboBox QAbstractItemView {
    background-color: #333333;
    color: #e0e0e0;
    border: 1px solid #444444;
    selection-background-color: #2e7d32;
}

/* ========== 文本输入框 ========== */
QLineEdit {
    background-color: #404040;
    color: #e0e0e0;
    border: 1px solid #444444;
    border-radius: 3px;
    padding: 4px 8px;
    selection-background-color: #2e7d32;
}

QLineEdit:hover, QLineEdit:focus {
    border: 1px solid #4caf50;
}

/* ========== 数字输入框 ========== */
QSpinBox, QDoubleSpinBox {
    background-color: #404040;
    color: #e0e0e0;
    border: 1px solid #444444;
    border-radius: 3px;
    padding: 4px;
}

QSpinBox:hover, QDoubleSpinBox:hover {
    border: 1px solid #4caf50;
}

/* ========== 分组框 ========== */
QGroupBox {
    background-color: #2d2d2d;
    border: 1px solid #444444;
    border-radius: 4px;
    margin-top: 12px;
    padding-top: 8px;
}

QGroupBox::title {
    subcontrol-origin: margin;
    subcontrol-position: top left;
    left: 12px;
    padding: 0 6px;
    color: #4caf50;
}

/* ========== 标签页 ========== */
QTabBar::tab {
    background-color: #333333;
    color: #e0e0e0;
    border: 1px solid #444444;
    padding: 6px 16px;
}

QTabBar::tab:selected {
    background-color: #2d2d2d;
    color: #4caf50;
    border-bottom: 2px solid #4caf50;
}

/* ========== 滚动条 ========== */
QScrollBar:vertical {
    background-color: #2d2d2d;
    width: 12px;
}

QScrollBar::handle:vertical {
    background-color: #606060;
    border-radius: 6px;
    margin: 2px;
}

QScrollBar::handle:vertical:hover {
    background-color: #808080;
}

QScrollBar:horizontal {
    background-color: #2d2d2d;
    height: 12px;
}

QScrollBar::handle:horizontal {
    background-color: #606060;
    border-radius: 6px;
    margin: 2px;
}

/* ========== 进度条 ========== */
QProgressBar {
    background-color: #404040;
    border: 1px solid #444444;
    border-radius: 3px;
    text-align: center;
}

QProgressBar::chunk {
    background-color: #4caf50;
    border-radius: 2px;
}
)";
}

QString Theme::getClassicLightStyleSheet()
{
    // 经典浅色主题 - 使用绿色作为强调色
    return R"(
/* ========== 经典浅色主题 ========== */
QWidget {
    background-color: #f0f0f0;
    color: #202020;
    font-family: "Segoe UI", "Microsoft YaHei";
    font-size: 9pt;
}

QMainWindow {
    background-color: #fafafa;
}

/* ========== 菜单栏 ========== */
QMenuBar {
    background-color: #e0e0e0;
    color: #202020;
    border-bottom: 1px solid #c0c0c0;
    padding: 2px;
}

QMenuBar::item {
    background-color: transparent;
    padding: 4px 12px;
}

QMenuBar::item:selected {
    background-color: #c8e6c9;
    border-radius: 2px;
}

QMenuBar::item:pressed {
    background-color: #4caf50;
    color: #ffffff;
}

/* ========== 菜单 ========== */
QMenu {
    background-color: #ffffff;
    color: #202020;
    border: 1px solid #c0c0c0;
    padding: 4px;
}

QMenu::item {
    padding: 6px 30px;
    margin: 2px;
    border-radius: 2px;
}

QMenu::item:selected {
    background-color: #c8e6c9;
    color: #2e7d32;
}

QMenu::item:pressed {
    background-color: #4caf50;
    color: #ffffff;
}

QMenu::separator {
    height: 1px;
    background-color: #c0c0c0;
    margin: 4px 8px;
}

/* ========== 工具栏 ========== */
QToolBar {
    background-color: #e0e0e0;
    border-bottom: 1px solid #c0c0c0;
    spacing: 4px;
    padding: 4px;
}

QToolButton {
    background-color: transparent;
    color: #202020;
    border: 1px solid transparent;
    border-radius: 3px;
    padding: 6px;
}

QToolButton:hover {
    background-color: #c8e6c9;
    border: 1px solid #4caf50;
}

QToolButton:pressed {
    background-color: #4caf50;
    color: #ffffff;
}

QToolButton:checked {
    background-color: #a5d6a7;
    border: 1px solid #4caf50;
}

/* ========== 状态栏 ========== */
QStatusBar {
    background-color: #4caf50;
    color: #ffffff;
    border-top: 1px solid #388e3c;
}

QStatusBar QLabel {
    background-color: transparent;
    color: #ffffff;
    padding: 2px 8px;
}

/* ========== 按钮 ========== */
QPushButton {
    background-color: #388e3c;
    color: #ffffff;
    border: 1px solid #4caf50;
    border-radius: 3px;
    padding: 6px 16px;
    min-width: 80px;
}

QPushButton:hover {
    background-color: #4caf50;
}

QPushButton:pressed {
    background-color: #2e7d32;
}

QPushButton:disabled {
    background-color: #c0c0c0;
    color: #808080;
    border: 1px solid #a0a0a0;
}

/* ========== 列表控件 ========== */
QListWidget {
    background-color: #ffffff;
    color: #202020;
    border: 1px solid #c0c0c0;
}

QListWidget::item:selected {
    background-color: #c8e6c9;
    color: #2e7d32;
}

QListWidget::item:hover {
    background-color: #e8f5e9;
}

/* ========== 组合框 ========== */
QComboBox {
    background-color: #ffffff;
    color: #202020;
    border: 1px solid #c0c0c0;
    border-radius: 3px;
    padding: 4px 8px;
}

QComboBox:hover {
    border: 1px solid #4caf50;
}

QComboBox QAbstractItemView {
    background-color: #ffffff;
    color: #202020;
    border: 1px solid #c0c0c0;
    selection-background-color: #c8e6c9;
}

/* ========== 文本输入框 ========== */
QLineEdit {
    background-color: #ffffff;
    color: #202020;
    border: 1px solid #c0c0c0;
    border-radius: 3px;
    padding: 4px 8px;
    selection-background-color: #c8e6c9;
}

QLineEdit:hover, QLineEdit:focus {
    border: 1px solid #4caf50;
}

/* ========== 数字输入框 ========== */
QSpinBox, QDoubleSpinBox {
    background-color: #ffffff;
    color: #202020;
    border: 1px solid #c0c0c0;
    border-radius: 3px;
    padding: 4px;
}

QSpinBox:hover, QDoubleSpinBox:hover {
    border: 1px solid #4caf50;
}

/* ========== 分组框 ========== */
QGroupBox {
    background-color: #f0f0f0;
    border: 1px solid #c0c0c0;
    border-radius: 4px;
    margin-top: 12px;
    padding-top: 8px;
}

QGroupBox::title {
    subcontrol-origin: margin;
    subcontrol-position: top left;
    left: 12px;
    padding: 0 6px;
    color: #2e7d32;
}

/* ========== 标签页 ========== */
QTabBar::tab {
    background-color: #e0e0e0;
    color: #202020;
    border: 1px solid #c0c0c0;
    padding: 6px 16px;
}

QTabBar::tab:selected {
    background-color: #f0f0f0;
    color: #2e7d32;
    border-bottom: 2px solid #4caf50;
}

/* ========== 表格控件 ========== */
QTableWidget, QTableView {
    background-color: #ffffff;
    color: #202020;
    border: 1px solid #c0c0c0;
    gridline-color: #e0e0e0;
    selection-background-color: #c8e6c9;
}

QHeaderView::section {
    background-color: #e0e0e0;
    color: #202020;
    padding: 6px;
    border: none;
    border-right: 1px solid #c0c0c0;
    border-bottom: 1px solid #c0c0c0;
    font-weight: bold;
}

/* ========== 滚动条 ========== */
QScrollBar:vertical {
    background-color: #f0f0f0;
    width: 12px;
}

QScrollBar::handle:vertical {
    background-color: #b0b0b0;
    border-radius: 6px;
    margin: 2px;
}

QScrollBar::handle:vertical:hover {
    background-color: #909090;
}

QScrollBar:horizontal {
    background-color: #f0f0f0;
    height: 12px;
}

QScrollBar::handle:horizontal {
    background-color: #b0b0b0;
    border-radius: 6px;
    margin: 2px;
}

/* ========== 进度条 ========== */
QProgressBar {
    background-color: #e0e0e0;
    border: 1px solid #c0c0c0;
    border-radius: 3px;
    text-align: center;
}

QProgressBar::chunk {
    background-color: #4caf50;
    border-radius: 2px;
}
)";
}

} // namespace UI
} // namespace VisionForge
