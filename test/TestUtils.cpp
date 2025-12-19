/**
 * @file TestUtils.cpp
 * @brief 测试辅助工具实现
 */

#include "TestUtils.h"
#include <QCoreApplication>
#include <QThread>
#include <QStandardPaths>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <random>

namespace VisionForge {
namespace Test {

QString TestUtils::createTempDir()
{
    QString tempPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    QString testDir = tempPath + "/visionforge_test_" + QString::number(QCoreApplication::applicationPid());
    QDir().mkpath(testDir);
    return testDir;
}

void TestUtils::removeTempDir(const QString& path)
{
    QDir dir(path);
    if (dir.exists()) {
        dir.removeRecursively();
    }
}

cv::Mat TestUtils::createGrayImage(int width, int height, uchar value)
{
    cv::Mat image(height, width, CV_8UC1, cv::Scalar(value));
    return image;
}

cv::Mat TestUtils::createColorImage(int width, int height, uchar r, uchar g, uchar b)
{
    cv::Mat image(height, width, CV_8UC3, cv::Scalar(b, g, r));  // BGR顺序
    return image;
}

cv::Mat TestUtils::createNoisyImage(int width, int height, int noiseLevel)
{
    cv::Mat image = createGrayImage(width, height, 128);
    cv::Mat noise(height, width, CV_8UC1);

    cv::randn(noise, 0, noiseLevel);
    image += noise;

    return image;
}

cv::Mat TestUtils::createCheckerboard(int width, int height, int cellSize)
{
    cv::Mat image(height, width, CV_8UC1);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int cellX = x / cellSize;
            int cellY = y / cellSize;
            image.at<uchar>(y, x) = ((cellX + cellY) % 2 == 0) ? 255 : 0;
        }
    }

    return image;
}

cv::Mat TestUtils::createCircleImage(int width, int height, int circleRadius)
{
    cv::Mat image = createGrayImage(width, height, 0);
    cv::Point center(width / 2, height / 2);
    cv::circle(image, center, circleRadius, cv::Scalar(255), -1);
    return image;
}

bool TestUtils::compareImages(const cv::Mat& img1, const cv::Mat& img2, double tolerance)
{
    if (img1.size() != img2.size() || img1.type() != img2.type()) {
        return false;
    }

    if (tolerance == 0.0) {
        cv::Mat diff;
        cv::absdiff(img1, img2, diff);
        return cv::countNonZero(diff.reshape(1)) == 0;
    } else {
        cv::Mat diff;
        cv::absdiff(img1, img2, diff);
        double maxDiff;
        cv::minMaxLoc(diff, nullptr, &maxDiff);
        return maxDiff <= tolerance;
    }
}

double TestUtils::calculatePSNR(const cv::Mat& img1, const cv::Mat& img2)
{
    if (img1.size() != img2.size() || img1.type() != img2.type()) {
        return -1.0;
    }

    cv::Mat diff;
    cv::absdiff(img1, img2, diff);
    diff.convertTo(diff, CV_32F);
    diff = diff.mul(diff);

    cv::Scalar sumVal = cv::sum(diff);
    double sse = sumVal[0];

    if (diff.channels() > 1) {
        sse += sumVal[1] + sumVal[2];
    }

    if (sse <= 1e-10) {
        return 100.0;  // 相同图像
    }

    double mse = sse / (double)(img1.channels() * img1.total());
    double psnr = 10.0 * log10(255.0 * 255.0 / mse);

    return psnr;
}

void TestUtils::waitMs(int ms)
{
    QThread::msleep(ms);
    QCoreApplication::processEvents();
}

QString TestUtils::getTestResourceDir()
{
    QString sourceDir = QString(__FILE__);
    QFileInfo fileInfo(sourceDir);
    return fileInfo.absolutePath() + "/resources";
}

void TestUtils::saveDebugImage(const cv::Mat& image, const QString& filename)
{
    QString debugDir = createTempDir() + "/debug";
    QDir().mkpath(debugDir);
    QString fullPath = debugDir + "/" + filename;
    cv::imwrite(fullPath.toStdString(), image);
}

// ============================================================
// ScopedTempDir
// ============================================================

ScopedTempDir::ScopedTempDir()
    : tempDir_("visionforge_test")
{
}

ScopedTempDir::~ScopedTempDir()
{
    // QTemporaryDir会自动清理
}

} // namespace Test
} // namespace VisionForge
