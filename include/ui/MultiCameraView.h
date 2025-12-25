/**
 * @file MultiCameraView.h
 * @brief 多相机显示视图组件
 *
 * 支持 1/2/4/6/8 路画面同时显示，可切换布局模式
 */

#ifndef VISIONFORGE_UI_MULTI_CAMERA_VIEW_H
#define VISIONFORGE_UI_MULTI_CAMERA_VIEW_H

#include <QWidget>
#include <QGridLayout>
#include <QVector>
#include <QMap>
#include <QLabel>
#include <QFrame>
#include <memory>

// 前向声明
namespace VisionForge {

namespace Base {
class ImageData;
}

namespace Core {
struct StationConfig;
}

namespace UI {

class HalconImageViewer;

/**
 * @brief 多相机显示视图
 *
 * 功能：
 * - 支持 1x1, 1x2, 2x2, 2x3, 2x4 多种布局
 * - 关联工位配置，自动创建对应数量的显示窗口
 * - 支持单画面选中和双击最大化
 * - 每个画面显示位置名称和状态
 */
class MultiCameraView : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief 布局模式
     */
    enum LayoutMode {
        Layout_1x1 = 1,     ///< 单画面
        Layout_1x2 = 2,     ///< 1行2列
        Layout_2x2 = 4,     ///< 2行2列
        Layout_2x3 = 6,     ///< 2行3列
        Layout_2x4 = 8      ///< 2行4列
    };
    Q_ENUM(LayoutMode)

    /**
     * @brief 构造函数
     */
    explicit MultiCameraView(QWidget* parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~MultiCameraView() override;

    // ========== 布局控制 ==========

    /**
     * @brief 设置布局模式
     */
    void setLayoutMode(LayoutMode mode);

    /**
     * @brief 获取当前布局模式
     */
    LayoutMode layoutMode() const { return layoutMode_; }

    /**
     * @brief 根据视图数量自动选择布局
     */
    void setAutoLayout(int viewCount);

    /**
     * @brief 获取布局的行列数
     */
    void getLayoutDimensions(int& rows, int& cols) const;

    // ========== 工位关联 ==========

    /**
     * @brief 关联工位配置
     *
     * 根据工位配置自动创建对应数量的显示窗口
     */
    void setStation(Core::StationConfig* station);

    /**
     * @brief 获取当前关联的工位
     */
    Core::StationConfig* currentStation() const { return currentStation_; }

    // ========== 图像更新 ==========

    /**
     * @brief 更新指定索引的画面
     */
    void updateImage(int viewIndex, const std::shared_ptr<Base::ImageData>& image);

    /**
     * @brief 更新指定位置ID的画面
     */
    void updateImage(const QString& positionId, const std::shared_ptr<Base::ImageData>& image);

    /**
     * @brief 批量更新所有画面
     */
    void updateAllImages(const QMap<QString, std::shared_ptr<Base::ImageData>>& images);

    /**
     * @brief 清空指定画面
     */
    void clearImage(int viewIndex);

    /**
     * @brief 清空所有画面
     */
    void clearAllImages();

    // ========== 选中控制 ==========

    /**
     * @brief 设置选中的视图索引
     */
    void setSelectedView(int index);

    /**
     * @brief 获取当前选中的视图索引
     */
    int selectedView() const { return selectedIndex_; }

    /**
     * @brief 选择下一个视图
     */
    void selectNextView();

    /**
     * @brief 选择上一个视图
     */
    void selectPrevView();

    // ========== 视图访问 ==========

    /**
     * @brief 获取指定索引的视图
     */
    HalconImageViewer* getViewer(int index);

    /**
     * @brief 获取当前选中的视图
     */
    HalconImageViewer* getSelectedViewer();

    /**
     * @brief 获取视图数量
     */
    int viewerCount() const { return viewers_.size(); }

    // ========== 最大化控制 ==========

    /**
     * @brief 最大化指定视图
     */
    void maximizeView(int index);

    /**
     * @brief 还原布局
     */
    void restoreLayout();

    /**
     * @brief 切换最大化状态
     */
    void toggleMaximize(int index);

    /**
     * @brief 是否处于最大化状态
     */
    bool isMaximized() const { return maximized_; }

    /**
     * @brief 获取最大化的视图索引
     */
    int maximizedIndex() const { return maximizedIndex_; }

    // ========== 状态显示 ==========

    /**
     * @brief 设置视图状态（OK/NG）
     */
    void setViewStatus(int index, bool ok, const QString& message = QString());

    /**
     * @brief 设置视图标题
     */
    void setViewTitle(int index, const QString& title);

signals:
    /**
     * @brief 视图被选中
     */
    void viewSelected(int index, const QString& positionId);

    /**
     * @brief 视图被双击（请求最大化/还原）
     */
    void viewDoubleClicked(int index);

    /**
     * @brief 布局模式改变
     */
    void layoutModeChanged(LayoutMode mode);

    /**
     * @brief 最大化状态改变
     */
    void maximizeStateChanged(bool maximized, int index);

protected:
    void resizeEvent(QResizeEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    bool eventFilter(QObject* obj, QEvent* event) override;

private slots:
    void onViewerClicked(int index);
    void onViewerDoubleClicked(int index);

private:
    /**
     * @brief 初始化UI
     */
    void setupUI();

    /**
     * @brief 创建视图容器
     */
    QFrame* createViewerContainer(int index);

    /**
     * @brief 根据工位配置更新布局
     */
    void updateLayoutFromStation();

    /**
     * @brief 重新排列视图
     */
    void rearrangeViewers();

    /**
     * @brief 清空所有视图
     */
    void clearAllViewers();

    /**
     * @brief 更新选中状态的视觉效果
     */
    void updateSelectionVisuals();

    /**
     * @brief 获取位置ID对应的视图索引
     */
    int getViewIndexForPosition(const QString& positionId) const;

    // ========== 成员变量 ==========
    QGridLayout* gridLayout_;                    ///< 网格布局
    QVector<QFrame*> viewerContainers_;          ///< 视图容器列表
    QVector<HalconImageViewer*> viewers_;        ///< 视图列表
    QVector<QLabel*> titleLabels_;               ///< 标题标签
    QVector<QLabel*> statusLabels_;              ///< 状态标签

    Core::StationConfig* currentStation_ = nullptr; ///< 当前工位配置
    QMap<QString, int> positionToIndex_;         ///< 位置ID到视图索引映射

    LayoutMode layoutMode_ = Layout_2x2;         ///< 当前布局模式
    int selectedIndex_ = 0;                      ///< 当前选中索引
    bool maximized_ = false;                     ///< 是否最大化
    int maximizedIndex_ = -1;                    ///< 最大化的视图索引

    // 样式
    QString normalStyle_;                        ///< 普通样式
    QString selectedStyle_;                      ///< 选中样式
    QString okStyle_;                            ///< OK状态样式
    QString ngStyle_;                            ///< NG状态样式
};

} // namespace UI
} // namespace VisionForge

#endif // VISIONFORGE_UI_MULTI_CAMERA_VIEW_H
