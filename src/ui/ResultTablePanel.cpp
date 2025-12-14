/**
 * @file ResultTablePanel.cpp
 * @brief 结果数据表格面板实现
 */

#include "ui/ResultTablePanel.h"
#include "ui/Theme.h"
#include "base/Logger.h"
#include <QHeaderView>
#include <QFileDialog>
#include <QTextStream>
#include <QClipboard>
#include <QApplication>
#include <QMessageBox>

namespace VisionForge {
namespace UI {

ResultTablePanel::ResultTablePanel(QWidget* parent)
    : QWidget(parent)
    , mainLayout_(nullptr)
    , toolBar_(nullptr)
    , table_(nullptr)
    , clearAction_(nullptr)
    , exportAction_(nullptr)
    , copyAction_(nullptr)
    , rowCount_(0)
{
    setupUI();
}

void ResultTablePanel::setupUI()
{
    mainLayout_ = new QVBoxLayout(this);
    mainLayout_->setContentsMargins(0, 0, 0, 0);
    mainLayout_->setSpacing(0);

    createToolBar();

    // 创建表格
    table_ = new QTableWidget(this);
    table_->setColumnCount(4);
    table_->setHorizontalHeaderLabels({"工具名称", "参数/结果", "值", "执行时间(ms)"});

    // 设置表格属性
    table_->setSelectionBehavior(QAbstractItemView::SelectRows);
    table_->setSelectionMode(QAbstractItemView::ExtendedSelection);
    table_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table_->setAlternatingRowColors(true);
    table_->setSortingEnabled(false);
    table_->verticalHeader()->setVisible(false);

    // 设置列宽
    table_->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    table_->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    table_->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    table_->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);

    mainLayout_->addWidget(toolBar_);
    mainLayout_->addWidget(table_);
}

void ResultTablePanel::createToolBar()
{
    toolBar_ = new QToolBar(this);
    toolBar_->setIconSize(QSize(16, 16));

    clearAction_ = toolBar_->addAction(Theme::getIcon(Icons::TOOL_REMOVE), "清除");
    clearAction_->setStatusTip("清除所有结果数据");
    connect(clearAction_, &QAction::triggered, this, &ResultTablePanel::onClearClicked);

    exportAction_ = toolBar_->addAction(Theme::getIcon(Icons::FILE_SAVE), "导出CSV");
    exportAction_->setStatusTip("导出结果数据到CSV文件");
    connect(exportAction_, &QAction::triggered, this, &ResultTablePanel::onExportClicked);

    copyAction_ = toolBar_->addAction(Theme::getIcon(Icons::TOOL_RUN), "复制");
    copyAction_->setStatusTip("复制选中的结果数据");
    connect(copyAction_, &QAction::triggered, this, &ResultTablePanel::onCopyClicked);
}

void ResultTablePanel::addResult(const QString& toolName, const Algorithm::ToolResult& result)
{
    // 添加成功状态
    QString status = result.success ? "成功" : QString("失败: %1").arg(result.errorMessage);
    addResultRow(toolName, "状态", status, result.executionTime);

    // 添加所有结果数据
    for (auto it = result.data.begin(); it != result.data.end(); ++it) {
        QString key = it.key();
        QVariant value = it.value().toVariant();
        addResultRow(toolName, key, value, 0.0);
    }

    rowCount_++;
    emit resultsUpdated();
}

void ResultTablePanel::updateResult(const QString& toolName, const Algorithm::ToolResult& result)
{
    // 移除该工具的旧结果
    for (int row = table_->rowCount() - 1; row >= 0; --row) {
        QTableWidgetItem* nameItem = table_->item(row, 0);
        if (nameItem && nameItem->text() == toolName) {
            table_->removeRow(row);
        }
    }

    // 添加新结果
    addResult(toolName, result);
}

void ResultTablePanel::clearResults()
{
    table_->setRowCount(0);
    rowCount_ = 0;
    emit resultsUpdated();
}

bool ResultTablePanel::exportToCSV(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        LOG_ERROR(QString("无法创建CSV文件: %1").arg(filePath));
        return false;
    }

    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);

    // 写入表头
    for (int col = 0; col < table_->columnCount(); ++col) {
        if (col > 0) out << ",";
        QString header = table_->horizontalHeaderItem(col)->text();
        out << "\"" << header << "\"";
    }
    out << "\n";

    // 写入数据
    for (int row = 0; row < table_->rowCount(); ++row) {
        for (int col = 0; col < table_->columnCount(); ++col) {
            if (col > 0) out << ",";
            QTableWidgetItem* item = table_->item(row, col);
            if (item) {
                QString text = item->text();
                out << "\"" << text << "\"";
            }
            else {
                out << "\"\"";
            }
        }
        out << "\n";
    }

    file.close();
    LOG_INFO(QString("结果数据已导出到: %1").arg(filePath));
    return true;
}

void ResultTablePanel::onClearClicked()
{
    if (table_->rowCount() > 0) {
        QMessageBox::StandardButton reply = QMessageBox::question(
            this,
            "确认清除",
            "确定要清除所有结果数据吗？",
            QMessageBox::Yes | QMessageBox::No
        );

        if (reply == QMessageBox::Yes) {
            clearResults();
        }
    }
}

void ResultTablePanel::onExportClicked()
{
    if (table_->rowCount() == 0) {
        QMessageBox::information(this, "提示", "没有可导出的结果数据");
        return;
    }

    QString filePath = QFileDialog::getSaveFileName(
        this,
        "导出结果数据",
        "results.csv",
        "CSV文件 (*.csv);;所有文件 (*.*)"
    );

    if (!filePath.isEmpty()) {
        if (exportToCSV(filePath)) {
            QMessageBox::information(this, "成功", "结果数据已成功导出到CSV文件");
        }
        else {
            QMessageBox::critical(this, "错误", "导出CSV文件失败");
        }
    }
}

void ResultTablePanel::onCopyClicked()
{
    QList<QTableWidgetItem*> selectedItems = table_->selectedItems();
    if (selectedItems.isEmpty()) {
        return;
    }

    // 获取选中的行
    QSet<int> selectedRows;
    for (const auto* item : selectedItems) {
        selectedRows.insert(item->row());
    }

    // 构建复制文本
    QString copyText;
    QList<int> sortedRows = selectedRows.values();
    std::sort(sortedRows.begin(), sortedRows.end());

    for (int row : sortedRows) {
        for (int col = 0; col < table_->columnCount(); ++col) {
            if (col > 0) copyText += "\t";
            QTableWidgetItem* item = table_->item(row, col);
            if (item) {
                copyText += item->text();
            }
        }
        copyText += "\n";
    }

    // 复制到剪贴板
    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setText(copyText);

    LOG_INFO(QString("已复制 %1 行结果数据到剪贴板").arg(sortedRows.size()));
}

QString ResultTablePanel::formatValue(const QVariant& value)
{
    switch (value.typeId()) {
    case QMetaType::Bool:
        return value.toBool() ? "是" : "否";
    case QMetaType::Int:
    case QMetaType::LongLong:
        return QString::number(value.toLongLong());
    case QMetaType::Double:
        return QString::number(value.toDouble(), 'f', 3);
    case QMetaType::QString:
        return value.toString();
    default:
        return value.toString();
    }
}

void ResultTablePanel::addResultRow(const QString& toolName, const QString& key,
                                    const QVariant& value, double executionTime)
{
    int row = table_->rowCount();
    table_->insertRow(row);

    // 工具名称
    QTableWidgetItem* nameItem = new QTableWidgetItem(toolName);
    table_->setItem(row, 0, nameItem);

    // 参数/结果名称
    QTableWidgetItem* keyItem = new QTableWidgetItem(key);
    table_->setItem(row, 1, keyItem);

    // 值
    QString valueStr = formatValue(value);
    QTableWidgetItem* valueItem = new QTableWidgetItem(valueStr);
    table_->setItem(row, 2, valueItem);

    // 执行时间
    QString timeStr = executionTime > 0.0 ? QString::number(executionTime, 'f', 2) : "";
    QTableWidgetItem* timeItem = new QTableWidgetItem(timeStr);
    table_->setItem(row, 3, timeItem);
}

} // namespace UI
} // namespace VisionForge
