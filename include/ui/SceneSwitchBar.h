/**
 * @file SceneSwitchBar.h
 * @brief 场景切换工具栏
 *
 * 在当前工位内切换不同检测场景
 */

#ifndef VISIONFORGE_UI_SCENE_SWITCH_BAR_H
#define VISIONFORGE_UI_SCENE_SWITCH_BAR_H

#include <QWidget>
#include <QHBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QVector>

namespace VisionForge {

namespace Core {
class SceneManager;
struct StationConfig;
}

namespace UI {

/**
 * @brief 场景切换工具栏
 *
 * 功能：
 * - 显示当前工位的所有场景
 * - 下拉选择或按钮切换场景
 * - 显示当前场景名称
 * - 支持快速切换（上一个/下一个）
 */
class SceneSwitchBar : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     */
    explicit SceneSwitchBar(QWidget* parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~SceneSwitchBar() override;

    /**
     * @brief 设置场景管理器
     * @param manager 场景管理器指针
     */
    void setSceneManager(Core::SceneManager* manager);

    /**
     * @brief 刷新场景列表
     * @param stationConfig 工位配置（获取场景列表）
     */
    void refreshScenes(Core::StationConfig* stationConfig);

    /**
     * @brief 设置当前场景
     * @param index 场景索引
     */
    void setCurrentScene(int index);

    /**
     * @brief 获取当前场景索引
     */
    int currentSceneIndex() const { return currentIndex_; }

    /**
     * @brief 获取场景数量
     */
    int sceneCount() const { return sceneNames_.size(); }

    /**
     * @brief 设置是否启用
     */
    void setEnabled(bool enabled);

signals:
    /**
     * @brief 场景被选中
     * @param index 场景索引
     */
    void sceneSelected(int index);

    /**
     * @brief 场景被选中（通过ID）
     * @param sceneId 场景ID
     */
    void sceneSelectedById(const QString& sceneId);

    /**
     * @brief 请求切换到上一个场景
     */
    void previousSceneRequested();

    /**
     * @brief 请求切换到下一个场景
     */
    void nextSceneRequested();

private slots:
    void onSceneComboChanged(int index);
    void onPreviousClicked();
    void onNextClicked();
    void onSceneChanged(int sceneIndex, const QString& sceneName);
    void onSceneLoadProgress(int progress, const QString& message);

private:
    /**
     * @brief 初始化UI
     */
    void setupUI();

    /**
     * @brief 连接信号
     */
    void connectSignals();

    /**
     * @brief 更新按钮状态
     */
    void updateButtonStates();

    // ========== UI 组件 ==========
    QHBoxLayout* layout_;               ///< 主布局
    QLabel* labelScene_;                ///< "场景:"标签
    QComboBox* comboScene_;             ///< 场景下拉框
    QPushButton* btnPrevious_;          ///< 上一个按钮
    QPushButton* btnNext_;              ///< 下一个按钮
    QLabel* labelStatus_;               ///< 状态标签

    // ========== 数据 ==========
    Core::SceneManager* sceneManager_ = nullptr;  ///< 场景管理器
    QVector<QString> sceneIds_;         ///< 场景ID列表
    QVector<QString> sceneNames_;       ///< 场景名称列表
    int currentIndex_ = 0;              ///< 当前场景索引
    bool isUpdating_ = false;           ///< 是否正在更新UI
};

} // namespace UI
} // namespace VisionForge

#endif // VISIONFORGE_UI_SCENE_SWITCH_BAR_H
