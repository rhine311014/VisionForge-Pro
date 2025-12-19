/**
 * @file StatisticsPanel.h
 * @brief 数据统计面板
 * @details 显示检测统计、执行时间、日志分布等统计信息
 * @author VisionForge Team
 * @date 2025-12-19
 */

#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QTableWidget>
#include <QProgressBar>
#include <QTimer>
#include <QDateTime>
#include <QVector>

namespace VisionForge {
namespace UI {

/**
 * @brief 单次运行记录
 */
struct RunRecord {
    QDateTime timestamp;        // 时间戳
    bool success;               // 是否成功(OK/NG)
    double executionTime;       // 执行时间(ms)
    QString toolChainName;      // 工具链名称
    int toolCount;              // 执行的工具数量

    RunRecord()
        : success(true)
        , executionTime(0.0)
        , toolCount(0)
    {}
};

/**
 * @brief 统计汇总数据
 */
struct StatisticsSummary {
    // 运行统计
    int totalRuns;              // 总运行次数
    int okCount;                // OK次数
    int ngCount;                // NG次数
    double okRate;              // 良品率(%)

    // 时间统计
    double minTime;             // 最短时间
    double maxTime;             // 最长时间
    double avgTime;             // 平均时间
    double totalTime;           // 总时间

    // 日志统计
    int debugCount;
    int infoCount;
    int warningCount;
    int errorCount;
    int criticalCount;

    StatisticsSummary()
        : totalRuns(0), okCount(0), ngCount(0), okRate(0.0)
        , minTime(0.0), maxTime(0.0), avgTime(0.0), totalTime(0.0)
        , debugCount(0), infoCount(0), warningCount(0)
        , errorCount(0), criticalCount(0)
    {}
};

/**
 * @class StatisticsPanel
 * @brief 数据统计面板
 *
 * 功能：
 * - 显示检测结果统计（OK/NG数量和比率）
 * - 显示执行时间统计（最小/最大/平均）
 * - 显示日志级别分布
 * - 支持时间范围筛选
 * - 支持统计数据重置
 * - 支持导出统计报告
 */
class StatisticsPanel : public QWidget
{
    Q_OBJECT

public:
    explicit StatisticsPanel(QWidget* parent = nullptr);
    ~StatisticsPanel() override = default;

    /**
     * @brief 记录一次运行
     * @param success 是否成功
     * @param executionTime 执行时间(ms)
     * @param toolChainName 工具链名称
     * @param toolCount 工具数量
     */
    void recordRun(bool success, double executionTime,
                   const QString& toolChainName = QString(),
                   int toolCount = 0);

    /**
     * @brief 获取统计汇总
     */
    StatisticsSummary getSummary() const;

    /**
     * @brief 获取运行记录列表
     * @param maxRecords 最大记录数（0为全部）
     */
    QVector<RunRecord> getRunRecords(int maxRecords = 0) const;

    /**
     * @brief 重置统计数据
     */
    void resetStatistics();

    /**
     * @brief 设置最大保留记录数
     */
    void setMaxRecordCount(int maxCount);

signals:
    /**
     * @brief 统计数据更新信号
     */
    void statisticsUpdated(const StatisticsSummary& summary);

    /**
     * @brief 请求导出报告
     */
    void exportRequested();

public slots:
    /**
     * @brief 刷新显示
     */
    void refreshDisplay();

    /**
     * @brief 更新日志统计
     */
    void updateLogStatistics();

private slots:
    void onResetClicked();
    void onExportClicked();
    void onTimeRangeChanged(int index);
    void onAutoRefreshTimer();

private:
    void setupUI();
    void createRunStatisticsGroup();
    void createTimeStatisticsGroup();
    void createLogStatisticsGroup();
    void createControlsGroup();
    void createRecentRunsTable();

    void updateRunStatisticsDisplay();
    void updateTimeStatisticsDisplay();
    void updateLogStatisticsDisplay();
    void updateRecentRunsTable();

    void recalculateStatistics();
    QString formatTime(double ms) const;
    QString formatRate(double rate) const;

private:
    // 主布局
    QVBoxLayout* mainLayout_;

    // 运行统计组
    QGroupBox* runStatsGroup_;
    QLabel* totalRunsLabel_;
    QLabel* okCountLabel_;
    QLabel* ngCountLabel_;
    QLabel* okRateLabel_;
    QProgressBar* okRateBar_;

    // 时间统计组
    QGroupBox* timeStatsGroup_;
    QLabel* minTimeLabel_;
    QLabel* maxTimeLabel_;
    QLabel* avgTimeLabel_;
    QLabel* totalTimeLabel_;

    // 日志统计组
    QGroupBox* logStatsGroup_;
    QLabel* debugCountLabel_;
    QLabel* infoCountLabel_;
    QLabel* warningCountLabel_;
    QLabel* errorCountLabel_;
    QLabel* criticalCountLabel_;

    // 控制区
    QGroupBox* controlsGroup_;
    QComboBox* timeRangeCombo_;
    QPushButton* resetButton_;
    QPushButton* exportButton_;
    QPushButton* refreshButton_;

    // 最近运行表格
    QGroupBox* recentRunsGroup_;
    QTableWidget* recentRunsTable_;

    // 数据
    QVector<RunRecord> runRecords_;
    StatisticsSummary currentSummary_;
    int maxRecordCount_;

    // 自动刷新
    QTimer* autoRefreshTimer_;
};

} // namespace UI
} // namespace VisionForge
