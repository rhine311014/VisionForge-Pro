/**
 * @file HistoryPanel.h
 * @brief 历史记录面板
 * @details 显示和管理处理历史记录
 * @author VisionForge Team
 * @date 2025-12-14
 */

#pragma once

#include "base/ImageData.h"
#include <QWidget>
#include <QListWidget>
#include <QToolBar>
#include <QVBoxLayout>
#include <QAction>
#include <QDateTime>
#include <QPixmap>

namespace VisionForge {
namespace UI {

/**
 * @struct HistoryRecord
 * @brief 历史记录项
 */
struct HistoryRecord {
    QDateTime timestamp;                    // 时间戳
    Base::ImageData::Ptr image;            // 图像数据
    QPixmap thumbnail;                     // 缩略图
    QString description;                   // 描述信息
    QString toolChain;                     // 工具链描述
    double totalExecutionTime;             // 总执行时间

    HistoryRecord()
        : timestamp(QDateTime::currentDateTime())
        , totalExecutionTime(0.0)
    {}
};

/**
 * @class HistoryPanel
 * @brief 历史记录面板
 *
 * 功能：
 * - 显示处理历史记录
 * - 支持撤销/重做操作
 * - 支持保存/加载会话
 * - 显示缩略图
 */
class HistoryPanel : public QWidget
{
    Q_OBJECT

public:
    explicit HistoryPanel(QWidget* parent = nullptr);
    ~HistoryPanel() override = default;

    /**
     * @brief 添加历史记录
     * @param image 图像数据
     * @param description 描述信息
     * @param toolChain 工具链描述
     * @param executionTime 执行时间
     */
    void addRecord(const Base::ImageData::Ptr& image,
                   const QString& description = QString(),
                   const QString& toolChain = QString(),
                   double executionTime = 0.0);

    /**
     * @brief 清空历史记录
     */
    void clearHistory();

    /**
     * @brief 获取历史记录数量
     */
    int getRecordCount() const { return records_.size(); }

    /**
     * @brief 获取当前选中的记录
     */
    HistoryRecord* getCurrentRecord();

    /**
     * @brief 是否可以撤销
     */
    bool canUndo() const { return currentIndex_ > 0; }

    /**
     * @brief 是否可以重做
     */
    bool canRedo() const { return currentIndex_ < records_.size() - 1; }

signals:
    /**
     * @brief 记录选择改变信号
     */
    void recordSelected(const HistoryRecord& record);

    /**
     * @brief 历史记录改变信号
     */
    void historyChanged();

public slots:
    /**
     * @brief 撤销到上一条记录
     */
    void undo();

    /**
     * @brief 重做到下一条记录
     */
    void redo();

private slots:
    void onClearClicked();
    void onUndoClicked();
    void onRedoClicked();
    void onSaveSessionClicked();
    void onLoadSessionClicked();
    void onItemDoubleClicked(QListWidgetItem* item);

private:
    void setupUI();
    void createToolBar();
    void updateHistoryList();
    void updateActions();
    QPixmap createThumbnail(const Base::ImageData::Ptr& image, const QSize& size);

private:
    QVBoxLayout* mainLayout_;
    QToolBar* toolBar_;
    QListWidget* historyList_;

    QAction* clearAction_;
    QAction* undoAction_;
    QAction* redoAction_;
    QAction* saveSessionAction_;
    QAction* loadSessionAction_;

    QList<HistoryRecord> records_;
    int currentIndex_;

    static constexpr int THUMBNAIL_SIZE = 64;
};

} // namespace UI
} // namespace VisionForge
