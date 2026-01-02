/**
 * @file CornerSearchToolDialog.h
 * @brief 角点搜索工具对话框
 * @details 基于Harris/Shi-Tomasi/FAST等算法的角点检测工具。
 *          角点是图像中亮度变化剧烈的点，常用于特征提取和图像匹配。
 *
 * 主要功能：
 * - 支持多种角点检测算法（Harris、Shi-Tomasi、FAST、ORB）
 * - 提供各算法的专用参数设置
 * - 支持亚像素精度角点定位
 * - 提供结果过滤功能（数量限制、距离过滤）
 *
 * 算法说明：
 * - Harris: 经典的角点检测算法，对旋转和亮度变化鲁棒
 * - Shi-Tomasi: Harris的改进版，特征点更稳定
 * - FAST: 快速角点检测，适合实时应用
 * - ORB: 带方向的FAST特征点，具有旋转不变性
 *
 * @author VisionForge Team
 * @date 2025-12-30
 */

#pragma once

#include "ui/SearchToolDialog.h"

namespace VisionForge {

namespace Algorithm {
    class CornerSearchTool;  // 前向声明角点搜索工具类
}

namespace UI {

/**
 * @brief 角点搜索工具对话框
 *
 * 提供多种角点检测算法的参数配置和搜索功能。
 * 支持Harris、Shi-Tomasi、FAST等多种角点检测方法。
 *
 * 使用流程：
 * 1. 加载图像
 * 2. 选择检测方法
 * 3. 设置算法参数
 * 4. 执行检测预览
 * 5. 调整过滤参数获得理想结果
 *
 * 继承关系：CornerSearchToolDialog -> SearchToolDialog -> IToolDialog
 */
class CornerSearchToolDialog : public SearchToolDialog
{
    Q_OBJECT

public:
    /**
     * @brief 角点检测方法枚举
     * @details 定义支持的角点检测算法
     */
    enum CornerMethod {
        Harris = 0,         ///< Harris角点检测 - 经典算法，基于自相关矩阵
        ShiTomasi,          ///< Shi-Tomasi角点检测 - 改进的Harris，使用最小特征值
        FAST,               ///< FAST角点检测 - 快速特征检测，适合实时应用
        ORB,                ///< ORB特征点检测 - Oriented FAST with Rotated BRIEF
        HalconPoints        ///< Halcon兴趣点检测 - 使用Halcon的点检测算法
    };

    /**
     * @brief 构造函数
     * @param tool 关联的角点搜索工具指针
     * @param parent 父窗口指针
     */
    explicit CornerSearchToolDialog(Algorithm::CornerSearchTool* tool, QWidget* parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~CornerSearchToolDialog() override;

protected:
    // ==================== SearchToolDialog接口实现 ====================

    /**
     * @brief 创建参数面板
     * @details 创建角点检测的参数设置控件，包括方法选择和各算法参数
     * @param parent 父控件
     * @return 参数面板控件指针
     */
    QWidget* createParameterPanel(QWidget* parent) override;

    /**
     * @brief 获取工具名称
     * @return 返回"角点搜索"
     */
    QString toolName() const override { return tr("角点搜索"); }

    /**
     * @brief 执行角点检测
     * @details 根据选择的算法执行角点检测
     * @param results 输出检测结果
     * @return 检测是否成功
     */
    bool executeSearch(std::vector<SearchResult>& results) override;

    /**
     * @brief 应用参数到工具对象
     */
    void applySpecificParameters() override;

    /**
     * @brief 从工具对象更新UI
     */
    void updateSpecificUI() override;

private:
    // ==================== UI创建方法 ====================

    /**
     * @brief 创建检测方法选择组
     * @param layout 父布局
     */
    void createMethodGroup(QVBoxLayout* layout);

    /**
     * @brief 创建Harris算法参数组
     * @param layout 父布局
     */
    void createHarrisParamGroup(QVBoxLayout* layout);

    /**
     * @brief 创建Shi-Tomasi算法参数组
     * @param layout 父布局
     */
    void createShiTomasiParamGroup(QVBoxLayout* layout);

    /**
     * @brief 创建FAST算法参数组
     * @param layout 父布局
     */
    void createFASTParamGroup(QVBoxLayout* layout);

    /**
     * @brief 创建结果过滤参数组
     * @param layout 父布局
     */
    void createFilterGroup(QVBoxLayout* layout);

    /**
     * @brief 连接特定信号槽
     */
    void connectSpecificSignals();

    /**
     * @brief 更新参数面板可见性
     * @details 根据选择的检测方法显示/隐藏对应的参数组
     */
    void updateParameterVisibility();

private slots:
    /**
     * @brief 检测方法切换槽
     * @param index 新选择的方法索引
     */
    void onMethodChanged(int index);

private:
    // ==================== 成员变量 ====================

    Algorithm::CornerSearchTool* cornerTool_ = nullptr;  ///< 角点搜索工具指针

    // ==================== UI组件 - 检测方法选择 ====================

    QGroupBox* methodGroup_ = nullptr;             ///< 检测方法分组框
    QComboBox* methodCombo_ = nullptr;             ///< 检测方法下拉框
    QLabel* methodDescLabel_ = nullptr;            ///< 方法描述标签

    // ==================== UI组件 - Harris算法参数 ====================

    QGroupBox* harrisGroup_ = nullptr;             ///< Harris参数分组框
    QSpinBox* harrisBlockSizeSpin_ = nullptr;      ///< 邻域大小（用于计算协方差矩阵）
    QSpinBox* harrisKSizeSpin_ = nullptr;          ///< Sobel算子孔径大小（必须为奇数）
    QDoubleSpinBox* harrisKSpin_ = nullptr;        ///< Harris自由参数k（通常0.04-0.06）
    QDoubleSpinBox* harrisThresholdSpin_ = nullptr; ///< 响应阈值（0-1，归一化后）
    QSlider* harrisThresholdSlider_ = nullptr;     ///< 响应阈值滑块

    // ==================== UI组件 - Shi-Tomasi算法参数 ====================

    QGroupBox* shiTomasiGroup_ = nullptr;          ///< Shi-Tomasi参数分组框
    QSpinBox* shiTomasiMaxCornersSpin_ = nullptr;  ///< 最大检测角点数
    QDoubleSpinBox* shiTomasiQualitySpin_ = nullptr; ///< 质量水平（最佳角点响应的比例阈值）
    QDoubleSpinBox* shiTomasiMinDistSpin_ = nullptr; ///< 角点之间的最小欧几里得距离
    QSpinBox* shiTomasiBlockSizeSpin_ = nullptr;   ///< 邻域大小

    // ==================== UI组件 - FAST算法参数 ====================

    QGroupBox* fastGroup_ = nullptr;               ///< FAST参数分组框
    QSpinBox* fastThresholdSpin_ = nullptr;        ///< 中心像素与周围像素的亮度差阈值
    QCheckBox* fastNMSCheck_ = nullptr;            ///< 是否启用非极大值抑制
    QComboBox* fastTypeCombo_ = nullptr;           ///< FAST检测类型（9_16/7_12/5_8）

    // ==================== UI组件 - 结果过滤参数 ====================

    QGroupBox* filterGroup_ = nullptr;             ///< 过滤参数分组框
    QSpinBox* maxCornersSpin_ = nullptr;           ///< 最大保留角点数
    QDoubleSpinBox* minDistanceSpin_ = nullptr;    ///< 角点最小间距（像素）
    QCheckBox* useSubPixelCheck_ = nullptr;        ///< 是否使用亚像素精化
    QSpinBox* subPixelWinSizeSpin_ = nullptr;      ///< 亚像素精化窗口大小
};

} // namespace UI
} // namespace VisionForge
