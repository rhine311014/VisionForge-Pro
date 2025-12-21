/**
 * @file BlobTool.cpp
 * @brief Blob分析工具实现
 */

#include "algorithm/BlobTool.h"
#include "base/Logger.h"
#include "base/ImageMemoryPool.h"
#include <opencv2/imgproc.hpp>
#include <QElapsedTimer>
#include <QJsonArray>
#include <algorithm>
#include <cmath>

#ifdef USE_HALCON
#include <HalconCpp.h>
using namespace HalconCpp;
#endif

namespace VisionForge {
namespace Algorithm {

BlobTool::BlobTool(QObject* parent)
    : VisionTool(parent)
    , backend_(Auto)
    , connectivity_(Connect8)
    , polarity_(Light)
    , sortBy_(SortByArea)
    , sortDescending_(true)
    , maxCount_(0)
    , threshold_(128)
    , autoThreshold_(true)
{
    setDisplayName(toolName());

    // 默认过滤器：最小面积100像素
    addFilter(BlobFilter(BlobFilter::AreaMin, 100.0, true));
}

bool BlobTool::process(const Base::ImageData::Ptr& input, ToolResult& output)
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

    bool success = false;

#ifdef USE_HALCON
    if (backend_ == Halcon || (backend_ == Auto)) {
        success = processWithHalcon(gray, output);
        if (success) {
            output.executionTime = timer.elapsed();
            output.setValue("backend", "Halcon");
        }
    }
#endif

    if (!success && (backend_ == OpenCV || backend_ == Auto)) {
        success = processWithOpenCV(gray, output);
        if (success) {
            output.executionTime = timer.elapsed();
            output.setValue("backend", "OpenCV");
        }
    }

    if (success) {
        // 应用过滤器
        blobFeatures_ = applyFilters(blobFeatures_);

        // 排序
        sortBlobs(blobFeatures_);

        // 限制数量
        if (maxCount_ > 0 && blobFeatures_.size() > static_cast<size_t>(maxCount_)) {
            blobFeatures_.resize(maxCount_);
        }

        // 设置输出结果
        output.setValue("blobCount", static_cast<int>(blobFeatures_.size()));

        // 创建带标注的输出图像
        cv::Mat resultImage = drawResults(src);
        output.outputImage = Base::ImageMemoryPool::instance().allocate(
            resultImage.cols, resultImage.rows, resultImage.type());
        if (output.outputImage) {
            resultImage.copyTo(output.outputImage->mat());
        }

        // 记录每个Blob的信息到结果数据
        QJsonArray blobArray;
        for (const auto& blob : blobFeatures_) {
            QJsonObject blobObj;
            blobObj["id"] = blob.id;
            blobObj["area"] = blob.area;
            blobObj["perimeter"] = blob.perimeter;
            blobObj["circularity"] = blob.circularity;
            blobObj["rectangularity"] = blob.rectangularity;
            blobObj["centerX"] = blob.center.x();
            blobObj["centerY"] = blob.center.y();
            blobObj["width"] = blob.boundingBox.width();
            blobObj["height"] = blob.boundingBox.height();
            blobObj["orientation"] = blob.orientation;
            blobArray.append(blobObj);
        }
        output.data["blobs"] = blobArray;

        setDebugImage(output.outputImage);
        setStatusText(QString("检测到 %1 个Blob").arg(blobFeatures_.size()));

        LOG_DEBUG(QString("Blob分析完成，检测到 %1 个Blob，耗时: %2ms")
                 .arg(blobFeatures_.size())
                 .arg(output.executionTime));
    }

    return success;
}

bool BlobTool::processWithOpenCV(const cv::Mat& input, ToolResult& output)
{
    try {
        cv::Mat binary;

        // 二值化处理
        if (autoThreshold_) {
            cv::threshold(input, binary, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
        } else {
            cv::threshold(input, binary, threshold_, 255, cv::THRESH_BINARY);
        }

        // 根据极性处理
        if (polarity_ == Dark) {
            cv::bitwise_not(binary, binary);
        }

        // 查找轮廓
        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierarchy;

        int connType = (connectivity_ == Connect8) ? cv::RETR_EXTERNAL : cv::RETR_EXTERNAL;
        cv::findContours(binary, contours, hierarchy, connType, cv::CHAIN_APPROX_SIMPLE);

        // 计算每个Blob的特征
        blobFeatures_.clear();
        blobFeatures_.reserve(contours.size());

        for (size_t i = 0; i < contours.size(); ++i) {
            if (contours[i].size() < 3) continue;  // 至少需要3个点

            BlobFeature feature = calculateFeatures(contours[i], static_cast<int>(i + 1));
            blobFeatures_.push_back(feature);
        }

        output.success = true;
        return true;

    } catch (const cv::Exception& e) {
        output.success = false;
        output.errorMessage = QString("OpenCV Blob分析失败: %1").arg(e.what());
        LOG_ERROR(output.errorMessage);
        return false;
    }
}

#ifdef USE_HALCON
bool BlobTool::processWithHalcon(const cv::Mat& input, ToolResult& output)
{
    try {
        // 创建Halcon图像
        HImage hImage;
        hImage.GenImage1("byte", input.cols, input.rows, (void*)input.data);

        // 二值化
        HRegion region;
        if (autoThreshold_) {
            // 自动阈值
            HTuple usedThreshold;
            region = hImage.BinaryThreshold("max_separability", polarity_ == Light ? "light" : "dark", &usedThreshold);
        } else {
            // 固定阈值
            if (polarity_ == Light) {
                region = hImage.Threshold(threshold_, 255);
            } else {
                region = hImage.Threshold(0, threshold_);
            }
        }

        // 连通域分析
        HRegion connectedRegions = region.Connection();

        // 获取区域数量
        Hlong count = connectedRegions.CountObj();

        blobFeatures_.clear();
        blobFeatures_.reserve(count);

        for (Hlong i = 1; i <= count; ++i) {
            HRegion singleRegion = connectedRegions.SelectObj(i);

            BlobFeature feature;
            feature.id = static_cast<int>(i);

            // 计算面积和质心 - 使用返回值形式
            HTuple row, col;
            HTuple area = singleRegion.AreaCenter(&row, &col);
            feature.area = area.D();
            feature.center = QPointF(col.D(), row.D());

            // 计算周长 - 使用返回值形式
            HTuple contlength = singleRegion.Contlength();
            feature.perimeter = contlength.D();

            // 计算圆度 - 使用返回值形式
            HTuple circularity = singleRegion.Circularity();
            feature.circularity = circularity.D();

            // 计算矩形度 - 使用返回值形式
            HTuple rectang = singleRegion.Rectangularity();
            feature.rectangularity = rectang.D();

            // 计算外接矩形
            HTuple row1, col1, row2, col2;
            singleRegion.SmallestRectangle1(&row1, &col1, &row2, &col2);
            feature.boundingBox = QRectF(col1.D(), row1.D(),
                                        col2.D() - col1.D(), row2.D() - row1.D());

            // 计算最小外接矩形
            HTuple minRow, minCol, minPhi, minLen1, minLen2;
            singleRegion.SmallestRectangle2(&minRow, &minCol, &minPhi, &minLen1, &minLen2);
            feature.minBoundingWidth = minLen1.D() * 2;
            feature.minBoundingHeight = minLen2.D() * 2;
            feature.minBoundingArea = feature.minBoundingWidth * feature.minBoundingHeight;
            feature.minBoundingAngle = minPhi.D() * 180.0 / M_PI;

            // 计算方向
            feature.orientation = feature.minBoundingAngle;

            // 长宽比
            if (feature.minBoundingHeight > 0) {
                feature.aspectRatio = feature.minBoundingWidth / feature.minBoundingHeight;
            }

            // 凸度 - 使用返回值形式
            HTuple convex = singleRegion.Convexity();
            feature.convexity = convex.D();

            // 紧凑度 - 使用返回值形式
            HTuple compact = singleRegion.Compactness();
            feature.compactness = compact.D();

            // 等效圆直径
            feature.equivalentDiameter = std::sqrt(4.0 * feature.area / M_PI);

            // 椭圆拟合 - 使用返回值形式
            HTuple rb, phi;
            HTuple ra = singleRegion.EllipticAxis(&rb, &phi);
            feature.majorAxis = ra.D() * 2;
            feature.minorAxis = rb.D() * 2;
            feature.elongation = (feature.minorAxis > 0) ? feature.majorAxis / feature.minorAxis : 0;

            blobFeatures_.push_back(feature);
        }

        output.success = true;
        return true;

    } catch (const HException& e) {
        output.success = false;
        output.errorMessage = QString("Halcon Blob分析失败: %1").arg(e.ErrorMessage().Text());
        LOG_ERROR(output.errorMessage);
        return false;
    }
}
#endif

BlobFeature BlobTool::calculateFeatures(const std::vector<cv::Point>& contour, int id) const
{
    BlobFeature feature;
    feature.id = id;
    feature.contour = contour;

    // 面积
    feature.area = cv::contourArea(contour);

    // 周长
    feature.perimeter = cv::arcLength(contour, true);

    // 圆度
    if (feature.perimeter > 0) {
        feature.circularity = 4.0 * M_PI * feature.area / (feature.perimeter * feature.perimeter);
    }

    // 外接矩形
    cv::Rect boundRect = cv::boundingRect(contour);
    feature.boundingBox = QRectF(boundRect.x, boundRect.y, boundRect.width, boundRect.height);

    // 矩形度
    double boundingArea = boundRect.width * boundRect.height;
    if (boundingArea > 0) {
        feature.rectangularity = feature.area / boundingArea;
    }

    // 最小外接矩形
    if (contour.size() >= 5) {
        cv::RotatedRect minRect = cv::minAreaRect(contour);
        feature.minBoundingWidth = std::max(minRect.size.width, minRect.size.height);
        feature.minBoundingHeight = std::min(minRect.size.width, minRect.size.height);
        feature.minBoundingArea = minRect.size.width * minRect.size.height;
        feature.minBoundingAngle = minRect.angle;

        // 方向
        feature.orientation = minRect.angle;

        // 质心（使用矩计算更精确）
        cv::Moments m = cv::moments(contour);
        if (m.m00 > 0) {
            feature.center = QPointF(m.m10 / m.m00, m.m01 / m.m00);
        } else {
            feature.center = QPointF(minRect.center.x, minRect.center.y);
        }
    } else {
        // 点数不足，使用简单方法
        cv::Moments m = cv::moments(contour);
        if (m.m00 > 0) {
            feature.center = QPointF(m.m10 / m.m00, m.m01 / m.m00);
        }
        feature.minBoundingWidth = boundRect.width;
        feature.minBoundingHeight = boundRect.height;
        feature.minBoundingArea = boundingArea;
    }

    // 长宽比
    if (feature.minBoundingHeight > 0) {
        feature.aspectRatio = feature.minBoundingWidth / feature.minBoundingHeight;
    }

    // 凸包和凸度
    std::vector<cv::Point> hull;
    cv::convexHull(contour, hull);
    double hullArea = cv::contourArea(hull);
    if (hullArea > 0) {
        feature.convexity = feature.area / hullArea;
    }

    // 等效圆直径
    feature.equivalentDiameter = std::sqrt(4.0 * feature.area / M_PI);

    // 椭圆拟合（需要至少5个点）
    if (contour.size() >= 5) {
        try {
            cv::RotatedRect ellipse = cv::fitEllipse(contour);
            feature.majorAxis = std::max(ellipse.size.width, ellipse.size.height);
            feature.minorAxis = std::min(ellipse.size.width, ellipse.size.height);
            if (feature.minorAxis > 0) {
                feature.elongation = feature.majorAxis / feature.minorAxis;
            }
        } catch (...) {
            // 椭圆拟合失败，使用默认值
        }
    }

    // 紧凑度
    if (feature.perimeter > 0) {
        feature.compactness = feature.perimeter * feature.perimeter / feature.area;
    }

    return feature;
}

std::vector<BlobFeature> BlobTool::applyFilters(const std::vector<BlobFeature>& blobs) const
{
    std::vector<BlobFeature> filtered;
    filtered.reserve(blobs.size());

    for (const auto& blob : blobs) {
        bool pass = true;

        for (const auto& filter : filters_) {
            if (!filter.enabled) continue;

            double value = 0;
            switch (filter.type) {
            case BlobFilter::AreaMin:
                pass = blob.area >= filter.value;
                break;
            case BlobFilter::AreaMax:
                pass = blob.area <= filter.value;
                break;
            case BlobFilter::PerimeterMin:
                pass = blob.perimeter >= filter.value;
                break;
            case BlobFilter::PerimeterMax:
                pass = blob.perimeter <= filter.value;
                break;
            case BlobFilter::CircularityMin:
                pass = blob.circularity >= filter.value;
                break;
            case BlobFilter::CircularityMax:
                pass = blob.circularity <= filter.value;
                break;
            case BlobFilter::RectangularityMin:
                pass = blob.rectangularity >= filter.value;
                break;
            case BlobFilter::RectangularityMax:
                pass = blob.rectangularity <= filter.value;
                break;
            case BlobFilter::AspectRatioMin:
                pass = blob.aspectRatio >= filter.value;
                break;
            case BlobFilter::AspectRatioMax:
                pass = blob.aspectRatio <= filter.value;
                break;
            case BlobFilter::WidthMin:
                pass = blob.boundingBox.width() >= filter.value;
                break;
            case BlobFilter::WidthMax:
                pass = blob.boundingBox.width() <= filter.value;
                break;
            case BlobFilter::HeightMin:
                pass = blob.boundingBox.height() >= filter.value;
                break;
            case BlobFilter::HeightMax:
                pass = blob.boundingBox.height() <= filter.value;
                break;
            }

            if (!pass) break;
        }

        if (pass) {
            filtered.push_back(blob);
        }
    }

    return filtered;
}

void BlobTool::sortBlobs(std::vector<BlobFeature>& blobs) const
{
    if (sortBy_ == NoSort || blobs.empty()) return;

    auto comparator = [this](const BlobFeature& a, const BlobFeature& b) -> bool {
        double va = 0, vb = 0;
        switch (sortBy_) {
        case SortByArea:
            va = a.area;
            vb = b.area;
            break;
        case SortByX:
            va = a.center.x();
            vb = b.center.x();
            break;
        case SortByY:
            va = a.center.y();
            vb = b.center.y();
            break;
        case SortByCircularity:
            va = a.circularity;
            vb = b.circularity;
            break;
        default:
            return false;
        }

        return sortDescending_ ? (va > vb) : (va < vb);
    };

    std::sort(blobs.begin(), blobs.end(), comparator);
}

cv::Mat BlobTool::drawResults(const cv::Mat& input) const
{
    cv::Mat result;
    if (input.channels() == 1) {
        cv::cvtColor(input, result, cv::COLOR_GRAY2BGR);
    } else {
        result = input.clone();
    }

    // 绘制每个Blob
    for (const auto& blob : blobFeatures_) {
        // 绘制轮廓
        if (!blob.contour.empty()) {
            std::vector<std::vector<cv::Point>> contours = { blob.contour };
            cv::drawContours(result, contours, 0, cv::Scalar(0, 255, 0), 2);
        }

        // 绘制外接矩形
        cv::rectangle(result,
                     cv::Point(static_cast<int>(blob.boundingBox.x()),
                              static_cast<int>(blob.boundingBox.y())),
                     cv::Point(static_cast<int>(blob.boundingBox.x() + blob.boundingBox.width()),
                              static_cast<int>(blob.boundingBox.y() + blob.boundingBox.height())),
                     cv::Scalar(255, 0, 0), 1);

        // 绘制质心
        cv::circle(result,
                  cv::Point(static_cast<int>(blob.center.x()),
                           static_cast<int>(blob.center.y())),
                  4, cv::Scalar(0, 0, 255), -1);

        // 绘制ID和面积
        QString text = QString("#%1 A:%2").arg(blob.id).arg(static_cast<int>(blob.area));
        cv::putText(result, text.toStdString(),
                   cv::Point(static_cast<int>(blob.boundingBox.x()),
                            static_cast<int>(blob.boundingBox.y()) - 5),
                   cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(0, 255, 255), 1);
    }

    return result;
}

QJsonObject BlobTool::serializeParams() const
{
    QJsonObject json;
    json["backend"] = static_cast<int>(backend_);
    json["connectivity"] = static_cast<int>(connectivity_);
    json["polarity"] = static_cast<int>(polarity_);
    json["sortBy"] = static_cast<int>(sortBy_);
    json["sortDescending"] = sortDescending_;
    json["maxCount"] = maxCount_;
    json["threshold"] = threshold_;
    json["autoThreshold"] = autoThreshold_;

    // 序列化过滤器
    QJsonArray filtersArray;
    for (const auto& filter : filters_) {
        QJsonObject filterObj;
        filterObj["type"] = static_cast<int>(filter.type);
        filterObj["value"] = filter.value;
        filterObj["enabled"] = filter.enabled;
        filtersArray.append(filterObj);
    }
    json["filters"] = filtersArray;

    return json;
}

void BlobTool::deserializeParams(const QJsonObject& json)
{
    backend_ = static_cast<BackendType>(json.value("backend").toInt(static_cast<int>(Auto)));
    connectivity_ = static_cast<Connectivity>(json.value("connectivity").toInt(static_cast<int>(Connect8)));
    polarity_ = static_cast<Polarity>(json.value("polarity").toInt(static_cast<int>(Light)));
    sortBy_ = static_cast<SortBy>(json.value("sortBy").toInt(static_cast<int>(SortByArea)));
    sortDescending_ = json.value("sortDescending").toBool(true);
    maxCount_ = json.value("maxCount").toInt(0);
    threshold_ = json.value("threshold").toInt(128);
    autoThreshold_ = json.value("autoThreshold").toBool(true);

    // 反序列化过滤器
    filters_.clear();
    QJsonArray filtersArray = json.value("filters").toArray();
    for (const auto& item : filtersArray) {
        QJsonObject filterObj = item.toObject();
        BlobFilter filter;
        filter.type = static_cast<BlobFilter::FilterType>(filterObj.value("type").toInt());
        filter.value = filterObj.value("value").toDouble();
        filter.enabled = filterObj.value("enabled").toBool(true);
        filters_.push_back(filter);
    }
}

void BlobTool::setBackend(BackendType backend)
{
    if (backend_ != backend) {
        backend_ = backend;
        emit paramChanged();
    }
}

void BlobTool::setConnectivity(Connectivity conn)
{
    if (connectivity_ != conn) {
        connectivity_ = conn;
        emit paramChanged();
    }
}

void BlobTool::setPolarity(Polarity pol)
{
    if (polarity_ != pol) {
        polarity_ = pol;
        emit paramChanged();
    }
}

void BlobTool::setSortBy(SortBy sort)
{
    if (sortBy_ != sort) {
        sortBy_ = sort;
        emit paramChanged();
    }
}

void BlobTool::setSortDescending(bool desc)
{
    if (sortDescending_ != desc) {
        sortDescending_ = desc;
        emit paramChanged();
    }
}

void BlobTool::setMaxCount(int count)
{
    if (maxCount_ != count) {
        maxCount_ = count;
        emit paramChanged();
    }
}

void BlobTool::setThreshold(int threshold)
{
    int t = qBound(0, threshold, 255);
    if (threshold_ != t) {
        threshold_ = t;
        emit paramChanged();
    }
}

void BlobTool::setAutoThreshold(bool autoTh)
{
    if (autoThreshold_ != autoTh) {
        autoThreshold_ = autoTh;
        emit paramChanged();
    }
}

void BlobTool::addFilter(const BlobFilter& filter)
{
    filters_.push_back(filter);
    emit paramChanged();
}

void BlobTool::removeFilter(int index)
{
    if (index >= 0 && index < static_cast<int>(filters_.size())) {
        filters_.erase(filters_.begin() + index);
        emit paramChanged();
    }
}

void BlobTool::clearFilters()
{
    filters_.clear();
    emit paramChanged();
}

void BlobTool::setAreaRange(double minArea, double maxArea)
{
    // 移除现有的面积过滤器
    filters_.erase(
        std::remove_if(filters_.begin(), filters_.end(),
            [](const BlobFilter& f) {
                return f.type == BlobFilter::AreaMin || f.type == BlobFilter::AreaMax;
            }),
        filters_.end()
    );

    // 添加新的面积过滤器
    if (minArea > 0) {
        filters_.push_back(BlobFilter(BlobFilter::AreaMin, minArea, true));
    }
    if (maxArea > 0) {
        filters_.push_back(BlobFilter(BlobFilter::AreaMax, maxArea, true));
    }

    emit paramChanged();
}

void BlobTool::setCircularityRange(double minCirc, double maxCirc)
{
    // 移除现有的圆度过滤器
    filters_.erase(
        std::remove_if(filters_.begin(), filters_.end(),
            [](const BlobFilter& f) {
                return f.type == BlobFilter::CircularityMin || f.type == BlobFilter::CircularityMax;
            }),
        filters_.end()
    );

    // 添加新的圆度过滤器
    if (minCirc > 0) {
        filters_.push_back(BlobFilter(BlobFilter::CircularityMin, minCirc, true));
    }
    if (maxCirc > 0 && maxCirc <= 1.0) {
        filters_.push_back(BlobFilter(BlobFilter::CircularityMax, maxCirc, true));
    }

    emit paramChanged();
}

} // namespace Algorithm
} // namespace VisionForge
