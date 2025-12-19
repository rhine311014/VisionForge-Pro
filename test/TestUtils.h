/**
 * @file TestUtils.h
 * @brief 测试辅助工具
 * @author VisionForge Team
 * @date 2025-12-19
 */

#pragma once

#include <QObject>
#include <QString>
#include <QDir>
#include <QTemporaryDir>
#include <QImage>
#include <opencv2/core.hpp>

namespace VisionForge {
namespace Test {

/**
 * @class TestUtils
 * @brief 测试辅助工具类
 */
class TestUtils
{
public:
    /**
     * @brief 创建测试用临时目录
     * @return 临时目录路径
     */
    static QString createTempDir();

    /**
     * @brief 删除临时目录
     * @param path 目录路径
     */
    static void removeTempDir(const QString& path);

    /**
     * @brief 创建测试用图像（CV_8UC1）
     * @param width 宽度
     * @param height 高度
     * @param value 填充值
     * @return cv::Mat图像
     */
    static cv::Mat createGrayImage(int width, int height, uchar value = 128);

    /**
     * @brief 创建测试用彩色图像（CV_8UC3）
     * @param width 宽度
     * @param height 高度
     * @param r 红色通道值
     * @param g 绿色通道值
     * @param b 蓝色通道值
     * @return cv::Mat图像
     */
    static cv::Mat createColorImage(int width, int height, uchar r = 128, uchar g = 128, uchar b = 128);

    /**
     * @brief 创建带噪声的测试图像
     * @param width 宽度
     * @param height 高度
     * @param noiseLevel 噪声级别(0-255)
     * @return cv::Mat图像
     */
    static cv::Mat createNoisyImage(int width, int height, int noiseLevel = 30);

    /**
     * @brief 创建棋盘格图像
     * @param width 宽度
     * @param height 高度
     * @param cellSize 格子大小
     * @return cv::Mat图像
     */
    static cv::Mat createCheckerboard(int width, int height, int cellSize = 32);

    /**
     * @brief 创建圆形测试图像
     * @param width 宽度
     * @param height 高度
     * @param circleRadius 圆形半径
     * @return cv::Mat图像
     */
    static cv::Mat createCircleImage(int width, int height, int circleRadius = 50);

    /**
     * @brief 比较两个图像是否相同
     * @param img1 图像1
     * @param img2 图像2
     * @param tolerance 容差值
     * @return true如果相同
     */
    static bool compareImages(const cv::Mat& img1, const cv::Mat& img2, double tolerance = 0.0);

    /**
     * @brief 计算两个图像的PSNR
     * @param img1 图像1
     * @param img2 图像2
     * @return PSNR值
     */
    static double calculatePSNR(const cv::Mat& img1, const cv::Mat& img2);

    /**
     * @brief 等待指定毫秒（支持Qt事件循环）
     * @param ms 毫秒数
     */
    static void waitMs(int ms);

    /**
     * @brief 获取测试资源目录
     * @return 资源目录路径
     */
    static QString getTestResourceDir();

    /**
     * @brief 保存测试结果图像（用于调试）
     * @param image 图像
     * @param filename 文件名
     */
    static void saveDebugImage(const cv::Mat& image, const QString& filename);
};

/**
 * @class ScopedTempDir
 * @brief RAII临时目录
 */
class ScopedTempDir
{
public:
    ScopedTempDir();
    ~ScopedTempDir();

    QString path() const { return tempDir_.path(); }
    bool isValid() const { return tempDir_.isValid(); }

private:
    QTemporaryDir tempDir_;
};

} // namespace Test
} // namespace VisionForge
