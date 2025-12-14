/**
 * @file HistoryPanel.cpp
 * @brief 历史记录面板实现
 */

#include "ui/HistoryPanel.h"
#include "ui/Theme.h"
#include "base/Logger.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QImage>

namespace VisionForge {
namespace UI {

HistoryPanel::HistoryPanel(QWidget* parent)
    : QWidget(parent)
    , mainLayout_(nullptr)
    , toolBar_(nullptr)
    , historyList_(nullptr)
    , clearAction_(nullptr)
    , undoAction_(nullptr)
    , redoAction_(nullptr)
    , saveSessionAction_(nullptr)
    , loadSessionAction_(nullptr)
    , currentIndex_(-1)
{
    setupUI();
}

void HistoryPanel::setupUI()
{
    mainLayout_ = new QVBoxLayout(this);
    mainLayout_->setContentsMargins(0, 0, 0, 0);
    mainLayout_->setSpacing(0);

    createToolBar();

    // 创建历史列表
    historyList_ = new QListWidget(this);
    historyList_->setIconSize(QSize(THUMBNAIL_SIZE, THUMBNAIL_SIZE));
    historyList_->setSelectionMode(QAbstractItemView::SingleSelection);
    historyList_->setViewMode(QListWidget::ListMode);
    historyList_->setResizeMode(QListWidget::Adjust);

    connect(historyList_, &QListWidget::itemDoubleClicked,
            this, &HistoryPanel::onItemDoubleClicked);

    mainLayout_->addWidget(toolBar_);
    mainLayout_->addWidget(historyList_);

    updateActions();
}

void HistoryPanel::createToolBar()
{
    toolBar_ = new QToolBar(this);
    toolBar_->setIconSize(QSize(16, 16));

    undoAction_ = toolBar_->addAction(Theme::getIcon(Icons::TOOL_REMOVE), "撤销");
    undoAction_->setShortcut(QKeySequence::Undo);
    undoAction_->setStatusTip("撤销到上一步");
    connect(undoAction_, &QAction::triggered, this, &HistoryPanel::onUndoClicked);

    redoAction_ = toolBar_->addAction(Theme::getIcon(Icons::TOOL_ADD), "重做");
    redoAction_->setShortcut(QKeySequence::Redo);
    redoAction_->setStatusTip("重做到下一步");
    connect(redoAction_, &QAction::triggered, this, &HistoryPanel::onRedoClicked);

    toolBar_->addSeparator();

    clearAction_ = toolBar_->addAction(Theme::getIcon(Icons::TOOL_REMOVE), "清除");
    clearAction_->setStatusTip("清除所有历史记录");
    connect(clearAction_, &QAction::triggered, this, &HistoryPanel::onClearClicked);

    toolBar_->addSeparator();

    saveSessionAction_ = toolBar_->addAction(Theme::getIcon(Icons::FILE_SAVE), "保存会话");
    saveSessionAction_->setStatusTip("保存当前会话到文件");
    connect(saveSessionAction_, &QAction::triggered, this, &HistoryPanel::onSaveSessionClicked);

    loadSessionAction_ = toolBar_->addAction(Theme::getIcon(Icons::FILE_OPEN), "加载会话");
    loadSessionAction_->setStatusTip("从文件加载会话");
    connect(loadSessionAction_, &QAction::triggered, this, &HistoryPanel::onLoadSessionClicked);
}

void HistoryPanel::addRecord(const Base::ImageData::Ptr& image,
                             const QString& description,
                             const QString& toolChain,
                             double executionTime)
{
    if (!image || image->isEmpty()) {
        return;
    }

    // 如果当前不在最后一个位置，删除之后的所有记录（分支历史）
    if (currentIndex_ < records_.size() - 1) {
        records_.erase(records_.begin() + currentIndex_ + 1, records_.end());
    }

    // 创建新记录
    HistoryRecord record;
    record.image = image;
    record.description = description;
    record.toolChain = toolChain;
    record.totalExecutionTime = executionTime;
    record.thumbnail = createThumbnail(image, QSize(THUMBNAIL_SIZE, THUMBNAIL_SIZE));

    records_.append(record);
    currentIndex_ = records_.size() - 1;

    updateHistoryList();
    updateActions();

    emit historyChanged();

    LOG_DEBUG(QString("添加历史记录: %1 [%2 ms]")
        .arg(description.isEmpty() ? "未命名" : description)
        .arg(executionTime, 0, 'f', 2));
}

void HistoryPanel::clearHistory()
{
    if (records_.isEmpty()) {
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "确认清除",
        "确定要清除所有历史记录吗？",
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply == QMessageBox::Yes) {
        records_.clear();
        currentIndex_ = -1;
        historyList_->clear();
        updateActions();
        emit historyChanged();
        LOG_INFO("历史记录已清除");
    }
}

HistoryRecord* HistoryPanel::getCurrentRecord()
{
    if (currentIndex_ >= 0 && currentIndex_ < records_.size()) {
        return &records_[currentIndex_];
    }
    return nullptr;
}

void HistoryPanel::undo()
{
    if (canUndo()) {
        currentIndex_--;
        updateHistoryList();
        updateActions();

        HistoryRecord* record = getCurrentRecord();
        if (record) {
            emit recordSelected(*record);
        }

        LOG_DEBUG(QString("撤销到历史记录 #%1").arg(currentIndex_ + 1));
    }
}

void HistoryPanel::redo()
{
    if (canRedo()) {
        currentIndex_++;
        updateHistoryList();
        updateActions();

        HistoryRecord* record = getCurrentRecord();
        if (record) {
            emit recordSelected(*record);
        }

        LOG_DEBUG(QString("重做到历史记录 #%1").arg(currentIndex_ + 1));
    }
}

void HistoryPanel::onClearClicked()
{
    clearHistory();
}

void HistoryPanel::onUndoClicked()
{
    undo();
}

void HistoryPanel::onRedoClicked()
{
    redo();
}

void HistoryPanel::onSaveSessionClicked()
{
    if (records_.isEmpty()) {
        QMessageBox::information(this, "提示", "没有可保存的历史记录");
        return;
    }

    QString filePath = QFileDialog::getSaveFileName(
        this,
        "保存会话",
        "session.json",
        "JSON文件 (*.json);;所有文件 (*.*)"
    );

    if (filePath.isEmpty()) {
        return;
    }

    QJsonArray recordsArray;
    for (int i = 0; i < records_.size(); ++i) {
        const HistoryRecord& record = records_[i];

        QJsonObject recordObj;
        recordObj["timestamp"] = record.timestamp.toString(Qt::ISODate);
        recordObj["description"] = record.description;
        recordObj["toolChain"] = record.toolChain;
        recordObj["executionTime"] = record.totalExecutionTime;

        recordsArray.append(recordObj);
    }

    QJsonObject sessionObj;
    sessionObj["version"] = "1.0";
    sessionObj["currentIndex"] = currentIndex_;
    sessionObj["records"] = recordsArray;

    QJsonDocument doc(sessionObj);
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, "错误", "无法创建会话文件");
        LOG_ERROR(QString("无法创建会话文件: %1").arg(filePath));
        return;
    }

    file.write(doc.toJson());
    file.close();

    QMessageBox::information(this, "成功", "会话已保存");
    LOG_INFO(QString("会话已保存到: %1").arg(filePath));
}

void HistoryPanel::onLoadSessionClicked()
{
    QString filePath = QFileDialog::getOpenFileName(
        this,
        "加载会话",
        QString(),
        "JSON文件 (*.json);;所有文件 (*.*)"
    );

    if (filePath.isEmpty()) {
        return;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "错误", "无法打开会话文件");
        LOG_ERROR(QString("无法打开会话文件: %1").arg(filePath));
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) {
        QMessageBox::critical(this, "错误", "会话文件格式错误");
        return;
    }

    QJsonObject sessionObj = doc.object();
    // 这里可以加载会话数据，但由于图像数据没有保存，所以只能加载元数据
    // 实际应用中可以保存图像文件路径或序列化图像数据

    QMessageBox::information(this, "提示", "会话加载功能需要配合图像文件一起使用");
    LOG_INFO(QString("会话文件已读取: %1").arg(filePath));
}

void HistoryPanel::onItemDoubleClicked(QListWidgetItem* item)
{
    int row = historyList_->row(item);
    if (row >= 0 && row < records_.size()) {
        currentIndex_ = row;
        updateActions();
        emit recordSelected(records_[row]);
        LOG_DEBUG(QString("选择历史记录 #%1").arg(row + 1));
    }
}

void HistoryPanel::updateHistoryList()
{
    historyList_->clear();

    for (int i = 0; i < records_.size(); ++i) {
        const HistoryRecord& record = records_[i];

        QString text = QString("#%1 - %2\n%3\n%4 ms")
            .arg(i + 1)
            .arg(record.timestamp.toString("yyyy-MM-dd hh:mm:ss"))
            .arg(record.description.isEmpty() ? "未命名" : record.description)
            .arg(record.totalExecutionTime, 0, 'f', 2);

        QListWidgetItem* item = new QListWidgetItem(record.thumbnail, text);

        // 当前记录高亮显示
        if (i == currentIndex_) {
            QFont font = item->font();
            font.setBold(true);
            item->setFont(font);
            item->setBackground(QColor(42, 130, 218, 50));
        }

        // 未来记录（重做目标）使用灰色
        if (i > currentIndex_) {
            item->setForeground(Qt::gray);
        }

        historyList_->addItem(item);
    }

    // 滚动到当前记录
    if (currentIndex_ >= 0 && currentIndex_ < records_.size()) {
        historyList_->setCurrentRow(currentIndex_);
    }
}

void HistoryPanel::updateActions()
{
    undoAction_->setEnabled(canUndo());
    redoAction_->setEnabled(canRedo());
    clearAction_->setEnabled(!records_.isEmpty());
    saveSessionAction_->setEnabled(!records_.isEmpty());
}

QPixmap HistoryPanel::createThumbnail(const Base::ImageData::Ptr& image, const QSize& size)
{
    if (!image || image->isEmpty()) {
        return QPixmap();
    }

    QImage qimage = image->toQImage();
    return QPixmap::fromImage(qimage.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

} // namespace UI
} // namespace VisionForge
