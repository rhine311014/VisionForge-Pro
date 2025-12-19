/**
 * @file StatisticsPanel.cpp
 * @brief 数据统计面板实现
 * @author VisionForge Team
 * @date 2025-12-19
 */

#include "ui/StatisticsPanel.h"
#include "base/Logger.h"

#include <QHeaderView>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <algorithm>
#include <numeric>

namespace VisionForge {
namespace UI {

StatisticsPanel::StatisticsPanel(QWidget* parent)
    : QWidget(parent)
    , maxRecordCount_(10000)
    , autoRefreshTimer_(new QTimer(this))
{
    setupUI();

    // 自动刷新定时器（每5秒刷新日志统计）
    connect(autoRefreshTimer_, &QTimer::timeout,
            this, &StatisticsPanel::onAutoRefreshTimer);
    autoRefreshTimer_->start(5000);

    // 初始刷新
    refreshDisplay();
}

void StatisticsPanel::setupUI()
{
    mainLayout_ = new QVBoxLayout(this);
    mainLayout_->setSpacing(10);
    mainLayout_->setContentsMargins(10, 10, 10, 10);

    // 创建各个统计组
    createControlsGroup();
    createRunStatisticsGroup();
    createTimeStatisticsGroup();
    createLogStatisticsGroup();
    createRecentRunsTable();

    // 添加弹性空间
    mainLayout_->addStretch();
}

void StatisticsPanel::createControlsGroup()
{
    controlsGroup_ = new QGroupBox(tr("统计控制"), this);
    QHBoxLayout* layout = new QHBoxLayout(controlsGroup_);

    // 时间范围选择
    QLabel* rangeLabel = new QLabel(tr("时间范围:"), this);
    timeRangeCombo_ = new QComboBox(this);
    timeRangeCombo_->addItem(tr("全部"), -1);
    timeRangeCombo_->addItem(tr("最近1小时"), 1);
    timeRangeCombo_->addItem(tr("最近24小时"), 24);
    timeRangeCombo_->addItem(tr("最近7天"), 168);
    timeRangeCombo_->addItem(tr("最近30天"), 720);

    connect(timeRangeCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &StatisticsPanel::onTimeRangeChanged);

    // 按钮
    refreshButton_ = new QPushButton(tr("刷新"), this);
    resetButton_ = new QPushButton(tr("重置统计"), this);
    exportButton_ = new QPushButton(tr("导出报告"), this);

    connect(refreshButton_, &QPushButton::clicked,
            this, &StatisticsPanel::refreshDisplay);
    connect(resetButton_, &QPushButton::clicked,
            this, &StatisticsPanel::onResetClicked);
    connect(exportButton_, &QPushButton::clicked,
            this, &StatisticsPanel::onExportClicked);

    layout->addWidget(rangeLabel);
    layout->addWidget(timeRangeCombo_);
    layout->addStretch();
    layout->addWidget(refreshButton_);
    layout->addWidget(resetButton_);
    layout->addWidget(exportButton_);

    mainLayout_->addWidget(controlsGroup_);
}

void StatisticsPanel::createRunStatisticsGroup()
{
    runStatsGroup_ = new QGroupBox(tr("运行统计"), this);
    QGridLayout* layout = new QGridLayout(runStatsGroup_);

    // 总运行次数
    layout->addWidget(new QLabel(tr("总运行次数:"), this), 0, 0);
    totalRunsLabel_ = new QLabel("0", this);
    totalRunsLabel_->setStyleSheet("font-weight: bold; font-size: 14px;");
    layout->addWidget(totalRunsLabel_, 0, 1);

    // OK次数
    layout->addWidget(new QLabel(tr("OK次数:"), this), 0, 2);
    okCountLabel_ = new QLabel("0", this);
    okCountLabel_->setStyleSheet("font-weight: bold; color: #4CAF50;");
    layout->addWidget(okCountLabel_, 0, 3);

    // NG次数
    layout->addWidget(new QLabel(tr("NG次数:"), this), 0, 4);
    ngCountLabel_ = new QLabel("0", this);
    ngCountLabel_->setStyleSheet("font-weight: bold; color: #F44336;");
    layout->addWidget(ngCountLabel_, 0, 5);

    // 良品率
    layout->addWidget(new QLabel(tr("良品率:"), this), 1, 0);
    okRateLabel_ = new QLabel("0.00%", this);
    okRateLabel_->setStyleSheet("font-weight: bold; font-size: 16px; color: #2196F3;");
    layout->addWidget(okRateLabel_, 1, 1);

    // 良品率进度条
    okRateBar_ = new QProgressBar(this);
    okRateBar_->setRange(0, 100);
    okRateBar_->setValue(0);
    okRateBar_->setFormat("%v%");
    okRateBar_->setStyleSheet(
        "QProgressBar { border: 1px solid #ccc; border-radius: 5px; text-align: center; }"
        "QProgressBar::chunk { background-color: #4CAF50; border-radius: 4px; }"
    );
    layout->addWidget(okRateBar_, 1, 2, 1, 4);

    mainLayout_->addWidget(runStatsGroup_);
}

void StatisticsPanel::createTimeStatisticsGroup()
{
    timeStatsGroup_ = new QGroupBox(tr("执行时间统计"), this);
    QGridLayout* layout = new QGridLayout(timeStatsGroup_);

    // 最短时间
    layout->addWidget(new QLabel(tr("最短时间:"), this), 0, 0);
    minTimeLabel_ = new QLabel("--", this);
    minTimeLabel_->setStyleSheet("font-weight: bold; color: #4CAF50;");
    layout->addWidget(minTimeLabel_, 0, 1);

    // 最长时间
    layout->addWidget(new QLabel(tr("最长时间:"), this), 0, 2);
    maxTimeLabel_ = new QLabel("--", this);
    maxTimeLabel_->setStyleSheet("font-weight: bold; color: #FF9800;");
    layout->addWidget(maxTimeLabel_, 0, 3);

    // 平均时间
    layout->addWidget(new QLabel(tr("平均时间:"), this), 1, 0);
    avgTimeLabel_ = new QLabel("--", this);
    avgTimeLabel_->setStyleSheet("font-weight: bold; color: #2196F3;");
    layout->addWidget(avgTimeLabel_, 1, 1);

    // 总时间
    layout->addWidget(new QLabel(tr("累计时间:"), this), 1, 2);
    totalTimeLabel_ = new QLabel("--", this);
    layout->addWidget(totalTimeLabel_, 1, 3);

    mainLayout_->addWidget(timeStatsGroup_);
}

void StatisticsPanel::createLogStatisticsGroup()
{
    logStatsGroup_ = new QGroupBox(tr("日志统计"), this);
    QGridLayout* layout = new QGridLayout(logStatsGroup_);

    // Debug
    layout->addWidget(new QLabel(tr("调试:"), this), 0, 0);
    debugCountLabel_ = new QLabel("0", this);
    debugCountLabel_->setStyleSheet("color: #9E9E9E;");
    layout->addWidget(debugCountLabel_, 0, 1);

    // Info
    layout->addWidget(new QLabel(tr("信息:"), this), 0, 2);
    infoCountLabel_ = new QLabel("0", this);
    infoCountLabel_->setStyleSheet("color: #2196F3;");
    layout->addWidget(infoCountLabel_, 0, 3);

    // Warning
    layout->addWidget(new QLabel(tr("警告:"), this), 0, 4);
    warningCountLabel_ = new QLabel("0", this);
    warningCountLabel_->setStyleSheet("color: #FF9800;");
    layout->addWidget(warningCountLabel_, 0, 5);

    // Error
    layout->addWidget(new QLabel(tr("错误:"), this), 1, 0);
    errorCountLabel_ = new QLabel("0", this);
    errorCountLabel_->setStyleSheet("color: #F44336;");
    layout->addWidget(errorCountLabel_, 1, 1);

    // Critical
    layout->addWidget(new QLabel(tr("严重:"), this), 1, 2);
    criticalCountLabel_ = new QLabel("0", this);
    criticalCountLabel_->setStyleSheet("color: #B71C1C; font-weight: bold;");
    layout->addWidget(criticalCountLabel_, 1, 3);

    mainLayout_->addWidget(logStatsGroup_);
}

void StatisticsPanel::createRecentRunsTable()
{
    recentRunsGroup_ = new QGroupBox(tr("最近运行记录"), this);
    QVBoxLayout* layout = new QVBoxLayout(recentRunsGroup_);

    recentRunsTable_ = new QTableWidget(this);
    recentRunsTable_->setColumnCount(5);
    recentRunsTable_->setHorizontalHeaderLabels({
        tr("时间"), tr("结果"), tr("执行时间"), tr("工具链"), tr("工具数")
    });

    // 设置表格属性
    recentRunsTable_->setAlternatingRowColors(true);
    recentRunsTable_->setSelectionBehavior(QAbstractItemView::SelectRows);
    recentRunsTable_->setSelectionMode(QAbstractItemView::SingleSelection);
    recentRunsTable_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    recentRunsTable_->horizontalHeader()->setStretchLastSection(true);
    recentRunsTable_->verticalHeader()->setVisible(false);
    recentRunsTable_->setMaximumHeight(200);

    layout->addWidget(recentRunsTable_);
    mainLayout_->addWidget(recentRunsGroup_);
}

void StatisticsPanel::recordRun(bool success, double executionTime,
                                 const QString& toolChainName, int toolCount)
{
    RunRecord record;
    record.timestamp = QDateTime::currentDateTime();
    record.success = success;
    record.executionTime = executionTime;
    record.toolChainName = toolChainName;
    record.toolCount = toolCount;

    runRecords_.prepend(record);

    // 限制记录数量
    if (maxRecordCount_ > 0 && runRecords_.size() > maxRecordCount_) {
        runRecords_.resize(maxRecordCount_);
    }

    // 重新计算统计
    recalculateStatistics();
    refreshDisplay();

    emit statisticsUpdated(currentSummary_);
}

StatisticsSummary StatisticsPanel::getSummary() const
{
    return currentSummary_;
}

QVector<RunRecord> StatisticsPanel::getRunRecords(int maxRecords) const
{
    if (maxRecords <= 0 || maxRecords >= runRecords_.size()) {
        return runRecords_;
    }
    return runRecords_.mid(0, maxRecords);
}

void StatisticsPanel::resetStatistics()
{
    runRecords_.clear();
    currentSummary_ = StatisticsSummary();
    refreshDisplay();
}

void StatisticsPanel::setMaxRecordCount(int maxCount)
{
    maxRecordCount_ = maxCount;
    if (maxCount > 0 && runRecords_.size() > maxCount) {
        runRecords_.resize(maxCount);
        recalculateStatistics();
    }
}

void StatisticsPanel::refreshDisplay()
{
    updateRunStatisticsDisplay();
    updateTimeStatisticsDisplay();
    updateLogStatisticsDisplay();
    updateRecentRunsTable();
}

void StatisticsPanel::updateLogStatistics()
{
    auto logStats = Base::Logger::instance().getLogStatistics();

    currentSummary_.debugCount = logStats.value(Base::Logger::Debug, 0);
    currentSummary_.infoCount = logStats.value(Base::Logger::Info, 0);
    currentSummary_.warningCount = logStats.value(Base::Logger::Warning, 0);
    currentSummary_.errorCount = logStats.value(Base::Logger::Error, 0);
    currentSummary_.criticalCount = logStats.value(Base::Logger::Critical, 0);

    updateLogStatisticsDisplay();
}

void StatisticsPanel::onResetClicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        tr("确认重置"),
        tr("确定要重置所有统计数据吗？"),
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply == QMessageBox::Yes) {
        resetStatistics();
        LOG_INFO(tr("统计数据已重置"));
    }
}

void StatisticsPanel::onExportClicked()
{
    QString fileName = QFileDialog::getSaveFileName(
        this,
        tr("导出统计报告"),
        QString(),
        tr("HTML文件 (*.html);;CSV文件 (*.csv);;文本文件 (*.txt)")
    );

    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("导出失败"),
                           tr("无法创建文件: %1").arg(fileName));
        return;
    }

    QTextStream stream(&file);
    stream.setEncoding(QStringConverter::Utf8);

    if (fileName.endsWith(".html", Qt::CaseInsensitive)) {
        // 导出为HTML
        stream << "<!DOCTYPE html>\n<html>\n<head>\n";
        stream << "<meta charset=\"UTF-8\">\n";
        stream << "<title>" << tr("VisionForge Pro 统计报告") << "</title>\n";
        stream << "<style>\n";
        stream << "body { font-family: Arial, sans-serif; margin: 20px; }\n";
        stream << "h1 { color: #2196F3; }\n";
        stream << "table { border-collapse: collapse; width: 100%; margin: 20px 0; }\n";
        stream << "th, td { border: 1px solid #ddd; padding: 8px; text-align: left; }\n";
        stream << "th { background-color: #2196F3; color: white; }\n";
        stream << ".ok { color: #4CAF50; }\n";
        stream << ".ng { color: #F44336; }\n";
        stream << "</style>\n</head>\n<body>\n";

        stream << "<h1>" << tr("VisionForge Pro 统计报告") << "</h1>\n";
        stream << "<p>" << tr("导出时间: ") << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "</p>\n";

        // 运行统计
        stream << "<h2>" << tr("运行统计") << "</h2>\n";
        stream << "<table>\n";
        stream << "<tr><td>" << tr("总运行次数") << "</td><td>" << currentSummary_.totalRuns << "</td></tr>\n";
        stream << "<tr><td>" << tr("OK次数") << "</td><td class='ok'>" << currentSummary_.okCount << "</td></tr>\n";
        stream << "<tr><td>" << tr("NG次数") << "</td><td class='ng'>" << currentSummary_.ngCount << "</td></tr>\n";
        stream << "<tr><td>" << tr("良品率") << "</td><td>" << formatRate(currentSummary_.okRate) << "</td></tr>\n";
        stream << "</table>\n";

        // 时间统计
        stream << "<h2>" << tr("执行时间统计") << "</h2>\n";
        stream << "<table>\n";
        stream << "<tr><td>" << tr("最短时间") << "</td><td>" << formatTime(currentSummary_.minTime) << "</td></tr>\n";
        stream << "<tr><td>" << tr("最长时间") << "</td><td>" << formatTime(currentSummary_.maxTime) << "</td></tr>\n";
        stream << "<tr><td>" << tr("平均时间") << "</td><td>" << formatTime(currentSummary_.avgTime) << "</td></tr>\n";
        stream << "<tr><td>" << tr("累计时间") << "</td><td>" << formatTime(currentSummary_.totalTime) << "</td></tr>\n";
        stream << "</table>\n";

        // 最近运行记录
        stream << "<h2>" << tr("最近运行记录") << "</h2>\n";
        stream << "<table>\n";
        stream << "<tr><th>" << tr("时间") << "</th><th>" << tr("结果") << "</th>";
        stream << "<th>" << tr("执行时间") << "</th><th>" << tr("工具链") << "</th></tr>\n";

        int count = qMin(100, runRecords_.size());
        for (int i = 0; i < count; ++i) {
            const auto& record = runRecords_[i];
            stream << "<tr>";
            stream << "<td>" << record.timestamp.toString("yyyy-MM-dd hh:mm:ss") << "</td>";
            stream << "<td class='" << (record.success ? "ok" : "ng") << "'>"
                   << (record.success ? "OK" : "NG") << "</td>";
            stream << "<td>" << formatTime(record.executionTime) << "</td>";
            stream << "<td>" << record.toolChainName << "</td>";
            stream << "</tr>\n";
        }
        stream << "</table>\n";

        stream << "</body>\n</html>\n";
    }
    else if (fileName.endsWith(".csv", Qt::CaseInsensitive)) {
        // 导出为CSV
        stream << "Timestamp,Result,ExecutionTime(ms),ToolChain,ToolCount\n";
        for (const auto& record : runRecords_) {
            stream << record.timestamp.toString(Qt::ISODate) << ","
                   << (record.success ? "OK" : "NG") << ","
                   << record.executionTime << ","
                   << record.toolChainName << ","
                   << record.toolCount << "\n";
        }
    }
    else {
        // 导出为纯文本
        stream << tr("VisionForge Pro 统计报告") << "\n";
        stream << tr("导出时间: ") << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "\n";
        stream << "================================================================================\n\n";

        stream << tr("运行统计:") << "\n";
        stream << tr("  总运行次数: ") << currentSummary_.totalRuns << "\n";
        stream << tr("  OK次数: ") << currentSummary_.okCount << "\n";
        stream << tr("  NG次数: ") << currentSummary_.ngCount << "\n";
        stream << tr("  良品率: ") << formatRate(currentSummary_.okRate) << "\n\n";

        stream << tr("执行时间统计:") << "\n";
        stream << tr("  最短时间: ") << formatTime(currentSummary_.minTime) << "\n";
        stream << tr("  最长时间: ") << formatTime(currentSummary_.maxTime) << "\n";
        stream << tr("  平均时间: ") << formatTime(currentSummary_.avgTime) << "\n";
        stream << tr("  累计时间: ") << formatTime(currentSummary_.totalTime) << "\n";
    }

    file.close();

    QMessageBox::information(this, tr("导出成功"),
                           tr("统计报告已导出到: %1").arg(fileName));
    LOG_INFO(tr("统计报告已导出: %1").arg(fileName));

    emit exportRequested();
}

void StatisticsPanel::onTimeRangeChanged(int index)
{
    Q_UNUSED(index)
    recalculateStatistics();
    refreshDisplay();
}

void StatisticsPanel::onAutoRefreshTimer()
{
    updateLogStatistics();
}

void StatisticsPanel::recalculateStatistics()
{
    // 获取时间范围
    int hoursRange = timeRangeCombo_->currentData().toInt();
    QDateTime cutoffTime;
    if (hoursRange > 0) {
        cutoffTime = QDateTime::currentDateTime().addSecs(-hoursRange * 3600);
    }

    // 重置统计
    currentSummary_.totalRuns = 0;
    currentSummary_.okCount = 0;
    currentSummary_.ngCount = 0;
    currentSummary_.minTime = std::numeric_limits<double>::max();
    currentSummary_.maxTime = 0.0;
    currentSummary_.totalTime = 0.0;

    // 计算统计
    for (const auto& record : runRecords_) {
        // 时间范围筛选
        if (hoursRange > 0 && record.timestamp < cutoffTime) {
            continue;
        }

        currentSummary_.totalRuns++;

        if (record.success) {
            currentSummary_.okCount++;
        } else {
            currentSummary_.ngCount++;
        }

        currentSummary_.minTime = qMin(currentSummary_.minTime, record.executionTime);
        currentSummary_.maxTime = qMax(currentSummary_.maxTime, record.executionTime);
        currentSummary_.totalTime += record.executionTime;
    }

    // 计算平均值和比率
    if (currentSummary_.totalRuns > 0) {
        currentSummary_.avgTime = currentSummary_.totalTime / currentSummary_.totalRuns;
        currentSummary_.okRate = 100.0 * currentSummary_.okCount / currentSummary_.totalRuns;
    } else {
        currentSummary_.minTime = 0.0;
        currentSummary_.avgTime = 0.0;
        currentSummary_.okRate = 0.0;
    }

    // 更新日志统计
    updateLogStatistics();
}

void StatisticsPanel::updateRunStatisticsDisplay()
{
    totalRunsLabel_->setText(QString::number(currentSummary_.totalRuns));
    okCountLabel_->setText(QString::number(currentSummary_.okCount));
    ngCountLabel_->setText(QString::number(currentSummary_.ngCount));
    okRateLabel_->setText(formatRate(currentSummary_.okRate));
    okRateBar_->setValue(static_cast<int>(currentSummary_.okRate));
}

void StatisticsPanel::updateTimeStatisticsDisplay()
{
    if (currentSummary_.totalRuns > 0) {
        minTimeLabel_->setText(formatTime(currentSummary_.minTime));
        maxTimeLabel_->setText(formatTime(currentSummary_.maxTime));
        avgTimeLabel_->setText(formatTime(currentSummary_.avgTime));
        totalTimeLabel_->setText(formatTime(currentSummary_.totalTime));
    } else {
        minTimeLabel_->setText("--");
        maxTimeLabel_->setText("--");
        avgTimeLabel_->setText("--");
        totalTimeLabel_->setText("--");
    }
}

void StatisticsPanel::updateLogStatisticsDisplay()
{
    debugCountLabel_->setText(QString::number(currentSummary_.debugCount));
    infoCountLabel_->setText(QString::number(currentSummary_.infoCount));
    warningCountLabel_->setText(QString::number(currentSummary_.warningCount));
    errorCountLabel_->setText(QString::number(currentSummary_.errorCount));
    criticalCountLabel_->setText(QString::number(currentSummary_.criticalCount));
}

void StatisticsPanel::updateRecentRunsTable()
{
    recentRunsTable_->setRowCount(0);

    int count = qMin(20, runRecords_.size());  // 最多显示20条
    for (int i = 0; i < count; ++i) {
        const auto& record = runRecords_[i];

        int row = recentRunsTable_->rowCount();
        recentRunsTable_->insertRow(row);

        // 时间
        auto* timeItem = new QTableWidgetItem(
            record.timestamp.toString("MM-dd hh:mm:ss"));
        recentRunsTable_->setItem(row, 0, timeItem);

        // 结果
        auto* resultItem = new QTableWidgetItem(record.success ? "OK" : "NG");
        resultItem->setForeground(record.success ?
            QColor("#4CAF50") : QColor("#F44336"));
        recentRunsTable_->setItem(row, 1, resultItem);

        // 执行时间
        auto* timeValueItem = new QTableWidgetItem(formatTime(record.executionTime));
        recentRunsTable_->setItem(row, 2, timeValueItem);

        // 工具链
        auto* chainItem = new QTableWidgetItem(record.toolChainName);
        recentRunsTable_->setItem(row, 3, chainItem);

        // 工具数
        auto* countItem = new QTableWidgetItem(QString::number(record.toolCount));
        recentRunsTable_->setItem(row, 4, countItem);
    }

    recentRunsTable_->resizeColumnsToContents();
}

QString StatisticsPanel::formatTime(double ms) const
{
    if (ms < 1.0) {
        return QString("%1 us").arg(ms * 1000, 0, 'f', 1);
    } else if (ms < 1000.0) {
        return QString("%1 ms").arg(ms, 0, 'f', 2);
    } else if (ms < 60000.0) {
        return QString("%1 s").arg(ms / 1000.0, 0, 'f', 2);
    } else {
        int minutes = static_cast<int>(ms / 60000);
        double seconds = (ms - minutes * 60000) / 1000.0;
        return QString("%1m %2s").arg(minutes).arg(seconds, 0, 'f', 1);
    }
}

QString StatisticsPanel::formatRate(double rate) const
{
    return QString("%1%").arg(rate, 0, 'f', 2);
}

} // namespace UI
} // namespace VisionForge
