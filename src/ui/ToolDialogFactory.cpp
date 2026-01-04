/**
 * @file ToolDialogFactory.cpp
 * @brief 工具对话框工厂实现
 */

#include "ui/ToolDialogFactory.h"
#include "base/Logger.h"
#include <QMetaMethod>

// 工具头文件
#include "algorithm/GrayTool.h"
#include "algorithm/BlurTool.h"
#include "algorithm/ThresholdTool.h"
#include "algorithm/EdgeTool.h"
#include "algorithm/MorphologyTool.h"
#include "algorithm/ROITool.h"
#include "algorithm/BlobTool.h"
#include "algorithm/CircleTool.h"
#include "algorithm/LineTool.h"
#include "algorithm/TemplateMatchTool.h"
#include "algorithm/ColorConvertTool.h"
#include "algorithm/FindEdgeTool.h"
#include "algorithm/CalcCenterTool.h"
#include "algorithm/CalcOrientationTool.h"
#include "algorithm/MeasureDistanceTool.h"
#include "algorithm/MeasureAngleTool.h"
#include "algorithm/MeasureAreaTool.h"
#include "algorithm/RangeJudgeTool.h"
#include "algorithm/SaveImageTool.h"
#include "algorithm/PLCOutputTool.h"
#include "algorithm/LogicOperationTool.h"
#include "algorithm/AIDetectionTool.h"
#include "algorithm/SubPixelEdgeTool.h"
#include "algorithm/VirtualObjectTool.h"
#include "algorithm/QuickSearchTool.h"
#include "algorithm/CornerSearchTool.h"
#include "algorithm/PatternSearchTool.h"
#include "algorithm/MultiPointAlignmentTool.h"
#include "algorithm/AlignmentOutputTool.h"
#include "algorithm/CameraCalibTool.h"
#include "algorithm/NinePointCalibTool.h"

// 对话框头文件
#include "ui/GrayToolDialog.h"
#include "ui/BlurToolDialog.h"
#include "ui/ThresholdToolDialog.h"
#include "ui/EdgeToolDialog.h"
#include "ui/MorphologyToolDialog.h"
#include "ui/ROIToolDialog.h"
#include "ui/BlobToolDialog.h"
#include "ui/CircleToolDialog.h"
#include "ui/LineToolDialog.h"
#include "ui/TemplateMatchToolDialog.h"
#include "ui/ColorConvertToolDialog.h"
#include "ui/FindEdgeToolDialog.h"
#include "ui/CalcCenterToolDialog.h"
#include "ui/CalcOrientationToolDialog.h"
#include "ui/MeasureDistanceToolDialog.h"
#include "ui/MeasureAngleToolDialog.h"
#include "ui/MeasureAreaToolDialog.h"
#include "ui/RangeJudgeToolDialog.h"
#include "ui/SaveImageToolDialog.h"
#include "ui/PLCOutputToolDialog.h"
#include "ui/LogicOperationToolDialog.h"
#include "ui/AIDetectionToolDialog.h"
#include "ui/SubPixelEdgeToolDialog.h"
#include "ui/VirtualObjectToolDialog.h"
#include "ui/QuickSearchToolDialog.h"
#include "ui/CornerSearchToolDialog.h"
#include "ui/PatternSearchToolDialog.h"
#include "ui/MultiPointAlignmentDialog.h"
#include "ui/AlignmentOutputDialog.h"
#include "ui/CameraCalibDialog.h"
#include "ui/NinePointCalibDialog.h"

#ifdef USE_HALCON
#include "algorithm/ShapeMatchTool.h"
#include "algorithm/CodeReadTool.h"
#include "ui/ShapeMatchToolDialog.h"
#include "ui/CodeReadToolDialog.h"
#endif

namespace VisionForge {
namespace UI {

ToolDialogFactory::ToolDialogFactory()
{
    registerBuiltInDialogs();

    // 设置 VisionTool 的对话框创建器回调
    Algorithm::VisionTool::setDialogCreator(
        [](Algorithm::VisionTool* tool, QWidget* parent, const Base::ImageData::Ptr& image) -> QDialog* {
            QDialog* dialog = ToolDialogFactory::instance().createDialog(tool, parent);
            if (dialog && image) {
                ToolDialogFactory::setDialogImage(dialog, image);
            }
            return dialog;
        }
    );
}

ToolDialogFactory& ToolDialogFactory::instance()
{
    static ToolDialogFactory instance;
    return instance;
}

void ToolDialogFactory::registerDialog(Algorithm::VisionTool::ToolType type, ToolDialogCreator creator)
{
    if (dialogs_.contains(type)) {
        LOG_WARNING(QString("工具对话框类型 %1 已注册，将被覆盖").arg(static_cast<int>(type)));
    }

    dialogs_[type] = creator;
    LOG_DEBUG(QString("注册工具对话框: 类型 %1").arg(static_cast<int>(type)));
}

QDialog* ToolDialogFactory::createDialog(Algorithm::VisionTool* tool, QWidget* parent)
{
    if (!tool) {
        LOG_ERROR("创建对话框失败：工具为空");
        return nullptr;
    }

    Algorithm::VisionTool::ToolType type = tool->toolType();

    if (!dialogs_.contains(type)) {
        LOG_DEBUG(QString("工具类型 %1 没有注册对话框").arg(static_cast<int>(type)));
        return nullptr;
    }

    try {
        QDialog* dialog = dialogs_[type](tool, parent);
        if (dialog) {
            LOG_DEBUG(QString("创建工具对话框成功: %1").arg(tool->displayName()));
        }
        return dialog;
    } catch (const std::exception& e) {
        LOG_ERROR(QString("创建工具对话框失败: %1").arg(e.what()));
        return nullptr;
    }
}

bool ToolDialogFactory::setDialogImage(QDialog* dialog, Base::ImageData::Ptr image)
{
    if (!dialog || !image) {
        return false;
    }

    // 使用Qt元对象系统尝试调用setImage或setPreviewImage方法
    // 这样可以兼容不同的对话框实现

    const QMetaObject* meta = dialog->metaObject();

    // 尝试调用 setImage(Base::ImageData::Ptr)
    int setImageIndex = meta->indexOfMethod("setImage(VisionForge::Base::ImageData::Ptr)");
    if (setImageIndex >= 0) {
        QMetaMethod method = meta->method(setImageIndex);
        bool ok = method.invoke(dialog, Q_ARG(VisionForge::Base::ImageData::Ptr, image));
        if (ok) return true;
    }

    // 尝试调用 setPreviewImage(Base::ImageData::Ptr)
    int setPreviewIndex = meta->indexOfMethod("setPreviewImage(VisionForge::Base::ImageData::Ptr)");
    if (setPreviewIndex >= 0) {
        QMetaMethod method = meta->method(setPreviewIndex);
        bool ok = method.invoke(dialog, Q_ARG(VisionForge::Base::ImageData::Ptr, image));
        if (ok) return true;
    }

    // 如果标准方法调用失败，尝试使用invokeMethod（更灵活但需要Q_INVOKABLE）
    bool result = QMetaObject::invokeMethod(dialog, "setImage",
                                            Q_ARG(Base::ImageData::Ptr, image));
    if (result) return true;

    result = QMetaObject::invokeMethod(dialog, "setPreviewImage",
                                       Q_ARG(Base::ImageData::Ptr, image));
    return result;
}

bool ToolDialogFactory::hasDialog(Algorithm::VisionTool::ToolType type) const
{
    return dialogs_.contains(type);
}

void ToolDialogFactory::registerBuiltInDialogs()
{
    using VT = Algorithm::VisionTool;

    // ========== 预处理工具 ==========

    // 灰度转换
    registerDialog(VT::Gray, [](Algorithm::VisionTool* tool, QWidget* parent) -> QDialog* {
        auto* t = dynamic_cast<Algorithm::GrayTool*>(tool);
        return t ? new GrayToolDialog(t, parent) : nullptr;
    });

    // 模糊
    registerDialog(VT::Blur, [](Algorithm::VisionTool* tool, QWidget* parent) -> QDialog* {
        auto* t = dynamic_cast<Algorithm::BlurTool*>(tool);
        return t ? new BlurToolDialog(t, parent) : nullptr;
    });

    // 二值化
    registerDialog(VT::Binary, [](Algorithm::VisionTool* tool, QWidget* parent) -> QDialog* {
        auto* t = dynamic_cast<Algorithm::ThresholdTool*>(tool);
        return t ? new ThresholdToolDialog(t, parent) : nullptr;
    });

    // 颜色转换
    registerDialog(VT::ColorConvert, [](Algorithm::VisionTool* tool, QWidget* parent) -> QDialog* {
        auto* t = dynamic_cast<Algorithm::ColorConvertTool*>(tool);
        return t ? new ColorConvertToolDialog(t, parent) : nullptr;
    });

    // 边缘检测
    registerDialog(VT::Edge, [](Algorithm::VisionTool* tool, QWidget* parent) -> QDialog* {
        auto* t = dynamic_cast<Algorithm::EdgeTool*>(tool);
        return t ? new EdgeToolDialog(t, parent) : nullptr;
    });

    // 形态学
    registerDialog(VT::Morphology, [](Algorithm::VisionTool* tool, QWidget* parent) -> QDialog* {
        auto* t = dynamic_cast<Algorithm::MorphologyTool*>(tool);
        return t ? new MorphologyToolDialog(t, parent) : nullptr;
    });

    // ROI
    registerDialog(VT::ROI, [](Algorithm::VisionTool* tool, QWidget* parent) -> QDialog* {
        auto* t = dynamic_cast<Algorithm::ROITool*>(tool);
        return t ? new ROIToolDialog(t, parent) : nullptr;
    });

    // ========== 检测工具 ==========

    // 模板匹配
    registerDialog(VT::TemplateMatch, [](Algorithm::VisionTool* tool, QWidget* parent) -> QDialog* {
        auto* t = dynamic_cast<Algorithm::TemplateMatchTool*>(tool);
        return t ? new TemplateMatchToolDialog(t, parent) : nullptr;
    });

    // 圆检测
    registerDialog(VT::FindCircle, [](Algorithm::VisionTool* tool, QWidget* parent) -> QDialog* {
        auto* t = dynamic_cast<Algorithm::CircleTool*>(tool);
        return t ? new CircleToolDialog(t, parent) : nullptr;
    });

    // 线检测
    registerDialog(VT::FindLine, [](Algorithm::VisionTool* tool, QWidget* parent) -> QDialog* {
        auto* t = dynamic_cast<Algorithm::LineTool*>(tool);
        return t ? new LineToolDialog(t, parent) : nullptr;
    });

    // 边缘查找
    registerDialog(VT::FindEdge, [](Algorithm::VisionTool* tool, QWidget* parent) -> QDialog* {
        auto* t = dynamic_cast<Algorithm::FindEdgeTool*>(tool);
        return t ? new FindEdgeToolDialog(t, parent) : nullptr;
    });

    // Blob分析
    registerDialog(VT::BlobAnalysis, [](Algorithm::VisionTool* tool, QWidget* parent) -> QDialog* {
        auto* t = dynamic_cast<Algorithm::BlobTool*>(tool);
        return t ? new BlobToolDialog(t, parent) : nullptr;
    });

    // AI检测
    registerDialog(VT::AIDetection, [](Algorithm::VisionTool* tool, QWidget* parent) -> QDialog* {
        auto* t = dynamic_cast<Algorithm::AIDetectionTool*>(tool);
        return t ? new AIDetectionToolDialog(t, parent) : nullptr;
    });

#ifdef USE_HALCON
    // 形状匹配
    registerDialog(VT::Match, [](Algorithm::VisionTool* tool, QWidget* parent) -> QDialog* {
        auto* t = dynamic_cast<Algorithm::ShapeMatchTool*>(tool);
        return t ? new ShapeMatchToolDialog(t, parent) : nullptr;
    });
#endif

    // ========== 测量工具 ==========

    // 距离测量
    registerDialog(VT::MeasureDistance, [](Algorithm::VisionTool* tool, QWidget* parent) -> QDialog* {
        auto* t = dynamic_cast<Algorithm::MeasureDistanceTool*>(tool);
        return t ? new MeasureDistanceToolDialog(t, parent) : nullptr;
    });

    // 角度测量
    registerDialog(VT::MeasureAngle, [](Algorithm::VisionTool* tool, QWidget* parent) -> QDialog* {
        auto* t = dynamic_cast<Algorithm::MeasureAngleTool*>(tool);
        return t ? new MeasureAngleToolDialog(t, parent) : nullptr;
    });

    // 面积测量
    registerDialog(VT::MeasureArea, [](Algorithm::VisionTool* tool, QWidget* parent) -> QDialog* {
        auto* t = dynamic_cast<Algorithm::MeasureAreaTool*>(tool);
        return t ? new MeasureAreaToolDialog(t, parent) : nullptr;
    });

    // 中心计算
    registerDialog(VT::CalcCenter, [](Algorithm::VisionTool* tool, QWidget* parent) -> QDialog* {
        auto* t = dynamic_cast<Algorithm::CalcCenterTool*>(tool);
        return t ? new CalcCenterToolDialog(t, parent) : nullptr;
    });

    // 方向计算
    registerDialog(VT::CalcOrientation, [](Algorithm::VisionTool* tool, QWidget* parent) -> QDialog* {
        auto* t = dynamic_cast<Algorithm::CalcOrientationTool*>(tool);
        return t ? new CalcOrientationToolDialog(t, parent) : nullptr;
    });

    // ========== 判定输出工具 ==========

    // 范围判定
    registerDialog(VT::RangeJudge, [](Algorithm::VisionTool* tool, QWidget* parent) -> QDialog* {
        auto* t = dynamic_cast<Algorithm::RangeJudgeTool*>(tool);
        return t ? new RangeJudgeToolDialog(t, parent) : nullptr;
    });

    // 逻辑运算
    registerDialog(VT::LogicOperation, [](Algorithm::VisionTool* tool, QWidget* parent) -> QDialog* {
        auto* t = dynamic_cast<Algorithm::LogicOperationTool*>(tool);
        return t ? new LogicOperationToolDialog(t, parent) : nullptr;
    });

    // 图像保存
    registerDialog(VT::SaveImage, [](Algorithm::VisionTool* tool, QWidget* parent) -> QDialog* {
        auto* t = dynamic_cast<Algorithm::SaveImageTool*>(tool);
        return t ? new SaveImageToolDialog(t, parent) : nullptr;
    });

    // PLC输出
    registerDialog(VT::PLCOutput, [](Algorithm::VisionTool* tool, QWidget* parent) -> QDialog* {
        auto* t = dynamic_cast<Algorithm::PLCOutputTool*>(tool);
        return t ? new PLCOutputToolDialog(t, parent) : nullptr;
    });

    // ========== 亚像素边缘检测 ==========
    registerDialog(VT::SubPixelEdge, [](Algorithm::VisionTool* tool, QWidget* parent) -> QDialog* {
        auto* t = dynamic_cast<Algorithm::SubPixelEdgeTool*>(tool);
        return t ? new SubPixelEdgeToolDialog(t, parent) : nullptr;
    });

    // ========== 搜索工具 ==========

    // 快速搜索
    registerDialog(VT::QuickSearch, [](Algorithm::VisionTool* tool, QWidget* parent) -> QDialog* {
        auto* t = dynamic_cast<Algorithm::QuickSearchTool*>(tool);
        return t ? new QuickSearchToolDialog(t, parent) : nullptr;
    });

    // 图案搜索
    registerDialog(VT::PatternSearch, [](Algorithm::VisionTool* tool, QWidget* parent) -> QDialog* {
        auto* t = dynamic_cast<Algorithm::PatternSearchTool*>(tool);
        return t ? new PatternSearchToolDialog(t, parent) : nullptr;
    });

    // 角点搜索
    registerDialog(VT::CornerSearch, [](Algorithm::VisionTool* tool, QWidget* parent) -> QDialog* {
        auto* t = dynamic_cast<Algorithm::CornerSearchTool*>(tool);
        return t ? new CornerSearchToolDialog(t, parent) : nullptr;
    });

    // ========== 计算工具 ==========

    // 虚拟对象
    registerDialog(VT::VirtualObject, [](Algorithm::VisionTool* tool, QWidget* parent) -> QDialog* {
        auto* t = dynamic_cast<Algorithm::VirtualObjectTool*>(tool);
        return t ? new VirtualObjectToolDialog(t, parent) : nullptr;
    });

    // ========== 标定工具 ==========

    // 相机标定
    registerDialog(VT::CameraCalib, [](Algorithm::VisionTool* tool, QWidget* parent) -> QDialog* {
        auto* t = dynamic_cast<Algorithm::CameraCalibTool*>(tool);
        if (!t) return nullptr;
        auto* dialog = new CameraCalibDialog(parent);
        return dialog;
    });

    // 九点标定
    registerDialog(VT::NinePointCalib, [](Algorithm::VisionTool* tool, QWidget* parent) -> QDialog* {
        auto* t = dynamic_cast<Algorithm::NinePointCalibTool*>(tool);
        if (!t) return nullptr;
        auto* dialog = new NinePointCalibDialog(parent);
        return dialog;
    });

    // 多点对位
    registerDialog(VT::MultiPointAlignment, [](Algorithm::VisionTool* tool, QWidget* parent) -> QDialog* {
        auto* t = dynamic_cast<Algorithm::MultiPointAlignmentTool*>(tool);
        if (!t) return nullptr;
        auto* dialog = new MultiPointAlignmentDialog(parent);
        dialog->setAlignmentTool(t);
        return dialog;
    });

    // 对位输出
    registerDialog(VT::AlignmentOutput, [](Algorithm::VisionTool* tool, QWidget* parent) -> QDialog* {
        auto* t = dynamic_cast<Algorithm::AlignmentOutputTool*>(tool);
        if (!t) return nullptr;
        auto* dialog = new AlignmentOutputDialog(parent);
        dialog->setOutputTool(t);
        return dialog;
    });

#ifdef USE_HALCON
    // 读码工具
    registerDialog(VT::CodeRead, [](Algorithm::VisionTool* tool, QWidget* parent) -> QDialog* {
        auto* t = dynamic_cast<Algorithm::CodeReadTool*>(tool);
        return t ? new CodeReadToolDialog(t, parent) : nullptr;
    });
#endif

    LOG_INFO(QString("已注册 %1 个工具对话框").arg(dialogs_.size()));
}

} // namespace UI
} // namespace VisionForge
