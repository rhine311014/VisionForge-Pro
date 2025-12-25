/**
 * @file MultiCameraView.cpp
 * @brief 多相机显示视图组件实现
 */

#include "ui/MultiCameraView.h"
#include "ui/HalconImageViewer.h"
#include "core/StationConfig.h"
#include "base/Logger.h"
#include "base/ImageData.h"

#include <memory>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QResizeEvent>
#include <QKeyEvent>
#include <QMouseEvent>

namespace VisionForge {
namespace UI {

MultiCameraView::MultiCameraView(QWidget* parent)
    : QWidget(parent)
{
    setupUI();
}

MultiCameraView::~MultiCameraView()
{
    clearAllViewers();
}

void MultiCameraView::setupUI()
{
    // 创建网格布局
    gridLayout_ = new QGridLayout(this);
    gridLayout_->setSpacing(4);
    gridLayout_->setContentsMargins(4, 4, 4, 4);

    // 设置样式
    normalStyle_ = "QFrame { border: 2px solid #555555; background-color: #2d2d2d; }";
    selectedStyle_ = "QFrame { border: 3px solid #0078d4; background-color: #2d2d2d; }";
    okStyle_ = "QLabel { color: #00ff00; font-weight: bold; }";
    ngStyle_ = "QLabel { color: #ff0000; font-weight: bold; }";

    // 默认创建4个视图 (2x2布局)
    setLayoutMode(Layout_2x2);

    // 启用键盘焦点
    setFocusPolicy(Qt::StrongFocus);
}

QFrame* MultiCameraView::createViewerContainer(int index)
{
    // 创建容器框架
    QFrame* container = new QFrame(this);
    container->setFrameStyle(QFrame::Box);
    container->setStyleSheet(normalStyle_);

    QVBoxLayout* layout = new QVBoxLayout(container);
    layout->setSpacing(2);
    layout->setContentsMargins(2, 2, 2, 2);

    // 创建标题标签
    QLabel* titleLabel = new QLabel(QString("视图 %1").arg(index + 1), container);
    titleLabel->setStyleSheet("QLabel { color: #cccccc; font-size: 12px; padding: 2px; background-color: #3d3d3d; }");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setFixedHeight(22);
    layout->addWidget(titleLabel);

    // 创建图像查看器
    HalconImageViewer* viewer = new HalconImageViewer(container);
    viewer->setMinimumSize(100, 100);
    layout->addWidget(viewer, 1);

    // 创建状态标签
    QLabel* statusLabel = new QLabel("", container);
    statusLabel->setStyleSheet("QLabel { color: #888888; font-size: 11px; padding: 2px; }");
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setFixedHeight(20);
    layout->addWidget(statusLabel);

    // 保存引用
    viewerContainers_.append(container);
    viewers_.append(viewer);
    titleLabels_.append(titleLabel);
    statusLabels_.append(statusLabel);

    // 连接事件 - 使用事件过滤器
    container->installEventFilter(this);
    container->setProperty("viewIndex", index);

    return container;
}

void MultiCameraView::setLayoutMode(LayoutMode mode)
{
    if (layoutMode_ == mode && !viewers_.isEmpty()) {
        return;
    }

    layoutMode_ = mode;

    // 计算需要的视图数量
    int viewCount = static_cast<int>(mode);

    // 如果有工位配置，使用工位的位置数量
    if (currentStation_) {
        viewCount = qMin(viewCount, currentStation_->positionNum);
    }

    // 清空现有视图
    clearAllViewers();

    // 创建新视图
    for (int i = 0; i < viewCount; ++i) {
        createViewerContainer(i);
    }

    // 重新排列
    rearrangeViewers();

    // 更新选中状态
    if (selectedIndex_ >= viewCount) {
        selectedIndex_ = 0;
    }
    updateSelectionVisuals();

    emit layoutModeChanged(mode);
}

void MultiCameraView::setAutoLayout(int viewCount)
{
    LayoutMode mode;
    if (viewCount <= 1) {
        mode = Layout_1x1;
    } else if (viewCount <= 2) {
        mode = Layout_1x2;
    } else if (viewCount <= 4) {
        mode = Layout_2x2;
    } else if (viewCount <= 6) {
        mode = Layout_2x3;
    } else {
        mode = Layout_2x4;
    }

    setLayoutMode(mode);
}

void MultiCameraView::getLayoutDimensions(int& rows, int& cols) const
{
    switch (layoutMode_) {
    case Layout_1x1:
        rows = 1; cols = 1;
        break;
    case Layout_1x2:
        rows = 1; cols = 2;
        break;
    case Layout_2x2:
        rows = 2; cols = 2;
        break;
    case Layout_2x3:
        rows = 2; cols = 3;
        break;
    case Layout_2x4:
        rows = 2; cols = 4;
        break;
    default:
        rows = 2; cols = 2;
        break;
    }
}

void MultiCameraView::rearrangeViewers()
{
    // 从布局中移除所有项
    while (gridLayout_->count() > 0) {
        gridLayout_->takeAt(0);
    }

    if (maximized_ && maximizedIndex_ >= 0 && maximizedIndex_ < viewerContainers_.size()) {
        // 最大化模式：只显示选中的视图
        for (int i = 0; i < viewerContainers_.size(); ++i) {
            viewerContainers_[i]->setVisible(i == maximizedIndex_);
        }
        gridLayout_->addWidget(viewerContainers_[maximizedIndex_], 0, 0);
    } else {
        // 正常布局
        int rows, cols;
        getLayoutDimensions(rows, cols);

        int idx = 0;
        for (int r = 0; r < rows && idx < viewerContainers_.size(); ++r) {
            for (int c = 0; c < cols && idx < viewerContainers_.size(); ++c) {
                viewerContainers_[idx]->setVisible(true);
                gridLayout_->addWidget(viewerContainers_[idx], r, c);
                ++idx;
            }
        }
    }
}

void MultiCameraView::clearAllViewers()
{
    // 从布局中移除
    while (gridLayout_->count() > 0) {
        gridLayout_->takeAt(0);
    }

    // 删除所有组件
    for (auto* container : viewerContainers_) {
        container->deleteLater();
    }

    viewerContainers_.clear();
    viewers_.clear();
    titleLabels_.clear();
    statusLabels_.clear();
    positionToIndex_.clear();
}

void MultiCameraView::setStation(Core::StationConfig* station)
{
    currentStation_ = station;

    if (!station) {
        clearAllViewers();
        return;
    }

    // 根据工位配置创建视图
    int viewCount = station->positionNum;
    setAutoLayout(viewCount);

    // 更新位置映射和标题
    positionToIndex_.clear();
    for (int i = 0; i < station->positionBindings.size() && i < viewers_.size(); ++i) {
        const auto& binding = station->positionBindings[i];
        positionToIndex_[binding.positionId] = i;

        // 更新标题
        if (i < titleLabels_.size()) {
            titleLabels_[i]->setText(binding.positionName);
        }
    }

    LOG_INFO(QString("MultiCameraView: 关联工位 %1, 创建 %2 个视图")
             .arg(station->name).arg(viewCount));
}

void MultiCameraView::updateImage(int viewIndex, const std::shared_ptr<Base::ImageData>& image)
{
    if (viewIndex >= 0 && viewIndex < viewers_.size()) {
        viewers_[viewIndex]->setImage(image);
    }
}

void MultiCameraView::updateImage(const QString& positionId, const std::shared_ptr<Base::ImageData>& image)
{
    int index = getViewIndexForPosition(positionId);
    if (index >= 0) {
        updateImage(index, image);
    }
}

void MultiCameraView::updateAllImages(const QMap<QString, std::shared_ptr<Base::ImageData>>& images)
{
    for (auto it = images.begin(); it != images.end(); ++it) {
        updateImage(it.key(), it.value());
    }
}

void MultiCameraView::clearImage(int viewIndex)
{
    if (viewIndex >= 0 && viewIndex < viewers_.size()) {
        viewers_[viewIndex]->clearImage();
    }
}

void MultiCameraView::clearAllImages()
{
    for (auto* viewer : viewers_) {
        viewer->clearImage();
    }
}

void MultiCameraView::setSelectedView(int index)
{
    if (index >= 0 && index < viewers_.size() && index != selectedIndex_) {
        selectedIndex_ = index;
        updateSelectionVisuals();

        QString positionId;
        if (currentStation_ && index < currentStation_->positionBindings.size()) {
            positionId = currentStation_->positionBindings[index].positionId;
        }

        emit viewSelected(index, positionId);
    }
}

void MultiCameraView::selectNextView()
{
    int next = (selectedIndex_ + 1) % viewers_.size();
    setSelectedView(next);
}

void MultiCameraView::selectPrevView()
{
    int prev = (selectedIndex_ - 1 + viewers_.size()) % viewers_.size();
    setSelectedView(prev);
}

HalconImageViewer* MultiCameraView::getViewer(int index)
{
    if (index >= 0 && index < viewers_.size()) {
        return viewers_[index];
    }
    return nullptr;
}

HalconImageViewer* MultiCameraView::getSelectedViewer()
{
    return getViewer(selectedIndex_);
}

void MultiCameraView::maximizeView(int index)
{
    if (index < 0 || index >= viewers_.size()) {
        return;
    }

    maximized_ = true;
    maximizedIndex_ = index;
    rearrangeViewers();

    emit maximizeStateChanged(true, index);
}

void MultiCameraView::restoreLayout()
{
    if (!maximized_) {
        return;
    }

    maximized_ = false;
    maximizedIndex_ = -1;
    rearrangeViewers();

    emit maximizeStateChanged(false, -1);
}

void MultiCameraView::toggleMaximize(int index)
{
    if (maximized_ && maximizedIndex_ == index) {
        restoreLayout();
    } else {
        maximizeView(index);
    }
}

void MultiCameraView::setViewStatus(int index, bool ok, const QString& message)
{
    if (index >= 0 && index < statusLabels_.size()) {
        QLabel* label = statusLabels_[index];
        if (message.isEmpty()) {
            label->setText(ok ? "OK" : "NG");
        } else {
            label->setText(message);
        }
        label->setStyleSheet(ok ? okStyle_ : ngStyle_);
    }
}

void MultiCameraView::setViewTitle(int index, const QString& title)
{
    if (index >= 0 && index < titleLabels_.size()) {
        titleLabels_[index]->setText(title);
    }
}

void MultiCameraView::updateSelectionVisuals()
{
    for (int i = 0; i < viewerContainers_.size(); ++i) {
        viewerContainers_[i]->setStyleSheet(
            i == selectedIndex_ ? selectedStyle_ : normalStyle_
        );
    }
}

int MultiCameraView::getViewIndexForPosition(const QString& positionId) const
{
    auto it = positionToIndex_.find(positionId);
    if (it != positionToIndex_.end()) {
        return it.value();
    }
    return -1;
}

void MultiCameraView::onViewerClicked(int index)
{
    setSelectedView(index);
}

void MultiCameraView::onViewerDoubleClicked(int index)
{
    toggleMaximize(index);
    emit viewDoubleClicked(index);
}

void MultiCameraView::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    // 可以在这里添加响应式布局调整
}

void MultiCameraView::keyPressEvent(QKeyEvent* event)
{
    switch (event->key()) {
    case Qt::Key_Tab:
        selectNextView();
        event->accept();
        break;

    case Qt::Key_Backtab:  // Shift+Tab
        selectPrevView();
        event->accept();
        break;

    case Qt::Key_Return:
    case Qt::Key_Enter:
        toggleMaximize(selectedIndex_);
        event->accept();
        break;

    case Qt::Key_Escape:
        if (maximized_) {
            restoreLayout();
            event->accept();
        }
        break;

    case Qt::Key_1:
    case Qt::Key_2:
    case Qt::Key_3:
    case Qt::Key_4:
    case Qt::Key_5:
    case Qt::Key_6:
    case Qt::Key_7:
    case Qt::Key_8:
        if (event->modifiers() & Qt::ControlModifier) {
            int index = event->key() - Qt::Key_1;
            if (index < viewers_.size()) {
                setSelectedView(index);
            }
            event->accept();
        }
        break;

    default:
        QWidget::keyPressEvent(event);
        break;
    }
}

// 事件过滤器处理容器点击
bool MultiCameraView::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        QFrame* container = qobject_cast<QFrame*>(obj);
        if (container) {
            int index = container->property("viewIndex").toInt();
            onViewerClicked(index);
            return true;
        }
    } else if (event->type() == QEvent::MouseButtonDblClick) {
        QFrame* container = qobject_cast<QFrame*>(obj);
        if (container) {
            int index = container->property("viewIndex").toInt();
            onViewerDoubleClicked(index);
            return true;
        }
    }

    return QWidget::eventFilter(obj, event);
}

} // namespace UI
} // namespace VisionForge
