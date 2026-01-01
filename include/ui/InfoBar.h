/**
 * @file InfoBar.h
 * @brief 顶部信息栏
 * @details 显示当前位置、检测信息、运行状态等
 * @author VisionForge Team
 * @date 2025-12-31
 */

#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QTabBar>

namespace VisionForge {
namespace UI {

/**
 * @class InfoBar
 * @brief 顶部信息栏
 *
 * 功能特点：
 * - 场景标签页切换
 * - 位置选择下拉框
 * - 检测信息显示
 * - 运行状态信息
 * - 快捷功能按钮
 */
class InfoBar : public QWidget
{
    Q_OBJECT

public:
    explicit InfoBar(QWidget* parent = nullptr);
    ~InfoBar() override;

    /**
     * @brief 设置场景列表
     * @param scenes 场景名称列表
     */
    void setScenes(const QStringList& scenes);

    /**
     * @brief 设置当前场景
     * @param index 场景索引
     */
    void setCurrentScene(int index);

    /**
     * @brief 获取当前场景索引
     */
    int currentScene() const;

    /**
     * @brief 设置位置列表
     * @param positions 位置名称列表
     */
    void setPositions(const QStringList& positions);

    /**
     * @brief 设置当前位置
     * @param index 位置索引
     */
    void setCurrentPosition(int index);

    /**
     * @brief 获取当前位置索引
     */
    int currentPosition() const;

    /**
     * @brief 设置检测信息
     * @param info 检测信息文本
     */
    void setDetectionInfo(const QString& info);

    /**
     * @brief 设置运行信息
     * @param info 运行信息文本
     */
    void setRunInfo(const QString& info);

    /**
     * @brief 设置检测结果
     * @param ok 是否OK
     * @param message 结果消息
     */
    void setDetectionResult(bool ok, const QString& message = QString());

    /**
     * @brief 清除显示信息
     */
    void clearInfo();

signals:
    void sceneChanged(int index);
    void positionChanged(int index);
    void clearDisplayClicked();
    void virtualCommClicked();
    void detectionInfoClicked();
    void runInfoClicked();

private:
    void setupUI();

private:
    QHBoxLayout* layout_;

    // 左侧场景标签
    QTabBar* sceneTabBar_;

    // 右侧控件
    QComboBox* positionCombo_;
    QPushButton* btnDetectionInfo_;
    QPushButton* btnRunInfo_;
    QPushButton* btnClearDisplay_;
    QPushButton* btnVirtualComm_;

    // 状态显示
    QLabel* resultLabel_;
};

} // namespace UI
} // namespace VisionForge
