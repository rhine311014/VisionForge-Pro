/**
 * @file main.cpp
 * @brief VisionForge Pro - 主程序入口
 * @author VisionForge Team
 * @date 2025-12-14
 * @version 1.0.0
 */

#include <QApplication>
#include <QMessageBox>
#include <QSplashScreen>
#include <QPixmap>
#include <QThread>
#include <QDebug>

// TODO: 待实现的头文件
// #include "ui/MainWindow.h"
// #include "core/RecipeManager.h"
// #include "base/Logger.h"
// #include "base/ConfigManager.h"

/**
 * @brief 显示启动画面
 * @param msec 显示时长（毫秒）
 */
void showSplashScreen(int msec = 2000) {
    // TODO: 创建启动画面
    // QPixmap pixmap(":/images/splash.png");
    // QSplashScreen splash(pixmap);
    // splash.show();
    // QThread::msleep(msec);
    // splash.finish(&mainWindow);

    qDebug() << "[启动] 启动画面显示";
}

/**
 * @brief 初始化应用程序
 * @return 是否成功
 */
bool initializeApplication() {
    qDebug() << "[启动] 初始化应用程序...";

    // TODO: 初始化日志系统
    // Logger::instance().initialize();

    // TODO: 加载配置文件
    // ConfigManager::instance().load();

    // TODO: 初始化硬件抽象层
    // HALManager::instance().initialize();

    qDebug() << "[启动] 初始化完成";
    return true;
}

/**
 * @brief 主函数
 */
int main(int argc, char *argv[])
{
    // 设置应用程序信息
    QApplication::setApplicationName("VisionForge Pro");
    QApplication::setApplicationVersion(VISIONFORGE_VERSION);
    QApplication::setOrganizationName("VisionForge Team");
    QApplication::setOrganizationDomain("visionforge.com");

    // 创建应用程序实例
    QApplication app(argc, argv);

    qInfo() << "========================================";
    qInfo() << "VisionForge Pro v" << VISIONFORGE_VERSION;
    qInfo() << "通用工业视觉检测平台";
    qInfo() << "========================================";

    // 启用高DPI支持
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    // 设置应用程序样式（可选）
    // app.setStyle("Fusion");

    // 显示启动画面
    showSplashScreen();

    // 初始化应用程序
    if (!initializeApplication()) {
        QMessageBox::critical(nullptr, "启动失败",
                            "应用程序初始化失败，请检查配置文件和日志。");
        return -1;
    }

    // TODO: 创建并显示主窗口
    // MainWindow mainWindow;
    // mainWindow.show();

    // 临时：显示欢迎对话框
    QMessageBox::information(nullptr, "VisionForge Pro",
                            "欢迎使用 VisionForge Pro！\n\n"
                            "当前版本: " VISIONFORGE_VERSION "\n"
                            "构建日期: " __DATE__ "\n\n"
                            "注意：这是一个开发中的版本，"
                            "UI界面尚未实现。");

    qInfo() << "[启动] 应用程序启动成功";

    // 进入事件循环
    // return app.exec();

    // 临时：直接退出
    qInfo() << "[退出] 应用程序正常退出";
    return 0;
}
