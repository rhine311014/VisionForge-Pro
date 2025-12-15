/**
 * @file main.cpp
 * @brief VisionForge Pro - 主程序入口
 * @author VisionForge Team
 * @date 2025-12-14
 * @version 1.0.0
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
#include "base/ConfigManager.h"

// UI层
#include "ui/MainWindow.h"
#include "ui/Theme.h"

using namespace VisionForge::Base;
using namespace VisionForge::UI;

/**
 * @brief 设置Windows控制台为UTF-8编码
 */
void setupConsoleEncoding()
{
#ifdef _WIN32
    // 设置控制台代码页为UTF-8 (65001)
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    // 注意：不使用_setmode(_O_U8TEXT)，因为Logger使用std::cout输出char*字符串
    // 只设置控制台代码页，QString::toStdString()会自动转换为UTF-8
#endif
}

/**
 * @brief 主函数
 */
int main(int argc, char *argv[])
{
    // 首先设置控制台UTF-8编码（必须在任何输出之前）
    setupConsoleEncoding();

    // 设置应用程序信息
    QApplication::setApplicationName("VisionForge Pro");
    QApplication::setApplicationVersion(VISIONFORGE_VERSION);
    QApplication::setOrganizationName("VisionForge Team");
    QApplication::setOrganizationDomain("visionforge.com");

    // 创建应用程序实例
    QApplication app(argc, argv);

    // 启用高DPI支持
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif

    // 初始化Logger（必须在记录日志之前）
    Logger::instance().setLogLevel(Logger::Debug);
    Logger::instance().enableConsoleOutput(true);
    Logger::instance().setLogFile("VisionForge.log");
    Logger::instance().enableFileOutput(true);

    // 加载Qt中文翻译
    QTranslator qtTranslator;
    QTranslator qtBaseTranslator;
    QString translationsPath = QLibraryInfo::path(QLibraryInfo::TranslationsPath);

    // 加载Qt基础控件翻译（按钮、菜单等标准控件）
    if (qtTranslator.load("qt_zh_CN", translationsPath)) {
        app.installTranslator(&qtTranslator);
        LOG_DEBUG("Qt中文翻译已加载: qt_zh_CN");
    } else {
        LOG_WARNING(QString("未找到Qt中文翻译文件: %1/qt_zh_CN.qm").arg(translationsPath));
    }

    // 加载Qt Base模块翻译（对话框、文件选择器等）
    if (qtBaseTranslator.load("qtbase_zh_CN", translationsPath)) {
        app.installTranslator(&qtBaseTranslator);
        LOG_DEBUG("Qt Base中文翻译已加载: qtbase_zh_CN");
    } else {
        LOG_WARNING(QString("未找到Qt Base中文翻译文件: %1/qtbase_zh_CN.qm").arg(translationsPath));
    }

    LOG_INFO("========================================");
    LOG_INFO(QString("VisionForge Pro v%1").arg(VISIONFORGE_VERSION));
    LOG_INFO("通用工业视觉检测平台");
    LOG_INFO("========================================");

    LOG_INFO("[启动] 应用程序初始化...");

    // 加载配置
    ConfigManager& config = ConfigManager::instance();
    LOG_INFO(QString("配置文件路径: %1").arg(config.configFilePath()));

    // 应用工业深色主题
    Theme::applyTheme(Theme::IndustrialDark);
    LOG_INFO("[主题] 应用工业深色主题");

    // 创建主窗口
    MainWindow mainWindow;
    mainWindow.show();

    LOG_INFO("[启动] 主窗口已显示");

    // 运行应用程序
    int ret = app.exec();

    LOG_INFO("[退出] 应用程序正常退出");
    LOG_INFO("========================================");

    return ret;
}
