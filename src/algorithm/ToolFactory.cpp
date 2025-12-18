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
#include "base/Logger.h"

#ifdef USE_HALCON
#include "algorithm/ShapeMatchTool.h"
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
    // 注册灰度转换工具
    registerTool(
        VisionTool::Gray,
        ToolInfo(
            "灰度转换",
            "图像预处理",
            "将彩色图像转换为灰度图像，支持多种转换算法",
            ":/icons/gray.png",
            VisionTool::Gray
        ),
        []() -> VisionTool* { return new GrayTool(); }
    );

    // 注册模糊工具
    registerTool(
        VisionTool::Blur,
        ToolInfo(
            "图像模糊",
            "图像预处理",
            "对图像进行模糊处理，支持均值、高斯、中值、双边滤波",
            ":/icons/blur.png",
            VisionTool::Blur
        ),
        []() -> VisionTool* { return new BlurTool(); }
    );

    // 注册二值化工具
    registerTool(
        VisionTool::Binary,
        ToolInfo(
            "图像二值化",
            "图像预处理",
            "将图像转换为二值图像，支持固定阈值、自适应阈值、OTSU等算法",
            ":/icons/binary.png",
            VisionTool::Binary
        ),
        []() -> VisionTool* { return new ThresholdTool(); }
    );

    // 注册边缘检测工具
    registerTool(
        VisionTool::Edge,
        ToolInfo(
            "边缘检测",
            "图像预处理",
            "检测图像边缘，支持Canny、Sobel、Laplacian、Scharr算法",
            ":/icons/edge.png",
            VisionTool::Edge
        ),
        []() -> VisionTool* { return new EdgeTool(); }
    );

    // 注册形态学处理工具
    registerTool(
        VisionTool::Morphology,
        ToolInfo(
            "形态学处理",
            "图像预处理",
            "形态学操作，支持腐蚀、膨胀、开闭运算、梯度、顶帽、黑帽",
            ":/icons/morphology.png",
            VisionTool::Morphology
        ),
        []() -> VisionTool* { return new MorphologyTool(); }
    );

    // 注册ROI区域工具
    registerTool(
        VisionTool::ROI,
        ToolInfo(
            "ROI区域",
            "图像预处理",
            "定义感兴趣区域，支持矩形、圆形、椭圆、多边形、旋转矩形",
            ":/icons/roi.png",
            VisionTool::ROI
        ),
        []() -> VisionTool* { return new ROITool(); }
    );

    // 注册Blob分析工具
    registerTool(
        VisionTool::BlobAnalysis,
        ToolInfo(
            "Blob分析",
            "定位检测",
            "连通域分析，提取面积、周长、圆度等特征，支持Halcon/OpenCV双后端",
            ":/icons/blob.png",
            VisionTool::BlobAnalysis
        ),
        []() -> VisionTool* { return new BlobTool(); }
    );

    // 注册圆检测工具
    registerTool(
        VisionTool::FindCircle,
        ToolInfo(
            "圆检测",
            "定位检测",
            "检测图像中的圆形，支持霍夫变换、轮廓拟合、边缘拟合等方法",
            ":/icons/circle.png",
            VisionTool::FindCircle
        ),
        []() -> VisionTool* { return new CircleTool(); }
    );

    // 注册线检测工具
    registerTool(
        VisionTool::FindLine,
        ToolInfo(
            "线检测",
            "定位检测",
            "检测图像中的直线，支持霍夫变换、轮廓拟合等方法",
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
            "形状匹配",
            "定位检测",
            "使用Halcon形状匹配算法查找图像中的目标，支持角度、缩放变化",
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
            "相机标定",
            "标定",
            "使用棋盘格/圆点阵进行相机内参标定，计算畸变校正参数",
            ":/icons/camera_calib.png",
            VisionTool::CameraCalib
        ),
        []() -> VisionTool* { return new CameraCalibTool(); }
    );

    // 注册九点标定工具
    registerTool(
        VisionTool::NinePointCalib,
        ToolInfo(
            "九点标定",
            "标定",
            "图像坐标与物理坐标映射标定，支持仿射/透视变换",
            ":/icons/ninepoint_calib.png",
            VisionTool::NinePointCalib
        ),
        []() -> VisionTool* { return new NinePointCalibTool(); }
    );

    // 注册距离测量工具
    registerTool(
        VisionTool::MeasureDistance,
        ToolInfo(
            "距离测量",
            "测量计算",
            "测量两点、点到线、线到线等距离，支持像素/毫米换算",
            ":/icons/measure_distance.png",
            VisionTool::MeasureDistance
        ),
        []() -> VisionTool* { return new MeasureDistanceTool(); }
    );

    // 注册角度测量工具
    registerTool(
        VisionTool::MeasureAngle,
        ToolInfo(
            "角度测量",
            "测量计算",
            "测量两线夹角、三点角度、线与水平/垂直方向角度",
            ":/icons/measure_angle.png",
            VisionTool::MeasureAngle
        ),
        []() -> VisionTool* { return new MeasureAngleTool(); }
    );

    // 注册面积测量工具
    registerTool(
        VisionTool::MeasureArea,
        ToolInfo(
            "面积测量",
            "测量计算",
            "测量轮廓、多边形、圆形、椭圆的面积，支持像素/毫米换算",
            ":/icons/measure_area.png",
            VisionTool::MeasureArea
        ),
        []() -> VisionTool* { return new MeasureAreaTool(); }
    );

    // 注册范围判定工具
    registerTool(
        VisionTool::RangeJudge,
        ToolInfo(
            "范围判定",
            "判定输出",
            "判断测量值是否在指定范围内，输出OK/NG结果",
            ":/icons/range_judge.png",
            VisionTool::RangeJudge
        ),
        []() -> VisionTool* { return new RangeJudgeTool(); }
    );

    // 注册图像保存工具
    registerTool(
        VisionTool::SaveImage,
        ToolInfo(
            "图像保存",
            "判定输出",
            "保存图像到文件，支持多种格式和命名规则",
            ":/icons/save_image.png",
            VisionTool::SaveImage
        ),
        []() -> VisionTool* { return new SaveImageTool(); }
    );

    // 注册PLC输出工具
    registerTool(
        VisionTool::PLCOutput,
        ToolInfo(
            "PLC输出",
            "判定输出",
            "将检测结果输出到PLC，支持位/字输出和地址映射",
            ":/icons/plc_output.png",
            VisionTool::PLCOutput
        ),
        []() -> VisionTool* { return new PLCOutputTool(); }
    );

    // 注册逻辑运算工具
    registerTool(
        VisionTool::LogicOperation,
        ToolInfo(
            "逻辑运算",
            "判定输出",
            "对多个检测结果进行逻辑运算(AND/OR/NOT/XOR等)",
            ":/icons/logic_op.png",
            VisionTool::LogicOperation
        ),
        []() -> VisionTool* { return new LogicOperationTool(); }
    );

    // 注册OpenCV模板匹配工具
    registerTool(
        VisionTool::TemplateMatch,
        ToolInfo(
            "模板匹配",
            "定位检测",
            "使用OpenCV模板匹配算法查找目标，支持金字塔加速和角度搜索",
            ":/icons/template_match.png",
            VisionTool::TemplateMatch
        ),
        []() -> VisionTool* { return new TemplateMatchTool(); }
    );

    LOG_INFO(QString("已注册 %1 个内置工具").arg(tools_.size()));
}

} // namespace Algorithm
} // namespace VisionForge
