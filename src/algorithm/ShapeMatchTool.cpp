/**
 * @file ShapeMatchTool.cpp
 * @brief Halcon形状匹配工具实现
 */

#include "algorithm/ShapeMatchTool.h"
#include "algorithm/HalconObjectWrapper.h"

// 解决Halcon与OpenCV/标准库的宏冲突
// 必须在包含OpenCV之前解除Halcon定义的宏
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

#include "base/Logger.h"
#include <opencv2/opencv.hpp>
#include <QFileInfo>

namespace VisionForge {
namespace Algorithm {

ShapeMatchTool::ShapeMatchTool(QObject* parent)
    : VisionTool(parent)
    , modelLoaded_(false)
    , minScore_(0.7)
    , numMatches_(1)
    , angleStart_(-0.39)      // -22.5度
    , angleExtent_(0.78)       // 45度范围
    , minContrast_(10)
    , scaleMin_(0.9)
    , scaleMax_(1.1)
    , useXLDDisplay_(true)    // 默认使用XLD轮廓显示
    , modelWidth_(0)
    , modelHeight_(0)
    , modelRefRow_(0.0)
    , modelRefCol_(0.0)
{
    setDisplayName("形状匹配");
#ifdef _WIN32
    shapeModel_ = HTuple();
#endif
}

ShapeMatchTool::~ShapeMatchTool()
{
    clearModel();
}

void ShapeMatchTool::clearModel()
{
#ifdef _WIN32
    try {
        if (modelLoaded_ && shapeModel_.Length() > 0) {
            ClearShapeModel(shapeModel_);
            LOG_INFO("形状模板已清除");
        }
    }
    catch (const HException& e) {
        LOG_ERROR(QString("清除形状模板失败: %1").arg(e.ErrorMessage().Text()));
    }
    shapeModel_ = HTuple();
#endif
    modelLoaded_ = false;
    modelPath_.clear();
    modelWidth_ = 0;
    modelHeight_ = 0;
}

bool ShapeMatchTool::loadModel(const QString& modelPath)
{
#ifdef _WIN32
    try {
        clearModel();

        QFileInfo fileInfo(modelPath);
        if (!fileInfo.exists()) {
            LOG_ERROR(QString("模板文件不存在: %1").arg(modelPath));
            return false;
        }

        // 读取模板
        ReadShapeModel(modelPath.toStdString().c_str(), &shapeModel_);

        modelLoaded_ = true;
        modelPath_ = modelPath;

        LOG_INFO(QString("成功加载形状模板: %1").arg(modelPath));
        LOG_INFO(QString("模板参考点: (%.1f, %.1f)").arg(modelRefRow_).arg(modelRefCol_));

        emit paramChanged();
        return true;
    }
    catch (const HException& e) {
        LOG_ERROR(QString("加载形状模板失败: %1").arg(e.ErrorMessage().Text()));
        return false;
    }
#else
    LOG_ERROR("Halcon功能未启用");
    return false;
#endif
}

bool ShapeMatchTool::trainModel(const Base::ImageData::Ptr& image,
                               int row1, int col1, int row2, int col2)
{
#ifdef _WIN32
    try {
        if (!image || image->isEmpty()) {
            LOG_ERROR("训练图像为空");
            return false;
        }

        clearModel();

        // 转换图像
        HImage hImg = imageDataToHImage(image);

        // 创建ROI
        HRegion roi;
        GenRectangle1(&roi, row1, col1, row2, col2);

        // 裁剪图像
        HImage modelImg = hImg.ReduceDomain(roi);

        // 创建形状模板
        CreateShapeModel(modelImg, "auto", -0.39, 0.78,  // 角度范围
                        "auto", "auto", "use_polarity",   // 自动参数
                        "auto", minContrast_, &shapeModel_);

        modelWidth_ = col2 - col1 + 1;
        modelHeight_ = row2 - row1 + 1;
        modelRefRow_ = modelHeight_ / 2.0;
        modelRefCol_ = modelWidth_ / 2.0;

        modelLoaded_ = true;

        LOG_INFO(QString("成功创建形状模板: %1x%2").arg(modelWidth_).arg(modelHeight_));
        LOG_INFO(QString("模板参考点: (%.1f, %.1f)").arg(modelRefRow_).arg(modelRefCol_));

        emit paramChanged();
        return true;
    }
    catch (const HException& e) {
        LOG_ERROR(QString("创建形状模板失败: %1").arg(e.ErrorMessage().Text()));
        return false;
    }
#else
    LOG_ERROR("Halcon功能未启用");
    return false;
#endif
}

bool ShapeMatchTool::saveModel(const QString& modelPath)
{
#ifdef _WIN32
    try {
        if (!modelLoaded_ || shapeModel_.Length() == 0) {
            LOG_ERROR("没有可保存的模板");
            return false;
        }

        // 保存模板
        WriteShapeModel(shapeModel_, modelPath.toStdString().c_str());

        modelPath_ = modelPath;

        LOG_INFO(QString("形状模板已保存: %1").arg(modelPath));
        return true;
    }
    catch (const HException& e) {
        LOG_ERROR(QString("保存形状模板失败: %1").arg(e.ErrorMessage().Text()));
        return false;
    }
#else
    LOG_ERROR("Halcon功能未启用");
    return false;
#endif
}

bool ShapeMatchTool::process(const Base::ImageData::Ptr& input, ToolResult& output)
{
    if (!input || input->isEmpty()) {
        output.success = false;
        output.errorMessage = "输入图像为空";
        return false;
    }

    if (!modelLoaded_) {
        output.success = false;
        output.errorMessage = "未加载形状模板";
        return false;
    }

#ifdef _WIN32
    try {
        // 转换图像
        HImage hImg = imageDataToHImage(input);

        // 执行形状匹配
        HTuple row, col, angle, score;

        FindShapeModel(hImg, shapeModel_,
                      angleStart_, angleExtent_,
                      minScore_, numMatches_,
                      0.5,  // max_overlap
                      "least_squares",  // sub_pixel
                      0,  // num_levels
                      0.9,  // greediness
                      &row, &col, &angle, &score);

        int numFound = row.Length();

        LOG_INFO(QString("形状匹配完成: 找到 %1 个匹配").arg(numFound));

        // 保存结果
        output.setValue("num_matches", numFound);

        if (numFound > 0) {
            // 保存第一个匹配的详细信息
            output.setValue("row", row[0].D());
            output.setValue("col", col[0].D());
            output.setValue("angle", angle[0].D());
            output.setValue("score", score[0].D());

            LOG_INFO(QString("最佳匹配: 位置(%.1f, %.1f), 角度%.2f°, 得分%.3f")
                .arg(row[0].D()).arg(col[0].D())
                .arg(angle[0].D() * 180.0 / 3.14159)
                .arg(score[0].D()));

            if (useXLDDisplay_) {
                // 使用XLD轮廓显示（非破坏性）
                HXLDCont contours = createMatchContours(row, col, angle);
                XLDContourPtr xldPtr = QSharedPointer<XLDContourWrapper>::create(contours);
                output.setDisplayObject("match_contours", QVariant::fromValue(
                    qSharedPointerCast<HalconObjectWrapper>(xldPtr)));
                output.outputImage = input;  // 返回原图
            } else {
                // 绘制结果到输出图像（破坏性）
                cv::Mat resultImg = input->mat().clone();
                drawMatchResults(resultImg, row, col, angle, score);
                output.outputImage = std::make_shared<Base::ImageData>(resultImg);
            }

            output.success = true;
        }
        else {
            LOG_WARNING("未找到匹配的形状");
            output.success = true;  // 没找到也算成功完成
            output.outputImage = input;  // 返回原图
        }

        setStatusText(QString("找到%1个匹配").arg(numFound));

        return true;
    }
    catch (const HException& e) {
        output.success = false;
        output.errorMessage = QString("形状匹配失败: %1").arg(e.ErrorMessage().Text());
        LOG_ERROR(output.errorMessage);
        return false;
    }
#else
    output.success = false;
    output.errorMessage = "Halcon功能未启用";
    return false;
#endif
}

#ifdef _WIN32
HImage ShapeMatchTool::imageDataToHImage(const Base::ImageData::Ptr& image)
{
    const cv::Mat& mat = image->mat();

    HImage hImg;

    if (mat.channels() == 1) {
        // 灰度图
        hImg.GenImage1(L"byte", mat.cols, mat.rows, (void*)mat.data);
    }
    else if (mat.channels() == 3) {
        // BGR转RGB
        cv::Mat rgb;
        cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);

        // 分离通道
        std::vector<cv::Mat> channels(3);
        cv::split(rgb, channels);

        hImg.GenImage3(L"byte", mat.cols, mat.rows,
                      (void*)channels[0].data,
                      (void*)channels[1].data,
                      (void*)channels[2].data);
    }
    else {
        throw HException("不支持的图像通道数");
    }

    return hImg;
}

void ShapeMatchTool::drawMatchResults(cv::Mat& image,
                                     const HTuple& row, const HTuple& col,
                                     const HTuple& angle, const HTuple& score)
{
    int numMatches = row.Length();

    for (int i = 0; i < numMatches; i++) {
        double r = row[i].D();
        double c = col[i].D();
        double a = angle[i].D();
        double s = score[i].D();

        cv::Point center(static_cast<int>(c), static_cast<int>(r));

        // 绘制十字标记
        cv::drawMarker(image, center, cv::Scalar(0, 255, 0),
                      cv::MARKER_CROSS, 30, 2);

        // 绘制方向线（表示角度）
        double length = 40.0;
        cv::Point endPt(
            static_cast<int>(c + length * cos(a)),
            static_cast<int>(r + length * sin(a))
        );
        cv::line(image, center, endPt, cv::Scalar(0, 255, 255), 2);

        // 绘制分数文本
        QString text = QString("#%1: %.3f").arg(i + 1).arg(s);
        cv::putText(image, text.toStdString(),
                   cv::Point(static_cast<int>(c) + 10, static_cast<int>(r) - 10),
                   cv::FONT_HERSHEY_SIMPLEX, 0.6,
                   cv::Scalar(0, 255, 0), 2);
    }
}

HXLDCont ShapeMatchTool::createMatchContours(const HTuple& row, const HTuple& col,
                                             const HTuple& angle)
{
    HXLDCont allContours;

    try {
        // 获取模板轮廓
        HXLDCont modelContours;
        GetShapeModelContours(&modelContours, shapeModel_, 1);

        int numMatches = row.Length();
        for (int i = 0; i < numMatches; i++) {
            // 计算仿射变换矩阵
            HTuple homMat2D;
            VectorAngleToRigid(modelRefRow_, modelRefCol_, 0,  // 模板参考点
                             row[i].D(), col[i].D(), angle[i].D(),  // 匹配结果
                             &homMat2D);

            // 变换轮廓
            HXLDCont transformedCont;
            AffineTransContourXld(modelContours, &transformedCont, homMat2D);

            // 合并到总轮廓
            if (i == 0) {
                allContours = transformedCont;
            } else {
                allContours = allContours.ConcatObj(transformedCont);
            }
        }
    }
    catch (const HException& e) {
        LOG_ERROR(QString("创建匹配轮廓失败: %1").arg(e.ErrorMessage().Text()));
    }

    return allContours;
}
#endif

QJsonObject ShapeMatchTool::serializeParams() const
{
    QJsonObject json;
    json["model_path"] = modelPath_;
    json["min_score"] = minScore_;
    json["num_matches"] = numMatches_;
    json["angle_start"] = angleStart_;
    json["angle_extent"] = angleExtent_;
    json["min_contrast"] = minContrast_;
    json["scale_min"] = scaleMin_;
    json["scale_max"] = scaleMax_;
    json["use_xld_display"] = useXLDDisplay_;
    return json;
}

void ShapeMatchTool::deserializeParams(const QJsonObject& json)
{
    modelPath_ = json.value("model_path").toString();
    minScore_ = json.value("min_score").toDouble(0.7);
    numMatches_ = json.value("num_matches").toInt(1);
    angleStart_ = json.value("angle_start").toDouble(-0.39);
    angleExtent_ = json.value("angle_extent").toDouble(0.78);
    minContrast_ = json.value("min_contrast").toInt(10);
    scaleMin_ = json.value("scale_min").toDouble(0.9);
    scaleMax_ = json.value("scale_max").toDouble(1.1);
    useXLDDisplay_ = json.value("use_xld_display").toBool(true);

    // 如果有模板路径，尝试加载
    if (!modelPath_.isEmpty()) {
        loadModel(modelPath_);
    }
}

} // namespace Algorithm
} // namespace VisionForge
