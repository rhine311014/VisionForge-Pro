/**
 * @file VirtualObjectToolDialog.h
 * @brief 虚拟对象计算工具对话框
 * @details 用于计算虚拟几何对象（虚拟点、虚拟线、虚拟圆等）
 * @author VisionForge Team
 * @date 2025-12-30
 */

#pragma once

#include "ui/IToolDialog.h"
#include "ui/PreviewHelper.h"

#include <QDialog>
#include <QSplitter>
#include <QTabWidget>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QTableWidget>
#include <QListWidget>
#include <QStackedWidget>

class QVBoxLayout;
class QHBoxLayout;

namespace VisionForge {

namespace Algorithm {
    class VirtualObjectTool;
}

namespace UI {

class HalconImageViewer;

/**
 * @brief 虚拟对象计算工具对话框
 *
 * 提供虚拟几何对象的计算功能：
 * - 虚拟点：两线交点、两圆交点、中点、投影点等
 * - 虚拟线：两点连线、平行线、垂直线、切线等
 * - 虚拟圆：三点拟合圆、同心圆等
 */
class VirtualObjectToolDialog : public IToolDialog
{
    Q_OBJECT

public:
    /**
     * @brief 虚拟对象类型
     */
    enum ObjectType {
        VirtualPoint = 0,   ///< 虚拟点
        VirtualLine,        ///< 虚拟线
        VirtualCircle       ///< 虚拟圆
    };

    /**
     * @brief 虚拟点计算方法
     */
    enum PointMethod {
        LineLineIntersection = 0,   ///< 两线交点
        CircleCircleIntersection,   ///< 两圆交点
        PointMidpoint,              ///< 两点中点
        PointsCentroid,             ///< 多点重心
        PointToLineProjection,      ///< 点到线投影
        LineCircleIntersection,     ///< 线圆交点
        FixedPoint                  ///< 固定点坐标
    };

    /**
     * @brief 虚拟线计算方法
     */
    enum LineMethod {
        TwoPointsLine = 0,          ///< 两点连线
        ParallelLine,               ///< 平行线
        PerpendicularLine,          ///< 垂直线
        AngleBisector,              ///< 角平分线
        TangentLine,                ///< 切线
        PointsRegression,           ///< 多点拟合线
        FixedLine                   ///< 固定线参数
    };

    /**
     * @brief 虚拟圆计算方法
     */
    enum CircleMethod {
        ThreePointsCircle = 0,      ///< 三点拟合圆
        ConcentricCircle,           ///< 同心圆
        ThroughPointCircle,         ///< 过点的圆
        PointsRegressionCircle,     ///< 多点拟合圆
        FixedCircle                 ///< 固定圆参数
    };

    explicit VirtualObjectToolDialog(Algorithm::VirtualObjectTool* tool, QWidget* parent = nullptr);
    ~VirtualObjectToolDialog() override;

    // IToolDialog接口实现
    void setImage(Base::ImageData::Ptr image) override;
    Algorithm::VisionTool* tool() const override;

private:
    void setupUI();
    void createLeftPanel(QWidget* parent);
    void createRightPanel(QWidget* parent);
    void createObjectTypeGroup(QVBoxLayout* layout);
    void createPointMethodPanel();
    void createLineMethodPanel();
    void createCircleMethodPanel();
    void createInputSourceGroup(QVBoxLayout* layout);
    void createResultGroup(QVBoxLayout* layout);
    void connectSignals();

    void updateMethodPanel();
    void updateInputSources();
    void updateResult();
    void drawVirtualObject();

private slots:
    void onObjectTypeChanged(int index);
    void onMethodChanged(int index);
    void onCalculateClicked();
    void onPreviewClicked();
    void onOkClicked();
    void onCancelClicked();
    void onApplyClicked();

private:
    Algorithm::VirtualObjectTool* tool_ = nullptr;
    Base::ImageData::Ptr currentImage_;
    PreviewHelper* previewHelper_ = nullptr;

    // UI组件 - 主布局
    QSplitter* mainSplitter_ = nullptr;
    HalconImageViewer* imageViewer_ = nullptr;
    QLabel* imageInfoLabel_ = nullptr;

    // 对象类型选择
    QGroupBox* objectTypeGroup_ = nullptr;
    QComboBox* objectTypeCombo_ = nullptr;
    QLabel* objectTypeDescLabel_ = nullptr;

    // 方法选择（堆叠面板）
    QGroupBox* methodGroup_ = nullptr;
    QStackedWidget* methodStack_ = nullptr;

    // 虚拟点方法面板
    QWidget* pointMethodPanel_ = nullptr;
    QComboBox* pointMethodCombo_ = nullptr;
    QLabel* pointMethodDescLabel_ = nullptr;
    QStackedWidget* pointParamStack_ = nullptr;

    // 虚拟线方法面板
    QWidget* lineMethodPanel_ = nullptr;
    QComboBox* lineMethodCombo_ = nullptr;
    QLabel* lineMethodDescLabel_ = nullptr;
    QStackedWidget* lineParamStack_ = nullptr;

    // 虚拟圆方法面板
    QWidget* circleMethodPanel_ = nullptr;
    QComboBox* circleMethodCombo_ = nullptr;
    QLabel* circleMethodDescLabel_ = nullptr;
    QStackedWidget* circleParamStack_ = nullptr;

    // 输入源选择
    QGroupBox* inputSourceGroup_ = nullptr;
    QListWidget* availableSourcesList_ = nullptr;
    QListWidget* selectedSourcesList_ = nullptr;
    QPushButton* addSourceBtn_ = nullptr;
    QPushButton* removeSourceBtn_ = nullptr;

    // 固定参数输入（用于固定点/线/圆）
    QDoubleSpinBox* fixedXSpin_ = nullptr;
    QDoubleSpinBox* fixedYSpin_ = nullptr;
    QDoubleSpinBox* fixedAngleSpin_ = nullptr;
    QDoubleSpinBox* fixedRadiusSpin_ = nullptr;

    // 附加参数
    QDoubleSpinBox* offsetDistanceSpin_ = nullptr;
    QSpinBox* intersectionIndexSpin_ = nullptr;

    // 结果显示
    QGroupBox* resultGroup_ = nullptr;
    QTableWidget* resultTable_ = nullptr;
    QLabel* resultStatusLabel_ = nullptr;

    // 底部按钮
    QPushButton* calculateBtn_ = nullptr;
    QPushButton* previewBtn_ = nullptr;
    QPushButton* okBtn_ = nullptr;
    QPushButton* cancelBtn_ = nullptr;
    QPushButton* applyBtn_ = nullptr;
};

} // namespace UI
} // namespace VisionForge
