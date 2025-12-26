/**
 * @file ShapeMatchTool.cpp
 * @brief Halcon形状匹配工具实现 (Pimpl模式)
 */

#include "algorithm/ShapeMatchTool.h"
#include "algorithm/ShapeMatchImpl.h"
#include "algorithm/ShapeMatchUtils.h"
#include "algorithm/HalconObjectWrapper.h"
#include "base/Logger.h"
#include "base/ImageMemoryPool.h"
#include <opencv2/opencv.hpp>
#include <QFileInfo>
#include <QJsonArray>
#include <cmath>
#include <string>

// 解决Halcon与OpenCV/标准库的宏冲突
#ifdef high
#undef high
#endif
#ifdef low
#undef low
#endif
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

namespace VisionForge {
namespace Algorithm {

// ============================================================
// ShapeMatchImpl 实现
// ============================================================

ShapeMatchImpl::ShapeMatchImpl()
    : modelLoaded_(false)
    , modelWidth_(0)
    , modelHeight_(0)
    , modelRefRow_(0.0)
    , modelRefCol_(0.0)
{
#ifdef _WIN32
    shapeModel_ = HTuple();
    useGenericModel_ = false;
#endif
}

ShapeMatchImpl::~ShapeMatchImpl()
{
    clearModel();
}

void ShapeMatchImpl::clearModel()
{
#ifdef _WIN32
    try {
        if (modelLoaded_ && shapeModel_.Length() > 0) {
            ClearShapeModel(shapeModel_);
            LOG_INFO(QString("%1已清除").arg(useGenericModel_ ? "通用形状模板" : "形状模板"));
        }
    }
    catch (const HException& e) {
        LOG_ERROR(QString("清除形状模板失败: %1").arg(e.ErrorMessage().Text()));
    }
    shapeModel_ = HTuple();
    standardContour_ = HXLDCont();
    useGenericModel_ = false;
#endif
    modelLoaded_ = false;
    modelPath_.clear();
    modelWidth_ = 0;
    modelHeight_ = 0;
}

bool ShapeMatchImpl::loadModel(const QString& modelPath)
{
#ifdef _WIN32
    clearModel();

    QFileInfo fileInfo(modelPath);
    if (!fileInfo.exists()) {
        LOG_ERROR(QString("模板文件不存在: %1").arg(modelPath));
        return false;
    }

    std::wstring wpath = modelPath.toStdWString();
    HTuple pathTuple(wpath.c_str());

    try {
        ReadShapeModel(pathTuple, &shapeModel_);
        modelLoaded_ = true;
        modelPath_ = modelPath;

        QString suffix = fileInfo.suffix().toLower();
        if (suffix == "gshm") {
            useGenericModel_ = true;
            LOG_INFO(QString("成功加载通用形状模板(GSHM): %1").arg(modelPath));
        } else {
            try {
                HTuple paramValue;
                GetGenericShapeModelParam(shapeModel_, "min_score", &paramValue);
                useGenericModel_ = true;
                LOG_INFO(QString("成功加载通用形状模板(检测): %1").arg(modelPath));
            }
            catch (const HException&) {
                useGenericModel_ = false;
                LOG_INFO(QString("成功加载传统形状模板: %1").arg(modelPath));
            }
        }

        // 获取模板参考点
        try {
            if (useGenericModel_) {
                HObject contoursObj;
                GetGenericShapeModelObject(&contoursObj, shapeModel_, "contours");
                HTuple countObj;
                CountObj(contoursObj, &countObj);
                if (countObj.I() > 0) {
                    HTuple minRow, maxRow, minCol, maxCol;
                    SmallestRectangle1Xld(HXLDCont(contoursObj), &minRow, &maxRow, &minCol, &maxCol);
                    modelRefRow_ = (minRow.D() + maxRow.D()) / 2.0;
                    modelRefCol_ = (minCol.D() + maxCol.D()) / 2.0;
                    modelWidth_ = static_cast<int>(maxCol.D() - minCol.D());
                    modelHeight_ = static_cast<int>(maxRow.D() - minRow.D());
                }
            } else {
                HTuple originRow, originCol;
                GetShapeModelOrigin(shapeModel_, &originRow, &originCol);
                HXLDCont modelContours;
                GetShapeModelContours(&modelContours, shapeModel_, 1);
                if (modelContours.CountObj() > 0) {
                    HTuple minRow, maxRow, minCol, maxCol;
                    SmallestRectangle1Xld(modelContours, &minRow, &maxRow, &minCol, &maxCol);
                    double centerRow = (minRow.D() + maxRow.D()) / 2.0;
                    double centerCol = (minCol.D() + maxCol.D()) / 2.0;
                    modelRefRow_ = centerRow + originRow.D();
                    modelRefCol_ = centerCol + originCol.D();
                    modelWidth_ = static_cast<int>(maxCol.D() - minCol.D());
                    modelHeight_ = static_cast<int>(maxRow.D() - minRow.D());
                }
            }
        }
        catch (const HException& e) {
            LOG_WARNING(QString("获取模板参考点失败: %1, 使用默认值(0,0)").arg(e.ErrorMessage().Text()));
            modelRefRow_ = 0.0;
            modelRefCol_ = 0.0;
        }

        return true;
    }
    catch (const HException& e) {
        LOG_ERROR(QString("加载形状模板失败: %1").arg(e.ErrorMessage().Text()));
        return false;
    }
#else
    return false;
#endif
}

bool ShapeMatchImpl::saveModel(const QString& modelPath)
{
#ifdef _WIN32
    try {
        if (!modelLoaded_ || shapeModel_.Length() == 0) return false;

        std::wstring wpath = modelPath.toStdWString();
        HTuple pathTuple(wpath.c_str());
        WriteShapeModel(shapeModel_, pathTuple);
        modelPath_ = modelPath;
        return true;
    }
    catch (const HException& e) {
        LOG_ERROR(QString("保存形状模板失败: %1").arg(e.ErrorMessage().Text()));
        return false;
    }
#else
    return false;
#endif
}

bool ShapeMatchImpl::trainModel(const Base::ImageData::Ptr& image, int row1, int col1, int row2, int col2)
{
#ifdef _WIN32
    try {
        if (!image || image->isEmpty()) return false;
        clearModel();

        HImage hImg = imageDataToHImage(image);
        HRegion roi;
        GenRectangle1(&roi, row1, col1, row2, col2);
        HImage modelImg = hImg.ReduceDomain(roi);

        CreateShapeModel(modelImg, "auto", -0.39, 0.78, "auto", "auto", "use_polarity", "auto", 30, &shapeModel_);

        modelWidth_ = col2 - col1 + 1;
        modelHeight_ = row2 - row1 + 1;
        modelRefRow_ = modelHeight_ / 2.0;
        modelRefCol_ = modelWidth_ / 2.0;
        modelLoaded_ = true;
        return true;
    }
    catch (const HException& e) {
        LOG_ERROR(QString("创建形状模板失败: %1").arg(e.ErrorMessage().Text()));
        return false;
    }
#else
    return false;
#endif
}

#ifdef _WIN32
bool ShapeMatchImpl::trainModelFromContour(const HXLDCont& contour,
                                           const Base::ImageData::Ptr& highNoiseImage,
                                           int sampleRow1, int sampleCol1,
                                           int sampleRow2, int sampleCol2)
{
    try {
        if (!contour.IsInitialized() || contour.CountObj() == 0) return false;
        clearModel();

        CreateGenericShapeModel(&shapeModel_);
        SetGenericShapeModelParam(shapeModel_, "iso_scale_min", 0.8);
        SetGenericShapeModelParam(shapeModel_, "iso_scale_max", 1.2);
        
        // 设置模型轮廓
        HObject contoursObj = contour;
        SetGenericShapeModelObject(contoursObj, shapeModel_, "contours");

        // 如果提供了高噪声样本图像，用于训练参数
        if (highNoiseImage && !highNoiseImage->isEmpty()) {
            HImage hImg = imageDataToHImage(highNoiseImage);
            HRegion roi;
            GenRectangle1(&roi, sampleRow1, sampleCol1, sampleRow2, sampleCol2);
            HImage reducedImg = hImg.ReduceDomain(roi);
            TrainGenericShapeModel(reducedImg, shapeModel_);
        } else {
            // 否则只训练模型本身
            TrainGenericShapeModel(HImage(), shapeModel_);
        }

        useGenericModel_ = true;
        modelLoaded_ = true;
        standardContour_ = contour;

        // 计算参考点
        HTuple minRow, maxRow, minCol, maxCol;
        SmallestRectangle1Xld(contour, &minRow, &maxRow, &minCol, &maxCol);
        modelRefRow_ = (minRow.D() + maxRow.D()) / 2.0;
        modelRefCol_ = (minCol.D() + maxCol.D()) / 2.0;
        modelWidth_ = static_cast<int>(maxCol.D() - minCol.D());
        modelHeight_ = static_cast<int>(maxRow.D() - minRow.D());

        return true;
    }
    catch (const HException& e) {
        LOG_ERROR(QString("从轮廓创建通用形状模板失败: %1").arg(e.ErrorMessage().Text()));
        return false;
    }
}

bool ShapeMatchImpl::process(const Base::ImageData::Ptr& input, ToolResult& output, ShapeMatchTool* parent)
{
    try {
        HImage hImg = imageDataToHImage(input);
        HTuple row, col, angle, score;
        HTuple scale, scaleRow, scaleCol;

        if (useGenericModel_) {
            double searchAngleStart = parent->angleStart_;
            double searchAngleEnd = parent->angleStart_ + parent->angleExtent_;
            if (parent->angleExtent_ < M_PI / 2) {
                searchAngleStart = -M_PI / 2;
                searchAngleEnd = M_PI / 2;
            }

            SetGenericShapeModelParam(shapeModel_, "angle_start", searchAngleStart);
            SetGenericShapeModelParam(shapeModel_, "angle_end", searchAngleEnd);
            SetGenericShapeModelParam(shapeModel_, "num_matches", parent->numMatches_ > 0 ? parent->numMatches_ : 10);
            SetGenericShapeModelParam(shapeModel_, "min_score", parent->minScore_);
            SetGenericShapeModelParam(shapeModel_, "max_overlap", parent->maxOverlap_);
            SetGenericShapeModelParam(shapeModel_, "greediness", parent->greediness_);

            HTuple matchResultID, numMatchResult;
            FindGenericShapeModel(hImg, shapeModel_, &matchResultID, &numMatchResult);

            if (numMatchResult.I() > 0) {
                try {
                    HObject contoursObj;
                    GetGenericShapeModelResultObject(&contoursObj, matchResultID, "all", "contours");
                    lastMatchContours_ = HXLDCont(contoursObj);
                } catch (...) {}

                for (int i = 0; i < numMatchResult.I(); i++) {
                    HTuple r, c, a, s;
                    GetGenericShapeModelResult(matchResultID, i, "row", &r);
                    GetGenericShapeModelResult(matchResultID, i, "column", &c);
                    GetGenericShapeModelResult(matchResultID, i, "angle", &a);
                    GetGenericShapeModelResult(matchResultID, i, "score", &s);
                    row = row.TupleConcat(r);
                    col = col.TupleConcat(c);
                    angle = angle.TupleConcat(a);
                    score = score.TupleConcat(s);
                }
            }
        } else {
            switch (parent->matchType_) {
            case ShapeMatchTool::Standard:
                FindShapeModel(hImg, shapeModel_, parent->angleStart_, parent->angleExtent_,
                              parent->minScore_, parent->numMatches_, 0.5, "least_squares",
                              0, 0.9, &row, &col, &angle, &score);
                break;
            case ShapeMatchTool::Scaled:
                FindScaledShapeModel(hImg, shapeModel_, parent->angleStart_, parent->angleExtent_,
                                    parent->scaleMin_, parent->scaleMax_,
                                    parent->minScore_, parent->numMatches_, 0.5, "least_squares",
                                    0, 0.9, &row, &col, &angle, &scale, &score);
                break;
            case ShapeMatchTool::Anisotropic:
                FindAnisoShapeModel(hImg, shapeModel_, parent->angleStart_, parent->angleExtent_,
                                   parent->scaleMin_, parent->scaleMax_, parent->scaleMin_, parent->scaleMax_,
                                   parent->minScore_, parent->numMatches_, 0.5, "least_squares",
                                   0, 0.9, &row, &col, &angle, &scaleRow, &scaleCol, &score);
                break;
            }
        }

        int numFound = row.Length();
        HTuple validRow, validCol, validAngle, validScore;
        int validCount = 0;

        if (parent->checkPointsEnabled_ && !parent->checkPoints_.empty()) {
            for (int i = 0; i < numFound; i++) {
                if (parent->validateCheckPoints(input, row[i].D(), col[i].D(), angle[i].D())) {
                    validRow = validRow.TupleConcat(row[i]);
                    validCol = validCol.TupleConcat(col[i]);
                    validAngle = validAngle.TupleConcat(angle[i]);
                    validScore = validScore.TupleConcat(score[i]);
                    validCount++;
                }
            }
        } else {
            validRow = row; validCol = col; validAngle = angle; validScore = score;
            validCount = numFound;
        }

        output.setValue("num_matches", validCount);
        if (validCount > 0) {
            output.setValue("row", validRow[0].D());
            output.setValue("col", validCol[0].D());
            output.setValue("angle", validAngle[0].D());
            output.setValue("score", validScore[0].D());

            if (parent->useXLDDisplay_) {
                HXLDCont contours = createMatchContours(validRow, validCol, validAngle, parent);
                XLDContourPtr xldPtr = QSharedPointer<XLDContourWrapper>::create(contours);
                output.setDisplayObject("match_contours", QVariant::fromValue(qSharedPointerCast<HalconObjectWrapper>(xldPtr)));
                
                cv::Mat resultImg = input->mat().clone();
                drawMatchResults(resultImg, validRow, validCol, validAngle, validScore);
                output.outputImage = Base::ImageMemoryPool::instance().allocate(resultImg.cols, resultImg.rows, resultImg.type());
                if (output.outputImage) resultImg.copyTo(output.outputImage->mat());
            } else {
                cv::Mat resultImg = input->mat().clone();
                drawMatchResults(resultImg, validRow, validCol, validAngle, validScore);
                output.outputImage = Base::ImageMemoryPool::instance().allocate(resultImg.cols, resultImg.rows, resultImg.type());
                if (output.outputImage) resultImg.copyTo(output.outputImage->mat());
            }
            output.success = true;
        } else {
            output.success = true; // 没找到也算成功
            output.outputImage = input;
        }
        return true;
    }
    catch (const HException& e) {
        output.success = false;
        output.errorMessage = QString("形状匹配失败: %1").arg(e.ErrorMessage().Text());
        return false;
    }
}

HImage ShapeMatchImpl::imageDataToHImage(const Base::ImageData::Ptr& image)
{
    const cv::Mat& mat = image->mat();
    HImage hImg;
    if (mat.channels() == 1) {
        hImg.GenImage1(L"byte", mat.cols, mat.rows, (void*)mat.data);
    } else if (mat.channels() == 3) {
        cv::Mat rgb;
        cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);
        std::vector<cv::Mat> channels(3);
        cv::split(rgb, channels);
        hImg.GenImage3(L"byte", mat.cols, mat.rows, (void*)channels[0].data, (void*)channels[1].data, (void*)channels[2].data);
    }
    return hImg;
}

void ShapeMatchImpl::drawMatchResults(cv::Mat& image, const HTuple& row, const HTuple& col, const HTuple& angle, const HTuple& score)
{
    int numMatches = row.Length();
    for (int i = 0; i < numMatches; i++) {
        double r = row[i].D();
        double c = col[i].D();
        cv::Point center(static_cast<int>(c), static_cast<int>(r));
        cv::drawMarker(image, center, cv::Scalar(0, 0, 255), cv::MARKER_CROSS, 20, 2);
        cv::putText(image, QString("#%1 S:%.2f").arg(i+1).arg(score[i].D()).toStdString(),
                   center + cv::Point(10, -10), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 1);
    }
}

HXLDCont ShapeMatchImpl::createMatchContours(const HTuple& row, const HTuple& col, const HTuple& angle, ShapeMatchTool* parent)
{
    HXLDCont allContours;
    try {
        if (useGenericModel_ && lastMatchContours_.IsInitialized() && lastMatchContours_.CountObj() > 0) {
            return lastMatchContours_;
        }
        
        HXLDCont modelContours;
        if (standardContour_.IsInitialized() && standardContour_.CountObj() > 0) {
            modelContours = standardContour_;
        } else {
             try {
                GetShapeModelContours(&modelContours, shapeModel_, 1);
            } catch (...) {}
        }

        if (modelContours.CountObj() == 0) return allContours;

        if (parent->contourFilterEnabled_) {
            modelContours = filterContours(modelContours);
        }

        int numMatches = row.Length();
        for (int i = 0; i < numMatches; i++) {
            HTuple homMat2D;
            VectorAngleToRigid(modelRefRow_, modelRefCol_, 0, row[i].D(), col[i].D(), angle[i].D(), &homMat2D);
            HXLDCont transformedCont;
            AffineTransContourXld(modelContours, &transformedCont, homMat2D);
            allContours = (i == 0) ? transformedCont : allContours.ConcatObj(transformedCont);
        }
    } catch (...) {}
    return allContours;
}

HXLDCont ShapeMatchImpl::filterContours(const HXLDCont& contours) const
{
    // 简化实现，实际应包含SelectShapeXld等逻辑
    return contours;
}
#endif

// ============================================================
// ShapeMatchTool 实现
// ============================================================

ShapeMatchTool::ShapeMatchTool(QObject* parent)
    : VisionTool(parent)
    , impl_(std::make_unique<ShapeMatchImpl>())
    , matchType_(Standard)
    , minScore_(0.7)
    , numMatches_(1)
    , angleStart_(-0.39)
    , angleExtent_(0.78)
    , minContrast_(30)
    , scaleMin_(0.9)
    , scaleMax_(1.1)
    , scaleStepRow_(0.95)
    , scaleStepCol_(0.95)
    , maxOverlap_(0.5)
    , greediness_(0.9)
    , useXLDDisplay_(true)
    , checkPointsEnabled_(false)
    , contourFilterEnabled_(true)
    , minContourLength_(100.0)
    , unionCollinearEnabled_(true)
    , unionAdjacentEnabled_(true)
    , maxDistCollinear_(15.0)
    , maxDistAdjacent_(15.0)
{
    setDisplayName("形状匹配");
}

ShapeMatchTool::~ShapeMatchTool() = default;

bool ShapeMatchTool::process(const Base::ImageData::Ptr& input, ToolResult& output)
{
    if (!input || input->isEmpty()) {
        output.success = false;
        output.errorMessage = "输入图像为空";
        return false;
    }

    if (!impl_->modelLoaded_) {
        output.success = false;
        output.errorMessage = "未加载形状模板";
        return false;
    }

#ifdef _WIN32
    return impl_->process(input, output, this);
#else
    output.success = false;
    output.errorMessage = "Halcon功能未启用";
    return false;
#endif
}

bool ShapeMatchTool::loadModel(const QString& modelPath)
{
    bool ret = impl_->loadModel(modelPath);
    if (ret) emit paramChanged();
    return ret;
}

bool ShapeMatchTool::trainModel(const Base::ImageData::Ptr& image, int row1, int col1, int row2, int col2)
{
    bool ret = impl_->trainModel(image, row1, col1, row2, col2);
    if (ret) emit paramChanged();
    return ret;
}

bool ShapeMatchTool::trainModelFromContour(const QVariant& contour,
                                           const Base::ImageData::Ptr& highNoiseImage,
                                           int sampleRow1, int sampleCol1,
                                           int sampleRow2, int sampleCol2)
{
#ifdef _WIN32
    if (contour.canConvert<HalconObjectPtr>()) {
        HalconObjectPtr ptr = contour.value<HalconObjectPtr>();
        if (ptr && ptr->type() == HalconObjectWrapper::XLD_Contour) {
            XLDContourWrapper* wrapper = static_cast<XLDContourWrapper*>(ptr.data());
            bool ret = impl_->trainModelFromContour(wrapper->contours(), highNoiseImage,
                                                    sampleRow1, sampleCol1, sampleRow2, sampleCol2);
            if (ret) emit paramChanged();
            return ret;
        }
    }
    return false;
#else
    return false;
#endif
}

bool ShapeMatchTool::saveModel(const QString& modelPath)
{
    return impl_->saveModel(modelPath);
}

void ShapeMatchTool::clearModel()
{
    impl_->clearModel();
}

bool ShapeMatchTool::hasModel() const
{
    return impl_->modelLoaded_;
}

QVariant ShapeMatchTool::getModelContours() const
{
#ifdef _WIN32
    if (!impl_->modelLoaded_) {
        return QVariant();
    }

    try {
        HXLDCont contours;

        if (impl_->useGenericModel_) {
            // 通用模型使用lastMatchContours_或standardContour_
            if (impl_->lastMatchContours_.IsInitialized() && impl_->lastMatchContours_.CountObj() > 0) {
                contours = impl_->lastMatchContours_;
            } else if (impl_->standardContour_.IsInitialized()) {
                contours = impl_->standardContour_;
            }
        } else {
            // 传统模型使用GetShapeModelContours
            HTuple modelID = impl_->shapeModel_;
            if (modelID.Length() > 0) {
                GetShapeModelContours(&contours, modelID, 1);
            }
        }

        if (contours.IsInitialized() && contours.CountObj() > 0) {
            auto wrapper = QSharedPointer<XLDContourWrapper>::create(contours);
            return QVariant::fromValue(HalconObjectPtr(wrapper));
        }
    }
    catch (const HException& e) {
        LOG_ERROR(QString("获取模型轮廓失败: %1").arg(e.ErrorMessage().Text()));
    }
#endif
    return QVariant();
}

QString ShapeMatchTool::getModelPath() const
{
    return impl_->modelPath_;
}

void ShapeMatchTool::setModelPath(const QString& path)
{
    impl_->modelPath_ = path;
}

// 参数设置器实现
void ShapeMatchTool::setMinScore(double score) { minScore_ = score; emit paramChanged(); }
void ShapeMatchTool::setNumMatches(int num) { numMatches_ = num; emit paramChanged(); }
void ShapeMatchTool::setAngleStart(double angle) { angleStart_ = angle; emit paramChanged(); }
void ShapeMatchTool::setAngleExtent(double angle) { angleExtent_ = angle; emit paramChanged(); }
void ShapeMatchTool::setMinContrast(int contrast) { minContrast_ = contrast; emit paramChanged(); }
void ShapeMatchTool::setScaleMin(double scale) { scaleMin_ = scale; emit paramChanged(); }
void ShapeMatchTool::setScaleMax(double scale) { scaleMax_ = scale; emit paramChanged(); }
void ShapeMatchTool::setMaxOverlap(double overlap) { maxOverlap_ = overlap; emit paramChanged(); }
void ShapeMatchTool::setGreediness(double greediness) { greediness_ = greediness; emit paramChanged(); }
void ShapeMatchTool::setUseXLDDisplay(bool use) { useXLDDisplay_ = use; emit paramChanged(); }
void ShapeMatchTool::setMatchType(MatchType type) { matchType_ = type; emit paramChanged(); }
void ShapeMatchTool::setScaleStepRow(double step) { scaleStepRow_ = step; emit paramChanged(); }
void ShapeMatchTool::setScaleStepCol(double step) { scaleStepCol_ = step; emit paramChanged(); }

void ShapeMatchTool::setContourFilterEnabled(bool enabled) { contourFilterEnabled_ = enabled; emit paramChanged(); }
void ShapeMatchTool::setMinContourLength(double length) { minContourLength_ = length; emit paramChanged(); }
void ShapeMatchTool::setUnionCollinearEnabled(bool enabled) { unionCollinearEnabled_ = enabled; emit paramChanged(); }
void ShapeMatchTool::setUnionAdjacentEnabled(bool enabled) { unionAdjacentEnabled_ = enabled; emit paramChanged(); }
void ShapeMatchTool::setMaxDistCollinear(double dist) { maxDistCollinear_ = dist; emit paramChanged(); }
void ShapeMatchTool::setMaxDistAdjacent(double dist) { maxDistAdjacent_ = dist; emit paramChanged(); }

// 校验点相关
void ShapeMatchTool::addCheckPoint(const ModelCheckPoint& checkPoint) { checkPoints_.push_back(checkPoint); emit paramChanged(); }
void ShapeMatchTool::removeCheckPoint(int index) {
    if (index >= 0 && index < checkPoints_.size()) {
        checkPoints_.erase(checkPoints_.begin() + index);
        emit paramChanged();
    }
}
void ShapeMatchTool::clearCheckPoints() { checkPoints_.clear(); emit paramChanged(); }
void ShapeMatchTool::setCheckPoints(const std::vector<ModelCheckPoint>& points) { checkPoints_ = points; emit paramChanged(); }
void ShapeMatchTool::setCheckPointsEnabled(bool enabled) { checkPointsEnabled_ = enabled; emit paramChanged(); }

bool ShapeMatchTool::validateCheckPoints(const Base::ImageData::Ptr& image, double row, double col, double angle, std::vector<int>* failedPoints)
{
    Q_UNUSED(image);
    Q_UNUSED(row);
    Q_UNUSED(col);
    Q_UNUSED(angle);
    Q_UNUSED(failedPoints);
    // 简化实现，实际需要仿射变换计算校验点位置并检查灰度
    return true; 
}

QJsonObject ShapeMatchTool::serializeParams() const
{
    QJsonObject json;
    json["matchType"] = static_cast<int>(matchType_);
    json["minScore"] = minScore_;
    json["numMatches"] = numMatches_;
    json["angleStart"] = angleStart_;
    json["angleExtent"] = angleExtent_;
    json["minContrast"] = minContrast_;
    json["scaleMin"] = scaleMin_;
    json["scaleMax"] = scaleMax_;
    json["modelPath"] = impl_->modelPath_;
    return json;
}

void ShapeMatchTool::deserializeParams(const QJsonObject& json)
{
    if (json.contains("matchType")) matchType_ = static_cast<MatchType>(json["matchType"].toInt());
    if (json.contains("minScore")) minScore_ = json["minScore"].toDouble();
    if (json.contains("numMatches")) numMatches_ = json["numMatches"].toInt();
    if (json.contains("angleStart")) angleStart_ = json["angleStart"].toDouble();
    if (json.contains("angleExtent")) angleExtent_ = json["angleExtent"].toDouble();
    if (json.contains("minContrast")) minContrast_ = json["minContrast"].toInt();
    if (json.contains("scaleMin")) scaleMin_ = json["scaleMin"].toDouble();
    if (json.contains("scaleMax")) scaleMax_ = json["scaleMax"].toDouble();
    if (json.contains("modelPath")) {
        QString path = json["modelPath"].toString();
        if (!path.isEmpty() && QFile::exists(path)) {
            loadModel(path);
        }
    }
}

} // namespace Algorithm
} // namespace VisionForge
