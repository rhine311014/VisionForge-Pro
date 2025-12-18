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
#include <QJsonArray>
#include <cmath>

namespace VisionForge {
namespace Algorithm {

ShapeMatchTool::ShapeMatchTool(QObject* parent)
    : VisionTool(parent)
    , modelLoaded_(false)
    , matchType_(Standard)    // 默认标准匹配
    , minScore_(0.7)
    , numMatches_(1)
    , angleStart_(-0.39)      // -22.5度
    , angleExtent_(0.78)       // 45度范围
    , minContrast_(30)        // 提高对比度阈值以减少噪声边缘
    , scaleMin_(0.9)
    , scaleMax_(1.1)
    , scaleStepRow_(0.95)     // 行方向缩放步长
    , scaleStepCol_(0.95)     // 列方向缩放步长
    , maxOverlap_(0.5)        // 默认最大重叠度
    , greediness_(0.9)        // 默认贪婪度
    , useXLDDisplay_(true)    // 默认使用XLD轮廓显示
    , modelWidth_(0)
    , modelHeight_(0)
    , modelRefRow_(0.0)
    , modelRefCol_(0.0)
    , checkPointsEnabled_(false)  // 默认禁用校验点
    , contourFilterEnabled_(true)  // 默认启用轮廓过滤
    , minContourLength_(100.0)     // 最小轮廓长度100像素（大幅过滤短边缘）
    , unionCollinearEnabled_(true) // 默认连接共线轮廓
    , unionAdjacentEnabled_(true)  // 默认连接相邻轮廓
    , maxDistCollinear_(15.0)      // 共线最大距离15像素
    , maxDistAdjacent_(15.0)       // 相邻最大距离15像素
{
    setDisplayName("形状匹配");
#ifdef _WIN32
    shapeModel_ = HTuple();
    useGenericModel_ = false;
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
            // 所有形状模型（包括Generic Shape Model）使用相同的清除函数
            ClearShapeModel(shapeModel_);
            LOG_INFO(QString("%1已清除").arg(useGenericModel_ ? "通用形状模板" : "形状模板"));
        }
    }
    catch (const HException& e) {
        LOG_ERROR(QString("清除形状模板失败: %1").arg(e.ErrorMessage().Text()));
    }
    shapeModel_ = HTuple();
    standardContour_ = HXLDCont();  // 清除标准轮廓
    useGenericModel_ = false;
#endif
    modelLoaded_ = false;
    modelPath_.clear();
    modelWidth_ = 0;
    modelHeight_ = 0;
}

bool ShapeMatchTool::loadModel(const QString& modelPath)
{
#ifdef _WIN32
    clearModel();

    QFileInfo fileInfo(modelPath);
    if (!fileInfo.exists()) {
        LOG_ERROR(QString("模板文件不存在: %1").arg(modelPath));
        return false;
    }

    // 读取模板（使用本地编码支持中文路径）
    QByteArray localPath = modelPath.toLocal8Bit();

    // 所有形状模型（包括Generic Shape Model）使用相同的读取函数
    try {
        ReadShapeModel(localPath.constData(), &shapeModel_);
        modelLoaded_ = true;
        modelPath_ = modelPath;

        // 根据文件扩展名判断是否为通用形状模型
        // .gshm 扩展名表示通用形状模型（从XLD轮廓创建）
        // .shm 扩展名表示传统形状模型（从图像区域创建）
        QString suffix = fileInfo.suffix().toLower();
        if (suffix == "gshm") {
            useGenericModel_ = true;
            LOG_INFO(QString("成功加载通用形状模板(GSHM): %1").arg(modelPath));
        } else {
            // 对于.shm文件，尝试检测模型类型
            // 通过尝试获取通用模型参数来判断
            try {
                HTuple paramValue;
                GetGenericShapeModelParam(shapeModel_, "min_score", &paramValue);
                // 如果能成功获取参数，说明是通用模型
                useGenericModel_ = true;
                LOG_INFO(QString("成功加载通用形状模板(检测): %1").arg(modelPath));
            }
            catch (const HException&) {
                // 获取参数失败，说明是传统模型
                useGenericModel_ = false;
                LOG_INFO(QString("成功加载传统形状模板: %1").arg(modelPath));
            }
        }

        LOG_INFO(QString("模板类型: %1").arg(useGenericModel_ ? "通用(Generic)" : "传统(Traditional)"));

        // 获取模板参考点（关键！用于正确变换轮廓）
        try {
            if (useGenericModel_) {
                // 通用形状模型：从模型轮廓获取中心点
                HObject contoursObj;
                GetGenericShapeModelObject(&contoursObj, shapeModel_, "contours");
                HTuple countObj;
                CountObj(contoursObj, &countObj);
                if (countObj.I() > 0) {
                    // 获取轮廓边界框的中心作为参考点
                    HTuple minRow, maxRow, minCol, maxCol;
                    SmallestRectangle1Xld(HXLDCont(contoursObj), &minRow, &maxRow, &minCol, &maxCol);
                    modelRefRow_ = (minRow.D() + maxRow.D()) / 2.0;
                    modelRefCol_ = (minCol.D() + maxCol.D()) / 2.0;
                    modelWidth_ = static_cast<int>(maxCol.D() - minCol.D());
                    modelHeight_ = static_cast<int>(maxRow.D() - minRow.D());
                    LOG_INFO(QString("从轮廓获取参考点: (%.1f, %.1f), 尺寸: %2x%3")
                        .arg(modelRefRow_).arg(modelRefCol_).arg(modelWidth_).arg(modelHeight_));
                }
            } else {
                // 传统形状模型：使用GetShapeModelOrigin获取参考点偏移
                HTuple originRow, originCol;
                GetShapeModelOrigin(shapeModel_, &originRow, &originCol);
                // 传统模型的原点偏移是相对于模型中心的
                // 需要获取模型轮廓来计算实际参考点
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
                LOG_INFO(QString("传统模型原点偏移: (%.1f, %.1f)").arg(originRow.D()).arg(originCol.D()));
            }
        }
        catch (const HException& e) {
            LOG_WARNING(QString("获取模板参考点失败: %1, 使用默认值(0,0)").arg(e.ErrorMessage().Text()));
            modelRefRow_ = 0.0;
            modelRefCol_ = 0.0;
        }

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
        LOG_DEBUG(QString("准备保存模板: modelLoaded_=%1, useGenericModel_=%2, shapeModel_.Length()=%3")
            .arg(modelLoaded_).arg(useGenericModel_).arg(shapeModel_.Length()));

        if (!modelLoaded_) {
            LOG_ERROR("没有已加载的模板");
            return false;
        }

        if (shapeModel_.Length() == 0) {
            LOG_ERROR("模板句柄为空");
            return false;
        }

        // 保存模板（使用本地编码支持中文路径）
        QByteArray localPath = modelPath.toLocal8Bit();
        LOG_DEBUG(QString("保存路径: %1 -> %2").arg(modelPath).arg(QString::fromLocal8Bit(localPath)));

        // 所有形状模型（包括Generic Shape Model）使用相同的保存函数
        WriteShapeModel(shapeModel_, localPath.constData());
        LOG_INFO(QString("%1已保存: %2").arg(useGenericModel_ ? "通用形状模板" : "形状模板").arg(modelPath));

        modelPath_ = modelPath;
        return true;
    }
    catch (const HException& e) {
        LOG_ERROR(QString("保存形状模板失败: 错误码=%1, 消息=%2")
            .arg(e.ErrorCode()).arg(e.ErrorMessage().Text()));
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

        // 执行形状匹配（根据匹配类型选择API）
        HTuple row, col, angle, score;
        HTuple scale, scaleRow, scaleCol;  // 缩放结果

        // 如果使用通用形状模型（从XLD轮廓创建），使用FindGenericShapeModel
        if (useGenericModel_) {
            // 设置搜索参数（参考 adapt_shape_model_high_noise.hdev）
            // 参考代码使用 rad(-50) 到 rad(50)，即 ±50 度
            // 如果角度范围太小，扩展到 ±90 度
            double searchAngleStart = angleStart_;
            double searchAngleEnd = angleStart_ + angleExtent_;

            // 确保角度范围足够大（至少 ±45 度）
            if (angleExtent_ < M_PI / 2) {  // 如果范围小于 90 度
                searchAngleStart = -M_PI / 2;  // -90 度
                searchAngleEnd = M_PI / 2;     // +90 度
            }

            SetGenericShapeModelParam(shapeModel_, "angle_start", searchAngleStart);
            SetGenericShapeModelParam(shapeModel_, "angle_end", searchAngleEnd);
            SetGenericShapeModelParam(shapeModel_, "num_matches", numMatches_ > 0 ? numMatches_ : 10);
            // 使用用户设置的 min_score（不再强制限制为 0.5）
            SetGenericShapeModelParam(shapeModel_, "min_score", minScore_);
            // 使用用户设置的 max_overlap 和 greediness
            SetGenericShapeModelParam(shapeModel_, "max_overlap", maxOverlap_);
            SetGenericShapeModelParam(shapeModel_, "greediness", greediness_);

            LOG_DEBUG(QString("Generic匹配参数: 角度[%1, %2]度, minScore=%3, maxOverlap=%4, greediness=%5, numMatches=%6")
                .arg(searchAngleStart * 180.0 / M_PI, 0, 'f', 1)
                .arg(searchAngleEnd * 180.0 / M_PI, 0, 'f', 1)
                .arg(minScore_, 0, 'f', 2)
                .arg(maxOverlap_, 0, 'f', 2)
                .arg(greediness_, 0, 'f', 2)
                .arg(numMatches_ > 0 ? numMatches_ : 10));

            HTuple matchResultID, numMatchResult;
            FindGenericShapeModel(hImg, shapeModel_, &matchResultID, &numMatchResult);

            LOG_DEBUG(QString("FindGenericShapeModel 返回: numMatchResult=%1").arg(numMatchResult.I()));

            // 从结果中提取位置信息
            if (numMatchResult.I() > 0) {
                // 直接从匹配结果获取变换后的轮廓（参考代码关键步骤）
                try {
                    HObject contoursObj;
                    GetGenericShapeModelResultObject(&contoursObj, matchResultID, "all", "contours");
                    lastMatchContours_ = HXLDCont(contoursObj);
                    LOG_INFO(QString("从匹配结果直接获取轮廓: %1个对象").arg(lastMatchContours_.CountObj()));
                }
                catch (const HException& e) {
                    LOG_WARNING(QString("获取匹配轮廓失败: %1").arg(e.ErrorMessage().Text()));
                }

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

                    LOG_DEBUG(QString("匹配结果[%1]: pos=(%2, %3), angle=%4°, score=%5")
                        .arg(i)
                        .arg(c.D(), 0, 'f', 1)
                        .arg(r.D(), 0, 'f', 1)
                        .arg(a.D() * 180.0 / M_PI, 0, 'f', 2)
                        .arg(s.D(), 0, 'f', 3));
                }
            }
        } else {
            // 使用传统形状模型API
            switch (matchType_) {
            case Standard:
                // 标准匹配（仅旋转）
                FindShapeModel(hImg, shapeModel_,
                              angleStart_, angleExtent_,
                              minScore_, numMatches_,
                              0.5,  // max_overlap
                              "least_squares",  // sub_pixel
                              0,  // num_levels
                              0.9,  // greediness
                              &row, &col, &angle, &score);
                break;

            case Scaled:
                // 缩放匹配（旋转+各向同性缩放）
                FindScaledShapeModel(hImg, shapeModel_,
                                    angleStart_, angleExtent_,
                                    scaleMin_, scaleMax_,
                                    minScore_, numMatches_,
                                    0.5,  // max_overlap
                                    "least_squares",  // sub_pixel
                                    0,  // num_levels
                                    0.9,  // greediness
                                    &row, &col, &angle, &scale, &score);
                break;

            case Anisotropic:
                // 仿射匹配（旋转+各向异性缩放）
                FindAnisoShapeModel(hImg, shapeModel_,
                                   angleStart_, angleExtent_,
                                   scaleMin_, scaleMax_,  // 行方向缩放范围
                                   scaleMin_, scaleMax_,  // 列方向缩放范围
                                   minScore_, numMatches_,
                                   0.5,  // max_overlap
                                   "least_squares",  // sub_pixel
                                   0,  // num_levels
                                   0.9,  // greediness
                                   &row, &col, &angle, &scaleRow, &scaleCol, &score);
                break;
            }
        }

        int numFound = row.Length();

        LOG_INFO(QString("形状匹配完成: 找到 %1 个匹配").arg(numFound));

        // 如果启用了校验点验证，筛选通过验证的匹配
        HTuple validRow, validCol, validAngle, validScore;
        int validCount = 0;

        if (checkPointsEnabled_ && !checkPoints_.empty()) {
            LOG_INFO("开始校验点验证...");
            for (int i = 0; i < numFound; i++) {
                std::vector<int> failedPoints;
                if (validateCheckPoints(input, row[i].D(), col[i].D(), angle[i].D(), &failedPoints)) {
                    validRow = validRow.TupleConcat(row[i]);
                    validCol = validCol.TupleConcat(col[i]);
                    validAngle = validAngle.TupleConcat(angle[i]);
                    validScore = validScore.TupleConcat(score[i]);
                    validCount++;
                    LOG_DEBUG(QString("匹配 %1 通过校验点验证").arg(i));
                } else {
                    LOG_DEBUG(QString("匹配 %1 未通过校验点验证，失败校验点数: %2")
                        .arg(i).arg(failedPoints.size()));
                }
            }
            LOG_INFO(QString("校验点验证完成: %1/%2 个匹配通过").arg(validCount).arg(numFound));
        } else {
            // 不使用校验点验证，保留所有匹配
            validRow = row;
            validCol = col;
            validAngle = angle;
            validScore = score;
            validCount = numFound;
        }

        // 保存结果
        output.setValue("num_matches", validCount);
        output.setValue("num_matches_before_validation", numFound);

        if (validCount > 0) {
            // 保存第一个匹配的详细信息
            output.setValue("row", validRow[0].D());
            output.setValue("col", validCol[0].D());
            output.setValue("angle", validAngle[0].D());
            output.setValue("score", validScore[0].D());

            LOG_INFO(QString("最佳匹配: 位置(%.1f, %.1f), 角度%.2f°, 得分%.3f")
                .arg(validRow[0].D()).arg(validCol[0].D())
                .arg(validAngle[0].D() * 180.0 / 3.14159)
                .arg(validScore[0].D()));

            if (useXLDDisplay_) {
                // 使用XLD轮廓显示（非破坏性）
                HXLDCont contours = createMatchContours(validRow, validCol, validAngle);
                XLDContourPtr xldPtr = QSharedPointer<XLDContourWrapper>::create(contours);
                output.setDisplayObject("match_contours", QVariant::fromValue(
                    qSharedPointerCast<HalconObjectWrapper>(xldPtr)));
                // 在原图上绘制中心参考点和坐标信息（XLD模式也需要显示）
                cv::Mat resultImg = input->mat().clone();
                drawMatchResults(resultImg, validRow, validCol, validAngle, validScore);
                output.outputImage = std::make_shared<Base::ImageData>(resultImg);
            } else {
                // 绘制结果到输出图像（破坏性）
                cv::Mat resultImg = input->mat().clone();
                drawMatchResults(resultImg, validRow, validCol, validAngle, validScore);
                output.outputImage = std::make_shared<Base::ImageData>(resultImg);
            }

            output.success = true;
        }
        else {
            if (numFound > 0 && validCount == 0) {
                LOG_WARNING("所有匹配都未通过校验点验证");
            } else {
                LOG_WARNING("未找到匹配的形状");
            }
            output.success = true;  // 没找到也算成功完成
            output.outputImage = input;  // 返回原图
        }

        QString statusText = QString("找到%1个匹配").arg(validCount);
        if (checkPointsEnabled_ && numFound != validCount) {
            statusText += QString(" (校验前%1个)").arg(numFound);
        }
        setStatusText(statusText);

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

        // 绘制中心参考点十字（红色，更醒目）
        int crossSize = 12;
        // 水平线
        cv::line(image, cv::Point(center.x - crossSize, center.y),
                cv::Point(center.x + crossSize, center.y),
                cv::Scalar(0, 0, 255), 2, cv::LINE_AA);
        // 垂直线
        cv::line(image, cv::Point(center.x, center.y - crossSize),
                cv::Point(center.x, center.y + crossSize),
                cv::Scalar(0, 0, 255), 2, cv::LINE_AA);
        // 中心圆点
        cv::circle(image, center, 4, cv::Scalar(0, 0, 255), -1, cv::LINE_AA);
        cv::circle(image, center, 4, cv::Scalar(255, 255, 255), 1, cv::LINE_AA);

        // 绘制序号和坐标信息
        // 第一行：序号（黄色背景增强可读性）
        QString textLine1 = QString("#%1").arg(i + 1);
        cv::putText(image, textLine1.toStdString(),
                   cv::Point(static_cast<int>(c) + 20, static_cast<int>(r) - 25),
                   cv::FONT_HERSHEY_SIMPLEX, 0.55,
                   cv::Scalar(0, 255, 255), 2, cv::LINE_AA);

        // 第二行：坐标 (X, Y)（绿色）
        QString textLine2 = QString("(%1, %2)").arg(c, 0, 'f', 1).arg(r, 0, 'f', 1);
        cv::putText(image, textLine2.toStdString(),
                   cv::Point(static_cast<int>(c) + 20, static_cast<int>(r) - 5),
                   cv::FONT_HERSHEY_SIMPLEX, 0.45,
                   cv::Scalar(0, 255, 0), 1, cv::LINE_AA);

        // 第三行：角度和分数（青黄色）
        QString textLine3 = QString("A:%.1f S:%.2f").arg(a * 180.0 / M_PI).arg(s);
        cv::putText(image, textLine3.toStdString(),
                   cv::Point(static_cast<int>(c) + 20, static_cast<int>(r) + 15),
                   cv::FONT_HERSHEY_SIMPLEX, 0.4,
                   cv::Scalar(255, 255, 0), 1, cv::LINE_AA);
    }
}

HXLDCont ShapeMatchTool::createMatchContours(const HTuple& row, const HTuple& col,
                                             const HTuple& angle)
{
    HXLDCont allContours;

    try {
        // 对于 Generic Shape Model，优先使用从匹配结果直接获取的轮廓
        // 这是参考代码 adapt_shape_model_high_noise.hdev 的关键步骤
        if (useGenericModel_) {
            try {
                if (lastMatchContours_.IsInitialized() && lastMatchContours_.CountObj() > 0) {
                    LOG_INFO(QString("使用从匹配结果直接获取的轮廓: %1个对象").arg(lastMatchContours_.CountObj()));
                    return lastMatchContours_;
                }
            }
            catch (...) {
                LOG_DEBUG("lastMatchContours_ 无效，回退到手动变换方式");
            }
        }

        // 获取模板轮廓（优先使用标准轮廓）
        HXLDCont modelContours;

        // 安全地检查标准轮廓（先检查是否初始化）
        Hlong standardContourCount = 0;
        bool hasStandardContour = false;
        try {
            if (standardContour_.IsInitialized()) {
                standardContourCount = standardContour_.CountObj();
                hasStandardContour = (standardContourCount > 0);
            }
        }
        catch (...) {
            // standardContour_ 未正确初始化，忽略
            hasStandardContour = false;
        }

        LOG_INFO(QString("createMatchContours: standardContourCount=%1, useGenericModel_=%2")
            .arg(standardContourCount).arg(useGenericModel_ ? "true" : "false"));

        if (hasStandardContour) {
            modelContours = standardContour_;
            LOG_INFO(QString("使用标准轮廓: %1个对象").arg(standardContourCount));
        } else {
            // 根据模型类型选择正确的API获取轮廓
            if (useGenericModel_) {
                // 通用形状模型使用 GetGenericShapeModelObject
                LOG_DEBUG("尝试从通用形状模型获取轮廓...");
                try {
                    HObject contoursObj;
                    GetGenericShapeModelObject(&contoursObj, shapeModel_, "contours");
                    HTuple countObj;
                    CountObj(contoursObj, &countObj);
                    LOG_DEBUG(QString("GetGenericShapeModelObject返回: %1个对象").arg(countObj.I()));
                    if (countObj.I() > 0) {
                        modelContours = HXLDCont(contoursObj);
                    }
                }
                catch (const HException& e) {
                    LOG_WARNING(QString("GetGenericShapeModelObject失败: %1").arg(e.ErrorMessage().Text()));
                }

                // 如果仍然没有轮廓，尝试使用GetShapeModelContours作为备用
                if (modelContours.CountObj() == 0) {
                    LOG_DEBUG("尝试使用GetShapeModelContours作为备用...");
                    try {
                        GetShapeModelContours(&modelContours, shapeModel_, 1);
                        LOG_DEBUG(QString("GetShapeModelContours返回: %1个对象").arg(modelContours.CountObj()));
                    }
                    catch (const HException& e) {
                        LOG_WARNING(QString("GetShapeModelContours也失败: %1").arg(e.ErrorMessage().Text()));
                    }
                }

                // 如果两种方法都失败，生成一个简单的十字标记
                if (modelContours.CountObj() == 0) {
                    LOG_WARNING("无法获取模型轮廓，生成简单标记...");
                    // 生成一个简单的十字标记（基于模型参考点）
                    double size = 20.0;  // 十字大小
                    HTuple rows, cols;
                    rows.Append(-size); cols.Append(0);
                    rows.Append(size);  cols.Append(0);
                    HXLDCont hLine;
                    GenContourPolygonXld(&hLine, rows, cols);
                    rows.Clear(); cols.Clear();
                    rows.Append(0); cols.Append(-size);
                    rows.Append(0); cols.Append(size);
                    HXLDCont vLine;
                    GenContourPolygonXld(&vLine, rows, cols);
                    modelContours = hLine.ConcatObj(vLine);
                }
            } else {
                // 传统形状模型使用 GetShapeModelContours
                LOG_INFO("使用传统形状模型获取轮廓...");
                try {
                    GetShapeModelContours(&modelContours, shapeModel_, 1);
                    LOG_INFO(QString("GetShapeModelContours返回: %1个对象").arg(modelContours.CountObj()));
                }
                catch (const HException& e) {
                    LOG_WARNING(QString("GetShapeModelContours失败: %1, 尝试备用方法...").arg(e.ErrorMessage().Text()));
                    // 可能是通用模型被错误识别为传统模型，尝试用通用模型API
                    try {
                        HObject contoursObj;
                        GetGenericShapeModelObject(&contoursObj, shapeModel_, "contours");
                        HTuple countObj;
                        CountObj(contoursObj, &countObj);
                        if (countObj.I() > 0) {
                            modelContours = HXLDCont(contoursObj);
                            LOG_INFO(QString("备用方法成功: %1个对象").arg(countObj.I()));
                        }
                    }
                    catch (const HException&) {
                        // 两种方法都失败
                    }
                }

                // 如果仍然没有轮廓，生成简单标记
                if (modelContours.CountObj() == 0) {
                    LOG_WARNING("传统模型也无法获取轮廓，生成简单标记...");
                    double size = 20.0;
                    HTuple rows, cols;
                    rows.Append(-size); cols.Append(0);
                    rows.Append(size);  cols.Append(0);
                    HXLDCont hLine;
                    GenContourPolygonXld(&hLine, rows, cols);
                    rows.Clear(); cols.Clear();
                    rows.Append(0); cols.Append(-size);
                    rows.Append(0); cols.Append(size);
                    HXLDCont vLine;
                    GenContourPolygonXld(&vLine, rows, cols);
                    modelContours = hLine.ConcatObj(vLine);
                }
            }
            // 应用轮廓过滤（使模板轮廓更干净）
            if (contourFilterEnabled_ && modelContours.CountObj() > 0) {
                modelContours = filterContours(modelContours);
            }
        }

        // 检查轮廓是否有效
        LOG_INFO(QString("获取到的轮廓数量: %1").arg(modelContours.CountObj()));
        if (modelContours.CountObj() == 0) {
            LOG_WARNING("模型轮廓为空，无法创建匹配轮廓");
            return allContours;
        }

        int numMatches = row.Length();
        LOG_INFO(QString("开始变换 %1 个匹配结果的轮廓").arg(numMatches));

        for (int i = 0; i < numMatches; i++) {
            try {
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
            catch (const HException& e) {
                LOG_WARNING(QString("变换第 %1 个轮廓失败: %2").arg(i).arg(e.ErrorMessage().Text()));
            }
        }
        LOG_INFO(QString("轮廓变换完成，结果轮廓数量: %1").arg(allContours.CountObj()));
    }
    catch (const HException& e) {
        LOG_ERROR(QString("创建匹配轮廓失败: %1").arg(e.ErrorMessage().Text()));
    }

    return allContours;
}

HXLDCont ShapeMatchTool::getModelContours() const
{
    // 如果有标准轮廓，优先返回（更干净）
    try {
        if (standardContour_.IsInitialized() && standardContour_.CountObj() > 0) {
            return standardContour_;
        }
    }
    catch (...) {
        // standardContour_ 未正确初始化，忽略
    }

    HXLDCont modelContours;
    if (!modelLoaded_ || shapeModel_.Length() == 0) {
        return modelContours;
    }

    try {
        // 根据模型类型选择正确的API获取轮廓
        if (useGenericModel_) {
            // 通用形状模型使用 GetGenericShapeModelObject
            try {
                HObject contoursObj;
                GetGenericShapeModelObject(&contoursObj, shapeModel_, "contours");
                HTuple countObj;
                CountObj(contoursObj, &countObj);
                if (countObj.I() > 0) {
                    modelContours = HXLDCont(contoursObj);
                }
            }
            catch (const HException&) {
                // 忽略，尝试备用方法
            }

            // 备用：尝试使用GetShapeModelContours
            if (modelContours.CountObj() == 0) {
                try {
                    GetShapeModelContours(&modelContours, shapeModel_, 1);
                }
                catch (const HException&) {
                    // 忽略
                }
            }

            // 最后备用：生成简单十字标记
            if (modelContours.CountObj() == 0) {
                double size = 20.0;
                HTuple rows, cols;
                rows.Append(-size); cols.Append(0);
                rows.Append(size);  cols.Append(0);
                HXLDCont hLine;
                GenContourPolygonXld(&hLine, rows, cols);
                rows.Clear(); cols.Clear();
                rows.Append(0); cols.Append(-size);
                rows.Append(0); cols.Append(size);
                HXLDCont vLine;
                GenContourPolygonXld(&vLine, rows, cols);
                modelContours = hLine.ConcatObj(vLine);
            }
        } else {
            // 传统形状模型使用 GetShapeModelContours
            try {
                GetShapeModelContours(&modelContours, shapeModel_, 1);
            }
            catch (const HException&) {
                // 尝试备用方法
                try {
                    HObject contoursObj;
                    GetGenericShapeModelObject(&contoursObj, shapeModel_, "contours");
                    HTuple countObj;
                    CountObj(contoursObj, &countObj);
                    if (countObj.I() > 0) {
                        modelContours = HXLDCont(contoursObj);
                    }
                }
                catch (const HException&) {
                    // 两种方法都失败
                }
            }

            // 如果仍然没有轮廓，生成简单标记
            if (modelContours.CountObj() == 0) {
                double size = 20.0;
                HTuple rows, cols;
                rows.Append(-size); cols.Append(0);
                rows.Append(size);  cols.Append(0);
                HXLDCont hLine;
                GenContourPolygonXld(&hLine, rows, cols);
                rows.Clear(); cols.Clear();
                rows.Append(0); cols.Append(-size);
                rows.Append(0); cols.Append(size);
                HXLDCont vLine;
                GenContourPolygonXld(&vLine, rows, cols);
                modelContours = hLine.ConcatObj(vLine);
            }
        }

        // 应用轮廓过滤
        if (contourFilterEnabled_ && modelContours.CountObj() > 0) {
            modelContours = filterContours(modelContours);
        }
    }
    catch (const HException& e) {
        LOG_ERROR(QString("获取模型轮廓失败: %1").arg(e.ErrorMessage().Text()));
    }

    return modelContours;
}

HXLDCont ShapeMatchTool::filterContours(const HXLDCont& contours) const
{
    HXLDCont result = contours;

    try {
        HTuple numContours;
        CountObj(result, &numContours);
        LOG_DEBUG(QString("轮廓过滤前: %1 条轮廓").arg(numContours.I()));

        if (numContours.I() == 0) {
            return result;
        }

        // 1. 首先按长度过滤掉非常短的轮廓（噪声）
        {
            HXLDCont preFiltered;
            SelectContoursXld(result, &preFiltered,
                             "contour_length",
                             5.0,      // 先过滤掉小于5像素的碎片
                             1000000,
                             0, 0);
            result = preFiltered;
            CountObj(result, &numContours);
            LOG_DEBUG(QString("预过滤后: %1 条轮廓").arg(numContours.I()));
        }

        // 2. 连接共线轮廓（共线的边缘片段合并成连续轮廓）
        // 多次迭代以连接更多轮廓
        if (unionCollinearEnabled_) {
            for (int iter = 0; iter < 3; iter++) {
                HXLDCont unionedCollinear;
                UnionCollinearContoursXld(result, &unionedCollinear,
                                          maxDistCollinear_,   // MaxDistAbs: 端点最大距离
                                          0.2,                 // MaxDistRel: 相对距离
                                          2.0,                 // MaxShift: 最大垂直偏移（增大）
                                          45,                  // MaxAngle: 最大角度差（度，增大到45）
                                          "attr_keep");
                result = unionedCollinear;
            }
            CountObj(result, &numContours);
            LOG_DEBUG(QString("连接共线轮廓后: %1 条轮廓").arg(numContours.I()));
        }

        // 3. 连接相邻轮廓（端点靠近的轮廓连接起来）
        // 多次迭代
        if (unionAdjacentEnabled_) {
            for (int iter = 0; iter < 3; iter++) {
                HXLDCont unionedAdjacent;
                UnionAdjacentContoursXld(result, &unionedAdjacent,
                                         maxDistAdjacent_,     // MaxDistAbs: 端点最大距离
                                         0.2,                  // MaxDistRel: 相对距离
                                         "attr_keep");
                result = unionedAdjacent;
            }
            CountObj(result, &numContours);
            LOG_DEBUG(QString("连接相邻轮廓后: %1 条轮廓").arg(numContours.I()));
        }

        // 4. 最终按长度过滤短轮廓
        if (minContourLength_ > 0) {
            HXLDCont filteredContours;
            SelectContoursXld(result, &filteredContours,
                             "contour_length",        // Feature: 轮廓长度
                             minContourLength_,       // Min: 最小长度
                             1000000,                 // Max: 最大长度
                             0, 0);
            result = filteredContours;
            CountObj(result, &numContours);
            LOG_DEBUG(QString("过滤短轮廓后: %1 条轮廓").arg(numContours.I()));
        }

        LOG_INFO(QString("轮廓过滤完成: 保留 %1 条轮廓").arg(numContours.I()));
    }
    catch (const HException& e) {
        LOG_ERROR(QString("轮廓过滤失败: %1").arg(e.ErrorMessage().Text()));
        return contours;  // 失败时返回原始轮廓
    }

    return result;
}

HXLDCont ShapeMatchTool::generateCrossContour(double armLength, double armWidth,
                                               double centerRow, double centerCol)
{
    HXLDCont crossContour;

    try {
        // 如果中心点为默认值(0,0)，使用256,256作为中心
        // 这样生成的轮廓可以直接用于极性图像（512x512）
        if (centerRow == 0 && centerCol == 0) {
            centerRow = 256.0;
            centerCol = 256.0;
        }

        double halfArm = armLength / 2.0;
        double halfWidth = armWidth / 2.0;

        // 生成十字形轮廓（由4条线段组成外轮廓）
        // 十字形状：
        //       |  |
        //   ----+  +----
        //   |          |
        //   ----+  +----
        //       |  |

        // 定义十字形状的12个顶点（顺时针）
        HTuple rows, cols;

        // 上臂顶部左角
        rows[0] = centerRow - halfArm;   cols[0] = centerCol - halfWidth;
        // 上臂顶部右角
        rows[1] = centerRow - halfArm;   cols[1] = centerCol + halfWidth;
        // 右臂连接点上
        rows[2] = centerRow - halfWidth; cols[2] = centerCol + halfWidth;
        // 右臂右上角
        rows[3] = centerRow - halfWidth; cols[3] = centerCol + halfArm;
        // 右臂右下角
        rows[4] = centerRow + halfWidth; cols[4] = centerCol + halfArm;
        // 右臂连接点下
        rows[5] = centerRow + halfWidth; cols[5] = centerCol + halfWidth;
        // 下臂底部右角
        rows[6] = centerRow + halfArm;   cols[6] = centerCol + halfWidth;
        // 下臂底部左角
        rows[7] = centerRow + halfArm;   cols[7] = centerCol - halfWidth;
        // 左臂连接点下
        rows[8] = centerRow + halfWidth; cols[8] = centerCol - halfWidth;
        // 左臂左下角
        rows[9] = centerRow + halfWidth; cols[9] = centerCol - halfArm;
        // 左臂左上角
        rows[10] = centerRow - halfWidth; cols[10] = centerCol - halfArm;
        // 左臂连接点上
        rows[11] = centerRow - halfWidth; cols[11] = centerCol - halfWidth;
        // 闭合回起点
        rows[12] = centerRow - halfArm;   cols[12] = centerCol - halfWidth;

        // 生成XLD轮廓
        GenContourPolygonXld(&crossContour, rows, cols);

        LOG_INFO(QString("生成标准十字轮廓: 臂长=%.1f, 臂宽=%.1f, 中心=(%.1f, %.1f)")
            .arg(armLength).arg(armWidth).arg(centerRow).arg(centerCol));
    }
    catch (const HException& e) {
        LOG_ERROR(QString("生成十字轮廓失败: %1").arg(e.ErrorMessage().Text()));
    }

    return crossContour;
}

HXLDCont ShapeMatchTool::generateTShapeContour(double width, double height, double thickness,
                                                double centerRow, double centerCol)
{
    HXLDCont contour;

    try {
        if (centerRow == 0 && centerCol == 0) {
            centerRow = 256.0;
            centerCol = 256.0;
        }

        double halfWidth = width / 2.0;
        double halfThick = thickness / 2.0;
        double topHeight = thickness;  // T字顶部高度
        double stemHeight = height - topHeight;  // T字茎部高度

        // T字形状：
        //   ___________
        //  |_____  ____|
        //        | |
        //        |_|

        HTuple rows, cols;

        // 顶部左上角
        rows[0] = centerRow - height/2;              cols[0] = centerCol - halfWidth;
        // 顶部右上角
        rows[1] = centerRow - height/2;              cols[1] = centerCol + halfWidth;
        // 顶部右下角
        rows[2] = centerRow - height/2 + topHeight;  cols[2] = centerCol + halfWidth;
        // 茎部右上角
        rows[3] = centerRow - height/2 + topHeight;  cols[3] = centerCol + halfThick;
        // 茎部右下角
        rows[4] = centerRow + height/2;              cols[4] = centerCol + halfThick;
        // 茎部左下角
        rows[5] = centerRow + height/2;              cols[5] = centerCol - halfThick;
        // 茎部左上角
        rows[6] = centerRow - height/2 + topHeight;  cols[6] = centerCol - halfThick;
        // 顶部左下角
        rows[7] = centerRow - height/2 + topHeight;  cols[7] = centerCol - halfWidth;
        // 闭合
        rows[8] = centerRow - height/2;              cols[8] = centerCol - halfWidth;

        GenContourPolygonXld(&contour, rows, cols);

        LOG_INFO(QString("生成T字轮廓: 宽=%1, 高=%2, 厚=%3").arg(width).arg(height).arg(thickness));
    }
    catch (const HException& e) {
        LOG_ERROR(QString("生成T字轮廓失败: %1").arg(e.ErrorMessage().Text()));
    }

    return contour;
}

HXLDCont ShapeMatchTool::generateLShapeContour(double width, double height, double thickness,
                                                double centerRow, double centerCol)
{
    HXLDCont contour;

    try {
        if (centerRow == 0 && centerCol == 0) {
            centerRow = 256.0;
            centerCol = 256.0;
        }

        // L字形状（左下角为拐点）：
        //   |  |
        //   |  |_____
        //   |________|

        HTuple rows, cols;

        // 左上角
        rows[0] = centerRow - height/2;           cols[0] = centerCol - width/2;
        // 左上内角
        rows[1] = centerRow - height/2;           cols[1] = centerCol - width/2 + thickness;
        // 拐点内上
        rows[2] = centerRow + height/2 - thickness; cols[2] = centerCol - width/2 + thickness;
        // 右上内角
        rows[3] = centerRow + height/2 - thickness; cols[3] = centerCol + width/2;
        // 右上外角
        rows[4] = centerRow + height/2;           cols[4] = centerCol + width/2;
        // 右下外角（底部右）
        rows[5] = centerRow + height/2;           cols[5] = centerCol - width/2;
        // 闭合
        rows[6] = centerRow - height/2;           cols[6] = centerCol - width/2;

        GenContourPolygonXld(&contour, rows, cols);

        LOG_INFO(QString("生成L字轮廓: 宽=%1, 高=%2, 厚=%3").arg(width).arg(height).arg(thickness));
    }
    catch (const HException& e) {
        LOG_ERROR(QString("生成L字轮廓失败: %1").arg(e.ErrorMessage().Text()));
    }

    return contour;
}

HXLDCont ShapeMatchTool::generateCircleContour(double radius,
                                                double centerRow, double centerCol)
{
    HXLDCont contour;

    try {
        if (centerRow == 0 && centerCol == 0) {
            centerRow = 256.0;
            centerCol = 256.0;
        }

        // 使用Halcon的圆形XLD生成
        GenCircleContourXld(&contour, centerRow, centerCol, radius, 0, 2 * M_PI, "positive", 1.0);

        LOG_INFO(QString("生成圆形轮廓: 半径=%1, 中心=(%2, %3)").arg(radius).arg(centerRow).arg(centerCol));
    }
    catch (const HException& e) {
        LOG_ERROR(QString("生成圆形轮廓失败: %1").arg(e.ErrorMessage().Text()));
    }

    return contour;
}

HXLDCont ShapeMatchTool::generateRingContour(double outerRadius, double innerRadius,
                                              double centerRow, double centerCol)
{
    HXLDCont contour;

    try {
        if (centerRow == 0 && centerCol == 0) {
            centerRow = 256.0;
            centerCol = 256.0;
        }

        // 生成外圆和内圆
        HXLDCont outerCircle, innerCircle;
        GenCircleContourXld(&outerCircle, centerRow, centerCol, outerRadius, 0, 2 * M_PI, "positive", 1.0);
        GenCircleContourXld(&innerCircle, centerRow, centerCol, innerRadius, 0, 2 * M_PI, "positive", 1.0);

        // 合并两个圆
        ConcatObj(outerCircle, innerCircle, &contour);

        LOG_INFO(QString("生成圆环轮廓: 外径=%1, 内径=%2").arg(outerRadius).arg(innerRadius));
    }
    catch (const HException& e) {
        LOG_ERROR(QString("生成圆环轮廓失败: %1").arg(e.ErrorMessage().Text()));
    }

    return contour;
}

HXLDCont ShapeMatchTool::generateRectangleContour(double width, double height,
                                                   double centerRow, double centerCol)
{
    HXLDCont contour;

    try {
        if (centerRow == 0 && centerCol == 0) {
            centerRow = 256.0;
            centerCol = 256.0;
        }

        double halfW = width / 2.0;
        double halfH = height / 2.0;

        HTuple rows, cols;
        rows[0] = centerRow - halfH; cols[0] = centerCol - halfW;
        rows[1] = centerRow - halfH; cols[1] = centerCol + halfW;
        rows[2] = centerRow + halfH; cols[2] = centerCol + halfW;
        rows[3] = centerRow + halfH; cols[3] = centerCol - halfW;
        rows[4] = centerRow - halfH; cols[4] = centerCol - halfW;

        GenContourPolygonXld(&contour, rows, cols);

        LOG_INFO(QString("生成矩形轮廓: 宽=%1, 高=%2").arg(width).arg(height));
    }
    catch (const HException& e) {
        LOG_ERROR(QString("生成矩形轮廓失败: %1").arg(e.ErrorMessage().Text()));
    }

    return contour;
}

HXLDCont ShapeMatchTool::generateTriangleContour(double sideLength,
                                                  double centerRow, double centerCol)
{
    HXLDCont contour;

    try {
        if (centerRow == 0 && centerCol == 0) {
            centerRow = 256.0;
            centerCol = 256.0;
        }

        // 等边三角形，顶点朝上
        double h = sideLength * std::sqrt(3.0) / 2.0;  // 三角形高度
        double r = h * 2.0 / 3.0;  // 外接圆半径（重心到顶点）

        HTuple rows, cols;
        // 顶点
        rows[0] = centerRow - r;                    cols[0] = centerCol;
        // 右下角
        rows[1] = centerRow + h - r;                cols[1] = centerCol + sideLength / 2.0;
        // 左下角
        rows[2] = centerRow + h - r;                cols[2] = centerCol - sideLength / 2.0;
        // 闭合
        rows[3] = centerRow - r;                    cols[3] = centerCol;

        GenContourPolygonXld(&contour, rows, cols);

        LOG_INFO(QString("生成三角形轮廓: 边长=%1").arg(sideLength));
    }
    catch (const HException& e) {
        LOG_ERROR(QString("生成三角形轮廓失败: %1").arg(e.ErrorMessage().Text()));
    }

    return contour;
}

HXLDCont ShapeMatchTool::generateDiamondContour(double width, double height,
                                                 double centerRow, double centerCol)
{
    HXLDCont contour;

    try {
        if (centerRow == 0 && centerCol == 0) {
            centerRow = 256.0;
            centerCol = 256.0;
        }

        double halfW = width / 2.0;
        double halfH = height / 2.0;

        HTuple rows, cols;
        // 上顶点
        rows[0] = centerRow - halfH; cols[0] = centerCol;
        // 右顶点
        rows[1] = centerRow;         cols[1] = centerCol + halfW;
        // 下顶点
        rows[2] = centerRow + halfH; cols[2] = centerCol;
        // 左顶点
        rows[3] = centerRow;         cols[3] = centerCol - halfW;
        // 闭合
        rows[4] = centerRow - halfH; cols[4] = centerCol;

        GenContourPolygonXld(&contour, rows, cols);

        LOG_INFO(QString("生成菱形轮廓: 宽=%1, 高=%2").arg(width).arg(height));
    }
    catch (const HException& e) {
        LOG_ERROR(QString("生成菱形轮廓失败: %1").arg(e.ErrorMessage().Text()));
    }

    return contour;
}

HXLDCont ShapeMatchTool::generateArrowContour(double length, double headWidth, double shaftWidth,
                                               double centerRow, double centerCol)
{
    HXLDCont contour;

    try {
        if (centerRow == 0 && centerCol == 0) {
            centerRow = 256.0;
            centerCol = 256.0;
        }

        double halfLen = length / 2.0;
        double halfHead = headWidth / 2.0;
        double halfShaft = shaftWidth / 2.0;
        double headLen = headWidth * 0.6;  // 箭头头部长度

        // 箭头指向右边
        //        /|
        //   ____/ |
        //  |      >
        //  |____\ |
        //        \|

        HTuple rows, cols;
        // 尾部左上
        rows[0] = centerRow - halfShaft;  cols[0] = centerCol - halfLen;
        // 箭头根部上
        rows[1] = centerRow - halfShaft;  cols[1] = centerCol + halfLen - headLen;
        // 箭头翼上
        rows[2] = centerRow - halfHead;   cols[2] = centerCol + halfLen - headLen;
        // 箭头尖端
        rows[3] = centerRow;              cols[3] = centerCol + halfLen;
        // 箭头翼下
        rows[4] = centerRow + halfHead;   cols[4] = centerCol + halfLen - headLen;
        // 箭头根部下
        rows[5] = centerRow + halfShaft;  cols[5] = centerCol + halfLen - headLen;
        // 尾部左下
        rows[6] = centerRow + halfShaft;  cols[6] = centerCol - halfLen;
        // 闭合
        rows[7] = centerRow - halfShaft;  cols[7] = centerCol - halfLen;

        GenContourPolygonXld(&contour, rows, cols);

        LOG_INFO(QString("生成箭头轮廓: 长=%1, 头宽=%2, 杆宽=%3").arg(length).arg(headWidth).arg(shaftWidth));
    }
    catch (const HException& e) {
        LOG_ERROR(QString("生成箭头轮廓失败: %1").arg(e.ErrorMessage().Text()));
    }

    return contour;
}

HXLDCont ShapeMatchTool::generateMarkContour(const MarkShapeParams& params)
{
    switch (params.type) {
        case MarkShapeType::Cross:
            return generateCrossContour(params.size1, params.size2, params.centerRow, params.centerCol);
        case MarkShapeType::TShape:
            return generateTShapeContour(params.size1, params.size2, params.size3 > 0 ? params.size3 : params.size2,
                                         params.centerRow, params.centerCol);
        case MarkShapeType::LShape:
            return generateLShapeContour(params.size1, params.size2, params.size3 > 0 ? params.size3 : params.size2 * 0.3,
                                         params.centerRow, params.centerCol);
        case MarkShapeType::Circle:
            return generateCircleContour(params.size1, params.centerRow, params.centerCol);
        case MarkShapeType::Ring:
            return generateRingContour(params.size1, params.size2, params.centerRow, params.centerCol);
        case MarkShapeType::Rectangle:
            return generateRectangleContour(params.size1, params.size2, params.centerRow, params.centerCol);
        case MarkShapeType::Triangle:
            return generateTriangleContour(params.size1, params.centerRow, params.centerCol);
        case MarkShapeType::Diamond:
            return generateDiamondContour(params.size1, params.size2 > 0 ? params.size2 : params.size1,
                                          params.centerRow, params.centerCol);
        case MarkShapeType::Arrow:
            return generateArrowContour(params.size1, params.size2, params.size3 > 0 ? params.size3 : params.size2 * 0.4,
                                        params.centerRow, params.centerCol);
        default:
            LOG_WARNING("未知的Mark形状类型，使用十字形");
            return generateCrossContour(params.size1, params.size2, params.centerRow, params.centerCol);
    }
}

bool ShapeMatchTool::trainModelFromContour(const HXLDCont& contour,
                                           const Base::ImageData::Ptr& highNoiseImage,
                                           int sampleRow1, int sampleCol1,
                                           int sampleRow2, int sampleCol2)
{
    try {
        if (contour.CountObj() == 0) {
            LOG_ERROR("训练轮廓为空");
            return false;
        }

        clearModel();

        // 使用通用形状模型API（支持从XLD轮廓创建）
        LOG_INFO("使用通用形状模型从XLD轮廓创建模板...");

        // 1. 创建通用形状模型
        CreateGenericShapeModel(&shapeModel_);

        // 2. 设置缩放范围（参考代码使用 0.75-1.25）
        double effectiveScaleMin = scaleMin_ < 0.75 ? scaleMin_ : 0.75;
        double effectiveScaleMax = scaleMax_ > 1.25 ? scaleMax_ : 1.25;
        SetGenericShapeModelParam(shapeModel_, "iso_scale_min", effectiveScaleMin);
        SetGenericShapeModelParam(shapeModel_, "iso_scale_max", effectiveScaleMax);
        LOG_INFO(QString("缩放范围: [%1, %2]").arg(effectiveScaleMin).arg(effectiveScaleMax));

        // 3. 使用轮廓训练模型
        TrainGenericShapeModel(contour, shapeModel_);
        LOG_INFO("模型训练完成");

        // 4. 创建极性图像并设置匹配度量
        // 参考代码使用固定 512x512 图像
        const int polaritySize = 512;

        // 获取轮廓边界
        HTuple minRow, maxRow, minCol, maxCol;
        SmallestRectangle1Xld(contour, &minRow, &maxRow, &minCol, &maxCol);

        // 将轮廓平移到极性图像中心
        double contourCenterRow = (minRow.D() + maxRow.D()) / 2.0;
        double contourCenterCol = (minCol.D() + maxCol.D()) / 2.0;
        double offsetRow = polaritySize / 2.0 - contourCenterRow;
        double offsetCol = polaritySize / 2.0 - contourCenterCol;

        LOG_DEBUG(QString("轮廓中心: (%.1f, %.1f), 偏移: (%.1f, %.1f)")
            .arg(contourCenterRow).arg(contourCenterCol).arg(offsetRow).arg(offsetCol));

        HTuple homMat2DTranslate;
        HomMat2dIdentity(&homMat2DTranslate);
        HomMat2dTranslate(homMat2DTranslate, offsetRow, offsetCol, &homMat2DTranslate);

        HXLDCont translatedContour;
        AffineTransContourXld(contour, &translatedContour, homMat2DTranslate);

        // 生成极性图像（黑色背景 + 白色轮廓，适用于"亮色目标在暗色背景上"）
        HImage polarityImage;
        GenImageConst(&polarityImage, "byte", polaritySize, polaritySize);

        HRegion fullRegion;
        GenRectangle1(&fullRegion, 0, 0, polaritySize - 1, polaritySize - 1);
        PaintRegion(fullRegion, polarityImage, &polarityImage, 0, "fill");  // 黑色背景

        // 在极性图像上绘制轮廓（白色，灰度值255）
        PaintXld(translatedContour, polarityImage, &polarityImage, 255);
        LOG_DEBUG("极性图像创建完成（黑色背景+白色轮廓，适用于亮色目标）");

        // 在极性图像上查找模型以获取单应性矩阵
        HTuple matchResultID, numMatches;
        FindGenericShapeModel(polarityImage, shapeModel_, &matchResultID, &numMatches);

        LOG_DEBUG(QString("极性图像匹配结果: %1 个").arg(numMatches.I()));

        if (numMatches.I() > 0) {
            HTuple homMat2D;
            GetGenericShapeModelResult(matchResultID, 0, "hom_mat_2d", &homMat2D);

            // 设置极性匹配度量（白色背景，黑色轮廓）
            SetShapeModelMetric(polarityImage, shapeModel_, homMat2D, "use_polarity");
            LOG_INFO("已设置极性匹配度量 (use_polarity)");
        } else {
            LOG_WARNING("极性图像上未找到匹配，跳过极性设置");
        }

        // 5. 如果提供了高噪声样本图像，使用它来估计最佳金字塔层级
        if (highNoiseImage && !highNoiseImage->isEmpty() &&
            sampleRow1 != sampleRow2 && sampleCol1 != sampleCol2) {

            LOG_INFO("使用高噪声样本图像估计金字塔层级...");

            HImage hImg = imageDataToHImage(highNoiseImage);

            HRegion sampleRegion;
            GenRectangle1(&sampleRegion, sampleRow1, sampleCol1, sampleRow2, sampleCol2);

            HImage reducedImage;
            ReduceDomain(hImg, sampleRegion, &reducedImage);

            // 设置高噪声样本
            SetGenericShapeModelObject(reducedImage, shapeModel_, "high_noise_sample");

            // 获取估计的最低金字塔层级
            HTuple pyramidLevelLowest;
            GetGenericShapeModelParam(shapeModel_, "pyramid_level_lowest", &pyramidLevelLowest);
            LOG_INFO(QString("估计的最低金字塔层级: %1").arg(pyramidLevelLowest.I()));
        }

        // 6. 设置搜索参数
        // 使用用户设置的角度范围
        double defaultAngleStart = angleStart_;
        double defaultAngleEnd = angleStart_ + angleExtent_;
        SetGenericShapeModelParam(shapeModel_, "angle_start", defaultAngleStart);
        SetGenericShapeModelParam(shapeModel_, "angle_end", defaultAngleEnd);
        SetGenericShapeModelParam(shapeModel_, "num_matches", numMatches_ > 0 ? numMatches_ : 5);
        // 使用用户设置的参数
        SetGenericShapeModelParam(shapeModel_, "min_score", minScore_);
        SetGenericShapeModelParam(shapeModel_, "max_overlap", maxOverlap_);
        SetGenericShapeModelParam(shapeModel_, "greediness", greediness_);

        LOG_INFO(QString("搜索参数设置完成: 角度[%1, %2]度, numMatches=%3, minScore=%4, greediness=%5")
            .arg(defaultAngleStart * 180.0 / M_PI, 0, 'f', 1)
            .arg(defaultAngleEnd * 180.0 / M_PI, 0, 'f', 1)
            .arg(numMatches_ > 0 ? numMatches_ : 5)
            .arg(minScore_, 0, 'f', 2)
            .arg(greediness_, 0, 'f', 2));

        // 保存标准轮廓用于显示
        standardContour_ = contour;
        useGenericModel_ = true;
        modelLoaded_ = true;

        // 计算模板尺寸
        modelWidth_ = static_cast<int>(maxCol.D() - minCol.D());
        modelHeight_ = static_cast<int>(maxRow.D() - minRow.D());
        modelRefRow_ = (minRow.D() + maxRow.D()) / 2.0;
        modelRefCol_ = (minCol.D() + maxCol.D()) / 2.0;

        LOG_INFO(QString("成功从XLD轮廓创建通用形状模板: %1x%2")
            .arg(modelWidth_).arg(modelHeight_));

        emit paramChanged();
        return true;
    }
    catch (const HException& e) {
        LOG_ERROR(QString("从XLD轮廓训练模板失败: %1").arg(e.ErrorMessage().Text()));
        return false;
    }
}
#endif

QJsonObject ShapeMatchTool::serializeParams() const
{
    QJsonObject json;
    json["model_path"] = modelPath_;
    json["match_type"] = static_cast<int>(matchType_);
    json["min_score"] = minScore_;
    json["num_matches"] = numMatches_;
    json["angle_start"] = angleStart_;
    json["angle_extent"] = angleExtent_;
    json["min_contrast"] = minContrast_;
    json["scale_min"] = scaleMin_;
    json["scale_max"] = scaleMax_;
    json["scale_step_row"] = scaleStepRow_;
    json["scale_step_col"] = scaleStepCol_;
    json["use_xld_display"] = useXLDDisplay_;

    // 保存校验点
    json["check_points_enabled"] = checkPointsEnabled_;
    QJsonArray checkPointsArray;
    for (const ModelCheckPoint& cp : checkPoints_) {
        QJsonObject cpObj;
        cpObj["rel_row"] = cp.relRow;
        cpObj["rel_col"] = cp.relCol;
        cpObj["expected_gray"] = cp.expectedGray;
        cpObj["tolerance"] = cp.tolerance;
        cpObj["name"] = cp.name;
        cpObj["enabled"] = cp.enabled;
        checkPointsArray.append(cpObj);
    }
    json["check_points"] = checkPointsArray;

    // 保存轮廓过滤参数
    json["contour_filter_enabled"] = contourFilterEnabled_;
    json["min_contour_length"] = minContourLength_;
    json["union_collinear_enabled"] = unionCollinearEnabled_;
    json["union_adjacent_enabled"] = unionAdjacentEnabled_;
    json["max_dist_collinear"] = maxDistCollinear_;
    json["max_dist_adjacent"] = maxDistAdjacent_;

    return json;
}

void ShapeMatchTool::deserializeParams(const QJsonObject& json)
{
    modelPath_ = json.value("model_path").toString();
    matchType_ = static_cast<MatchType>(json.value("match_type").toInt(Standard));
    minScore_ = json.value("min_score").toDouble(0.7);
    numMatches_ = json.value("num_matches").toInt(1);
    angleStart_ = json.value("angle_start").toDouble(-0.39);
    angleExtent_ = json.value("angle_extent").toDouble(0.78);
    minContrast_ = json.value("min_contrast").toInt(30);
    scaleMin_ = json.value("scale_min").toDouble(0.9);
    scaleMax_ = json.value("scale_max").toDouble(1.1);
    scaleStepRow_ = json.value("scale_step_row").toDouble(0.95);
    scaleStepCol_ = json.value("scale_step_col").toDouble(0.95);
    useXLDDisplay_ = json.value("use_xld_display").toBool(true);

    // 如果有模板路径，尝试加载
    if (!modelPath_.isEmpty()) {
        loadModel(modelPath_);
    }

    // 加载校验点
    checkPointsEnabled_ = json.value("check_points_enabled").toBool(false);
    checkPoints_.clear();
    QJsonArray checkPointsArray = json.value("check_points").toArray();
    for (const QJsonValue& val : checkPointsArray) {
        QJsonObject cpObj = val.toObject();
        ModelCheckPoint cp;
        cp.relRow = cpObj.value("rel_row").toDouble(0);
        cp.relCol = cpObj.value("rel_col").toDouble(0);
        cp.expectedGray = cpObj.value("expected_gray").toInt(128);
        cp.tolerance = cpObj.value("tolerance").toInt(30);
        cp.name = cpObj.value("name").toString();
        cp.enabled = cpObj.value("enabled").toBool(true);
        checkPoints_.push_back(cp);
    }

    // 加载轮廓过滤参数
    contourFilterEnabled_ = json.value("contour_filter_enabled").toBool(true);
    minContourLength_ = json.value("min_contour_length").toDouble(100.0);
    unionCollinearEnabled_ = json.value("union_collinear_enabled").toBool(true);
    unionAdjacentEnabled_ = json.value("union_adjacent_enabled").toBool(true);
    maxDistCollinear_ = json.value("max_dist_collinear").toDouble(15.0);
    maxDistAdjacent_ = json.value("max_dist_adjacent").toDouble(15.0);
}

// ========== 校验点方法实现 ==========

void ShapeMatchTool::addCheckPoint(const ModelCheckPoint& checkPoint)
{
    checkPoints_.push_back(checkPoint);
    emit paramChanged();
    LOG_INFO(QString("添加校验点: (%1, %2), 灰度=%3±%4")
        .arg(checkPoint.relRow).arg(checkPoint.relCol)
        .arg(checkPoint.expectedGray).arg(checkPoint.tolerance));
}

void ShapeMatchTool::removeCheckPoint(int index)
{
    if (index >= 0 && index < static_cast<int>(checkPoints_.size())) {
        checkPoints_.erase(checkPoints_.begin() + index);
        emit paramChanged();
        LOG_INFO(QString("删除校验点: 索引 %1").arg(index));
    }
}

void ShapeMatchTool::clearCheckPoints()
{
    checkPoints_.clear();
    emit paramChanged();
    LOG_INFO("清除所有校验点");
}

bool ShapeMatchTool::validateCheckPoints(const Base::ImageData::Ptr& image,
                                         double row, double col, double angle,
                                         std::vector<int>* failedPoints)
{
    if (!checkPointsEnabled_ || checkPoints_.empty()) {
        return true;  // 未启用或无校验点，直接返回成功
    }

    if (!image || image->isEmpty()) {
        LOG_WARNING("校验点验证: 图像为空");
        return false;
    }

    const cv::Mat& mat = image->mat();
    bool allPassed = true;
    double cosA = std::cos(angle);
    double sinA = std::sin(angle);

    for (size_t i = 0; i < checkPoints_.size(); i++) {
        const ModelCheckPoint& cp = checkPoints_[i];
        if (!cp.enabled) continue;

        // 根据匹配角度变换相对坐标到绝对坐标
        double absRow = row + cp.relRow * cosA - cp.relCol * sinA;
        double absCol = col + cp.relRow * sinA + cp.relCol * cosA;

        int pixelRow = static_cast<int>(std::round(absRow));
        int pixelCol = static_cast<int>(std::round(absCol));

        // 检查坐标是否在图像范围内
        if (pixelRow < 0 || pixelRow >= mat.rows ||
            pixelCol < 0 || pixelCol >= mat.cols) {
            LOG_WARNING(QString("校验点 %1 超出图像范围: (%2, %3)")
                .arg(i).arg(pixelRow).arg(pixelCol));
            if (failedPoints) failedPoints->push_back(static_cast<int>(i));
            allPassed = false;
            continue;
        }

        // 获取灰度值
        int grayValue = 0;
        if (mat.channels() == 1) {
            grayValue = mat.at<uchar>(pixelRow, pixelCol);
        } else if (mat.channels() == 3) {
            cv::Vec3b bgr = mat.at<cv::Vec3b>(pixelRow, pixelCol);
            grayValue = static_cast<int>(0.299 * bgr[2] + 0.587 * bgr[1] + 0.114 * bgr[0]);
        }

        // 检查灰度值是否在容差范围内
        int diff = std::abs(grayValue - cp.expectedGray);
        if (diff > cp.tolerance) {
            LOG_DEBUG(QString("校验点 %1 '%2' 验证失败: 灰度=%3, 期望=%4±%5")
                .arg(i).arg(cp.name).arg(grayValue)
                .arg(cp.expectedGray).arg(cp.tolerance));
            if (failedPoints) failedPoints->push_back(static_cast<int>(i));
            allPassed = false;
        } else {
            LOG_DEBUG(QString("校验点 %1 '%2' 验证通过: 灰度=%3, 期望=%4±%5")
                .arg(i).arg(cp.name).arg(grayValue)
                .arg(cp.expectedGray).arg(cp.tolerance));
        }
    }

    return allPassed;
}

} // namespace Algorithm
} // namespace VisionForge
