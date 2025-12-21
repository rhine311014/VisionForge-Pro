/**
 * @file SubPixelEdgeTool.cpp
 * @brief 亚像素边缘检测工具实现
 * @author VisionForge Team
 * @date 2025-12-20
 */

#include "algorithm/SubPixelEdgeTool.h"
#include "base/Logger.h"
#include "base/ImageMemoryPool.h"
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <QElapsedTimer>
#include <QJsonArray>
#include <cmath>
#include <complex>
#include <algorithm>

namespace VisionForge {
namespace Algorithm {

// ============================================================
// 构造函数和VisionTool接口实现
// ============================================================

SubPixelEdgeTool::SubPixelEdgeTool(QObject* parent)
    : VisionTool(parent)
{
    setDisplayName(toolName());
}

bool SubPixelEdgeTool::process(const Base::ImageData::Ptr& input, ToolResult& output)
{
    if (!input || input->isEmpty()) {
        output.success = false;
        output.errorMessage = "输入图像为空";
        return false;
    }

    QElapsedTimer timer;
    timer.start();

    cv::Mat src = input->mat();
    cv::Mat gray;

    // 转换为灰度图
    if (src.channels() == 3) {
        cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    } else if (src.channels() == 4) {
        cv::cvtColor(src, gray, cv::COLOR_BGRA2GRAY);
    } else {
        gray = src.clone();
    }

    // 检测亚像素边缘
    cv::Rect roi(0, 0, gray.cols, gray.rows);
    lastResult_ = detectSubPixelEdge(gray, roi, config_);
    lastResult_.processTime = timer.elapsed();

    // 构建输出
    output.success = (lastResult_.validPointCount > 0);
    output.executionTime = timer.elapsed();

    output.setValue("edgePointCount", lastResult_.validPointCount);
    output.setValue("averageSubPixelShift", lastResult_.averageSubPixelShift);
    output.setValue("maxGradient", lastResult_.maxGradient);

    // 记录边缘点信息
    QJsonArray edgeArray;
    for (const auto& pt : lastResult_.edgePoints) {
        if (pt.isValid) {
            QJsonObject edgeObj;
            edgeObj["x"] = pt.position.x;
            edgeObj["y"] = pt.position.y;
            edgeObj["gradient"] = pt.gradient;
            edgeObj["direction"] = pt.direction;
            edgeObj["subPixelShift"] = pt.subPixelShift;
            edgeArray.append(edgeObj);
        }
    }
    output.data["edges"] = edgeArray;

    // 绘制结果
    cv::Mat resultImage;
    if (src.channels() == 1) {
        cv::cvtColor(src, resultImage, cv::COLOR_GRAY2BGR);
    } else {
        resultImage = src.clone();
    }

    // 绘制亚像素边缘点
    for (const auto& pt : lastResult_.edgePoints) {
        if (pt.isValid) {
            cv::circle(resultImage,
                      cv::Point(static_cast<int>(pt.position.x + 0.5f),
                               static_cast<int>(pt.position.y + 0.5f)),
                      1, cv::Scalar(0, 255, 0), -1);
        }
    }

    output.outputImage = Base::ImageMemoryPool::instance().allocate(
        resultImage.cols, resultImage.rows, resultImage.type());
    if (output.outputImage) {
        resultImage.copyTo(output.outputImage->mat());
    }
    setDebugImage(output.outputImage);
    setStatusText(QString("检测到 %1 个亚像素边缘点，平均偏移: %.3f像素")
                 .arg(lastResult_.validPointCount)
                 .arg(lastResult_.averageSubPixelShift));

    LOG_DEBUG(QString("亚像素边缘检测完成，点数: %1，耗时: %2ms")
             .arg(lastResult_.validPointCount)
             .arg(output.executionTime));

    return output.success;
}

QJsonObject SubPixelEdgeTool::serializeParams() const
{
    QJsonObject json;
    json["method"] = static_cast<int>(config_.method);
    json["gradientThreshold"] = config_.gradientThreshold;
    json["windowSize"] = config_.windowSize;
    json["useSubPixel"] = config_.useSubPixel;
    json["sigma"] = config_.sigma;
    json["cannyLow"] = config_.cannyLow;
    json["cannyHigh"] = config_.cannyHigh;
    return json;
}

void SubPixelEdgeTool::deserializeParams(const QJsonObject& json)
{
    config_.method = static_cast<SubPixelMethod>(json.value("method").toInt(
        static_cast<int>(SubPixelMethod::QuadraticFit)));
    config_.gradientThreshold = json.value("gradientThreshold").toDouble(20.0);
    config_.windowSize = json.value("windowSize").toInt(5);
    config_.useSubPixel = json.value("useSubPixel").toBool(true);
    config_.sigma = json.value("sigma").toDouble(1.0);
    config_.cannyLow = json.value("cannyLow").toInt(50);
    config_.cannyHigh = json.value("cannyHigh").toInt(150);
}

// ============================================================
// 核心亚像素边缘检测实现
// ============================================================

SubPixelEdgeResult SubPixelEdgeTool::detectSubPixelEdge(
    const cv::Mat& image,
    const cv::Rect& roi,
    const SubPixelEdgeConfig& config)
{
    SubPixelEdgeResult result;
    QElapsedTimer timer;
    timer.start();

    if (image.empty()) {
        return result;
    }

    // 提取ROI区域
    cv::Mat roiImage = image(roi);
    cv::Mat gray;

    if (roiImage.channels() > 1) {
        cv::cvtColor(roiImage, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = roiImage.clone();
    }

    // 高斯平滑
    if (config.sigma > 0) {
        int ksize = static_cast<int>(std::ceil(config.sigma * 3)) * 2 + 1;
        cv::GaussianBlur(gray, gray, cv::Size(ksize, ksize), config.sigma);
    }

    // 计算Sobel梯度
    cv::Mat gradX, gradY, magnitude;
    computeSobelGradient(gray, gradX, gradY, magnitude);

    // 非极大值抑制
    cv::Mat nms = nonMaximumSuppression(magnitude, gradX, gradY);

    // 遍历NMS结果，对每个边缘点进行亚像素精化
    double totalShift = 0;
    result.maxGradient = 0;

    for (int y = config.windowSize; y < nms.rows - config.windowSize; ++y) {
        for (int x = config.windowSize; x < nms.cols - config.windowSize; ++x) {
            float mag = nms.at<float>(y, x);

            if (mag < config.gradientThreshold) {
                continue;
            }

            SubPixelEdgePoint edgePoint;
            edgePoint.gradient = mag;
            edgePoint.direction = std::atan2(
                static_cast<float>(gradY.at<short>(y, x)),
                static_cast<float>(gradX.at<short>(y, x)));

            if (config.useSubPixel) {
                // 根据方法选择亚像素定位算法
                cv::Point2f subPixelPos;

                switch (config.method) {
                case SubPixelMethod::SobelInterpolation:
                    subPixelPos = interpolateEdgePosition(gradX, gradY, x, y);
                    break;

                case SubPixelMethod::ZernikeMoment:
                    subPixelPos = zernikeMomentSubPixel(gray, x, y, config.windowSize);
                    break;

                case SubPixelMethod::QuadraticFit: {
                    // 沿梯度方向提取剖面并拟合
                    std::vector<float> profile = extractProfileAlongNormal(
                        magnitude, cv::Point2f(static_cast<float>(x), static_cast<float>(y)),
                        edgePoint.direction, config.windowSize);
                    int peakIdx = config.windowSize / 2;
                    float offset = quadraticFitSubPixel(profile, peakIdx);

                    // 计算亚像素位置
                    float dx = std::cos(edgePoint.direction) * offset;
                    float dy = std::sin(edgePoint.direction) * offset;
                    subPixelPos = cv::Point2f(x + dx, y + dy);
                    break;
                }

                case SubPixelMethod::Gaussian1D: {
                    std::vector<float> profile = extractProfileAlongNormal(
                        magnitude, cv::Point2f(static_cast<float>(x), static_cast<float>(y)),
                        edgePoint.direction, config.windowSize);
                    int peakIdx = config.windowSize / 2;
                    float offset = gaussianFitSubPixel(profile, peakIdx, config.windowSize);

                    float dx = std::cos(edgePoint.direction) * offset;
                    float dy = std::sin(edgePoint.direction) * offset;
                    subPixelPos = cv::Point2f(x + dx, y + dy);
                    break;
                }

                case SubPixelMethod::ESRFit:
                    subPixelPos = esrSubPixel(gray, x, y);
                    break;
                }

                edgePoint.position = cv::Point2f(
                    subPixelPos.x + roi.x,
                    subPixelPos.y + roi.y);

                // 计算亚像素偏移量
                float shiftX = subPixelPos.x - x;
                float shiftY = subPixelPos.y - y;
                edgePoint.subPixelShift = std::sqrt(shiftX * shiftX + shiftY * shiftY);
            } else {
                edgePoint.position = cv::Point2f(
                    static_cast<float>(x + roi.x),
                    static_cast<float>(y + roi.y));
                edgePoint.subPixelShift = 0;
            }

            edgePoint.isValid = true;
            result.edgePoints.push_back(edgePoint);
            result.gradients.push_back(edgePoint.gradient);
            result.directions.push_back(edgePoint.direction);

            totalShift += edgePoint.subPixelShift;
            if (mag > result.maxGradient) {
                result.maxGradient = mag;
            }
        }
    }

    result.validPointCount = static_cast<int>(result.edgePoints.size());
    if (result.validPointCount > 0) {
        result.averageSubPixelShift = totalShift / result.validPointCount;
    }

    result.processTime = timer.elapsed();
    return result;
}

// ============================================================
// Sobel梯度插值
// ============================================================

cv::Point2f SubPixelEdgeTool::interpolateEdgePosition(
    const cv::Mat& gradX,
    const cv::Mat& gradY,
    int x, int y)
{
    // 获取3x3邻域的梯度
    float gx = static_cast<float>(gradX.at<short>(y, x));
    float gy = static_cast<float>(gradY.at<short>(y, x));

    // 梯度幅值
    float mag = std::sqrt(gx * gx + gy * gy);
    if (mag < 1e-6f) {
        return cv::Point2f(static_cast<float>(x), static_cast<float>(y));
    }

    // 归一化梯度方向
    float nx = gx / mag;
    float ny = gy / mag;

    // 沿梯度方向插值
    // 前向和后向采样
    float gxP = static_cast<float>(gradX.at<short>(
        y + static_cast<int>(std::round(ny)),
        x + static_cast<int>(std::round(nx))));
    float gyP = static_cast<float>(gradY.at<short>(
        y + static_cast<int>(std::round(ny)),
        x + static_cast<int>(std::round(nx))));
    float magP = std::sqrt(gxP * gxP + gyP * gyP);

    float gxN = static_cast<float>(gradX.at<short>(
        y - static_cast<int>(std::round(ny)),
        x - static_cast<int>(std::round(nx))));
    float gyN = static_cast<float>(gradY.at<short>(
        y - static_cast<int>(std::round(ny)),
        x - static_cast<int>(std::round(nx))));
    float magN = std::sqrt(gxN * gxN + gyN * gyN);

    // 抛物线拟合确定亚像素位置
    float denom = 2.0f * (magP - 2.0f * mag + magN);
    float offset = 0.0f;
    if (std::abs(denom) > 1e-6f) {
        offset = (magP - magN) / denom;
        offset = std::max(-0.5f, std::min(0.5f, offset));  // 限制范围
    }

    return cv::Point2f(x - offset * nx, y - offset * ny);
}

// ============================================================
// Zernike矩亚像素定位
// ============================================================

cv::Point2f SubPixelEdgeTool::zernikeMomentSubPixel(
    const cv::Mat& image,
    int x, int y,
    int windowSize)
{
    int halfSize = windowSize / 2;

    // 边界检查
    if (x - halfSize < 0 || x + halfSize >= image.cols ||
        y - halfSize < 0 || y + halfSize >= image.rows) {
        return cv::Point2f(static_cast<float>(x), static_cast<float>(y));
    }

    // 提取窗口
    cv::Rect patchRect(x - halfSize, y - halfSize, windowSize, windowSize);
    cv::Mat patch = image(patchRect);

    double subPixelX, subPixelY, edgeAngle;
    computeEdgeParamsFromZernike(patch, subPixelX, subPixelY, edgeAngle);

    return cv::Point2f(
        static_cast<float>(x + subPixelX),
        static_cast<float>(y + subPixelY));
}

void SubPixelEdgeTool::computeEdgeParamsFromZernike(
    const cv::Mat& patch,
    double& subPixelX,
    double& subPixelY,
    double& edgeAngle)
{
    int n = patch.rows;
    double radius = (n - 1) / 2.0;

    // 计算Zernike矩 Z11 (n=1, m=1)
    std::complex<double> Z11(0, 0);
    std::complex<double> Z20(0, 0);

    for (int j = 0; j < n; ++j) {
        for (int i = 0; i < n; ++i) {
            double x = (i - radius) / radius;
            double y = (j - radius) / radius;
            double r = std::sqrt(x * x + y * y);

            if (r > 1.0) continue;

            double theta = std::atan2(y, x);
            double intensity = static_cast<double>(patch.at<uchar>(j, i)) / 255.0;

            // Z11: n=1, m=1
            Z11 += intensity * r * std::exp(std::complex<double>(0, theta));

            // Z20: n=2, m=0
            Z20 += intensity * (2 * r * r - 1);
        }
    }

    // 归一化
    double area = M_PI * radius * radius;
    Z11 *= (2.0 / area);
    Z20 *= (3.0 / area);

    // 从Zernike矩计算边缘参数
    double A11 = std::abs(Z11);
    edgeAngle = std::arg(Z11);

    // 亚像素偏移由Z11的相位确定
    if (A11 > 1e-6) {
        subPixelX = -std::real(Z11) / (A11 * 2);
        subPixelY = -std::imag(Z11) / (A11 * 2);

        // 限制偏移范围
        subPixelX = std::max(-0.5, std::min(0.5, subPixelX));
        subPixelY = std::max(-0.5, std::min(0.5, subPixelY));
    } else {
        subPixelX = 0;
        subPixelY = 0;
    }
}

std::complex<double> SubPixelEdgeTool::computeZernikeMoment(const cv::Mat& image, int n, int m)
{
    // Zernike矩计算
    // 此处实现简化版本，完整版需要Zernike多项式表

    int size = image.rows;
    double radius = (size - 1) / 2.0;
    std::complex<double> moment(0, 0);

    for (int j = 0; j < size; ++j) {
        for (int i = 0; i < size; ++i) {
            double x = (i - radius) / radius;
            double y = (j - radius) / radius;
            double r = std::sqrt(x * x + y * y);

            if (r > 1.0) continue;

            double theta = std::atan2(y, x);
            double intensity = static_cast<double>(image.at<uchar>(j, i));

            // 简化的Zernike基函数
            double Rnm = 0;
            if (n == 1 && std::abs(m) == 1) {
                Rnm = r;
            } else if (n == 2 && m == 0) {
                Rnm = 2 * r * r - 1;
            } else if (n == 2 && std::abs(m) == 2) {
                Rnm = r * r;
            }

            moment += intensity * Rnm * std::exp(std::complex<double>(0, -m * theta));
        }
    }

    // 归一化
    moment *= (n + 1) / M_PI;
    return moment;
}

// ============================================================
// 二次曲线拟合
// ============================================================

float SubPixelEdgeTool::quadraticFitSubPixel(
    const std::vector<float>& profile,
    int peakIndex)
{
    if (profile.size() < 3 || peakIndex < 1 ||
        peakIndex >= static_cast<int>(profile.size()) - 1) {
        return 0.0f;
    }

    float y0 = profile[peakIndex - 1];
    float y1 = profile[peakIndex];
    float y2 = profile[peakIndex + 1];

    // 抛物线拟合: y = ax^2 + bx + c
    // 极值点: x = -b/(2a) = (y0 - y2) / (2*(y0 - 2*y1 + y2))
    float denom = 2.0f * (y0 - 2.0f * y1 + y2);

    if (std::abs(denom) < 1e-6f) {
        return 0.0f;
    }

    float offset = (y0 - y2) / denom;

    // 限制偏移范围在[-0.5, 0.5]
    return std::max(-0.5f, std::min(0.5f, offset));
}

// ============================================================
// 高斯拟合
// ============================================================

float SubPixelEdgeTool::gaussianFitSubPixel(
    const std::vector<float>& profile,
    int peakIndex,
    int windowSize)
{
    if (profile.size() < 3) {
        return 0.0f;
    }

    // 使用对数线性化拟合高斯
    // ln(y) = ln(A) - (x-mu)^2 / (2*sigma^2)

    int halfWindow = windowSize / 2;
    int start = std::max(0, peakIndex - halfWindow);
    int end = std::min(static_cast<int>(profile.size()) - 1, peakIndex + halfWindow);

    if (end - start < 2) {
        return quadraticFitSubPixel(profile, peakIndex);  // 回退到二次拟合
    }

    // 简化：使用三点法
    if (peakIndex >= 1 && peakIndex < static_cast<int>(profile.size()) - 1) {
        float y0 = profile[peakIndex - 1];
        float y1 = profile[peakIndex];
        float y2 = profile[peakIndex + 1];

        // 确保值为正（高斯拟合需要）
        if (y0 > 0 && y1 > 0 && y2 > 0) {
            float ln0 = std::log(y0);
            float ln1 = std::log(y1);
            float ln2 = std::log(y2);

            float denom = 2.0f * (ln0 - 2.0f * ln1 + ln2);
            if (std::abs(denom) > 1e-6f) {
                float offset = (ln0 - ln2) / denom;
                return std::max(-0.5f, std::min(0.5f, offset));
            }
        }
    }

    return 0.0f;
}

// ============================================================
// ESR边缘定位
// ============================================================

cv::Point2f SubPixelEdgeTool::esrSubPixel(const cv::Mat& image, int x, int y)
{
    // Error Surface Regression方法
    // 在3x3邻域内拟合误差曲面

    if (x < 1 || x >= image.cols - 1 || y < 1 || y >= image.rows - 1) {
        return cv::Point2f(static_cast<float>(x), static_cast<float>(y));
    }

    // 提取3x3邻域
    cv::Mat A(9, 6, CV_64F);
    cv::Mat b(9, 1, CV_64F);

    int idx = 0;
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            double xx = dx;
            double yy = dy;
            double zz = static_cast<double>(image.at<uchar>(y + dy, x + dx));

            // 二次曲面: z = a*x^2 + b*y^2 + c*x*y + d*x + e*y + f
            A.at<double>(idx, 0) = xx * xx;
            A.at<double>(idx, 1) = yy * yy;
            A.at<double>(idx, 2) = xx * yy;
            A.at<double>(idx, 3) = xx;
            A.at<double>(idx, 4) = yy;
            A.at<double>(idx, 5) = 1;

            b.at<double>(idx, 0) = zz;
            ++idx;
        }
    }

    // 最小二乘拟合 (使用SVD分解)
    cv::Mat coeffs;
    cv::solve(A, b, coeffs, cv::DECOMP_SVD);

    double a = coeffs.at<double>(0, 0);
    double bb = coeffs.at<double>(1, 0);
    double c = coeffs.at<double>(2, 0);
    double d = coeffs.at<double>(3, 0);
    double e = coeffs.at<double>(4, 0);

    // 求极值点: dz/dx = 0, dz/dy = 0
    // 2*a*x + c*y + d = 0
    // c*x + 2*b*y + e = 0
    double det = 4 * a * bb - c * c;
    if (std::abs(det) < 1e-6) {
        return cv::Point2f(static_cast<float>(x), static_cast<float>(y));
    }

    double offsetX = (c * e - 2 * bb * d) / det;
    double offsetY = (c * d - 2 * a * e) / det;

    // 限制偏移范围
    offsetX = std::max(-0.5, std::min(0.5, offsetX));
    offsetY = std::max(-0.5, std::min(0.5, offsetY));

    return cv::Point2f(
        static_cast<float>(x + offsetX),
        static_cast<float>(y + offsetY));
}

// ============================================================
// 辅助方法
// ============================================================

void SubPixelEdgeTool::computeSobelGradient(
    const cv::Mat& gray,
    cv::Mat& gradX,
    cv::Mat& gradY,
    cv::Mat& magnitude)
{
    cv::Sobel(gray, gradX, CV_16S, 1, 0, 3);
    cv::Sobel(gray, gradY, CV_16S, 0, 1, 3);

    // 计算幅值
    cv::Mat absGradX, absGradY;
    cv::convertScaleAbs(gradX, absGradX);
    cv::convertScaleAbs(gradY, absGradY);

    // 精确计算幅值
    magnitude = cv::Mat(gray.size(), CV_32F);
    for (int y = 0; y < gray.rows; ++y) {
        for (int x = 0; x < gray.cols; ++x) {
            float gx = static_cast<float>(gradX.at<short>(y, x));
            float gy = static_cast<float>(gradY.at<short>(y, x));
            magnitude.at<float>(y, x) = std::sqrt(gx * gx + gy * gy);
        }
    }
}

cv::Mat SubPixelEdgeTool::nonMaximumSuppression(
    const cv::Mat& magnitude,
    const cv::Mat& gradX,
    const cv::Mat& gradY)
{
    cv::Mat result = cv::Mat::zeros(magnitude.size(), CV_32F);

    for (int y = 1; y < magnitude.rows - 1; ++y) {
        for (int x = 1; x < magnitude.cols - 1; ++x) {
            float mag = magnitude.at<float>(y, x);

            if (mag < 1e-6f) continue;

            float gx = static_cast<float>(gradX.at<short>(y, x));
            float gy = static_cast<float>(gradY.at<short>(y, x));

            // 计算梯度方向
            float angle = std::atan2(gy, gx);

            // 将角度归一化到[0, 180)
            if (angle < 0) angle += static_cast<float>(M_PI);

            // 沿梯度方向插值邻域值
            float mag1, mag2;

            if (angle < M_PI / 8 || angle >= 7 * M_PI / 8) {
                // 水平方向
                mag1 = magnitude.at<float>(y, x - 1);
                mag2 = magnitude.at<float>(y, x + 1);
            } else if (angle < 3 * M_PI / 8) {
                // 45度方向
                mag1 = magnitude.at<float>(y - 1, x + 1);
                mag2 = magnitude.at<float>(y + 1, x - 1);
            } else if (angle < 5 * M_PI / 8) {
                // 垂直方向
                mag1 = magnitude.at<float>(y - 1, x);
                mag2 = magnitude.at<float>(y + 1, x);
            } else {
                // 135度方向
                mag1 = magnitude.at<float>(y - 1, x - 1);
                mag2 = magnitude.at<float>(y + 1, x + 1);
            }

            // 非极大值抑制
            if (mag >= mag1 && mag >= mag2) {
                result.at<float>(y, x) = mag;
            }
        }
    }

    return result;
}

std::vector<float> SubPixelEdgeTool::extractProfileAlongNormal(
    const cv::Mat& image,
    const cv::Point2f& point,
    float direction,
    int length)
{
    std::vector<float> profile;
    profile.reserve(length);

    float dx = std::cos(direction);
    float dy = std::sin(direction);

    int halfLen = length / 2;

    for (int i = -halfLen; i <= halfLen; ++i) {
        float px = point.x + i * dx;
        float py = point.y + i * dy;

        // 边界检查
        if (px < 0 || px >= image.cols - 1 || py < 0 || py >= image.rows - 1) {
            profile.push_back(0);
        } else {
            profile.push_back(bilinearInterpolate(image, px, py));
        }
    }

    return profile;
}

float SubPixelEdgeTool::bilinearInterpolate(const cv::Mat& image, float x, float y)
{
    int x0 = static_cast<int>(x);
    int y0 = static_cast<int>(y);
    int x1 = x0 + 1;
    int y1 = y0 + 1;

    if (x0 < 0 || x1 >= image.cols || y0 < 0 || y1 >= image.rows) {
        return 0;
    }

    float dx = x - x0;
    float dy = y - y0;

    float v00, v01, v10, v11;

    if (image.type() == CV_8U) {
        v00 = image.at<uchar>(y0, x0);
        v01 = image.at<uchar>(y0, x1);
        v10 = image.at<uchar>(y1, x0);
        v11 = image.at<uchar>(y1, x1);
    } else if (image.type() == CV_32F) {
        v00 = image.at<float>(y0, x0);
        v01 = image.at<float>(y0, x1);
        v10 = image.at<float>(y1, x0);
        v11 = image.at<float>(y1, x1);
    } else {
        return 0;
    }

    return v00 * (1 - dx) * (1 - dy) +
           v01 * dx * (1 - dy) +
           v10 * (1 - dx) * dy +
           v11 * dx * dy;
}

// ============================================================
// 参数设置方法
// ============================================================

void SubPixelEdgeTool::setConfig(const SubPixelEdgeConfig& config)
{
    config_ = config;
    emit paramChanged();
}

void SubPixelEdgeTool::setMethod(SubPixelMethod method)
{
    if (config_.method != method) {
        config_.method = method;
        emit paramChanged();
    }
}

void SubPixelEdgeTool::setGradientThreshold(double threshold)
{
    if (std::abs(config_.gradientThreshold - threshold) > 0.001) {
        config_.gradientThreshold = threshold;
        emit paramChanged();
    }
}

void SubPixelEdgeTool::setWindowSize(int size)
{
    // 确保是奇数
    if (size % 2 == 0) size++;
    size = std::max(3, std::min(15, size));

    if (config_.windowSize != size) {
        config_.windowSize = size;
        emit paramChanged();
    }
}

void SubPixelEdgeTool::setUseSubPixel(bool enable)
{
    if (config_.useSubPixel != enable) {
        config_.useSubPixel = enable;
        emit paramChanged();
    }
}

void SubPixelEdgeTool::setSigma(double sigma)
{
    if (std::abs(config_.sigma - sigma) > 0.001) {
        config_.sigma = sigma;
        emit paramChanged();
    }
}

// ============================================================
// 辅助函数实现
// ============================================================

std::vector<cv::Point2f> refineEdgesToSubPixel(
    const cv::Mat& grayImage,
    const std::vector<cv::Point>& edgePoints,
    SubPixelMethod method,
    int windowSize)
{
    std::vector<cv::Point2f> refinedPoints;
    refinedPoints.reserve(edgePoints.size());

    SubPixelEdgeTool tool;
    SubPixelEdgeConfig config;
    config.method = method;
    config.windowSize = windowSize;
    config.useSubPixel = true;

    // 计算梯度
    cv::Mat gradX, gradY, magnitude;
    cv::Sobel(grayImage, gradX, CV_16S, 1, 0, 3);
    cv::Sobel(grayImage, gradY, CV_16S, 0, 1, 3);

    for (const auto& pt : edgePoints) {
        cv::Point2f refined;

        switch (method) {
        case SubPixelMethod::SobelInterpolation:
            refined = tool.interpolateEdgePosition(gradX, gradY, pt.x, pt.y);
            break;

        case SubPixelMethod::ZernikeMoment:
            refined = tool.zernikeMomentSubPixel(grayImage, pt.x, pt.y, windowSize);
            break;

        default:
            refined = cv::Point2f(static_cast<float>(pt.x), static_cast<float>(pt.y));
        }

        refinedPoints.push_back(refined);
    }

    return refinedPoints;
}

double fitCircleSubPixel(
    const std::vector<cv::Point2f>& edgePoints,
    double& centerX,
    double& centerY,
    double& radius)
{
    if (edgePoints.size() < 3) {
        return -1.0;
    }

    // 使用最小二乘法拟合圆
    // 圆方程: (x - cx)^2 + (y - cy)^2 = r^2
    // 展开: x^2 + y^2 - 2*cx*x - 2*cy*y + (cx^2 + cy^2 - r^2) = 0
    // 令 A = -2*cx, B = -2*cy, C = cx^2 + cy^2 - r^2
    // 则: x^2 + y^2 + A*x + B*y + C = 0

    int n = static_cast<int>(edgePoints.size());
    cv::Mat A(n, 3, CV_64F);
    cv::Mat b(n, 1, CV_64F);

    for (int i = 0; i < n; ++i) {
        double x = edgePoints[i].x;
        double y = edgePoints[i].y;

        A.at<double>(i, 0) = x;
        A.at<double>(i, 1) = y;
        A.at<double>(i, 2) = 1.0;

        b.at<double>(i, 0) = -(x * x + y * y);
    }

    // 最小二乘求解 (使用SVD分解)
    cv::Mat params;
    cv::solve(A, b, params, cv::DECOMP_SVD);

    centerX = -params.at<double>(0, 0) / 2.0;
    centerY = -params.at<double>(1, 0) / 2.0;
    radius = std::sqrt(centerX * centerX + centerY * centerY - params.at<double>(2, 0));

    // 计算残差
    double residual = 0;
    for (const auto& pt : edgePoints) {
        double dx = pt.x - centerX;
        double dy = pt.y - centerY;
        double dist = std::sqrt(dx * dx + dy * dy);
        residual += (dist - radius) * (dist - radius);
    }

    return std::sqrt(residual / n);
}

double fitLineSubPixel(
    const std::vector<cv::Point2f>& edgePoints,
    cv::Vec3d& lineParams)
{
    if (edgePoints.size() < 2) {
        return -1.0;
    }

    int n = static_cast<int>(edgePoints.size());

    // 计算质心
    double meanX = 0, meanY = 0;
    for (const auto& pt : edgePoints) {
        meanX += pt.x;
        meanY += pt.y;
    }
    meanX /= n;
    meanY /= n;

    // 主成分分析
    double sxx = 0, syy = 0, sxy = 0;
    for (const auto& pt : edgePoints) {
        double dx = pt.x - meanX;
        double dy = pt.y - meanY;
        sxx += dx * dx;
        syy += dy * dy;
        sxy += dx * dy;
    }

    // 计算主方向
    double theta = 0.5 * std::atan2(2 * sxy, sxx - syy);

    // 直线参数: a*x + b*y + c = 0
    double a = std::sin(theta);
    double b = -std::cos(theta);
    double c = -(a * meanX + b * meanY);

    lineParams = cv::Vec3d(a, b, c);

    // 计算残差
    double residual = 0;
    for (const auto& pt : edgePoints) {
        double dist = std::abs(a * pt.x + b * pt.y + c);
        residual += dist * dist;
    }

    return std::sqrt(residual / n);
}

} // namespace Algorithm
} // namespace VisionForge
