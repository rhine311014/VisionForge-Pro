/**
 * @file ImageViewer.h
 * @brief 图像显示组件
 * @details 支持图像缩放、平移、适应窗口、ROI绘制等功能
 * @author VisionForge Team
 * @date 2025-12-14
 */

#pragma once

#include "base/ImageData.h"
#include "ui/ROI.h"
#include <QWidget>
#include <QPixmap>
#include <QPoint>
#include <vector>

namespace VisionForge {
namespace UI {

/**
 * @class ImageViewer
 * @brief 图像查看器组件
 *
 * 功能：
 * - 显示ImageData图像
 * - 鼠标滚轮缩放
 * - 鼠标拖拽平移
 * - 适应窗口大小
 * - 1:1原始大小显示
 * - 显示图像信息
 * - ROI绘制和编辑
 */
class ImageViewer : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 缩放模式
     */
    enum FitMode {
        NoFit,          // 不自动适应
        FitToWindow,    // 适应窗口
        FitToWidth,     // 适应宽度
        FitToHeight     // 适应高度
    };

    /**
     * @brief 交互模式
     */
    enum InteractionMode {
        PanMode,        // 平移模式
        DrawRectangle,  // 绘制矩形
        DrawCircle,     // 绘制圆形
        DrawEllipse,    // 绘制椭圆
        DrawPolygon,    // 绘制多边形
        DrawLine,       // 绘制直线
        DrawPoint,      // 绘制点
        SelectMode      // 选择模式
    };

    explicit ImageViewer(QWidget* parent = nullptr);
    ~ImageViewer() override = default;

    /**
     * @brief 设置显示的图像
     * @param image 图像数据
     */
    void setImage(const Base::ImageData::Ptr& image);

    /**
     * @brief 清空图像
     */
    void clearImage();

    /**
     * @brief 获取当前图像
     */
    Base::ImageData::Ptr getImage() const { return currentImage_; }

    /**
     * @brief 设置缩放比例
     * @param scale 缩放比例（1.0 = 100%）
     */
    void setScale(double scale);

    /**
     * @brief 获取缩放比例
     */
    double getScale() const { return scale_; }

    /**
     * @brief 适应窗口大小
     */
    void fitToWindow();

    /**
     * @brief 适应宽度
     */
    void fitToWidth();

    /**
     * @brief 适应高度
     */
    void fitToHeight();

    /**
     * @brief 1:1原始大小
     */
    void actualSize();

    /**
     * @brief 放大
     */
    void zoomIn();

    /**
     * @brief 缩小
     */
    void zoomOut();

    /**
     * @brief 设置自动适应模式
     */
    void setFitMode(FitMode mode);

    /**
     * @brief 获取自动适应模式
     */
    FitMode getFitMode() const { return fitMode_; }

    // ========== ROI相关接口 ==========

    /**
     * @brief 设置交互模式
     */
    void setInteractionMode(InteractionMode mode);

    /**
     * @brief 获取交互模式
     */
    InteractionMode getInteractionMode() const { return interactionMode_; }

    /**
     * @brief 添加ROI
     */
    void addROI(const ROIShapePtr& roi);

    /**
     * @brief 移除ROI
     */
    void removeROI(const ROIShapePtr& roi);

    /**
     * @brief 移除所有ROI
     */
    void clearROIs();

    /**
     * @brief 获取所有ROI
     */
    std::vector<ROIShapePtr> getROIs() const { return rois_; }

    /**
     * @brief 获取选中的ROI
     */
    ROIShapePtr getSelectedROI() const { return selectedROI_; }

    /**
     * @brief 设置ROI颜色
     */
    void setROIColor(const QColor& color) { roiColor_ = color; }

    /**
     * @brief 获取ROI颜色
     */
    QColor getROIColor() const { return roiColor_; }

    /**
     * @brief 设置ROI线宽
     */
    void setROILineWidth(int width) { roiLineWidth_ = width; }

    /**
     * @brief 获取ROI线宽
     */
    int getROILineWidth() const { return roiLineWidth_; }

signals:
    /**
     * @brief 缩放比例改变信号
     * @param scale 新的缩放比例
     */
    void scaleChanged(double scale);

    /**
     * @brief 鼠标位置改变信号（图像坐标）
     * @param x 图像x坐标
     * @param y 图像y坐标
     * @param valid 坐标是否有效
     */
    void mousePositionChanged(int x, int y, bool valid);

    /**
     * @brief ROI创建完成信号
     */
    void roiCreated(ROIShapePtr roi);

    /**
     * @brief ROI选择改变信号
     */
    void roiSelectionChanged(ROIShapePtr roi);

    /**
     * @brief ROI移除信号
     */
    void roiRemoved(ROIShapePtr roi);

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    /**
     * @brief 更新显示的QPixmap
     */
    void updatePixmap();

    /**
     * @brief 计算适应窗口的缩放比例
     */
    double calculateFitScale() const;

    /**
     * @brief 计算适应宽度的缩放比例
     */
    double calculateFitWidthScale() const;

    /**
     * @brief 计算适应高度的缩放比例
     */
    double calculateFitHeightScale() const;

    /**
     * @brief 将窗口坐标转换为图像坐标
     */
    QPoint widgetToImage(const QPoint& pos) const;

    /**
     * @brief 将图像坐标转换为窗口坐标
     */
    QPoint imageToWidget(const QPoint& pos) const;

    /**
     * @brief 绘制所有ROI
     */
    void drawROIs(QPainter& painter);

    /**
     * @brief 绘制单个ROI
     */
    void drawROI(QPainter& painter, const ROIShapePtr& roi);

    /**
     * @brief 查找鼠标位置的ROI
     */
    ROIShapePtr findROIAt(const QPoint& imagePos);

    /**
     * @brief 完成当前ROI绘制
     */
    void finishCurrentROI();

private:
    Base::ImageData::Ptr currentImage_;     // 当前显示的图像
    QPixmap displayPixmap_;                 // 显示的QPixmap

    double scale_;                          // 缩放比例
    FitMode fitMode_;                       // 适应模式

    // 平移相关
    bool isPanning_;                        // 是否正在拖拽
    QPoint lastMousePos_;                   // 上次鼠标位置
    QPoint offset_;                         // 图像偏移量

    // ROI相关
    InteractionMode interactionMode_;       // 交互模式
    std::vector<ROIShapePtr> rois_;        // ROI列表
    ROIShapePtr selectedROI_;              // 选中的ROI
    ROIShapePtr currentROI_;               // 正在绘制的ROI
    QPoint roiStartPoint_;                 // ROI起始点
    QColor roiColor_;                      // ROI颜色
    int roiLineWidth_;                     // ROI线宽
    bool isDrawing_;                       // 是否正在绘制

    // 缩放步进
    static constexpr double ZOOM_STEP = 1.2;        // 缩放步进
    static constexpr double MIN_SCALE = 0.01;       // 最小缩放
    static constexpr double MAX_SCALE = 100.0;      // 最大缩放
};

} // namespace UI
} // namespace VisionForge
