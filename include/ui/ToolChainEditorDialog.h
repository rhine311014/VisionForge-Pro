/**
 * @file ToolChainEditorDialog.h
 * @brief 工具链编辑器对话框 - 参考VisionASM设计
 * @details 提供工具添加、参数调整、训练等完整功能
 * @author VisionForge Team
 * @date 2025-01-04
 */

#pragma once

#include <QDialog>
#include <QVector>
#include <QString>
#include <QJsonObject>
#include <memory>

class QTableWidget;
class QTreeWidget;
class QTreeWidgetItem;
class QComboBox;
class QPushButton;
class QLabel;
class QLineEdit;
class QGroupBox;
class QSplitter;
class QVBoxLayout;
class QHBoxLayout;
class QStackedWidget;
class QScrollArea;
class QDoubleSpinBox;
class QSpinBox;
class QCheckBox;
class QRadioButton;
class QToolButton;
class QButtonGroup;
class QTabWidget;
class QListWidget;
class QSlider;

namespace VisionForge {

// 前向声明
namespace Algorithm {
    class VisionTool;
}

namespace UI {

class HalconImageViewer;

/**
 * @brief 工具类别
 */
enum class ToolCategory {
    ImagePreprocess,    ///< 图像预处理
    PositionDetect,     ///< 位置检测
    Measurement,        ///< 测量
    Judgment,           ///< 判定
    CodeRead,           ///< 识别
    Calibration,        ///< 标定
    Output              ///< 输出
};

/**
 * @brief 工具信息
 */
struct ToolItemInfo {
    int index;                      ///< 工具索引
    QString name;                   ///< 工具名称
    QString typeName;               ///< 工具类型名
    ToolCategory category;          ///< 工具类别
    bool enabled = true;            ///< 是否启用
    double executionTime = 0.0;     ///< 执行时间(ms)
    bool isOK = true;               ///< 执行结果

    ToolItemInfo() : index(0), category(ToolCategory::PositionDetect) {}
    ToolItemInfo(int idx, const QString& toolName, const QString& type)
        : index(idx), name(toolName), typeName(type), category(ToolCategory::PositionDetect) {}
};

/**
 * @class ToolChainEditorDialog
 * @brief 工具链编辑器对话框
 *
 * 功能：
 * - 图像显示与ROI绘制
 * - 工具类别树形列表
 * - 工具列表管理（添加、删除、上移、下移）
 * - 工具参数编辑面板
 * - 训练设置（搜索区域、训练区域）
 * - 搜索设置（角度、分数、数量等）
 * - 相机参数设置
 * - 光源控制
 */
class ToolChainEditorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ToolChainEditorDialog(QWidget* parent = nullptr);
    ~ToolChainEditorDialog() override;

    /**
     * @brief 设置工位ID
     */
    void setWorkStationId(const QString& wsId);

    /**
     * @brief 设置产品索引
     */
    void setProductIndex(int index);

    /**
     * @brief 设置场景索引
     */
    void setSceneIndex(int index);

    /**
     * @brief 获取工具数量
     */
    int toolCount() const;

    /**
     * @brief 获取工具
     */
    Algorithm::VisionTool* getTool(int index) const;

    /**
     * @brief 添加工具
     * @return 新工具的索引
     */
    int addTool(Algorithm::VisionTool* tool);

    /**
     * @brief 删除工具
     */
    bool removeTool(int index);

    /**
     * @brief 加载工具链配置
     */
    bool loadConfig(const QString& configPath);

    /**
     * @brief 保存工具链配置
     */
    bool saveConfig(const QString& configPath);

signals:
    /**
     * @brief 工具选中变化
     */
    void toolSelected(int index);

    /**
     * @brief 工具配置变化
     */
    void toolChanged(int index);

    /**
     * @brief 请求采集图像
     */
    void captureImageRequested();

    /**
     * @brief 请求相机参数设置
     */
    void cameraParamRequested();

    /**
     * @brief 请求光源控制
     */
    void lightControlRequested();

    /**
     * @brief 请求执行当前工具
     */
    void executeToolRequested(int index);

    /**
     * @brief 请求执行全部工具
     */
    void executeAllRequested();

private slots:
    // 工具列表操作
    void onToolSelectionChanged();
    void onToolDoubleClicked(int row, int column);
    void onAddTool();
    void onRemoveTool();
    void onMoveToolUp();
    void onMoveToolDown();
    void onCopyTool();

    // 工具类别树操作
    void onCategoryItemClicked(QTreeWidgetItem* item, int column);
    void onCategoryItemDoubleClicked(QTreeWidgetItem* item, int column);

    // 侧边栏按钮
    void onCaptureImage();
    void onCameraParam();
    void onLightControl();
    void onRunTool();
    void onRunAll();
    void onSaveConfig();
    void onLoadConfig();

    // 训练设置
    void onTrainAreaChanged();
    void onSearchAreaChanged();
    void onTrainClicked();
    void onClearTrainClicked();

    // 搜索设置
    void onAngleRangeChanged();
    void onScoreThresholdChanged();
    void onMaxCountChanged();

    // 位置检测设置
    void onPositionModeChanged();
    void onPositionOffsetChanged();

    // 对话框按钮
    void onOkClicked();
    void onCancelClicked();
    void onApplyClicked();

private:
    void setupUI();
    void createMainLayout();
    void createLeftPanel(QWidget* parent);      ///< 图像显示区域
    void createRightPanel(QWidget* parent);     ///< 工具设置区域
    void createSideBar(QVBoxLayout* layout);    ///< 侧边栏按钮
    void createBottomPanel(QWidget* parent);    ///< 底部工具列表区域

    // 右侧面板创建
    void createToolSettingPanel();              ///< 工具设置面板
    void createTrainSettingGroup();             ///< 训练设置组
    void createSearchSettingGroup();            ///< 搜索设置组
    void createPositionSettingGroup();          ///< 位置设置组

    // 底部面板创建
    void createCategoryTree();                  ///< 工具类别树
    void createToolTable();                     ///< 工具列表表格
    void createToolManageButtons();             ///< 工具管理按钮

    void updateToolTable();
    void updateToolSettings();
    void updateCategoryTree();
    void initToolCategories();

    QString getCategoryName(ToolCategory category);
    QIcon getCategoryIcon(ToolCategory category);
    QIcon getToolIcon(const QString& toolType);

private:
    QString workStationId_;
    int productIndex_;
    int sceneIndex_;
    QVector<Algorithm::VisionTool*> tools_;
    int currentToolIndex_;

    // ========== 主布局 ==========
    QVBoxLayout* mainLayout_;
    QSplitter* mainSplitter_;           ///< 主分割器（上下）
    QWidget* topWidget_;                ///< 上部区域
    QWidget* bottomWidget_;             ///< 下部区域

    // ========== 上部区域 ==========
    QSplitter* topSplitter_;            ///< 上部分割器（左右）

    // ========== 左侧面板 - 图像显示 ==========
    QWidget* leftPanel_;
    HalconImageViewer* imageViewer_;    ///< 图像显示器
    QLabel* imageInfoLabel_;            ///< 图像信息标签

    // ========== 右侧面板 - 工具设置 ==========
    QWidget* rightPanel_;
    QScrollArea* settingScrollArea_;    ///< 设置滚动区域
    QWidget* toolSettingContainer_;     ///< 工具设置容器
    QVBoxLayout* toolSettingLayout_;    ///< 工具设置布局
    QDialog* currentToolDialog_;        ///< 当前嵌入的工具对话框

    // 训练设置组
    QGroupBox* trainSettingGroup_;
    QLabel* trainAreaLabel_;            ///< 训练区域标签
    QPushButton* trainAreaBtn_;         ///< 设置训练区域按钮
    QLabel* searchAreaLabel_;           ///< 搜索区域标签
    QPushButton* searchAreaBtn_;        ///< 设置搜索区域按钮
    QPushButton* trainBtn_;             ///< 训练按钮
    QPushButton* clearTrainBtn_;        ///< 清除训练按钮
    QCheckBox* showTrainCheck_;         ///< 显示训练区域
    QCheckBox* showSearchCheck_;        ///< 显示搜索区域

    // 搜索设置组
    QGroupBox* searchSettingGroup_;
    QDoubleSpinBox* angleStartSpin_;    ///< 起始角度
    QDoubleSpinBox* angleEndSpin_;      ///< 结束角度
    QDoubleSpinBox* angleStepSpin_;     ///< 角度步长
    QDoubleSpinBox* scoreThresholdSpin_;///< 分数阈值
    QSpinBox* maxCountSpin_;            ///< 最大数量
    QComboBox* pyramidLevelCombo_;      ///< 金字塔层数
    QComboBox* subPixelCombo_;          ///< 亚像素模式
    QCheckBox* multiMatchCheck_;        ///< 多目标匹配

    // 位置检测设置组
    QGroupBox* positionSettingGroup_;
    QComboBox* positionModeCombo_;      ///< 位置模式
    QDoubleSpinBox* offsetXSpin_;       ///< X偏移
    QDoubleSpinBox* offsetYSpin_;       ///< Y偏移
    QDoubleSpinBox* offsetAngleSpin_;   ///< 角度偏移
    QCheckBox* invertXCheck_;           ///< X取反
    QCheckBox* invertYCheck_;           ///< Y取反
    QCheckBox* invertAngleCheck_;       ///< 角度取反

    // ========== 侧边栏按钮 ==========
    QWidget* sideBarWidget_;
    QPushButton* captureBtn_;           ///< 采集图像
    QPushButton* cameraParamBtn_;       ///< 相机参数
    QPushButton* lightControlBtn_;      ///< 光源控制
    QPushButton* runToolBtn_;           ///< 运行工具
    QPushButton* runAllBtn_;            ///< 运行全部
    QPushButton* saveConfigBtn_;        ///< 保存配置
    QPushButton* loadConfigBtn_;        ///< 加载配置

    // ========== 底部面板 - 工具列表 ==========
    QSplitter* bottomSplitter_;         ///< 底部分割器

    // 工具类别树
    QWidget* categoryWidget_;
    QTreeWidget* categoryTree_;         ///< 工具类别树

    // 工具列表
    QWidget* toolListWidget_;
    QTableWidget* toolTable_;           ///< 工具列表表格
    QPushButton* addToolBtn_;           ///< 添加工具
    QPushButton* removeToolBtn_;        ///< 删除工具
    QPushButton* moveUpBtn_;            ///< 上移
    QPushButton* moveDownBtn_;          ///< 下移
    QPushButton* copyToolBtn_;          ///< 复制工具

    // 位置检测设置（底部右侧）
    QWidget* positionWidget_;
    QLabel* positionImageLabel_;        ///< 位置示意图

    // ========== 底部按钮 ==========
    QPushButton* okBtn_;
    QPushButton* cancelBtn_;
    QPushButton* applyBtn_;
};

} // namespace UI
} // namespace VisionForge
