/**
 * @file Theme.h
 * @brief 主题管理器
 * @details 管理应用程序的视觉主题和样式
 * @author VisionForge Team
 * @date 2025-12-14
 */

#pragma once

#include <QString>
#include <QIcon>
#include <QStyle>
#include <QApplication>

namespace VisionForge {
namespace UI {

/**
 * @class Theme
 * @brief 主题管理类
 *
 * 提供专业工业风格的深色主题
 */
class Theme
{
public:
    /**
     * @brief 主题类型
     */
    enum ThemeType {
        IndustrialDark,     // 工业深色主题（默认）
        IndustrialLight,    // 工业浅色主题
        ClassicDark,        // 经典深色主题
        ClassicLight        // 经典浅色主题
    };

    /**
     * @brief 应用主题到应用程序
     * @param type 主题类型
     */
    static void applyTheme(ThemeType type = IndustrialDark);

    /**
     * @brief 获取主题样式表
     * @param type 主题类型
     */
    static QString getStyleSheet(ThemeType type);

    /**
     * @brief 获取图标
     * @param name 图标名称
     */
    static QIcon getIcon(const QString& name);

    /**
     * @brief 获取标准图标
     * @param icon Qt标准图标
     */
    static QIcon getStandardIcon(QStyle::StandardPixmap icon);

private:
    /**
     * @brief 获取工业深色主题样式表
     */
    static QString getIndustrialDarkStyleSheet();

    /**
     * @brief 获取工业浅色主题样式表
     */
    static QString getIndustrialLightStyleSheet();

    /**
     * @brief 获取经典深色主题样式表
     */
    static QString getClassicDarkStyleSheet();

    /**
     * @brief 获取经典浅色主题样式表
     */
    static QString getClassicLightStyleSheet();
};

/**
 * @brief 图标名称常量
 */
namespace Icons {
    // 文件操作
    constexpr const char* FILE_OPEN = "file-open";
    constexpr const char* FILE_SAVE = "file-save";
    constexpr const char* FILE_SAVE_AS = "file-save-as";
    constexpr const char* FILE_CLOSE = "file-close";
    constexpr const char* EXIT = "application-exit";

    // 编辑操作
    constexpr const char* EDIT_CUT = "edit-cut";
    constexpr const char* EDIT_COPY = "edit-copy";
    constexpr const char* EDIT_PASTE = "edit-paste";
    constexpr const char* EDIT_DELETE = "edit-delete";

    // 视图操作
    constexpr const char* ZOOM_IN = "zoom-in";
    constexpr const char* ZOOM_OUT = "zoom-out";
    constexpr const char* ZOOM_FIT = "zoom-fit-best";
    constexpr const char* ZOOM_ORIGINAL = "zoom-original";

    // 工具操作
    constexpr const char* TOOL_ADD = "list-add";
    constexpr const char* TOOL_REMOVE = "list-remove";
    constexpr const char* TOOL_UP = "go-up";
    constexpr const char* TOOL_DOWN = "go-down";
    constexpr const char* TOOL_RUN = "media-playback-start";
    constexpr const char* TOOL_STOP = "media-playback-stop";

    // 相机操作
    constexpr const char* CAMERA_PHOTO = "camera-photo";
    constexpr const char* CAMERA_VIDEO = "camera-video";
    constexpr const char* CAMERA_WEB = "camera-web";

    // 应用程序
    constexpr const char* APP_SETTINGS = "preferences-system";
    constexpr const char* APP_HELP = "help-browser";
    constexpr const char* APP_ABOUT = "help-about";
}

} // namespace UI
} // namespace VisionForge
