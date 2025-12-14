/**
 * @file ResultTablePanel.h
 * @brief 结果数据表格面板
 * @details 显示视觉工具处理结果的数据表格
 * @author VisionForge Team
 * @date 2025-12-14
 */

#pragma once

#include "algorithm/VisionTool.h"
#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QToolBar>
#include <QVBoxLayout>
#include <QAction>

namespace VisionForge {
namespace UI {

/**
 * @class ResultTablePanel
 * @brief 结果数据表格面板
 *
 * 功能：
 * - 显示工具处理结果的数据表格
 * - 支持多个工具的结果同时显示
 * - 提供导出CSV功能
 * - 实时更新显示
 */
class ResultTablePanel : public QWidget
{
    Q_OBJECT

public:
    explicit ResultTablePanel(QWidget* parent = nullptr);
    ~ResultTablePanel() override = default;

    /**
     * @brief 添加工具结果
     * @param toolName 工具名称
     * @param result 工具处理结果
     */
    void addResult(const QString& toolName, const Algorithm::ToolResult& result);

    /**
     * @brief 更新工具结果
     * @param toolName 工具名称
     * @param result 工具处理结果
     */
    void updateResult(const QString& toolName, const Algorithm::ToolResult& result);

    /**
     * @brief 清除所有结果
     */
    void clearResults();

    /**
     * @brief 导出为CSV文件
     * @param filePath 文件路径
     * @return 是否成功
     */
    bool exportToCSV(const QString& filePath);

signals:
    /**
     * @brief 结果更新信号
     */
    void resultsUpdated();

private slots:
    /**
     * @brief 清除按钮点击
     */
    void onClearClicked();

    /**
     * @brief 导出按钮点击
     */
    void onExportClicked();

    /**
     * @brief 复制选中项
     */
    void onCopyClicked();

private:
    /**
     * @brief 初始化UI
     */
    void setupUI();

    /**
     * @brief 创建工具栏
     */
    void createToolBar();

    /**
     * @brief 格式化结果值
     */
    QString formatValue(const QVariant& value);

    /**
     * @brief 添加结果行
     */
    void addResultRow(const QString& toolName, const QString& key, const QVariant& value, double executionTime = 0.0);

private:
    QVBoxLayout* mainLayout_;
    QToolBar* toolBar_;
    QTableWidget* table_;

    QAction* clearAction_;
    QAction* exportAction_;
    QAction* copyAction_;

    int rowCount_;
};

} // namespace UI
} // namespace VisionForge
