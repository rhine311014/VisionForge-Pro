/**
 * @file ToolFactory.cpp
 * @brief 工具工厂实现
 */

#include "algorithm/ToolFactory.h"
#include "algorithm/GrayTool.h"
#include "algorithm/BlurTool.h"
#include "algorithm/ThresholdTool.h"
#include "algorithm/EdgeTool.h"
#include "algorithm/MorphologyTool.h"
#include "algorithm/ROITool.h"
#include "algorithm/BlobTool.h"
#include "algorithm/CircleTool.h"
#include "algorithm/LineTool.h"
#include "algorithm/CameraCalibTool.h"
#include "algorithm/NinePointCalibTool.h"
#include "algorithm/MeasureDistanceTool.h"
#include "algorithm/MeasureAngleTool.h"
#include "algorithm/MeasureAreaTool.h"
#include "algorithm/RangeJudgeTool.h"
#include "algorithm/SaveImageTool.h"
#include "algorithm/PLCOutputTool.h"
#include "algorithm/LogicOperationTool.h"
#include "algorithm/TemplateMatchTool.h"
#include "algorithm/ColorConvertTool.h"
#include "algorithm/FindEdgeTool.h"
#include "algorithm/CalcCenterTool.h"
#include "algorithm/CalcOrientationTool.h"
#include "algorithm/AIDetectionTool.h"
#include "base/Logger.h"

#ifdef USE_HALCON
#include "algorithm/ShapeMatchTool.h"
#include "algorithm/CodeReadTool.h"
#endif

namespace VisionForge {
namespace Algorithm {

ToolFactory::ToolFactory()
{
    // 注册内置工具
    registerBuiltInTools();
}

ToolFactory::~ToolFactory()
{
}

ToolFactory& ToolFactory::instance()
{
    static ToolFactory instance;
    return instance;
}

void ToolFactory::registerTool(VisionTool::ToolType type,
                               const ToolInfo& info,
                               ToolCreator creator)
{
    if (tools_.contains(type)) {
        LOG_WARNING(QString("工具类型 %1 已注册，将被覆盖").arg(static_cast<int>(type)));
    }

    if (nameToType_.contains(info.name)) {
        LOG_WARNING(QString("工具名称 \"%1\" 已注册，将被覆盖").arg(info.name));
    }

    tools_[type] = ToolRegistration(info, creator);
    nameToType_[info.name] = type;

    LOG_DEBUG(QString("注册工具: %1 [%2]").arg(info.name).arg(info.category));
}

VisionTool* ToolFactory::createTool(VisionTool::ToolType type)
{
    if (!tools_.contains(type)) {
        LOG_ERROR(QString("未注册的工具类型: %1").arg(static_cast<int>(type)));
        return nullptr;
    }

    try {
        VisionTool* tool = tools_[type].creator();
        LOG_DEBUG(QString("创建工具: %1").arg(tools_[type].info.name));
        return tool;
    } catch (const std::exception& e) {
        LOG_ERROR(QString("创建工具失败: %1").arg(e.what()));
        return nullptr;
    }
}

VisionTool* ToolFactory::createTool(const QString& name)
{
    if (!nameToType_.contains(name)) {
        LOG_ERROR(QString("未注册的工具名称: %1").arg(name));
        return nullptr;
    }

    VisionTool::ToolType type = nameToType_[name];
    return createTool(type);
}

QList<ToolFactory::ToolInfo> ToolFactory::getAllTools() const
{
    QList<ToolInfo> result;
    for (const auto& registration : tools_) {
        result.append(registration.info);
    }
    return result;
}

QList<ToolFactory::ToolInfo> ToolFactory::getToolsByCategory(const QString& category) const
{
    QList<ToolInfo> result;
    for (const auto& registration : tools_) {
        if (registration.info.category == category) {
            result.append(registration.info);
        }
    }
    return result;
}

ToolFactory::ToolInfo ToolFactory::getToolInfo(VisionTool::ToolType type) const
{
    if (tools_.contains(type)) {
        return tools_[type].info;
    }
    return ToolInfo();
}

bool ToolFactory::isRegistered(VisionTool::ToolType type) const
{
    return tools_.contains(type);
}

QStringList ToolFactory::getAllCategories() const
{
    QSet<QString> categories;
    for (const auto& registration : tools_) {
        categories.insert(registration.info.category);
    }
    return QStringList(categories.begin(), categories.end());
}

// ========== 私有方法 ==========

void ToolFactory::registerBuiltInTools()
{
    // 工具类别（可翻译）
    const QString CAT_PREPROCESS = tr("图像预处理");
    const QString CAT_DETECTION = tr("定位检测");
    const QString CAT_CALIBRATION = tr("标定");
    const QString CAT_MEASURE = tr("测量计算");
    const QString CAT_OUTPUT = tr("判定输出");
    const QString CAT_AI = tr("AI视觉");

    // 注册灰度转换工具
    registerTool(
        VisionTool::Gray,
        ToolInfo(
            tr("灰度转换"),
            CAT_PREPROCESS,
            tr("将彩色图像转换为灰度图像，支持多种转换算法"),
            ":/icons/gray.png",
            VisionTool::Gray
        ),
        []() -> VisionTool* { return new GrayTool(); }
    );

    // 注册模糊工具
    registerTool(
        VisionTool::Blur,
        ToolInfo(
            tr("图像模糊"),
            CAT_PREPROCESS,
            tr("对图像进行模糊处理，支持均值、高斯、中值、双边滤波"),
            ":/icons/blur.png",
            VisionTool::Blur
        ),
        []() -> VisionTool* { return new BlurTool(); }
    );

    // 注册二值化工具
    registerTool(
        VisionTool::Binary,
        ToolInfo(
            tr("图像二值化"),
            CAT_PREPROCESS,
            tr("将图像转换为二值图像，支持固定阈值、自适应阈值、OTSU等算法"),
            ":/icons/binary.png",
            VisionTool::Binary
        ),
        []() -> VisionTool* { return new ThresholdTool(); }
    );

    // 注册边缘检测工具
    registerTool(
        VisionTool::Edge,
        ToolInfo(
            tr("边缘检测"),
            CAT_PREPROCESS,
            tr("检测图像边缘，支持Canny、Sobel、Laplacian、Scharr算法"),
            ":/icons/edge.png",
            VisionTool::Edge
        ),
        []() -> VisionTool* { return new EdgeTool(); }
    );

    // 注册形态学处理工具
    registerTool(
        VisionTool::Morphology,
        ToolInfo(
            tr("形态学处理"),
            CAT_PREPROCESS,
            tr("形态学操作，支持腐蚀、膨胀、开闭运算、梯度、顶帽、黑帽"),
            ":/icons/morphology.png",
            VisionTool::Morphology
        ),
        []() -> VisionTool* { return new MorphologyTool(); }
    );

    // 注册ROI区域工具
    registerTool(
        VisionTool::ROI,
        ToolInfo(
            tr("ROI区域"),
            CAT_PREPROCESS,
            tr("定义感兴趣区域，支持矩形、圆形、椭圆、多边形、旋转矩形"),
            ":/icons/roi.png",
            VisionTool::ROI
        ),
        []() -> VisionTool* { return new ROITool(); }
    );

    // 注册Blob分析工具
    registerTool(
        VisionTool::BlobAnalysis,
        ToolInfo(
            tr("Blob分析"),
            CAT_DETECTION,
            tr("连通域分析，提取面积、周长、圆度等特征，支持Halcon/OpenCV双后端"),
            ":/icons/blob.png",
            VisionTool::BlobAnalysis
        ),
        []() -> VisionTool* { return new BlobTool(); }
    );

    // 注册圆检测工具
    registerTool(
        VisionTool::FindCircle,
        ToolInfo(
            tr("圆检测"),
            CAT_DETECTION,
            tr("检测图像中的圆形，支持霍夫变换、轮廓拟合、边缘拟合等方法"),
            ":/icons/circle.png",
            VisionTool::FindCircle
        ),
        []() -> VisionTool* { return new CircleTool(); }
    );

    // 注册线检测工具
    registerTool(
        VisionTool::FindLine,
        ToolInfo(
            tr("线检测"),
            CAT_DETECTION,
            tr("检测图像中的直线，支持霍夫变换、轮廓拟合等方法"),
            ":/icons/line.png",
            VisionTool::FindLine
        ),
        []() -> VisionTool* { return new LineTool(); }
    );

#ifdef USE_HALCON
    // 注册Halcon形状匹配工具
    registerTool(
        VisionTool::Match,
        ToolInfo(
            tr("形状匹配"),
            CAT_DETECTION,
            tr("使用Halcon形状匹配算法查找图像中的目标，支持角度、缩放变化"),
            ":/icons/match.png",
            VisionTool::Match
        ),
        []() -> VisionTool* { return new ShapeMatchTool(); }
    );
#endif

    // 注册相机标定工具
    registerTool(
        VisionTool::CameraCalib,
        ToolInfo(
            tr("相机标定"),
            CAT_CALIBRATION,
            tr("使用棋盘格/圆点阵进行相机内参标定，计算畸变校正参数"),
            ":/icons/camera_calib.png",
            VisionTool::CameraCalib
        ),
        []() -> VisionTool* { return new CameraCalibTool(); }
    );

    // 注册九点标定工具
    registerTool(
        VisionTool::NinePointCalib,
        ToolInfo(
            tr("九点标定"),
            CAT_CALIBRATION,
            tr("图像坐标与物理坐标映射标定，支持仿射/透视变换"),
            ":/icons/ninepoint_calib.png",
            VisionTool::NinePointCalib
        ),
        []() -> VisionTool* { return new NinePointCalibTool(); }
    );

    // 注册距离测量工具
    registerTool(
        VisionTool::MeasureDistance,
        ToolInfo(
            tr("距离测量"),
            CAT_MEASURE,
            tr("测量两点、点到线、线到线等距离，支持像素/毫米换算"),
            ":/icons/measure_distance.png",
            VisionTool::MeasureDistance
        ),
        []() -> VisionTool* { return new MeasureDistanceTool(); }
    );

    // 注册角度测量工具
    registerTool(
        VisionTool::MeasureAngle,
        ToolInfo(
            tr("角度测量"),
            CAT_MEASURE,
            tr("测量两线夹角、三点角度、线与水平/垂直方向角度"),
            ":/icons/measure_angle.png",
            VisionTool::MeasureAngle
        ),
        []() -> VisionTool* { return new MeasureAngleTool(); }
    );

    // 注册面积测量工具
    registerTool(
        VisionTool::MeasureArea,
        ToolInfo(
            tr("面积测量"),
            CAT_MEASURE,
            tr("测量轮廓、多边形、圆形、椭圆的面积，支持像素/毫米换算"),
            ":/icons/measure_area.png",
            VisionTool::MeasureArea
        ),
        []() -> VisionTool* { return new MeasureAreaTool(); }
    );

    // 注册范围判定工具
    registerTool(
        VisionTool::RangeJudge,
        ToolInfo(
            tr("范围判定"),
            CAT_OUTPUT,
            tr("判断测量值是否在指定范围内，输出OK/NG结果"),
            ":/icons/range_judge.png",
            VisionTool::RangeJudge
        ),
        []() -> VisionTool* { return new RangeJudgeTool(); }
    );

    // 注册图像保存工具
    registerTool(
        VisionTool::SaveImage,
        ToolInfo(
            tr("图像保存"),
            CAT_OUTPUT,
            tr("保存图像到文件，支持多种格式和命名规则"),
            ":/icons/save_image.png",
            VisionTool::SaveImage
        ),
        []() -> VisionTool* { return new SaveImageTool(); }
    );

    // 注册PLC输出工具
    registerTool(
        VisionTool::PLCOutput,
        ToolInfo(
            tr("PLC输出"),
            CAT_OUTPUT,
            tr("将检测结果输出到PLC，支持位/字输出和地址映射"),
            ":/icons/plc_output.png",
            VisionTool::PLCOutput
        ),
        []() -> VisionTool* { return new PLCOutputTool(); }
    );

    // 注册逻辑运算工具
    registerTool(
        VisionTool::LogicOperation,
        ToolInfo(
            tr("逻辑运算"),
            CAT_OUTPUT,
            tr("对多个检测结果进行逻辑运算(AND/OR/NOT/XOR等)"),
            ":/icons/logic_op.png",
            VisionTool::LogicOperation
        ),
        []() -> VisionTool* { return new LogicOperationTool(); }
    );

    // 注册OpenCV模板匹配工具
    registerTool(
        VisionTool::TemplateMatch,
        ToolInfo(
            tr("模板匹配"),
            CAT_DETECTION,
            tr("使用OpenCV模板匹配算法查找目标，支持金字塔加速和角度搜索"),
            ":/icons/template_match.png",
            VisionTool::TemplateMatch
        ),
        []() -> VisionTool* { return new TemplateMatchTool(); }
    );

    // 注册颜色转换工具
    registerTool(
        VisionTool::ColorConvert,
        ToolInfo(
            tr("颜色转换"),
            CAT_PREPROCESS,
            tr("在不同颜色空间之间转换，支持BGR/RGB/HSV/Lab等格式"),
            ":/icons/color_convert.png",
            VisionTool::ColorConvert
        ),
        []() -> VisionTool* { return new ColorConvertTool(); }
    );

    // 注册边缘查找工具
    registerTool(
        VisionTool::FindEdge,
        ToolInfo(
            tr("边缘查找"),
            CAT_DETECTION,
            tr("沿指定方向查找边缘点，支持线搜索和矩形搜索模式"),
            ":/icons/find_edge.png",
            VisionTool::FindEdge
        ),
        []() -> VisionTool* { return new FindEdgeTool(); }
    );

    // 注册中心计算工具
    registerTool(
        VisionTool::CalcCenter,
        ToolInfo(
            tr("中心计算"),
            CAT_MEASURE,
            tr("计算目标中心点，支持质心、几何中心、最小包围圆等方法"),
            ":/icons/calc_center.png",
            VisionTool::CalcCenter
        ),
        []() -> VisionTool* { return new CalcCenterTool(); }
    );

    // 注册方向计算工具
    registerTool(
        VisionTool::CalcOrientation,
        ToolInfo(
            tr("方向计算"),
            CAT_MEASURE,
            tr("计算目标方向角度，支持PCA、最小外接矩形、椭圆拟合等方法"),
            ":/icons/calc_orientation.png",
            VisionTool::CalcOrientation
        ),
        []() -> VisionTool* { return new CalcOrientationTool(); }
    );

    // 注册AI检测工具
    registerTool(
        VisionTool::AIDetection,
        ToolInfo(
            tr("AI检测"),
            CAT_AI,
            tr("基于深度学习的目标检测、分类、缺陷检测，支持ONNX/TensorRT推理"),
            ":/icons/ai_detection.png",
            VisionTool::AIDetection
        ),
        []() -> VisionTool* { return new AIDetectionTool(); }
    );

    // 注册读码工具
#ifdef USE_HALCON
    registerTool(
        VisionTool::CodeRead,
        ToolInfo(
            tr("读码工具"),
            CAT_DETECTION,
            tr("识别二维码(DataMatrix/QR)和一维码，支持粗定位修正"),
            ":/icons/code_read.png",
            VisionTool::CodeRead
        ),
        []() -> VisionTool* { return new CodeReadTool(); }
    );
#endif

    LOG_INFO(QString(tr("已注册 %1 个内置工具")).arg(tools_.size()));
}

} // namespace Algorithm
} // namespace VisionForge
