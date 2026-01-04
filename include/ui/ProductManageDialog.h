/**
 * @file ProductManageDialog.h
 * @brief 产品管理对话框 - 参考VisionASM设计
 * @details 管理产品列表、场景配置、标定、对位等
 * @author VisionForge Team
 * @date 2025-01-04
 */

#pragma once

#include <QDialog>
#include <QVector>
#include <QString>
#include <QJsonObject>
#include <memory>

class QTableWidget;
class QComboBox;
class QPushButton;
class QLabel;
class QLineEdit;
class QGroupBox;
class QSplitter;
class QVBoxLayout;
class QHBoxLayout;
class QStackedWidget;
class QScrollArea;

namespace VisionForge {

// 前向声明
namespace Core {
    class WorkStation;
}

namespace UI {

class HalconImageViewer;

/**
 * @brief 场景信息
 */
struct SceneInfo {
    QString id;                 ///< 场景ID
    QString name;               ///< 场景名称
    QString imagePath;          ///< 场景图片路径
    QJsonObject cameraParams;   ///< 拍照参数
    bool enabled = true;        ///< 是否启用

    SceneInfo() = default;
    SceneInfo(const QString& sceneId, const QString& sceneName)
        : id(sceneId), name(sceneName) {}
};

/**
 * @brief 产品信息
 */
struct ProductInfo {
    int index;                      ///< 产品索引（序号）
    QString name;                   ///< 产品名称
    QString description;            ///< 产品描述
    bool enabled = true;            ///< 是否启用
    QVector<SceneInfo> scenes;      ///< 场景列表
    QString workStationId;          ///< 关联的工位ID
    QJsonObject alignParams;        ///< 对位参数
    QJsonObject calibParams;        ///< 标定参数
    QJsonObject customParams;       ///< 定制参数
    QJsonObject inspectParams;      ///< 边检参数

    ProductInfo() : index(0) {}
    ProductInfo(int idx, const QString& productName)
        : index(idx), name(productName) {}

    QJsonObject toJson() const;
    static ProductInfo fromJson(const QJsonObject& json);
};

/**
 * @class ProductManageDialog
 * @brief 产品管理对话框
 *
 * 功能：
 * - 产品列表管理（添加、删除、重命名）
 * - 场景配置（预览、拍照参数）
 * - 标定配置
 * - 对位配置
 * - 定制功能
 * - 边检配置
 * - 产品拷贝
 */
class ProductManageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProductManageDialog(QWidget* parent = nullptr);
    ~ProductManageDialog() override;

    /**
     * @brief 设置工位ID（用于关联配置）
     */
    void setWorkStationId(const QString& wsId);

    /**
     * @brief 获取当前选中的产品索引
     */
    int currentProductIndex() const;

    /**
     * @brief 设置当前选中的产品索引
     */
    void setCurrentProductIndex(int index);

    /**
     * @brief 获取产品数量
     */
    int productCount() const;

    /**
     * @brief 获取产品信息
     */
    ProductInfo getProduct(int index) const;

    /**
     * @brief 设置产品信息
     */
    void setProduct(int index, const ProductInfo& product);

    /**
     * @brief 添加产品
     * @return 新产品的索引
     */
    int addProduct(const QString& name = QString());

    /**
     * @brief 删除产品
     */
    bool removeProduct(int index);

    /**
     * @brief 拷贝产品
     */
    bool copyProduct(int fromIndex, int toIndex);

    /**
     * @brief 加载产品配置
     */
    bool loadConfig(const QString& configPath);

    /**
     * @brief 保存产品配置
     */
    bool saveConfig(const QString& configPath);

signals:
    /**
     * @brief 产品选中变化
     */
    void productSelected(int index);

    /**
     * @brief 产品配置变化
     */
    void productChanged(int index);

    /**
     * @brief 请求预览
     */
    void previewRequested(int productIndex, int sceneIndex);

    /**
     * @brief 请求拍照参数设置
     */
    void cameraParamRequested(int productIndex, int sceneIndex);

    /**
     * @brief 请求标定设置
     */
    void calibrationRequested(int productIndex);

    /**
     * @brief 请求对位设置
     */
    void alignmentRequested(int productIndex);

    /**
     * @brief 请求定制设置
     */
    void customSettingRequested(int productIndex);

    /**
     * @brief 请求边检设置
     */
    void inspectionRequested(int productIndex);

private slots:
    // 产品列表操作
    void onProductSelectionChanged();
    void onProductDoubleClicked(int row, int column);
    void onAddProduct();
    void onRemoveProduct();
    void onRenameProduct();

    // 工具栏按钮
    void onPreviewClicked();
    void onSceneClicked();
    void onCalibrationClicked();
    void onAlignmentClicked();
    void onCustomClicked();
    void onInspectionClicked();

    // 场景操作
    void onCameraParamClicked();
    void onSceneImageClicked(int sceneIndex);

    // 产品拷贝
    void onCopyProduct();

    // 对话框按钮
    void onOkClicked();
    void onCancelClicked();

private:
    void setupUI();
    void createLeftPanel(QWidget* parent);
    void createRightPanel(QWidget* parent);
    void createToolBar(QVBoxLayout* layout);
    void createContentArea(QVBoxLayout* layout);
    void createCopyPanel(QVBoxLayout* layout);
    void createButtonPanel();

    void updateProductTable();
    void updateProductContent();
    void updateSceneDisplay();
    void updateCopyComboBoxes();

    void initDefaultProducts(int count = 80);
    QString generateProductName(int index);

private:
    QString workStationId_;
    QVector<ProductInfo> products_;
    int currentProductIndex_;
    int currentSceneIndex_;

    // 主布局
    QSplitter* mainSplitter_;

    // ========== 左侧面板 ==========
    QWidget* leftPanel_;
    QLabel* stationLabel_;          ///< 工位标签
    QTableWidget* productTable_;    ///< 产品列表

    // ========== 右侧面板 ==========
    QWidget* rightPanel_;

    // 工具栏
    QWidget* toolBarWidget_;
    QPushButton* previewBtn_;       ///< 预览
    QPushButton* sceneBtn_;         ///< 场景
    QPushButton* calibBtn_;         ///< 标定
    QPushButton* alignBtn_;         ///< 对位
    QPushButton* customBtn_;        ///< 定制
    QPushButton* inspectBtn_;       ///< 边检

    // 内容区域
    QGroupBox* contentGroup_;
    QLabel* productTitleLabel_;     ///< 产品标题
    QScrollArea* sceneScrollArea_;  ///< 场景滚动区域
    QWidget* sceneContainer_;       ///< 场景容器
    QVector<QLabel*> sceneImageLabels_;     ///< 场景图片标签
    QVector<QPushButton*> cameraParamBtns_; ///< 拍照参数按钮

    // 产品拷贝面板
    QWidget* copyPanel_;
    QComboBox* copyFromCombo_;      ///< 源产品下拉框
    QComboBox* copyToCombo_;        ///< 目标产品下拉框
    QPushButton* copyArrowBtn_;     ///< 箭头按钮
    QPushButton* copyProductBtn_;   ///< 产品拷贝按钮

    // 底部按钮
    QPushButton* okBtn_;
    QPushButton* cancelBtn_;
};

} // namespace UI
} // namespace VisionForge
