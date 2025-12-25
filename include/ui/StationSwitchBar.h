/**
 * @file StationSwitchBar.h
 * @brief 工位切换工具栏
 *
 * 显示所有工位按钮，支持快速切换 (F1-F6)
 */

#ifndef VISIONFORGE_UI_STATION_SWITCH_BAR_H
#define VISIONFORGE_UI_STATION_SWITCH_BAR_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QButtonGroup>
#include <QVector>

namespace VisionForge {
namespace UI {

/**
 * @brief 工位切换工具栏
 *
 * 功能：
 * - 显示最多6个工位按钮
 * - 支持"全部"按钮显示所有工位
 * - 显示工位状态指示（OK/NG）
 * - 支持 F1-F6 快捷键
 */
class StationSwitchBar : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     */
    explicit StationSwitchBar(QWidget* parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~StationSwitchBar() override;

    // ========== 工位管理 ==========

    /**
     * @brief 刷新工位按钮（从 MultiStationManager 获取）
     */
    void refreshStations();

    /**
     * @brief 设置当前选中的工位（通过索引）
     */
    void setCurrentStation(int index);

    /**
     * @brief 设置当前选中的工位（通过ID）
     */
    void setCurrentStation(const QString& stationId);

    /**
     * @brief 获取当前选中的工位索引
     */
    int currentStationIndex() const { return currentIndex_; }

    // ========== 显示设置 ==========

    /**
     * @brief 设置是否显示"全部"按钮
     */
    void setShowAllButton(bool show);

    /**
     * @brief 是否显示"全部"按钮
     */
    bool isShowAllButton() const { return showAllButton_; }

    /**
     * @brief 设置是否显示工位状态
     */
    void setShowStationStatus(bool show);

    /**
     * @brief 是否显示工位状态
     */
    bool isShowStationStatus() const { return showStatus_; }

    // ========== 状态更新 ==========

    /**
     * @brief 设置工位状态
     * @param index 工位索引
     * @param ok 是否OK
     */
    void setStationStatus(int index, bool ok);

    /**
     * @brief 清除所有状态
     */
    void clearAllStatus();

    // ========== 快捷键 ==========

    /**
     * @brief 安装全局快捷键 (F1-F6)
     * @param parent 父窗口（用于接收快捷键）
     */
    void installShortcuts(QWidget* parent);

signals:
    /**
     * @brief 工位被选中
     */
    void stationSelected(int index);

    /**
     * @brief 工位被选中（通过ID）
     */
    void stationSelectedById(const QString& stationId);

    /**
     * @brief 请求显示所有工位
     */
    void showAllStations();

private slots:
    void onStationButtonClicked(int index);
    void onShowAllClicked();

private:
    /**
     * @brief 初始化UI
     */
    void setupUI();

    /**
     * @brief 更新按钮状态
     */
    void updateButtonStates();

    /**
     * @brief 创建工位按钮
     */
    QPushButton* createStationButton(int index, const QString& name);

    /**
     * @brief 创建状态指示灯
     */
    QLabel* createStatusLabel();

    // ========== 成员变量 ==========
    QHBoxLayout* layout_;                    ///< 主布局
    QPushButton* btnShowAll_;                ///< "全部"按钮
    QVector<QPushButton*> stationBtns_;      ///< 工位按钮列表
    QVector<QLabel*> statusLabels_;          ///< 状态指示灯列表
    QVector<QString> stationIds_;            ///< 工位ID列表
    QButtonGroup* buttonGroup_;              ///< 按钮组（互斥选择）

    int currentIndex_ = 0;                   ///< 当前选中索引
    bool showAllButton_ = true;              ///< 是否显示"全部"按钮
    bool showStatus_ = true;                 ///< 是否显示状态

    // 样式
    QString normalBtnStyle_;                 ///< 普通按钮样式
    QString selectedBtnStyle_;               ///< 选中按钮样式
    QString okStatusStyle_;                  ///< OK状态样式
    QString ngStatusStyle_;                  ///< NG状态样式
    QString unknownStatusStyle_;             ///< 未知状态样式
};

} // namespace UI
} // namespace VisionForge

#endif // VISIONFORGE_UI_STATION_SWITCH_BAR_H
