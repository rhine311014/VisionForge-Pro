/**
 * @file ToolFactory.cpp
 * @brief 工具工厂实现
 */

#include "algorithm/ToolFactory.h"
#include "algorithm/GrayTool.h"
#include "algorithm/BlurTool.h"
#include "algorithm/ThresholdTool.h"
#include "algorithm/EdgeDetectionTool.h"
#include "base/Logger.h"

#ifdef _WIN32
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
        VisionTool::FindEdge,
        ToolInfo(
            "边缘检测",
            "定位检测",
            "检测图像中的边缘，支持Sobel、Scharr、Laplacian、Canny算法",
            ":/icons/edge.png",
            VisionTool::FindEdge
        ),
        []() -> VisionTool* { return new EdgeDetectionTool(); }
    );

#ifdef _WIN32
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

    // TODO: 注册其他内置工具
    // 例如:
    // - ROI工具 (ROITool)
    // - 形态学工具 (MorphologyTool)
    // - 模板匹配工具 (TemplateMatchTool)
    // - 圆检测工具 (FindCircleTool)
    // 等等...

    LOG_INFO(QString("已注册 %1 个内置工具").arg(tools_.size()));
}

} // namespace Algorithm
} // namespace VisionForge
