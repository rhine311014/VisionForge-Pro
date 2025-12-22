/**
 * @file DepthMap.cpp
 * @brief VisionForge Pro 深度图类实现
 */

#include "algorithm3d/DepthMap.h"
#include "algorithm3d/PointCloud3D.h"
#include <QFile>
#include <algorithm>
#include <cmath>

namespace VisionForge {
namespace Algorithm3D {

// ============================================================
// 构造函数
// ============================================================

DepthMap::DepthMap(QObject* parent)
    : QObject(parent)
{
    m_timestamp = QDateTime::currentDateTime();
}

DepthMap::DepthMap(int width, int height, DepthFormat format, QObject* parent)
    : QObject(parent)
{
    allocate(width, height, format);
    m_timestamp = QDateTime::currentDateTime();
}

DepthMap::DepthMap(const QImage& image, double depthScale, QObject* parent)
    : QObject(parent)
{
    m_width = image.width();
    m_height = image.height();
    m_format = DepthFormat::Float32;
    m_dataFloat32.resize(static_cast<size_t>(m_width * m_height));

    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            uint16_t grayValue = static_cast<uint16_t>(qGray(image.pixel(x, y)));
            m_dataFloat32[static_cast<size_t>(y * m_width + x)] =
                static_cast<float>(grayValue * depthScale);
        }
    }

    m_timestamp = QDateTime::currentDateTime();
}

DepthMap::DepthMap(const DepthMap& other)
    : QObject(other.parent())
    , m_width(other.m_width)
    , m_height(other.m_height)
    , m_format(other.m_format)
    , m_dataFloat32(other.m_dataFloat32)
    , m_dataFloat64(other.m_dataFloat64)
    , m_dataUInt16(other.m_dataUInt16)
    , m_dataUInt8(other.m_dataUInt8)
    , m_source(other.m_source)
    , m_intrinsics(other.m_intrinsics)
    , m_depthUnit(other.m_depthUnit)
    , m_timestamp(other.m_timestamp)
{
}

DepthMap::DepthMap(DepthMap&& other) noexcept
    : QObject(other.parent())
    , m_width(other.m_width)
    , m_height(other.m_height)
    , m_format(other.m_format)
    , m_dataFloat32(std::move(other.m_dataFloat32))
    , m_dataFloat64(std::move(other.m_dataFloat64))
    , m_dataUInt16(std::move(other.m_dataUInt16))
    , m_dataUInt8(std::move(other.m_dataUInt8))
    , m_source(other.m_source)
    , m_intrinsics(other.m_intrinsics)
    , m_depthUnit(other.m_depthUnit)
    , m_timestamp(other.m_timestamp)
{
    other.m_width = 0;
    other.m_height = 0;
}

DepthMap::~DepthMap() = default;

// ============================================================
// 赋值运算符
// ============================================================

DepthMap& DepthMap::operator=(const DepthMap& other) {
    if (this != &other) {
        QMutexLocker locker(&m_mutex);
        m_width = other.m_width;
        m_height = other.m_height;
        m_format = other.m_format;
        m_dataFloat32 = other.m_dataFloat32;
        m_dataFloat64 = other.m_dataFloat64;
        m_dataUInt16 = other.m_dataUInt16;
        m_dataUInt8 = other.m_dataUInt8;
        m_source = other.m_source;
        m_intrinsics = other.m_intrinsics;
        m_depthUnit = other.m_depthUnit;
        m_timestamp = other.m_timestamp;
        invalidateCache();
    }
    return *this;
}

DepthMap& DepthMap::operator=(DepthMap&& other) noexcept {
    if (this != &other) {
        QMutexLocker locker(&m_mutex);
        m_width = other.m_width;
        m_height = other.m_height;
        m_format = other.m_format;
        m_dataFloat32 = std::move(other.m_dataFloat32);
        m_dataFloat64 = std::move(other.m_dataFloat64);
        m_dataUInt16 = std::move(other.m_dataUInt16);
        m_dataUInt8 = std::move(other.m_dataUInt8);
        m_source = other.m_source;
        m_intrinsics = other.m_intrinsics;
        m_depthUnit = other.m_depthUnit;
        m_timestamp = other.m_timestamp;
        other.m_width = 0;
        other.m_height = 0;
        invalidateCache();
    }
    return *this;
}

// ============================================================
// 深度访问
// ============================================================

double DepthMap::getDepth(int x, int y) const {
    if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
        return 0.0;
    }

    size_t idx = static_cast<size_t>(y * m_width + x);

    switch (m_format) {
        case DepthFormat::Float32:
            return static_cast<double>(m_dataFloat32[idx]);
        case DepthFormat::Float64:
            return m_dataFloat64[idx];
        case DepthFormat::UInt16:
            return static_cast<double>(m_dataUInt16[idx]) * 0.1;
        case DepthFormat::UInt8:
            return static_cast<double>(m_dataUInt8[idx]);
        default:
            return 0.0;
    }
}

void DepthMap::setDepth(int x, int y, double depth) {
    if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
        return;
    }

    size_t idx = static_cast<size_t>(y * m_width + x);

    switch (m_format) {
        case DepthFormat::Float32:
            m_dataFloat32[idx] = static_cast<float>(depth);
            break;
        case DepthFormat::Float64:
            m_dataFloat64[idx] = depth;
            break;
        case DepthFormat::UInt16:
            m_dataUInt16[idx] = static_cast<uint16_t>(depth * 10.0);
            break;
        case DepthFormat::UInt8:
            m_dataUInt8[idx] = static_cast<uint8_t>(std::min(255.0, depth));
            break;
    }

    invalidateCache();
}

double DepthMap::getDepthInterpolated(double x, double y) const {
    return bilinearInterpolate(x, y);
}

bool DepthMap::isValid(int x, int y) const {
    double depth = getDepth(x, y);
    return std::isfinite(depth) && depth > 0;
}

int DepthMap::getValidPixelCount() const {
    int count = 0;
    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            if (isValid(x, y)) {
                ++count;
            }
        }
    }
    return count;
}

QImage DepthMap::getInvalidMask() const {
    QImage mask(m_width, m_height, QImage::Format_Grayscale8);

    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            mask.setPixel(x, y, isValid(x, y) ? 255 : 0);
        }
    }

    return mask;
}

// ============================================================
// 数据访问
// ============================================================

float* DepthMap::dataFloat32() {
    return m_dataFloat32.data();
}

const float* DepthMap::dataFloat32() const {
    return m_dataFloat32.data();
}

double* DepthMap::dataFloat64() {
    return m_dataFloat64.data();
}

const double* DepthMap::dataFloat64() const {
    return m_dataFloat64.data();
}

uint16_t* DepthMap::dataUInt16() {
    return m_dataUInt16.data();
}

const uint16_t* DepthMap::dataUInt16() const {
    return m_dataUInt16.data();
}

void* DepthMap::data() {
    switch (m_format) {
        case DepthFormat::Float32: return m_dataFloat32.data();
        case DepthFormat::Float64: return m_dataFloat64.data();
        case DepthFormat::UInt16: return m_dataUInt16.data();
        case DepthFormat::UInt8: return m_dataUInt8.data();
        default: return nullptr;
    }
}

const void* DepthMap::data() const {
    switch (m_format) {
        case DepthFormat::Float32: return m_dataFloat32.data();
        case DepthFormat::Float64: return m_dataFloat64.data();
        case DepthFormat::UInt16: return m_dataUInt16.data();
        case DepthFormat::UInt8: return m_dataUInt8.data();
        default: return nullptr;
    }
}

size_t DepthMap::dataSize() const {
    size_t pixelCount = static_cast<size_t>(m_width * m_height);
    switch (m_format) {
        case DepthFormat::Float32: return pixelCount * sizeof(float);
        case DepthFormat::Float64: return pixelCount * sizeof(double);
        case DepthFormat::UInt16: return pixelCount * sizeof(uint16_t);
        case DepthFormat::UInt8: return pixelCount * sizeof(uint8_t);
        default: return 0;
    }
}

// ============================================================
// 统计信息
// ============================================================

double DepthMap::getMinDepth() const {
    if (!m_rangeValid) {
        const_cast<DepthMap*>(this)->getDepthRange(
            const_cast<double&>(m_minDepth),
            const_cast<double&>(m_maxDepth));
        m_rangeValid = true;
    }
    return m_minDepth;
}

double DepthMap::getMaxDepth() const {
    if (!m_rangeValid) {
        const_cast<DepthMap*>(this)->getDepthRange(
            const_cast<double&>(m_minDepth),
            const_cast<double&>(m_maxDepth));
        m_rangeValid = true;
    }
    return m_maxDepth;
}

void DepthMap::getDepthRange(double& minDepth, double& maxDepth) const {
    minDepth = std::numeric_limits<double>::max();
    maxDepth = std::numeric_limits<double>::lowest();

    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            double depth = getDepth(x, y);
            if (isValid(x, y)) {
                minDepth = std::min(minDepth, depth);
                maxDepth = std::max(maxDepth, depth);
            }
        }
    }

    if (minDepth > maxDepth) {
        minDepth = 0;
        maxDepth = 0;
    }
}

double DepthMap::getAverageDepth() const {
    double sum = 0;
    int count = 0;

    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            if (isValid(x, y)) {
                sum += getDepth(x, y);
                ++count;
            }
        }
    }

    return count > 0 ? sum / count : 0;
}

double DepthMap::getDepthStdDev() const {
    double mean = getAverageDepth();
    double sumSq = 0;
    int count = 0;

    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            if (isValid(x, y)) {
                double diff = getDepth(x, y) - mean;
                sumSq += diff * diff;
                ++count;
            }
        }
    }

    return count > 1 ? std::sqrt(sumSq / (count - 1)) : 0;
}

// ============================================================
// 滤波
// ============================================================

DepthMap DepthMap::medianFilter(int kernelSize) const {
    DepthMap result(m_width, m_height, m_format);
    int halfK = kernelSize / 2;

    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            std::vector<double> values;

            for (int dy = -halfK; dy <= halfK; ++dy) {
                for (int dx = -halfK; dx <= halfK; ++dx) {
                    int nx = x + dx;
                    int ny = y + dy;

                    if (nx >= 0 && nx < m_width && ny >= 0 && ny < m_height) {
                        if (isValid(nx, ny)) {
                            values.push_back(getDepth(nx, ny));
                        }
                    }
                }
            }

            if (!values.empty()) {
                std::sort(values.begin(), values.end());
                result.setDepth(x, y, values[values.size() / 2]);
            } else {
                result.setDepth(x, y, 0);
            }
        }
    }

    return result;
}

DepthMap DepthMap::gaussianFilter(int kernelSize, double sigma) const {
    DepthMap result(m_width, m_height, m_format);
    int halfK = kernelSize / 2;

    // 预计算高斯核
    std::vector<std::vector<double>> kernel(
        static_cast<size_t>(kernelSize),
        std::vector<double>(static_cast<size_t>(kernelSize)));

    double sum = 0;
    for (int i = 0; i < kernelSize; ++i) {
        for (int j = 0; j < kernelSize; ++j) {
            double x = i - halfK;
            double y = j - halfK;
            kernel[static_cast<size_t>(i)][static_cast<size_t>(j)] =
                std::exp(-(x*x + y*y) / (2 * sigma * sigma));
            sum += kernel[static_cast<size_t>(i)][static_cast<size_t>(j)];
        }
    }

    // 归一化
    for (int i = 0; i < kernelSize; ++i) {
        for (int j = 0; j < kernelSize; ++j) {
            kernel[static_cast<size_t>(i)][static_cast<size_t>(j)] /= sum;
        }
    }

    // 应用滤波
    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            double value = 0;
            double weightSum = 0;

            for (int dy = -halfK; dy <= halfK; ++dy) {
                for (int dx = -halfK; dx <= halfK; ++dx) {
                    int nx = x + dx;
                    int ny = y + dy;

                    if (nx >= 0 && nx < m_width && ny >= 0 && ny < m_height) {
                        if (isValid(nx, ny)) {
                            double w = kernel[static_cast<size_t>(dy + halfK)]
                                            [static_cast<size_t>(dx + halfK)];
                            value += getDepth(nx, ny) * w;
                            weightSum += w;
                        }
                    }
                }
            }

            result.setDepth(x, y, weightSum > 0 ? value / weightSum : 0);
        }
    }

    return result;
}

DepthMap DepthMap::bilateralFilter(int d, double sigmaColor, double sigmaSpace) const {
    DepthMap result(m_width, m_height, m_format);
    int halfD = d / 2;

    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            if (!isValid(x, y)) {
                result.setDepth(x, y, 0);
                continue;
            }

            double centerDepth = getDepth(x, y);
            double value = 0;
            double weightSum = 0;

            for (int dy = -halfD; dy <= halfD; ++dy) {
                for (int dx = -halfD; dx <= halfD; ++dx) {
                    int nx = x + dx;
                    int ny = y + dy;

                    if (nx >= 0 && nx < m_width && ny >= 0 && ny < m_height) {
                        if (isValid(nx, ny)) {
                            double neighborDepth = getDepth(nx, ny);

                            // 空间权重
                            double spatialWeight = std::exp(
                                -(dx*dx + dy*dy) / (2 * sigmaSpace * sigmaSpace));

                            // 颜色（深度）权重
                            double depthDiff = centerDepth - neighborDepth;
                            double colorWeight = std::exp(
                                -(depthDiff * depthDiff) / (2 * sigmaColor * sigmaColor));

                            double w = spatialWeight * colorWeight;
                            value += neighborDepth * w;
                            weightSum += w;
                        }
                    }
                }
            }

            result.setDepth(x, y, weightSum > 0 ? value / weightSum : centerDepth);
        }
    }

    return result;
}

DepthMap DepthMap::guidedFilter(const QImage& guide, int radius, double eps) const {
    Q_UNUSED(guide);
    Q_UNUSED(radius);
    Q_UNUSED(eps);
    // 简化实现：返回双边滤波结果
    return bilateralFilter(radius * 2 + 1, eps * 1000, static_cast<double>(radius));
}

DepthMap DepthMap::edgePreservingFilter(double threshold) const {
    return bilateralFilter(9, threshold, 75);
}

DepthMap DepthMap::applyFilter(const DepthMapConfig& config) const {
    DepthMap result = *this;

    if (config.enableMedianFilter) {
        result = result.medianFilter(config.medianKernelSize);
    }

    if (config.enableBilateralFilter) {
        result = result.bilateralFilter(config.bilateralD,
                                        config.bilateralSigmaColor,
                                        config.bilateralSigmaSpace);
    }

    if (config.enableHoleFilling) {
        result = result.fillHolesInterpolation(config.holeFillMaxSize);
    }

    return result;
}

// ============================================================
// 空洞填充
// ============================================================

DepthMap DepthMap::fillHolesNearestNeighbor() const {
    DepthMap result = *this;

    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            if (!isValid(x, y)) {
                // 找最近的有效像素
                double minDist = std::numeric_limits<double>::max();
                double nearestDepth = 0;

                for (int radius = 1; radius < std::max(m_width, m_height); ++radius) {
                    bool found = false;
                    for (int dx = -radius; dx <= radius && !found; ++dx) {
                        for (int dy = -radius; dy <= radius && !found; ++dy) {
                            int nx = x + dx;
                            int ny = y + dy;

                            if (nx >= 0 && nx < m_width && ny >= 0 && ny < m_height) {
                                if (isValid(nx, ny)) {
                                    double dist = std::sqrt(
                                        static_cast<double>(dx*dx + dy*dy));
                                    if (dist < minDist) {
                                        minDist = dist;
                                        nearestDepth = getDepth(nx, ny);
                                        found = true;
                                    }
                                }
                            }
                        }
                    }
                    if (found) break;
                }

                result.setDepth(x, y, nearestDepth);
            }
        }
    }

    return result;
}

DepthMap DepthMap::fillHolesInterpolation(int maxHoleSize) const {
    DepthMap result = *this;

    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            if (!isValid(x, y)) {
                // 简单双线性插值
                double sum = 0;
                int count = 0;

                for (int radius = 1; radius <= maxHoleSize; ++radius) {
                    for (int dx = -radius; dx <= radius; ++dx) {
                        for (int dy = -radius; dy <= radius; ++dy) {
                            if (std::abs(dx) == radius || std::abs(dy) == radius) {
                                int nx = x + dx;
                                int ny = y + dy;

                                if (nx >= 0 && nx < m_width && ny >= 0 && ny < m_height) {
                                    if (isValid(nx, ny)) {
                                        double dist = std::sqrt(
                                            static_cast<double>(dx*dx + dy*dy));
                                        double weight = 1.0 / (dist + 1);
                                        sum += getDepth(nx, ny) * weight;
                                        count++;
                                    }
                                }
                            }
                        }
                    }
                    if (count > 0) break;
                }

                if (count > 0) {
                    result.setDepth(x, y, sum / count);
                }
            }
        }
    }

    return result;
}

DepthMap DepthMap::fillHolesInpaint(int radius) const {
    // 简化实现
    return fillHolesInterpolation(radius);
}

DepthMap DepthMap::fillHolesWithValue(double value) const {
    DepthMap result = *this;

    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            if (!isValid(x, y)) {
                result.setDepth(x, y, value);
            }
        }
    }

    return result;
}

// ============================================================
// 格式转换
// ============================================================

DepthMap DepthMap::convertTo(DepthFormat targetFormat) const {
    DepthMap result(m_width, m_height, targetFormat);

    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            result.setDepth(x, y, getDepth(x, y));
        }
    }

    result.m_intrinsics = m_intrinsics;
    result.m_source = m_source;
    result.m_depthUnit = m_depthUnit;

    return result;
}

DepthMap DepthMap::fromImage16(const QImage& image, double depthScale) {
    return DepthMap(image, depthScale);
}

QImage DepthMap::toImage16(double depthScale) const {
    QImage image(m_width, m_height, QImage::Format_Grayscale16);

    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            double depth = getDepth(x, y);
            uint16_t value = static_cast<uint16_t>(depth / depthScale);
            // Qt的Grayscale16需要特殊处理
            image.setPixel(x, y, qRgb(value >> 8, value & 0xFF, 0));
        }
    }

    return image;
}

QImage DepthMap::toColorImage(double minDepth, double maxDepth) const {
    if (minDepth >= maxDepth) {
        getDepthRange(minDepth, maxDepth);
    }

    QImage image(m_width, m_height, QImage::Format_RGB888);
    double range = maxDepth - minDepth;
    if (range <= 0) range = 1;

    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            if (!isValid(x, y)) {
                image.setPixel(x, y, qRgb(0, 0, 0));
                continue;
            }

            double depth = getDepth(x, y);
            double t = (depth - minDepth) / range;
            t = std::max(0.0, std::min(1.0, t));

            // 彩虹色映射
            int r, g, b;
            if (t < 0.25) {
                r = 0;
                g = static_cast<int>(255 * 4 * t);
                b = 255;
            } else if (t < 0.5) {
                r = 0;
                g = 255;
                b = static_cast<int>(255 * (1 - 4 * (t - 0.25)));
            } else if (t < 0.75) {
                r = static_cast<int>(255 * 4 * (t - 0.5));
                g = 255;
                b = 0;
            } else {
                r = 255;
                g = static_cast<int>(255 * (1 - 4 * (t - 0.75)));
                b = 0;
            }

            image.setPixel(x, y, qRgb(r, g, b));
        }
    }

    return image;
}

// ============================================================
// 点云转换
// ============================================================

PointCloud3D DepthMap::toPointCloud(const CameraIntrinsics& intrinsics) const {
    PointCloud3D cloud;

    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            if (isValid(x, y)) {
                Point3D point = pixelTo3D(x, y, intrinsics);
                cloud.addPoint(point);
            }
        }
    }

    return cloud;
}

PointCloud3D DepthMap::toPointCloud(const CameraIntrinsics& intrinsics,
                                     const QImage& colorImage) const {
    PointCloud3D cloud;

    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            if (isValid(x, y)) {
                Point3D point = pixelTo3D(x, y, intrinsics);

                if (x < colorImage.width() && y < colorImage.height()) {
                    QRgb color = colorImage.pixel(x, y);
                    point.r = static_cast<uint8_t>(qRed(color));
                    point.g = static_cast<uint8_t>(qGreen(color));
                    point.b = static_cast<uint8_t>(qBlue(color));
                }

                cloud.addPoint(point);
            }
        }
    }

    return cloud;
}

Point3D DepthMap::pixelTo3D(int x, int y, const CameraIntrinsics& intrinsics) const {
    double depth = getDepth(x, y);

    if (depth <= 0 || !intrinsics.isValid()) {
        return Point3D();
    }

    double px = (x - intrinsics.cx) * depth / intrinsics.fx;
    double py = (y - intrinsics.cy) * depth / intrinsics.fy;

    return Point3D(static_cast<float>(px),
                   static_cast<float>(py),
                   static_cast<float>(depth));
}

std::vector<Point3D> DepthMap::pixelsTo3D(const std::vector<QPoint>& pixels,
                                           const CameraIntrinsics& intrinsics) const {
    std::vector<Point3D> points;
    points.reserve(pixels.size());

    for (const auto& pixel : pixels) {
        points.push_back(pixelTo3D(pixel.x(), pixel.y(), intrinsics));
    }

    return points;
}

QPoint DepthMap::project3DToPixel(const Point3D& point,
                                   const CameraIntrinsics& intrinsics) const {
    if (point.z <= 0 || !intrinsics.isValid()) {
        return QPoint(-1, -1);
    }

    int x = static_cast<int>(point.x * intrinsics.fx / point.z + intrinsics.cx);
    int y = static_cast<int>(point.y * intrinsics.fy / point.z + intrinsics.cy);

    return QPoint(x, y);
}

// ============================================================
// 立体视觉
// ============================================================

DepthMap DepthMap::fromDisparity(const QImage& disparity,
                                  const StereoParameters& stereoParams) {
    int w = disparity.width();
    int h = disparity.height();
    DepthMap result(w, h, DepthFormat::Float32);

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            int d = qGray(disparity.pixel(x, y));
            if (d > 0) {
                double depth = stereoParams.disparityToDepth(d);
                result.setDepth(x, y, depth);
            }
        }
    }

    result.m_intrinsics = stereoParams.leftCamera;
    result.m_source = DepthSource::StereoCamera;

    return result;
}

DepthMap DepthMap::fromDisparity(const DepthMap& disparity,
                                  const StereoParameters& stereoParams) {
    DepthMap result(disparity.width(), disparity.height(), DepthFormat::Float32);

    for (int y = 0; y < disparity.height(); ++y) {
        for (int x = 0; x < disparity.width(); ++x) {
            double d = disparity.getDepth(x, y);
            if (d > 0) {
                double depth = stereoParams.disparityToDepth(d);
                result.setDepth(x, y, depth);
            }
        }
    }

    result.m_intrinsics = stereoParams.leftCamera;
    result.m_source = DepthSource::StereoCamera;

    return result;
}

DepthMap DepthMap::toDisparity(const StereoParameters& stereoParams) const {
    DepthMap result(m_width, m_height, DepthFormat::Float32);

    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            double depth = getDepth(x, y);
            if (depth > 0) {
                double disparity = stereoParams.depthToDisparity(depth);
                result.setDepth(x, y, disparity);
            }
        }
    }

    return result;
}

// ============================================================
// 元数据
// ============================================================

void DepthMap::setIntrinsics(const CameraIntrinsics& intrinsics) {
    m_intrinsics = intrinsics;
}

// ============================================================
// 序列化
// ============================================================

QJsonObject DepthMap::toJson() const {
    QJsonObject json;
    json["width"] = m_width;
    json["height"] = m_height;
    json["format"] = static_cast<int>(m_format);
    json["source"] = static_cast<int>(m_source);
    json["depthUnit"] = m_depthUnit;
    json["timestamp"] = m_timestamp.toString(Qt::ISODate);
    json["intrinsics"] = m_intrinsics.toJson();
    return json;
}

DepthMap DepthMap::fromJson(const QJsonObject& json) {
    int width = json["width"].toInt();
    int height = json["height"].toInt();
    DepthFormat format = static_cast<DepthFormat>(json["format"].toInt());

    DepthMap result(width, height, format);
    result.m_source = static_cast<DepthSource>(json["source"].toInt());
    result.m_depthUnit = json["depthUnit"].toDouble(1.0);
    result.m_timestamp = QDateTime::fromString(json["timestamp"].toString(), Qt::ISODate);
    result.m_intrinsics = CameraIntrinsics::fromJson(json["intrinsics"].toObject());

    return result;
}

DepthMap DepthMap::clone() const {
    return DepthMap(*this);
}

// ============================================================
// 文件I/O
// ============================================================

bool DepthMap::load(const QString& filename) {
    if (filename.endsWith(".png", Qt::CaseInsensitive)) {
        return loadPNG16(filename);
    }
    return false;
}

bool DepthMap::save(const QString& filename) const {
    if (filename.endsWith(".png", Qt::CaseInsensitive)) {
        return savePNG16(filename);
    }
    return false;
}

bool DepthMap::loadPNG16(const QString& filename, double depthScale) {
    QImage image(filename);
    if (image.isNull()) {
        return false;
    }

    *this = DepthMap(image, depthScale);
    return true;
}

bool DepthMap::savePNG16(const QString& filename, double depthScale) const {
    QImage image = toImage16(depthScale);
    return image.save(filename);
}

bool DepthMap::loadEXR(const QString& filename) {
    Q_UNUSED(filename);
    // EXR需要OpenEXR库支持
    return false;
}

bool DepthMap::saveEXR(const QString& filename) const {
    Q_UNUSED(filename);
    return false;
}

// ============================================================
// 私有方法
// ============================================================

void DepthMap::allocate(int width, int height, DepthFormat format) {
    m_width = width;
    m_height = height;
    m_format = format;

    size_t pixelCount = static_cast<size_t>(width * height);

    m_dataFloat32.clear();
    m_dataFloat64.clear();
    m_dataUInt16.clear();
    m_dataUInt8.clear();

    switch (format) {
        case DepthFormat::Float32:
            m_dataFloat32.resize(pixelCount, 0.0f);
            break;
        case DepthFormat::Float64:
            m_dataFloat64.resize(pixelCount, 0.0);
            break;
        case DepthFormat::UInt16:
            m_dataUInt16.resize(pixelCount, 0);
            break;
        case DepthFormat::UInt8:
            m_dataUInt8.resize(pixelCount, 0);
            break;
    }

    invalidateCache();
}

void DepthMap::invalidateCache() {
    m_rangeValid = false;
}

double DepthMap::bilinearInterpolate(double x, double y) const {
    int x0 = static_cast<int>(std::floor(x));
    int y0 = static_cast<int>(std::floor(y));
    int x1 = x0 + 1;
    int y1 = y0 + 1;

    if (x0 < 0 || x1 >= m_width || y0 < 0 || y1 >= m_height) {
        return getDepth(static_cast<int>(x + 0.5), static_cast<int>(y + 0.5));
    }

    double fx = x - x0;
    double fy = y - y0;

    double d00 = getDepth(x0, y0);
    double d10 = getDepth(x1, y0);
    double d01 = getDepth(x0, y1);
    double d11 = getDepth(x1, y1);

    double d0 = d00 * (1 - fx) + d10 * fx;
    double d1 = d01 * (1 - fx) + d11 * fx;

    return d0 * (1 - fy) + d1 * fy;
}

// ============================================================
// StereoMatcher
// ============================================================

StereoMatcher::StereoMatcher(QObject* parent)
    : QObject(parent)
{
}

StereoMatcher::~StereoMatcher() = default;

void StereoMatcher::setStereoParameters(const StereoParameters& params) {
    m_stereoParams = params;
}

void StereoMatcher::setConfig(const StereoMatchConfig& config) {
    m_config = config;
}

DepthMap StereoMatcher::computeDisparity(const QImage& leftImage,
                                          const QImage& rightImage) {
    Q_UNUSED(leftImage);
    Q_UNUSED(rightImage);

    // 简化实现：返回空深度图
    // 完整实现需要OpenCV支持
    m_lastError = "立体匹配需要OpenCV支持";
    return DepthMap();
}

DepthMap StereoMatcher::computeDepth(const QImage& leftImage,
                                      const QImage& rightImage) {
    DepthMap disparity = computeDisparity(leftImage, rightImage);

    if (disparity.isEmpty()) {
        return DepthMap();
    }

    return DepthMap::fromDisparity(disparity, m_stereoParams);
}

} // namespace Algorithm3D
} // namespace VisionForge
