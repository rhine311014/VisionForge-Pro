/**
 * @file StationConfigToolMain.cpp
 * @brief 工位配置工具独立程序入口
 * @details 可单独运行的工位配置工具，用于现场配置工位参数
 */

#include <QApplication>
#include <QMessageBox>
#include <QTranslator>
#include <QLibraryInfo>

// Windows平台UTF-8控制台支持
#ifdef _WIN32
#include <windows.h>
#endif

// 基础设施层
#include "base/Logger.h"

// UI层
#include "ui/StationConfigTool.h"
#include "ui/Theme.h"

// 核心层
#include "core/MultiStationManager.h"

using namespace VisionForge::Base;
using namespace VisionForge::UI;

/**
 * @brief 设置Windows控制台为UTF-8编码
 */
void setupConsoleEncoding()
{
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif
}

/**
 * @brief 主函数
 */
int main(int argc, char *argv[])
{
    // 首先设置控制台UTF-8编码
    setupConsoleEncoding();

    // 设置应用程序信息
    QApplication::setApplicationName("VisionForge StationConfig");
    QApplication::setApplicationVersion("1.0.0");
    QApplication::setOrganizationName("VisionForge Team");
    QApplication::setOrganizationDomain("visionforge.com");

    // 创建应用程序实例
    QApplication app(argc, argv);

    // 启用高DPI支持
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif

    // 初始化Logger
    Logger::instance().setLogLevel(Logger::Debug);
    Logger::instance().enableConsoleOutput(true);
    Logger::instance().setLogFile("StationConfig.log");
    Logger::instance().enableFileOutput(true);

    // 加载Qt中文翻译
    QTranslator qtTranslator;
    QTranslator qtBaseTranslator;
    QString translationsPath = QLibraryInfo::path(QLibraryInfo::TranslationsPath);

    if (qtTranslator.load("qt_zh_CN", translationsPath)) {
        app.installTranslator(&qtTranslator);
    }

    if (qtBaseTranslator.load("qtbase_zh_CN", translationsPath)) {
        app.installTranslator(&qtBaseTranslator);
    }

    LOG_INFO("========================================");
    LOG_INFO("VisionForge 工位配置工具 v1.0.0");
    LOG_INFO("========================================");

    // 应用工业深色主题
    Theme::applyTheme(Theme::IndustrialDark);
    LOG_INFO("[主题] 应用工业深色主题");

    // 加载现有配置
    auto& stationManager = VisionForge::Core::MultiStationManager::instance();
    LOG_INFO(QString("[配置] 配置目录: %1").arg(stationManager.getConfigDirectory()));
    stationManager.loadConfig();

    int existingCount = stationManager.getStationCount();
    if (existingCount > 0) {
        LOG_INFO(QString("[配置] 已加载现有配置: %1 个工位").arg(existingCount));
    } else {
        LOG_INFO("[配置] 未找到现有配置，将创建新配置");
    }

    // 显示工位配置工具
    StationConfigTool configTool;
    configTool.setWindowTitle("VisionForge 工位配置工具");

    int result = configTool.exec();

    if (result == QDialog::Accepted && configTool.isSaveAndExit()) {
        LOG_INFO(QString("[配置] 配置已保存: %1 个平台").arg(configTool.getStationCount()));
        QMessageBox::information(nullptr, "配置完成",
            QString("工位配置已保存成功！\n共配置 %1 个平台。")
            .arg(configTool.getStationCount()));
    } else {
        LOG_INFO("[配置] 用户取消配置");
    }

    LOG_INFO("[退出] 工位配置工具正常退出");
    LOG_INFO("========================================");

    return 0;
}
