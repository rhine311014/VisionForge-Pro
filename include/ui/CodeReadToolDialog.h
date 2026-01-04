/**
 * @file CodeReadToolDialog.h
 * @brief 读码工具配置对话框
 * @details 支持粗定位+读码的完整配置界面
 */

#pragma once

#include <QDialog>
#include <QButtonGroup>
#include "algorithm/CodeReadTool.h"
#include "ui/ROI.h"
#include "hal/ICamera.h"

class QVBoxLayout;
class QHBoxLayout;
class QFormLayout;
class QComboBox;
class QSpinBox;
class QDoubleSpinBox;
class QCheckBox;
class QPushButton;
class QLabel;
class QGroupBox;
class QRadioButton;
class QTableWidget;
class QTextEdit;
class QSplitter;
class QToolButton;
class QFileDialog;
class QSlider;

namespace VisionForge {
namespace UI {

class ImageViewer;

/**
 * @class CodeReadToolDialog
 * @brief 读码工具配置对话框
 *
 * 功能：
 * - 图像加载和预览
 * - 定位模板ROI绘制
 * - 读码区域ROI绘制
 * - 码制选择（二维码/一维码）
 * - 模型训练
 * - 读码测试
 */
class CodeReadToolDialog : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief ROI绘制模式
     */
    enum DrawMode {
        None,           ///< 无绘制
        DrawTemplate,   ///< 绘制定位模板ROI
        DrawCodeROI     ///< 绘制读码区域ROI
    };

    explicit CodeReadToolDialog(Algorithm::CodeReadTool* tool, QWidget* parent = nullptr);
    ~CodeReadToolDialog();

    /**
     * @brief 设置当前图像
     */
    void setImage(const Base::ImageData::Ptr& image);

    /**
     * @brief 设置相机指针（用于采集功能）
     */
    void setCamera(HAL::ICamera* camera);

signals:
    void trainModelRequested();
    /**
     * @brief 请求采集图像信号
     */
    void captureImageRequested();

private slots:
    // 图像操作
    void onLoadImageClicked();
    void onCaptureImageClicked();
    void onLoadFolderClicked();
    void onCameraGrabClicked();
    void onPrevImageClicked();
    void onNextImageClicked();

    // 绘制模式
    void onDrawModeChanged(int mode);
    void onClearTemplateROI();
    void onClearCodeROI();

    // 参数设置
    void onCodeTypeChanged(int index);
    void onTimeoutChanged(int value);

    // 训练和测试
    void onTrainClicked();
    void onTestClicked();
    void onBatchTestClicked();

    // ROI事件
    void onROICreated(ROIShapePtr roi);
    void onROISelectionChanged(ROIShapePtr roi);

    // 粗定位ROI参数调整
    void onTemplateROIParamChanged();
    void onCodeROIParamChanged();
    void onGenerateTemplateROI();
    void onGenerateCodeROI();

    // 其他
    void onApplyClicked();
    void onCloseClicked();

private:
    void setupUi();
    void createImagePanel(QSplitter* splitter);
    void createControlPanel(QSplitter* splitter);
    void createCodeTypeGroup(QVBoxLayout* layout);
    void createROIGroup(QVBoxLayout* layout);
    void createTrainGroup(QVBoxLayout* layout);
    void createTestGroup(QVBoxLayout* layout);
    void createButtonGroup(QVBoxLayout* layout);

    void updateUi();
    void updateROIDisplay();
    void updateImageNavigation();
    void loadImageAt(int index);
    void appendLog(const QString& message, bool isError = false);
    void updateTemplateROIFromParams();
    void updateCodeROIFromParams();

private:
    Algorithm::CodeReadTool* tool_;

    // 图像相关
    ImageViewer* imageViewer_;
    QStringList imageFiles_;
    int currentImageIndex_;
    Base::ImageData::Ptr currentImage_;

    // 图像导航
    QPushButton* loadImageBtn_;
    QPushButton* captureImageBtn_;
    QPushButton* loadFolderBtn_;
    QPushButton* cameraGrabBtn_;
    QPushButton* prevImageBtn_;
    QPushButton* nextImageBtn_;
    QLabel* imageIndexLabel_;

    // 相机
    HAL::ICamera* camera_;

    // 绘制模式
    DrawMode currentDrawMode_;
    QButtonGroup* drawModeGroup_;
    QRadioButton* noneDrawRadio_;
    QRadioButton* templateDrawRadio_;
    QRadioButton* codeDrawRadio_;
    QPushButton* clearTemplateBtn_;
    QPushButton* clearCodeBtn_;

    // ROI显示
    QLabel* templateROILabel_;
    QLabel* codeROILabel_;
    QRect templateROI_;
    QRect codeROI_;

    // 粗定位模板ROI参数控件
    QSpinBox* templateCenterXSpin_;
    QSpinBox* templateCenterYSpin_;
    QSpinBox* templateWidthSpin_;
    QSpinBox* templateHeightSpin_;
    QDoubleSpinBox* templateAngleSpin_;
    QPushButton* generateTemplateBtn_;
    std::shared_ptr<ROIRotatedRectangle> templateRotatedROI_;

    // 读码区域ROI参数控件
    QSpinBox* codeCenterXSpin_;
    QSpinBox* codeCenterYSpin_;
    QSpinBox* codeWidthSpin_;
    QSpinBox* codeHeightSpin_;
    QDoubleSpinBox* codeAngleSpin_;
    QPushButton* generateCodeBtn_;
    std::shared_ptr<ROIRotatedRectangle> codeRotatedROI_;

    // 码制选择
    QComboBox* codeTypeCombo_;
    QSpinBox* timeoutSpin_;
    QCheckBox* useCoarseLocCheckbox_;

    // 训练
    QPushButton* trainButton_;
    QLabel* trainStatusLabel_;
    bool isTrained_;

    // 测试
    QPushButton* testButton_;
    QPushButton* batchTestButton_;
    QLabel* resultLabel_;
    QTextEdit* logTextEdit_;

    // 底部按钮
    QPushButton* applyButton_;
    QPushButton* closeButton_;
};

} // namespace UI
} // namespace VisionForge
