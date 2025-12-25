/**
 * @file BlobTool.cpp
 * @brief Blob分析工具实现
 * @details 使用抽象后端接口，无需 #ifdef USE_HALCON
 */

#include "algorithm/BlobTool.h"
#include "base/Logger.h"
#include "base/ImageMemoryPool.h"
#include <opencv2/imgproc.hpp>
#include <QElapsedTimer>
#include <QJsonArray>
#include <algorithm>
#include <cmath>

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

    blobFeatures_.clear();

    // 确保分析器已创建
    ensureAnalyzer();

    // 使用抽象接口进行分析（无需 #ifdef）
    std::vector<Backend::BlobAnalyzeResult> backendResults;
    QString errorMsg;
    bool success = analyzer_->analyze(gray, toBackendParams(), backendResults, errorMsg);

    if (success) {
        // 转换后端结果到本地格式
        blobFeatures_.reserve(backendResults.size());
        for (const auto& br : backendResults) {
            BlobFeature bf;
            bf.id = br.id;
            bf.area = br.area;
            bf.perimeter = br.perimeter;
            bf.circularity = br.circularity;
            bf.rectangularity = br.rectangularity;
            bf.aspectRatio = br.aspectRatio;
            bf.orientation = br.orientation;
            bf.center = br.center;
            bf.boundingBox = br.boundingBox;
            bf.equivalentDiameter = br.equivalentDiameter;
            bf.contour = br.contour;
            blobFeatures_.push_back(bf);
        }

        // 应用过滤器
        blobFeatures_ = applyFilters(blobFeatures_);

        // 排序
        sortBlobs(blobFeatures_);

        // 限制数量
        if (maxCount_ > 0 && blobFeatures_.size() > static_cast<size_t>(maxCount_)) {
            blobFeatures_.resize(maxCount_);
        }

        output.success = true;
        output.executionTime = timer.elapsed();
        output.setValue("blobCount", static_cast<int>(blobFeatures_.size()));
        output.setValue("backend", analyzer_->backendName());

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

        LOG_DEBUG(QString("Blob分析完成，检测到 %1 个Blob，耗时: %2ms，后端: %3")
                 .arg(blobFeatures_.size())
                 .arg(output.executionTime)
                 .arg(analyzer_->backendName()));
    } else {
        output.success = false;
        output.errorMessage = errorMsg;
    }

    return success;
}

void BlobTool::ensureAnalyzer()
{
    if (!analyzer_) {
        Backend::BackendType backendType = Backend::BackendType::Auto;
        switch (backend_) {
            case Auto:   backendType = Backend::BackendType::Auto; break;
            case OpenCV: backendType = Backend::BackendType::OpenCV; break;
            case Halcon: backendType = Backend::BackendType::Halcon; break;
        }
        analyzer_ = Backend::AlgorithmBackendFactory::instance().createBlobAnalyzer(backendType);
    }
}

Backend::BlobAnalyzeParams BlobTool::toBackendParams() const
{
    Backend::BlobAnalyzeParams params;

    params.connectivity = (connectivity_ == Connect8) ? 8 : 4;
    params.detectLight = (polarity_ == Light);
    params.autoThreshold = autoThreshold_;
    params.threshold = threshold_;

    // 从过滤器中提取面积和圆度范围
    for (const auto& filter : filters_) {
        if (!filter.enabled) continue;
        switch (filter.type) {
            case BlobFilter::AreaMin: params.minArea = filter.value; break;
            case BlobFilter::AreaMax: params.maxArea = filter.value; break;
            case BlobFilter::CircularityMin: params.minCircularity = filter.value; break;
            case BlobFilter::CircularityMax: params.maxCircularity = filter.value; break;
            default: break;
        }
    }

    params.maxCount = (maxCount_ > 0) ? maxCount_ : 1000;
    params.sortByArea = (sortBy_ == SortByArea);
    params.sortDescending = sortDescending_;

    return params;
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
        analyzer_.reset();  // 重置分析器，下次 process() 时重新创建
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
