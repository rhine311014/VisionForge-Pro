/**
 * @file CornerSearchToolDialog.h
 * @brief 角点搜索工具对话框
 * @details 基于Harris/ShiTomasi算法的角点检测工具
 * @author VisionForge Team
 * @date 2025-12-30
 */

#pragma once

#include "ui/SearchToolDialog.h"

namespace VisionForge {

namespace Algorithm {
    class CornerSearchTool;
}

namespace UI {

/**
 * @brief 角点搜索工具对话框
 *
 * 提供多种角点检测算法的参数配置和搜索功能。
 * 支持Harris、ShiTomasi、FAST等多种角点检测方法。
 */
class CornerSearchToolDialog : public SearchToolDialog
{
    Q_OBJECT

public:
    /**
     * @brief 角点检测方法
     */
    enum CornerMethod {
        Harris = 0,         ///< Harris角点检测
        ShiTomasi,          ///< Shi-Tomasi角点检测
        FAST,               ///< FAST角点检测
        ORB,                ///< ORB特征点检测
        HalconPoints        ///< Halcon兴趣点检测
    };

    explicit CornerSearchToolDialog(Algorithm::CornerSearchTool* tool, QWidget* parent = nullptr);
    ~CornerSearchToolDialog() override;

protected:
    // SearchToolDialog接口实现
    QWidget* createParameterPanel(QWidget* parent) override;
    QString toolName() const override { return tr("角点搜索"); }
    bool executeSearch(std::vector<SearchResult>& results) override;
    void applySpecificParameters() override;
    void updateSpecificUI() override;

private:
    void createMethodGroup(QVBoxLayout* layout);
    void createHarrisParamGroup(QVBoxLayout* layout);
    void createShiTomasiParamGroup(QVBoxLayout* layout);
    void createFASTParamGroup(QVBoxLayout* layout);
    void createFilterGroup(QVBoxLayout* layout);
    void connectSpecificSignals();
    void updateParameterVisibility();

private slots:
    void onMethodChanged(int index);

private:
    Algorithm::CornerSearchTool* cornerTool_ = nullptr;

    // 检测方法
    QGroupBox* methodGroup_ = nullptr;
    QComboBox* methodCombo_ = nullptr;
    QLabel* methodDescLabel_ = nullptr;

    // Harris参数
    QGroupBox* harrisGroup_ = nullptr;
    QSpinBox* harrisBlockSizeSpin_ = nullptr;
    QSpinBox* harrisKSizeSpin_ = nullptr;
    QDoubleSpinBox* harrisKSpin_ = nullptr;
    QDoubleSpinBox* harrisThresholdSpin_ = nullptr;
    QSlider* harrisThresholdSlider_ = nullptr;

    // Shi-Tomasi参数
    QGroupBox* shiTomasiGroup_ = nullptr;
    QSpinBox* shiTomasiMaxCornersSpin_ = nullptr;
    QDoubleSpinBox* shiTomasiQualitySpin_ = nullptr;
    QDoubleSpinBox* shiTomasiMinDistSpin_ = nullptr;
    QSpinBox* shiTomasiBlockSizeSpin_ = nullptr;

    // FAST参数
    QGroupBox* fastGroup_ = nullptr;
    QSpinBox* fastThresholdSpin_ = nullptr;
    QCheckBox* fastNMSCheck_ = nullptr;
    QComboBox* fastTypeCombo_ = nullptr;

    // 过滤参数
    QGroupBox* filterGroup_ = nullptr;
    QSpinBox* maxCornersSpin_ = nullptr;
    QDoubleSpinBox* minDistanceSpin_ = nullptr;
    QCheckBox* useSubPixelCheck_ = nullptr;
    QSpinBox* subPixelWinSizeSpin_ = nullptr;
};

} // namespace UI
} // namespace VisionForge
