/**
 * @file AIDetectionToolDialog.cpp
 * @brief AI检测工具参数编辑对话框实现
 * @author VisionForge Team
 * @date 2025-12-18
 */

#include "ui/AIDetectionToolDialog.h"
#include "ui/HalconImageViewer.h"
#include "algorithm/AIDetectionTool.h"
#include "base/Logger.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QSplitter>
#include <QMessageBox>
#include <QFileDialog>
#include <QTimer>
#include <QSlider>
#include <QHeaderView>
#include <QStyle>

namespace VisionForge {
namespace UI {

AIDetectionToolDialog::AIDetectionToolDialog(Algorithm::AIDetectionTool* tool, QWidget* parent)
    : QDialog(parent)
    , tool_(tool)
    , leftPanel_(nullptr)
    , imageViewer_(nullptr)
    , mainSplitter_(nullptr)
    , loadImageBtn_(nullptr)
    , captureImageBtn_(nullptr)
    , paramTabWidget_(nullptr)
    , modelPathEdit_(nullptr)
    , browseModelBtn_(nullptr)
    , configPathEdit_(nullptr)
    , browseConfigBtn_(nullptr)
    , loadModelBtn_(nullptr)
    , unloadModelBtn_(nullptr)
    , modelStatusLabel_(nullptr)
    , taskTypeCombo_(nullptr)
    , inferenceEngineCombo_(nullptr)
    , useGPUCheckBox_(nullptr)
    , confidenceThresholdSpinBox_(nullptr)
    , confidenceSlider_(nullptr)
    , nmsThresholdSpinBox_(nullptr)
    , inputWidthSpinBox_(nullptr)
    , inputHeightSpinBox_(nullptr)
    , classNamesEdit_(nullptr)
    , loadClassNamesBtn_(nullptr)
    , clearClassNamesBtn_(nullptr)
    , resultsTable_(nullptr)
    , previewBtn_(nullptr)
    , okBtn_(nullptr)
    , cancelBtn_(nullptr)
    , applyBtn_(nullptr)
    , previewHelper_(nullptr)
    , autoPreviewCheck_(nullptr)
{
    setWindowTitle("AI检测设置");
    setMinimumSize(950, 650);
    resize(1100, 750);
    setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint);

    createUI();
    connectSignals();
    updateUI();
}

AIDetectionToolDialog::~AIDetectionToolDialog()
{
}

void AIDetectionToolDialog::setImage(const Base::ImageData::Ptr& image)
{
    currentImage_ = image;
    if (imageViewer_ && image) {
        imageViewer_->setImage(image);
    }
}

void AIDetectionToolDialog::updateUI()
{
    if (!tool_) return;

    if (!taskTypeCombo_ || !inferenceEngineCombo_ || !confidenceThresholdSpinBox_) {
        return;
    }

    // 阻塞信号
    taskTypeCombo_->blockSignals(true);
    inferenceEngineCombo_->blockSignals(true);
    useGPUCheckBox_->blockSignals(true);
    confidenceThresholdSpinBox_->blockSignals(true);
    confidenceSlider_->blockSignals(true);
    nmsThresholdSpinBox_->blockSignals(true);
    inputWidthSpinBox_->blockSignals(true);
    inputHeightSpinBox_->blockSignals(true);

    // 更新值
    modelPathEdit_->setText(tool_->modelPath());
    taskTypeCombo_->setCurrentIndex(static_cast<int>(tool_->taskType()));
    // 使用 findData 确保正确匹配推理引擎
    int engineIndex = inferenceEngineCombo_->findData(static_cast<int>(tool_->inferenceEngine()));
    if (engineIndex >= 0) {
        inferenceEngineCombo_->setCurrentIndex(engineIndex);
    }
    useGPUCheckBox_->setChecked(tool_->useGPU());
    confidenceThresholdSpinBox_->setValue(tool_->confidenceThreshold());
    confidenceSlider_->setValue(static_cast<int>(tool_->confidenceThreshold() * 100));
    nmsThresholdSpinBox_->setValue(tool_->nmsThreshold());
    inputWidthSpinBox_->setValue(tool_->inputSize().width());
    inputHeightSpinBox_->setValue(tool_->inputSize().height());

    // 更新类别名称
    QStringList classNames = tool_->classNames();
    classNamesEdit_->setText(classNames.join("\n"));

    // 恢复信号
    taskTypeCombo_->blockSignals(false);
    inferenceEngineCombo_->blockSignals(false);
    useGPUCheckBox_->blockSignals(false);
    confidenceThresholdSpinBox_->blockSignals(false);
    confidenceSlider_->blockSignals(false);
    nmsThresholdSpinBox_->blockSignals(false);
    inputWidthSpinBox_->blockSignals(false);
    inputHeightSpinBox_->blockSignals(false);

    updateModelStatus();
}

void AIDetectionToolDialog::showEvent(QShowEvent* event)
{
    QDialog::showEvent(event);
    updateUI();

    if (currentImage_ && imageViewer_) {
        QTimer::singleShot(200, this, [this]() {
            if (currentImage_ && imageViewer_) {
                imageViewer_->setImage(currentImage_);
            }
        });
    }
}

void AIDetectionToolDialog::createUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(8, 8, 8, 8);
    mainLayout->setSpacing(8);

    // 主分割器
    mainSplitter_ = new QSplitter(Qt::Horizontal, this);

    // 左侧面板
    leftPanel_ = new QWidget(mainSplitter_);
    createLeftPanel(leftPanel_);

    // 右侧面板
    QWidget* rightPanel = new QWidget(mainSplitter_);
    createRightPanel(rightPanel);

    mainSplitter_->addWidget(leftPanel_);
    mainSplitter_->addWidget(rightPanel);
    mainSplitter_->setStretchFactor(0, 3);
    mainSplitter_->setStretchFactor(1, 2);

    mainLayout->addWidget(mainSplitter_, 1);

    // 创建预览辅助器
    previewHelper_ = new PreviewHelper(this, 300);  // AI检测耗时较长，使用300ms延迟

    // 预览选项行
    QHBoxLayout* previewOptLayout = new QHBoxLayout();
    autoPreviewCheck_ = new QCheckBox("实时预览", this);
    autoPreviewCheck_->setChecked(false);  // 默认关闭，因为AI检测较慢
    autoPreviewCheck_->setToolTip("启用后参数修改会自动更新预览（需模型已加载）");
    previewOptLayout->addWidget(autoPreviewCheck_);
    previewOptLayout->addStretch();
    mainLayout->addLayout(previewOptLayout);

    // 底部按钮
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();

    previewBtn_ = new QPushButton("预览", this);
    okBtn_ = new QPushButton("确定", this);
    cancelBtn_ = new QPushButton("取消", this);
    applyBtn_ = new QPushButton("应用", this);

    previewBtn_->setMinimumWidth(80);
    okBtn_->setMinimumWidth(80);
    cancelBtn_->setMinimumWidth(80);
    applyBtn_->setMinimumWidth(80);

    buttonLayout->addWidget(previewBtn_);
    buttonLayout->addWidget(okBtn_);
    buttonLayout->addWidget(cancelBtn_);
    buttonLayout->addWidget(applyBtn_);

    mainLayout->addLayout(buttonLayout);
}

void AIDetectionToolDialog::createLeftPanel(QWidget* parent)
{
    QVBoxLayout* layout = new QVBoxLayout(parent);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(4);

    // 使用垂直分割器
    QSplitter* splitter = new QSplitter(Qt::Vertical, parent);

    // 图像显示
    QWidget* imageWidget = new QWidget(splitter);
    QVBoxLayout* imageLayout = new QVBoxLayout(imageWidget);
    imageLayout->setContentsMargins(0, 0, 0, 0);

    // 标题栏布局
    QHBoxLayout* titleLayout = new QHBoxLayout();
    QLabel* titleLabel = new QLabel("检测图像", imageWidget);
    titleLabel->setStyleSheet("font-weight: bold; font-size: 12px;");
    titleLayout->addWidget(titleLabel);
    titleLayout->addStretch();

    // 加载图片按钮
    loadImageBtn_ = new QPushButton(tr("加载图片"), imageWidget);
    loadImageBtn_->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
    loadImageBtn_->setToolTip(tr("从文件加载图片"));
    loadImageBtn_->setMaximumWidth(100);
    titleLayout->addWidget(loadImageBtn_);

    // 采集图像按钮
    captureImageBtn_ = new QPushButton(tr("采集图像"), imageWidget);
    captureImageBtn_->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    captureImageBtn_->setToolTip(tr("从相机采集图像"));
    captureImageBtn_->setMaximumWidth(100);
    titleLayout->addWidget(captureImageBtn_);

    imageViewer_ = new HalconImageViewer(imageWidget);
    imageViewer_->setMinimumSize(400, 300);
    imageViewer_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    imageLayout->addLayout(titleLayout);
    imageLayout->addWidget(imageViewer_, 1);

    // 结果表格
    QWidget* tableWidget = new QWidget(splitter);
    QVBoxLayout* tableLayout = new QVBoxLayout(tableWidget);
    tableLayout->setContentsMargins(0, 0, 0, 0);

    QLabel* tableTitle = new QLabel("检测结果", tableWidget);
    tableTitle->setStyleSheet("font-weight: bold; font-size: 12px;");

    resultsTable_ = new QTableWidget(tableWidget);
    resultsTable_->setColumnCount(5);
    resultsTable_->setHorizontalHeaderLabels({"ID", "类别", "置信度", "中心X", "中心Y"});
    resultsTable_->horizontalHeader()->setStretchLastSection(true);
    resultsTable_->setSelectionBehavior(QAbstractItemView::SelectRows);
    resultsTable_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    resultsTable_->setFixedHeight(150);

    tableLayout->addWidget(tableTitle);
    tableLayout->addWidget(resultsTable_);

    splitter->addWidget(imageWidget);
    splitter->addWidget(tableWidget);
    splitter->setStretchFactor(0, 3);
    splitter->setStretchFactor(1, 1);

    layout->addWidget(splitter);
}

void AIDetectionToolDialog::createRightPanel(QWidget* parent)
{
    QVBoxLayout* layout = new QVBoxLayout(parent);
    layout->setContentsMargins(4, 0, 0, 0);
    layout->setSpacing(8);

    // 选项卡控件
    paramTabWidget_ = new QTabWidget(parent);

    // Tab 1: 模型设置
    QWidget* modelTab = new QWidget(paramTabWidget_);
    QVBoxLayout* modelLayout = new QVBoxLayout(modelTab);
    modelLayout->setContentsMargins(8, 8, 8, 8);
    modelLayout->setSpacing(8);
    createModelGroup(modelLayout);
    createInferenceGroup(modelLayout);
    modelLayout->addStretch();
    paramTabWidget_->addTab(modelTab, "模型设置");

    // Tab 2: 检测参数
    QWidget* paramTab = new QWidget(paramTabWidget_);
    QVBoxLayout* paramLayout = new QVBoxLayout(paramTab);
    paramLayout->setContentsMargins(8, 8, 8, 8);
    paramLayout->setSpacing(8);
    createDetectionParamGroup(paramLayout);
    paramLayout->addStretch();
    paramTabWidget_->addTab(paramTab, "检测参数");

    // Tab 3: 类别名称
    QWidget* classTab = new QWidget(paramTabWidget_);
    QVBoxLayout* classLayout = new QVBoxLayout(classTab);
    classLayout->setContentsMargins(8, 8, 8, 8);
    classLayout->setSpacing(8);
    createClassNamesGroup(classLayout);
    paramTabWidget_->addTab(classTab, "类别名称");

    layout->addWidget(paramTabWidget_, 1);
}

void AIDetectionToolDialog::createModelGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("模型文件", this);
    QVBoxLayout* groupLayout = new QVBoxLayout(group);

    // 模型路径
    QHBoxLayout* modelLayout = new QHBoxLayout();
    modelLayout->addWidget(new QLabel("模型路径:"));
    modelPathEdit_ = new QLineEdit(this);
    modelPathEdit_->setPlaceholderText("选择模型文件 (.onnx, .pb, .bin, .dltp, etc.)");
    modelLayout->addWidget(modelPathEdit_, 1);
    browseModelBtn_ = new QPushButton("浏览...", this);
    modelLayout->addWidget(browseModelBtn_);
    groupLayout->addLayout(modelLayout);

    // 配置路径
    QHBoxLayout* configLayout = new QHBoxLayout();
    configLayout->addWidget(new QLabel("配置路径:"));
    configPathEdit_ = new QLineEdit(this);
    configPathEdit_->setPlaceholderText("选择配置文件（可选）");
    configLayout->addWidget(configPathEdit_, 1);
    browseConfigBtn_ = new QPushButton("浏览...", this);
    configLayout->addWidget(browseConfigBtn_);
    groupLayout->addLayout(configLayout);

    // 加载/卸载按钮
    QHBoxLayout* btnLayout = new QHBoxLayout();
    loadModelBtn_ = new QPushButton("加载模型", this);
    unloadModelBtn_ = new QPushButton("卸载模型", this);
    btnLayout->addWidget(loadModelBtn_);
    btnLayout->addWidget(unloadModelBtn_);
    btnLayout->addStretch();
    groupLayout->addLayout(btnLayout);

    // 状态标签
    modelStatusLabel_ = new QLabel("模型状态: 未加载", this);
    modelStatusLabel_->setStyleSheet("color: #FF9800;");
    groupLayout->addWidget(modelStatusLabel_);

    layout->addWidget(group);
}

void AIDetectionToolDialog::createInferenceGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("推理设置", this);
    QGridLayout* groupLayout = new QGridLayout(group);
    groupLayout->setSpacing(6);

    int row = 0;

    // 任务类型
    groupLayout->addWidget(new QLabel("任务类型:"), row, 0);
    taskTypeCombo_ = new QComboBox(this);
    taskTypeCombo_->addItem("目标检测", static_cast<int>(Algorithm::AIDetectionTool::ObjectDetection));
    taskTypeCombo_->addItem("图像分类", static_cast<int>(Algorithm::AIDetectionTool::Classification));
    taskTypeCombo_->addItem("缺陷检测", static_cast<int>(Algorithm::AIDetectionTool::DefectDetection));
    taskTypeCombo_->addItem("语义分割", static_cast<int>(Algorithm::AIDetectionTool::Segmentation));
    taskTypeCombo_->addItem("文字识别(OCR)", static_cast<int>(Algorithm::AIDetectionTool::OCR));
    groupLayout->addWidget(taskTypeCombo_, row, 1);
    row++;

    // 推理引擎
    groupLayout->addWidget(new QLabel("推理引擎:"), row, 0);
    inferenceEngineCombo_ = new QComboBox(this);
    inferenceEngineCombo_->addItem("OpenCV DNN", static_cast<int>(Algorithm::AIDetectionTool::OpenCVDNN));
    inferenceEngineCombo_->addItem("ONNX Runtime", static_cast<int>(Algorithm::AIDetectionTool::ONNXRuntime));
    inferenceEngineCombo_->addItem("TensorRT", static_cast<int>(Algorithm::AIDetectionTool::TensorRT));
    inferenceEngineCombo_->addItem("海康深度学习", static_cast<int>(Algorithm::AIDetectionTool::HikvisionDL));
    inferenceEngineCombo_->addItem("Halcon深度学习", static_cast<int>(Algorithm::AIDetectionTool::HalconDL));
    groupLayout->addWidget(inferenceEngineCombo_, row, 1);
    row++;

    // GPU开关
    groupLayout->addWidget(new QLabel("硬件加速:"), row, 0);
    useGPUCheckBox_ = new QCheckBox("使用GPU加速 (CUDA)", this);
    groupLayout->addWidget(useGPUCheckBox_, row, 1);

    layout->addWidget(group);
}

void AIDetectionToolDialog::createDetectionParamGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("检测参数", this);
    QGridLayout* groupLayout = new QGridLayout(group);
    groupLayout->setSpacing(6);

    int row = 0;

    // 置信度阈值
    groupLayout->addWidget(new QLabel("置信度阈值:"), row, 0);
    QHBoxLayout* confLayout = new QHBoxLayout();
    confidenceThresholdSpinBox_ = new QDoubleSpinBox(this);
    confidenceThresholdSpinBox_->setRange(0.0, 1.0);
    confidenceThresholdSpinBox_->setSingleStep(0.05);
    confidenceThresholdSpinBox_->setValue(0.5);
    confLayout->addWidget(confidenceThresholdSpinBox_);
    confidenceSlider_ = new QSlider(Qt::Horizontal, this);
    confidenceSlider_->setRange(0, 100);
    confidenceSlider_->setValue(50);
    confLayout->addWidget(confidenceSlider_);
    groupLayout->addLayout(confLayout, row, 1);
    row++;

    // NMS阈值
    groupLayout->addWidget(new QLabel("NMS阈值:"), row, 0);
    nmsThresholdSpinBox_ = new QDoubleSpinBox(this);
    nmsThresholdSpinBox_->setRange(0.0, 1.0);
    nmsThresholdSpinBox_->setSingleStep(0.05);
    nmsThresholdSpinBox_->setValue(0.45);
    nmsThresholdSpinBox_->setToolTip("非极大值抑制阈值，用于去除重叠检测框");
    groupLayout->addWidget(nmsThresholdSpinBox_, row, 1);
    row++;

    // 输入尺寸
    groupLayout->addWidget(new QLabel("输入尺寸:"), row, 0);
    QHBoxLayout* sizeLayout = new QHBoxLayout();
    inputWidthSpinBox_ = new QSpinBox(this);
    inputWidthSpinBox_->setRange(32, 2048);
    inputWidthSpinBox_->setValue(640);
    inputWidthSpinBox_->setSingleStep(32);
    sizeLayout->addWidget(inputWidthSpinBox_);
    sizeLayout->addWidget(new QLabel("x"));
    inputHeightSpinBox_ = new QSpinBox(this);
    inputHeightSpinBox_->setRange(32, 2048);
    inputHeightSpinBox_->setValue(640);
    inputHeightSpinBox_->setSingleStep(32);
    sizeLayout->addWidget(inputHeightSpinBox_);
    sizeLayout->addWidget(new QLabel("px"));
    sizeLayout->addStretch();
    groupLayout->addLayout(sizeLayout, row, 1);

    layout->addWidget(group);

    // 说明
    QLabel* noteLabel = new QLabel(
        "说明:\n"
        "- 置信度阈值: 低于此值的检测结果将被过滤\n"
        "- NMS阈值: 控制重叠检测框的去除程度\n"
        "- 输入尺寸: 需与模型训练时的尺寸匹配", this);
    noteLabel->setStyleSheet("color: #888; font-size: 11px;");
    noteLabel->setWordWrap(true);
    layout->addWidget(noteLabel);
}

void AIDetectionToolDialog::createClassNamesGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("类别名称列表", this);
    QVBoxLayout* groupLayout = new QVBoxLayout(group);

    // 说明
    QLabel* infoLabel = new QLabel("每行一个类别名称，顺序需与模型输出一致:", this);
    infoLabel->setStyleSheet("color: #888;");
    groupLayout->addWidget(infoLabel);

    // 文本编辑框
    classNamesEdit_ = new QTextEdit(this);
    classNamesEdit_->setPlaceholderText("person\ncar\nbicycle\n...");
    groupLayout->addWidget(classNamesEdit_, 1);

    // 按钮
    QHBoxLayout* btnLayout = new QHBoxLayout();
    loadClassNamesBtn_ = new QPushButton("从文件加载", this);
    clearClassNamesBtn_ = new QPushButton("清空", this);
    btnLayout->addWidget(loadClassNamesBtn_);
    btnLayout->addWidget(clearClassNamesBtn_);
    btnLayout->addStretch();
    groupLayout->addLayout(btnLayout);

    layout->addWidget(group, 1);
}

void AIDetectionToolDialog::connectSignals()
{
    // 模型管理
    connect(browseModelBtn_, &QPushButton::clicked, this, &AIDetectionToolDialog::onBrowseModelClicked);
    connect(browseConfigBtn_, &QPushButton::clicked, this, &AIDetectionToolDialog::onBrowseConfigClicked);
    connect(loadModelBtn_, &QPushButton::clicked, this, &AIDetectionToolDialog::onLoadModelClicked);
    connect(unloadModelBtn_, &QPushButton::clicked, this, &AIDetectionToolDialog::onUnloadModelClicked);

    // 推理参数
    connect(taskTypeCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &AIDetectionToolDialog::onTaskTypeChanged);
    connect(inferenceEngineCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &AIDetectionToolDialog::onInferenceEngineChanged);
    connect(useGPUCheckBox_, &QCheckBox::toggled,
            this, &AIDetectionToolDialog::onUseGPUChanged);

    // 检测参数
    connect(confidenceThresholdSpinBox_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &AIDetectionToolDialog::onConfidenceThresholdChanged);
    connect(confidenceSlider_, &QSlider::valueChanged, this, [this](int value) {
        confidenceThresholdSpinBox_->setValue(value / 100.0);
    });
    connect(nmsThresholdSpinBox_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &AIDetectionToolDialog::onNmsThresholdChanged);
    connect(inputWidthSpinBox_, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &AIDetectionToolDialog::onInputSizeChanged);
    connect(inputHeightSpinBox_, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &AIDetectionToolDialog::onInputSizeChanged);

    // 类别名称
    connect(loadClassNamesBtn_, &QPushButton::clicked, this, &AIDetectionToolDialog::onLoadClassNamesClicked);
    connect(clearClassNamesBtn_, &QPushButton::clicked, this, &AIDetectionToolDialog::onClearClassNamesClicked);

    // 实时预览
    connect(autoPreviewCheck_, &QCheckBox::toggled,
            previewHelper_, &PreviewHelper::setAutoPreviewEnabled);
    connect(previewHelper_, &PreviewHelper::previewTriggered,
            this, &AIDetectionToolDialog::onAutoPreview);

    // 图像操作按钮
    connect(loadImageBtn_, &QPushButton::clicked, this, &AIDetectionToolDialog::onLoadImageClicked);
    connect(captureImageBtn_, &QPushButton::clicked, this, &AIDetectionToolDialog::onCaptureImageClicked);

    // 对话框按钮
    connect(previewBtn_, &QPushButton::clicked, this, &AIDetectionToolDialog::onPreviewClicked);
    connect(okBtn_, &QPushButton::clicked, this, &AIDetectionToolDialog::onOkClicked);
    connect(cancelBtn_, &QPushButton::clicked, this, &AIDetectionToolDialog::onCancelClicked);
    connect(applyBtn_, &QPushButton::clicked, this, &AIDetectionToolDialog::onApplyClicked);
}

void AIDetectionToolDialog::onBrowseModelClicked()
{
    QString filePath = QFileDialog::getOpenFileName(
        this, "选择模型文件",
        QString(),
        "所有模型文件 (*.onnx *.pb *.caffemodel *.weights *.pt *.bin *.dltp);;"
        "ONNX模型 (*.onnx);;"
        "TensorFlow模型 (*.pb);;"
        "Caffe模型 (*.caffemodel);;"
        "YOLO模型 (*.weights);;"
        "海康模型 (*.bin);;"
        "Halcon深度学习模型 (*.dltp);;"
        "所有文件 (*)");

    if (!filePath.isEmpty()) {
        modelPathEdit_->setText(filePath);
    }
}

void AIDetectionToolDialog::onBrowseConfigClicked()
{
    QString filePath = QFileDialog::getOpenFileName(
        this, "选择配置文件",
        QString(),
        "配置文件 (*.cfg *.pbtxt *.prototxt *.yaml *.json);;所有文件 (*)");

    if (!filePath.isEmpty()) {
        configPathEdit_->setText(filePath);
    }
}

void AIDetectionToolDialog::onLoadModelClicked()
{
    if (!tool_) return;

    QString modelPath = modelPathEdit_->text().trimmed();
    if (modelPath.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先选择模型文件");
        return;
    }

    // 检查文件是否存在
    QFileInfo fileInfo(modelPath);
    if (!fileInfo.exists()) {
        QMessageBox::warning(this, "加载失败",
            QString("模型文件不存在:\n%1").arg(modelPath));
        return;
    }

    // 检查文件格式
    QString ext = fileInfo.suffix().toLower();
    QStringList supportedFormats = {"onnx", "pb", "pbtxt", "caffemodel", "weights", "bin", "hdl", "hdict", "hdev", "hdm", "hdvppmodel", "model", "dltp"};
    if (!supportedFormats.contains(ext)) {
        QMessageBox::warning(this, "加载失败",
            QString("不支持的模型格式: .%1\n\n支持的格式:\n"
                    "- OpenCV DNN: .onnx, .pb, .caffemodel, .weights\n"
                    "- 海康SDK: .bin, .hdvppmodel, .model\n"
                    "- Halcon DL: .hdl, .hdict, .hdm, .dltp").arg(ext));
        return;
    }

    QString configPath = configPathEdit_->text().trimmed();

    // 设置GPU加速选项
    tool_->setUseGPU(useGPUCheckBox_->isChecked());

    // loadModel 会根据文件扩展名自动设置正确的推理引擎
    if (tool_->loadModel(modelPath, configPath)) {
        updateModelStatus();

        // 更新下拉框以反映正确的推理引擎
        int engineIndex = inferenceEngineCombo_->findData(static_cast<int>(tool_->inferenceEngine()));
        if (engineIndex >= 0) {
            inferenceEngineCombo_->blockSignals(true);
            inferenceEngineCombo_->setCurrentIndex(engineIndex);
            inferenceEngineCombo_->blockSignals(false);
        }

        QMessageBox::information(this, "成功", "模型加载成功!");
        LOG_INFO(QString("模型已加载: %1, 推理引擎: %2").arg(modelPath).arg(inferenceEngineCombo_->currentText()));
    } else {
        // 根据文件类型给出更具体的错误提示
        QString errorMsg;
        if (ext == "dltp") {
            errorMsg = QString("Deep Learning Tool项目文件加载失败\n\n可能的原因:\n"
                              "1. .dltp同名目录中未找到.hdl模型文件\n"
                              "2. 请确保已在Deep Learning Tool中完成训练\n"
                              "3. 或直接选择训练目录中的.hdl文件\n\n"
                              "请查看日志文件获取详细错误信息");
        } else if (ext == "hdl" || ext == "hdict" || ext == "hdev" || ext == "hdm") {
            errorMsg = QString("Halcon深度学习模型加载失败\n\n可能的原因:\n"
                              "1. Halcon DL许可证未激活或已过期\n"
                              "2. 模型文件损坏或格式不兼容\n"
                              "3. Halcon版本不匹配\n\n"
                              "请查看日志文件获取详细错误信息");
        } else if (ext == "bin" || ext == "hdvppmodel" || ext == "model") {
            errorMsg = "海康深度学习模型加载失败\n\n请确保已安装海康MVS SDK并正确配置";
        } else {
            errorMsg = QString("无法加载模型文件\n\n文件: %1\n\n请检查:\n"
                              "1. 模型文件是否完整\n"
                              "2. 模型格式是否正确\n"
                              "3. 是否需要配置文件(.cfg/.prototxt)").arg(modelPath);
        }
        QMessageBox::warning(this, "加载失败", errorMsg);
        LOG_ERROR(QString("模型加载失败: %1").arg(modelPath));
    }
}

void AIDetectionToolDialog::onUnloadModelClicked()
{
    if (!tool_) return;

    tool_->unloadModel();
    updateModelStatus();
    LOG_INFO("模型已卸载");
}

void AIDetectionToolDialog::onTaskTypeChanged(int index)
{
    if (!tool_) return;

    auto newTaskType = static_cast<Algorithm::AIDetectionTool::TaskType>(
        taskTypeCombo_->itemData(index).toInt());

    tool_->setTaskType(newTaskType);

    // OCR任务现在支持两种模型：
    // 1. Deep OCR模型 (.dltp) - 优先使用
    // 2. 普通DL文字检测模型 (.hdl) - 作为备选
    // 因此不需要强制重新加载模型

    emit parameterChanged();
    previewHelper_->requestPreview();
}

void AIDetectionToolDialog::onInferenceEngineChanged(int index)
{
    if (!tool_) return;
    tool_->setInferenceEngine(static_cast<Algorithm::AIDetectionTool::InferenceEngine>(
        inferenceEngineCombo_->itemData(index).toInt()));
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void AIDetectionToolDialog::onConfidenceThresholdChanged(double value)
{
    if (!tool_) return;
    tool_->setConfidenceThreshold(static_cast<float>(value));
    confidenceSlider_->blockSignals(true);
    confidenceSlider_->setValue(static_cast<int>(value * 100));
    confidenceSlider_->blockSignals(false);
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void AIDetectionToolDialog::onNmsThresholdChanged(double value)
{
    if (!tool_) return;
    tool_->setNmsThreshold(static_cast<float>(value));
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void AIDetectionToolDialog::onInputSizeChanged()
{
    if (!tool_) return;
    tool_->setInputSize(inputWidthSpinBox_->value(), inputHeightSpinBox_->value());
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void AIDetectionToolDialog::onUseGPUChanged(bool checked)
{
    if (!tool_) return;
    tool_->setUseGPU(checked);
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void AIDetectionToolDialog::onLoadClassNamesClicked()
{
    QString filePath = QFileDialog::getOpenFileName(
        this, "选择类别名称文件",
        QString(),
        "文本文件 (*.txt *.names);;所有文件 (*)");

    if (!filePath.isEmpty()) {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            classNamesEdit_->setText(file.readAll());
            file.close();
            LOG_INFO(QString("已加载类别名称: %1").arg(filePath));
        } else {
            QMessageBox::warning(this, "错误", "无法打开文件");
        }
    }
}

void AIDetectionToolDialog::onClearClassNamesClicked()
{
    classNamesEdit_->clear();
}

void AIDetectionToolDialog::onPreviewClicked()
{
    if (!tool_ || !currentImage_) {
        QMessageBox::warning(this, "警告", "请先加载图像");
        return;
    }

    if (!tool_->isModelLoaded()) {
        QMessageBox::warning(this, "警告", "请先加载模型");
        return;
    }

    applyParameters();

    Algorithm::ToolResult result;
    if (tool_->process(currentImage_, result)) {
        if (result.outputImage) {
            imageViewer_->setImage(result.outputImage);
        }
        updateResults();
        LOG_INFO(QString("AI检测完成，找到 %1 个目标").arg(tool_->detectionCount()));
    } else {
        QMessageBox::warning(this, "检测失败", result.errorMessage);
    }
}

void AIDetectionToolDialog::onOkClicked()
{
    applyParameters();
    accept();
}

void AIDetectionToolDialog::onCancelClicked()
{
    reject();
}

void AIDetectionToolDialog::onApplyClicked()
{
    applyParameters();
    emit parameterChanged();
}

void AIDetectionToolDialog::applyParameters()
{
    if (!tool_) return;

    tool_->setTaskType(static_cast<Algorithm::AIDetectionTool::TaskType>(
        taskTypeCombo_->currentData().toInt()));
    tool_->setInferenceEngine(static_cast<Algorithm::AIDetectionTool::InferenceEngine>(
        inferenceEngineCombo_->currentData().toInt()));
    tool_->setUseGPU(useGPUCheckBox_->isChecked());
    tool_->setConfidenceThreshold(static_cast<float>(confidenceThresholdSpinBox_->value()));
    tool_->setNmsThreshold(static_cast<float>(nmsThresholdSpinBox_->value()));
    tool_->setInputSize(inputWidthSpinBox_->value(), inputHeightSpinBox_->value());

    // 解析类别名称
    QString classNamesText = classNamesEdit_->toPlainText().trimmed();
    if (!classNamesText.isEmpty()) {
        QStringList classNames = classNamesText.split('\n', Qt::SkipEmptyParts);
        for (QString& name : classNames) {
            name = name.trimmed();
        }
        tool_->setClassNames(classNames);
    }

    LOG_INFO("AI检测参数已应用");
}

void AIDetectionToolDialog::updateModelStatus()
{
    if (!tool_ || !modelStatusLabel_) return;

    if (tool_->isModelLoaded()) {
        modelStatusLabel_->setText("模型状态: 已加载");
        modelStatusLabel_->setStyleSheet("color: #4CAF50;");
    } else {
        modelStatusLabel_->setText("模型状态: 未加载");
        modelStatusLabel_->setStyleSheet("color: #FF9800;");
    }
}

void AIDetectionToolDialog::updateResults()
{
    if (!tool_ || !resultsTable_) return;

    const auto& detections = tool_->detections();
    resultsTable_->setRowCount(static_cast<int>(detections.size()));

    for (size_t i = 0; i < detections.size(); ++i) {
        const auto& det = detections[i];
        int row = static_cast<int>(i);

        resultsTable_->setItem(row, 0, new QTableWidgetItem(QString::number(det.classId)));
        resultsTable_->setItem(row, 1, new QTableWidgetItem(det.className));
        resultsTable_->setItem(row, 2, new QTableWidgetItem(QString::number(det.confidence, 'f', 3)));
        resultsTable_->setItem(row, 3, new QTableWidgetItem(QString::number(det.center.x(), 'f', 1)));
        resultsTable_->setItem(row, 4, new QTableWidgetItem(QString::number(det.center.y(), 'f', 1)));
    }
}

void AIDetectionToolDialog::onAutoPreview()
{
    if (!tool_ || !currentImage_ || !tool_->isModelLoaded()) {
        return;
    }

    applyParameters();

    Algorithm::ToolResult result;
    if (tool_->process(currentImage_, result)) {
        if (result.outputImage && imageViewer_) {
            imageViewer_->setImage(result.outputImage);
        }
        updateResults();
    }
}

void AIDetectionToolDialog::onLoadImageClicked()
{
    QString filePath = QFileDialog::getOpenFileName(
        this,
        tr("选择图片文件"),
        QString(),
        tr("图片文件 (*.bmp *.png *.jpg *.jpeg *.tiff *.tif);;所有文件 (*.*)")
    );

    if (filePath.isEmpty()) {
        return;
    }

    Base::ImageData::Ptr image = Base::ImageData::loadFromFile(filePath);
    if (image) {
        setImage(image);
        LOG_INFO(QString("加载图片成功: %1").arg(filePath));
    } else {
        QMessageBox::warning(this, tr("加载失败"), tr("无法加载图片文件: %1").arg(filePath));
    }
}

void AIDetectionToolDialog::onCaptureImageClicked()
{
    emit captureImageRequested();
    LOG_INFO("请求采集图像");
}

void AIDetectionToolDialog::setEmbeddedMode(bool embedded)
{
    embeddedMode_ = embedded;

    if (embedded) {
        // 隐藏左侧面板（图像查看器）
        if (leftPanel_) {
            leftPanel_->hide();
        }

        // 隐藏底部按钮
        if (okBtn_) okBtn_->hide();
        if (cancelBtn_) cancelBtn_->hide();
        if (applyBtn_) applyBtn_->hide();
        if (previewBtn_) previewBtn_->hide();
        if (autoPreviewCheck_) autoPreviewCheck_->hide();

        // 调整分割器，让右侧面板占据全部空间
        if (mainSplitter_) {
            mainSplitter_->setSizes({0, 1});
        }

        // 设置最小尺寸以适应嵌入环境
        setMinimumSize(0, 0);
        resize(400, 500);
    } else {
        // 恢复显示
        if (leftPanel_) leftPanel_->show();
        if (okBtn_) okBtn_->show();
        if (cancelBtn_) cancelBtn_->show();
        if (applyBtn_) applyBtn_->show();
        if (previewBtn_) previewBtn_->show();
        if (autoPreviewCheck_) autoPreviewCheck_->show();

        if (mainSplitter_) {
            mainSplitter_->setSizes({600, 400});
        }
    }
}

} // namespace UI
} // namespace VisionForge
