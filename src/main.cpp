/**
 * @file main.cpp
 * @brief VisionForge Pro - 主程序入口
 * @author VisionForge Team
 * @date 2025-12-14
 * @version 1.0.0
 */

#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include <QElapsedTimer>
#include <opencv2/opencv.hpp>

// 基础设施层
#include "base/Logger.h"
#include "base/ConfigManager.h"
#include "base/ImageData.h"
#include "base/ImageMemoryPool.h"

using namespace VisionForge::Base;

/**
 * @brief 测试基础设施层功能
 */
void testBaseInfrastructure()
{
    qInfo() << "========================================";
    qInfo() << "基础设施层功能测试";
    qInfo() << "========================================";

    // ========== 测试Logger ==========
    qInfo() << "\n[测试1] Logger日志系统";
    LOG_INFO("Logger初始化成功");
    LOG_DEBUG("这是一条调试信息");
    LOG_WARNING("这是一条警告信息");
    LOG_ERROR("这是一条错误信息");

    // 设置日志文件
    Logger::instance().setLogFile("VisionForge.log");
    Logger::instance().enableFileOutput(true);
    LOG_INFO("日志文件设置完成: VisionForge.log");

    // ========== 测试ConfigManager ==========
    qInfo() << "\n[测试2] ConfigManager配置管理";
    ConfigManager& config = ConfigManager::instance();

    // 写入配置
    config.setValue("app/version", "1.0.0");
    config.setValue("app/language", "zh_CN");
    config.setValue("camera/width", 1920);
    config.setValue("camera/height", 1080);
    config.save();

    LOG_INFO(QString("配置文件路径: %1").arg(config.configFilePath()));
    LOG_INFO(QString("应用版本: %1").arg(config.getValue("app/version").toString()));
    LOG_INFO(QString("相机分辨率: %1x%2")
        .arg(config.getValue("camera/width").toInt())
        .arg(config.getValue("camera/height").toInt()));

    // ========== 测试ImageData ==========
    qInfo() << "\n[测试3] ImageData图像数据";

    // 创建测试图像
    ImageData::Ptr image1 = std::make_shared<ImageData>(1920, 1080, CV_8UC3);
    LOG_INFO(QString("创建图像: %1x%2, %3字节")
        .arg(image1->width())
        .arg(image1->height())
        .arg(image1->totalBytes()));
    LOG_INFO(QString("内存对齐: %1, 对齐字节: %2")
        .arg(image1->isMemoryAligned() ? "是" : "否")
        .arg(image1->getAlignment()));

    // 填充测试图像
    image1->mat() = cv::Scalar(100, 150, 200);
    LOG_INFO("图像填充完成");

    // 浅拷贝测试
    ImageData::Ptr image2 = image1->clone();
    LOG_INFO("浅拷贝完成");

    // 深拷贝测试
    ImageData::Ptr image3 = image1->deepCopy();
    LOG_INFO("深拷贝完成");

    // ========== 测试ImageMemoryPool ==========
    qInfo() << "\n[测试4] ImageMemoryPool内存池";

    ImageMemoryPool& pool = ImageMemoryPool::instance();

    // 预分配
    pool.preallocate(640, 480, CV_8UC3, 5);
    LOG_INFO("预分配5个640x480图像");

    // 性能测试：分配1000次
    QElapsedTimer timer;
    timer.start();

    for (int i = 0; i < 1000; ++i) {
        ImageData::Ptr img = pool.allocate(640, 480, CV_8UC3);
        // 使用完自动归还
    }

    qint64 elapsed = timer.elapsed();
    LOG_INFO(QString("分配1000次耗时: %1 ms").arg(elapsed));

    // 统计信息
    auto stats = pool.getStatistics();
    LOG_INFO(QString("总分配次数: %1").arg(stats.totalAllocations));
    LOG_INFO(QString("缓存命中次数: %1").arg(stats.cacheHits));
    LOG_INFO(QString("缓存未命中次数: %1").arg(stats.cacheMisses));
    LOG_INFO(QString("命中率: %1%").arg(stats.hitRate * 100, 0, 'f', 2));
    LOG_INFO(QString("池大小: %1").arg(stats.poolSize));
    LOG_INFO(QString("内存使用: %1 MB").arg(stats.memoryUsage / 1024.0 / 1024.0, 0, 'f', 2));

    qInfo() << "\n========================================";
    qInfo() << "基础设施层测试完成";
    qInfo() << "========================================";
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
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif

    // 初始化Logger
    Logger::instance().setLogLevel(Logger::Debug);
    Logger::instance().enableConsoleOutput(true);

    qInfo() << "[启动] 应用程序初始化...";

    // 测试基础设施层
    testBaseInfrastructure();

    // 显示欢迎对话框
    QMessageBox::information(nullptr, "VisionForge Pro",
                            QString("欢迎使用 VisionForge Pro！\n\n"
                                   "当前版本: %1\n"
                                   "构建日期: %2\n\n"
                                   "基础设施层测试成功！\n"
                                   "请查看控制台输出和日志文件。")
                            .arg(VISIONFORGE_VERSION)
                            .arg(__DATE__));

    qInfo() << "[退出] 应用程序正常退出";
    qInfo() << "========================================";

    return 0;
}
