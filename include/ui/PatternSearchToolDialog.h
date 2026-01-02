/**
 * @file PatternSearchToolDialog.h
 * @brief 图案搜索工具对话框
 * @details 基于Halcon形状匹配的图案搜索工具
 * @author VisionForge Team
 * @date 2025-12-30
 */

#pragma once

#include "ui/SearchToolDialog.h"
#include <QLineEdit>
#include <QRadioButton>

namespace VisionForge {

namespace Algorithm {
    class PatternSearchTool;
}

namespace UI {

/**
 * @brief 图案搜索工具对话框
 *
 * 提供图案（模板）创建、参数配置和搜索功能。
 * 支持灰度匹配、边缘匹配等多种匹配模式。
 */
class PatternSearchToolDialog : public SearchToolDialog
{
    Q_OBJECT

public:
    /**
     * @brief 匹配模式
     */
    enum MatchMode {
        GrayMatch = 0,      ///< 灰度匹配
        EdgeMatch,          ///< 边缘匹配
        CorrelationMatch    ///< 相关匹配
    };

    explicit PatternSearchToolDialog(Algorithm::PatternSearchTool* tool, QWidget* parent = nullptr);
    ~PatternSearchToolDialog() override;

protected:
    // SearchToolDialog接口实现
    QWidget* createParameterPanel(QWidget* parent) override;
    QString toolName() const override { return tr("图案搜索"); }
    bool executeSearch(std::vector<SearchResult>& results) override;
    void applySpecificParameters() override;
    void updateSpecificUI() override;

private:
    void createMatchModeGroup(QVBoxLayout* layout);
    void createPatternGroup(QVBoxLayout* layout);
    void createSearchParamGroup(QVBoxLayout* layout);
    void createAngleScaleGroup(QVBoxLayout* layout);
    void connectSpecificSignals();

private slots:
    void onCreatePatternClicked();
    void onLoadPatternClicked();
    void onSavePatternClicked();
    void onClearPatternClicked();
    void onMatchModeChanged(int mode);

private:
    Algorithm::PatternSearchTool* patternTool_ = nullptr;

    // 匹配模式
    QGroupBox* matchModeGroup_ = nullptr;
    QRadioButton* radioGrayMatch_ = nullptr;
    QRadioButton* radioEdgeMatch_ = nullptr;
    QRadioButton* radioCorrelationMatch_ = nullptr;

    // 图案管理
    QGroupBox* patternGroup_ = nullptr;
    QPushButton* createPatternBtn_ = nullptr;
    QPushButton* loadPatternBtn_ = nullptr;
    QPushButton* savePatternBtn_ = nullptr;
    QPushButton* clearPatternBtn_ = nullptr;
    QLabel* patternStatusLabel_ = nullptr;
    QLabel* patternPreviewLabel_ = nullptr;

    // 搜索参数
    QGroupBox* searchParamGroup_ = nullptr;
    QDoubleSpinBox* minScoreSpin_ = nullptr;
    QSlider* minScoreSlider_ = nullptr;
    QSpinBox* maxMatchesSpin_ = nullptr;
    QDoubleSpinBox* overlapSpin_ = nullptr;
    QComboBox* subPixelCombo_ = nullptr;
    QSpinBox* pyramidLevelsSpin_ = nullptr;

    // 角度缩放参数
    QGroupBox* angleScaleGroup_ = nullptr;
    QCheckBox* enableAngleCheck_ = nullptr;
    QDoubleSpinBox* angleMinSpin_ = nullptr;
    QDoubleSpinBox* angleMaxSpin_ = nullptr;
    QDoubleSpinBox* angleStepSpin_ = nullptr;
    QCheckBox* enableScaleCheck_ = nullptr;
    QDoubleSpinBox* scaleMinSpin_ = nullptr;
    QDoubleSpinBox* scaleMaxSpin_ = nullptr;
    QDoubleSpinBox* scaleStepSpin_ = nullptr;
    QWidget* angleWidget_ = nullptr;
    QWidget* scaleWidget_ = nullptr;
};

} // namespace UI
} // namespace VisionForge
