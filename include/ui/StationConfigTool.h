/**
 * @file StationConfigTool.h
 * @brief 工位配置工具
 *
 * 参考 VisionASM VisionInspectConfigTool 设计
 * 在软件启动前配置工位、相机、位置等参数
 */

#ifndef VISIONFORGE_UI_STATION_CONFIG_TOOL_H
#define VISIONFORGE_UI_STATION_CONFIG_TOOL_H

#include <QDialog>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QGroupBox>
#include <QLabel>
#include <QVector>

namespace VisionForge {

namespace Core {
struct StationConfig;
}

namespace UI {

/**
 * @brief 工位配置工具对话框
 *
 * 功能：
 * - 左侧树形显示所有平台/工位配置
 * - 右侧编辑选中平台的详细参数
 * - 支持新增/删除平台
 * - 保存配置到文件
 */
class StationConfigTool : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     */
    explicit StationConfigTool(QWidget* parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~StationConfigTool() override;

    /**
     * @brief 获取是否有配置变更
     */
    bool hasChanges() const { return hasChanges_; }

    /**
     * @brief 是否保存并退出
     */
    bool isSaveAndExit() const { return saveAndExit_; }

    /**
     * @brief 获取配置的工位数量
     */
    int getStationCount() const;

private slots:
    // 树形控件
    void onTreeItemClicked(QTreeWidgetItem* item, int column);
    void onTreeItemChanged(QTreeWidgetItem* item, int column);

    // 参数变更
    void onPlatformIndexChanged(int index);
    void onCameraCountChanged(int value);
    void onSharedCameraChanged(int state);
    void onSceneCountChanged(int value);
    void onPositionCountChanged(int value);
    void onDisplayModeChanged(int index);
    void onProductCountChanged(int value);
    void onPreloadCountChanged(int value);
    void onFunctionModeChanged(int index);

    // 按钮操作
    void onAddPlatform();
    void onDeletePlatform();
    void onSaveAndExit();
    void onCancelExit();

private:
    /**
     * @brief 初始化UI
     */
    void setupUI();

    /**
     * @brief 创建左侧树形区域
     */
    QWidget* createTreeArea();

    /**
     * @brief 创建右侧设置区域
     */
    QGroupBox* createSettingsArea();

    /**
     * @brief 创建底部按钮区域
     */
    QWidget* createButtonArea();

    /**
     * @brief 建立信号连接
     */
    void setupConnections();

    /**
     * @brief 加载现有配置
     */
    void loadExistingConfig();

    /**
     * @brief 保存配置
     */
    bool saveConfig();

    /**
     * @brief 刷新树形显示
     */
    void refreshTree();

    /**
     * @brief 更新树形项目显示
     */
    void updateTreeItem(QTreeWidgetItem* item, int platformIndex);

    /**
     * @brief 更新右侧设置面板
     */
    void updateSettingsPanel(int platformIndex);

    /**
     * @brief 启用/禁用设置面板
     */
    void setSettingsPanelEnabled(bool enabled);

    /**
     * @brief 标记已修改
     */
    void markAsChanged();

    /**
     * @brief 获取显示模式文本
     */
    QString getDisplayModeText(int mode) const;

    /**
     * @brief 获取功能模式文本
     */
    QString getFunctionModeText(int mode) const;

    /**
     * @brief 根据功能模式更新UI可见性
     */
    void updateFunctionModeUI(int mode);

    // ========== 平台配置数据结构 ==========
    struct PlatformConfig {
        int index = 0;              ///< 平台序号 (1-6)
        int cameraCount = 1;        ///< 相机数量 (1-8)
        bool sharedCamera = false;  ///< 是否共享相机系统
        int sceneCount = 2;         ///< 场景数量 (1-8)
        int positionCount = 4;      ///< 位置数量 (1-8)
        int displayMode = 0;        ///< 显示模式 (0=按位置, 1=按相机)
        int productCount = 5;       ///< 产品数量 (1-20)
        int preloadCount = 1;       ///< 产品预加载数量 (1-5)
        int functionMode = 2;       ///< 功能模式 (0=仅检测, 1=仅对位, 2=检测+对位)

        // 检测功能参数
        float detectionConfidence = 0.5f;   ///< 检测置信度阈值
        float detectionNms = 0.4f;          ///< 检测NMS阈值

        // 对位功能参数
        double alignmentToleranceX = 0.1;   ///< 对位X容差 (mm)
        double alignmentToleranceY = 0.1;   ///< 对位Y容差 (mm)
        double alignmentToleranceD = 0.1;   ///< 对位角度容差 (度)
    };

    // ========== UI 组件 ==========
    QTreeWidget* treeWidget_;           ///< 左侧树形控件
    QTreeWidgetItem* rootItem_;         ///< 根节点

    // 右侧设置控件
    QGroupBox* settingsGroup_;
    QComboBox* comboPlatformIndex_;     ///< 平台序
    QComboBox* comboCameraCount_;       ///< 相机数
    QCheckBox* checkSharedCamera_;      ///< 相机是否共享系统
    QComboBox* comboSceneCount_;        ///< 场景数量
    QComboBox* comboPositionCount_;     ///< 位置数量
    QComboBox* comboDisplayMode_;       ///< 显示模式
    QComboBox* comboProductCount_;      ///< 产品数量
    QComboBox* comboPreloadCount_;      ///< 产品预加载数量
    QComboBox* comboFunctionMode_;      ///< 功能模式

    // 检测参数组
    QGroupBox* detectionGroup_;         ///< 检测参数分组
    QDoubleSpinBox* spinDetectionConfidence_;   ///< 检测置信度
    QDoubleSpinBox* spinDetectionNms_;          ///< 检测NMS阈值

    // 对位参数组
    QGroupBox* alignmentGroup_;         ///< 对位参数分组
    QDoubleSpinBox* spinAlignToleranceX_;   ///< X容差
    QDoubleSpinBox* spinAlignToleranceY_;   ///< Y容差
    QDoubleSpinBox* spinAlignToleranceD_;   ///< 角度容差

    // 底部按钮
    QPushButton* btnAddPlatform_;       ///< 新增平台
    QPushButton* btnDeletePlatform_;    ///< 删除平台
    QPushButton* btnSaveExit_;          ///< 保存退出
    QPushButton* btnCancelExit_;        ///< 取消退出

    // ========== 数据成员 ==========
    QVector<PlatformConfig> platforms_; ///< 平台配置列表
    int currentPlatformIndex_ = -1;     ///< 当前选中的平台索引
    bool hasChanges_ = false;           ///< 是否有修改
    bool saveAndExit_ = false;          ///< 是否保存并退出
    bool isUpdating_ = false;           ///< 是否正在更新UI（防止循环触发）
};

} // namespace UI
} // namespace VisionForge

#endif // VISIONFORGE_UI_STATION_CONFIG_TOOL_H
