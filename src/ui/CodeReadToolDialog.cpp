/**
 * @file CodeReadToolDialog.cpp
 * @brief 读码工具配置对话框实现
 */

#include "ui/CodeReadToolDialog.h"
#include "ui/ImageViewer.h"
#include "ui/ROI.h"
#include "base/Logger.h"
#include "base/ImageMemoryPool.h"
#include <opencv2/imgproc.hpp>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QComboBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <QRadioButton>
#include <QButtonGroup>
#include <QTextEdit>
#include <QSplitter>
#include <QFileDialog>
#include <QMessageBox>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QStandardItemModel>
#include <QApplication>
#include <opencv2/imgcodecs.hpp>

namespace VisionForge {
namespace UI {

CodeReadToolDialog::CodeReadToolDialog(Algorithm::CodeReadTool* tool, QWidget* parent)
    : QDialog(parent)
    , tool_(tool)
    , currentImageIndex_(-1)
    , currentDrawMode_(None)
    , isTrained_(false)
    , camera_(nullptr)
    , cameraGrabBtn_(nullptr)
{
    setupUi();
    updateUi();
}

CodeReadToolDialog::~CodeReadToolDialog()
{
}

void CodeReadToolDialog::setImage(const Base::ImageData::Ptr& image)
{
    if (!image) return;

    currentImage_ = image;
    imageViewer_->setImage(image);
    appendLog("已加载图像");
}

void CodeReadToolDialog::setupUi()
{
    setWindowTitle(tr("读码工具配置"));
    setMinimumSize(1200, 800);
    resize(1400, 900);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    // 使用分割器
    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);
    splitter->setChildrenCollapsible(false);

    createImagePanel(splitter);
    createControlPanel(splitter);

    // 设置分割比例
    splitter->setStretchFactor(0, 3);  // 图像区域占3份
    splitter->setStretchFactor(1, 1);  // 控制面板占1份

    mainLayout->addWidget(splitter);
}

void CodeReadToolDialog::createImagePanel(QSplitter* splitter)
{
    QWidget* imagePanel = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(imagePanel);
    layout->setContentsMargins(0, 0, 0, 0);

    // 图像导航工具栏
    QHBoxLayout* navLayout = new QHBoxLayout();

    loadImageBtn_ = new QPushButton(tr("加载图像"), this);
    loadImageBtn_->setToolTip(tr("加载单张图像"));
    connect(loadImageBtn_, &QPushButton::clicked, this, &CodeReadToolDialog::onLoadImageClicked);
    navLayout->addWidget(loadImageBtn_);

    loadFolderBtn_ = new QPushButton(tr("加载文件夹"), this);
    loadFolderBtn_->setToolTip(tr("加载文件夹中的所有图像"));
    connect(loadFolderBtn_, &QPushButton::clicked, this, &CodeReadToolDialog::onLoadFolderClicked);
    navLayout->addWidget(loadFolderBtn_);

    cameraGrabBtn_ = new QPushButton(tr("相机采集"), this);
    cameraGrabBtn_->setToolTip(tr("从相机采集一张图像"));
    cameraGrabBtn_->setEnabled(false);  // 默认禁用，需要setCamera后启用
    connect(cameraGrabBtn_, &QPushButton::clicked, this, &CodeReadToolDialog::onCameraGrabClicked);
    navLayout->addWidget(cameraGrabBtn_);

    navLayout->addSpacing(20);

    prevImageBtn_ = new QPushButton(tr("< 上一张"), this);
    prevImageBtn_->setEnabled(false);
    connect(prevImageBtn_, &QPushButton::clicked, this, &CodeReadToolDialog::onPrevImageClicked);
    navLayout->addWidget(prevImageBtn_);

    imageIndexLabel_ = new QLabel(tr("0/0"), this);
    imageIndexLabel_->setMinimumWidth(80);
    imageIndexLabel_->setAlignment(Qt::AlignCenter);
    navLayout->addWidget(imageIndexLabel_);

    nextImageBtn_ = new QPushButton(tr("下一张 >"), this);
    nextImageBtn_->setEnabled(false);
    connect(nextImageBtn_, &QPushButton::clicked, this, &CodeReadToolDialog::onNextImageClicked);
    navLayout->addWidget(nextImageBtn_);

    navLayout->addStretch();

    layout->addLayout(navLayout);

    // 图像查看器
    imageViewer_ = new ImageViewer(this);
    imageViewer_->setMinimumSize(600, 500);

    // 连接ROI信号
    connect(imageViewer_, &ImageViewer::roiCreated, this, &CodeReadToolDialog::onROICreated);
    connect(imageViewer_, &ImageViewer::roiSelectionChanged, this, &CodeReadToolDialog::onROISelectionChanged);

    layout->addWidget(imageViewer_, 1);

    splitter->addWidget(imagePanel);
}

void CodeReadToolDialog::createControlPanel(QSplitter* splitter)
{
    QWidget* controlPanel = new QWidget(this);
    controlPanel->setMinimumWidth(350);
    controlPanel->setMaximumWidth(450);

    QVBoxLayout* layout = new QVBoxLayout(controlPanel);
    layout->setSpacing(10);

    createCodeTypeGroup(layout);
    createROIGroup(layout);
    createTrainGroup(layout);
    createTestGroup(layout);

    layout->addStretch();

    createButtonGroup(layout);

    splitter->addWidget(controlPanel);
}

void CodeReadToolDialog::createCodeTypeGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox(tr("码制设置"), this);
    QFormLayout* formLayout = new QFormLayout(group);

    // 码制类型
    codeTypeCombo_ = new QComboBox(this);
    codeTypeCombo_->addItem(tr("Data Matrix ECC 200"), Algorithm::CodeReadTool::DataMatrixECC200);
    codeTypeCombo_->addItem(tr("QR Code"), Algorithm::CodeReadTool::QRCode);
    codeTypeCombo_->addItem(tr("PDF417"), Algorithm::CodeReadTool::PDF417);
    codeTypeCombo_->addItem(tr("Aztec Code"), Algorithm::CodeReadTool::Aztec);
    codeTypeCombo_->addItem(tr("--- 一维码 ---"), -1);
    codeTypeCombo_->addItem(tr("Code 128"), Algorithm::CodeReadTool::Code128);
    codeTypeCombo_->addItem(tr("Code 39"), Algorithm::CodeReadTool::Code39);
    codeTypeCombo_->addItem(tr("EAN-13"), Algorithm::CodeReadTool::EAN13);
    codeTypeCombo_->addItem(tr("Code 93"), Algorithm::CodeReadTool::Code93);
    codeTypeCombo_->addItem(tr("Interleaved 2/5"), Algorithm::CodeReadTool::Interleaved2of5);
    codeTypeCombo_->addItem(tr("Codabar"), Algorithm::CodeReadTool::Codabar);

    // 禁用分隔行
    auto* model = qobject_cast<QStandardItemModel*>(codeTypeCombo_->model());
    if (model) {
        QStandardItem* item = model->item(4);
        if (item) {
            item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
        }
    }

    connect(codeTypeCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &CodeReadToolDialog::onCodeTypeChanged);
    formLayout->addRow(tr("码制类型:"), codeTypeCombo_);

    // 超时时间
    timeoutSpin_ = new QSpinBox(this);
    timeoutSpin_->setRange(10, 10000);
    timeoutSpin_->setValue(500);
    timeoutSpin_->setSuffix(tr(" ms"));
    timeoutSpin_->setToolTip(tr("读码超时时间，单位毫秒"));
    connect(timeoutSpin_, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &CodeReadToolDialog::onTimeoutChanged);
    formLayout->addRow(tr("超时时间:"), timeoutSpin_);

    // 是否使用粗定位
    useCoarseLocCheckbox_ = new QCheckBox(tr("启用粗定位修正"), this);
    useCoarseLocCheckbox_->setChecked(true);
    useCoarseLocCheckbox_->setToolTip(tr("使用形状匹配进行粗定位，修正读码区域位置"));
    formLayout->addRow(useCoarseLocCheckbox_);

    layout->addWidget(group);
}

void CodeReadToolDialog::createROIGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox(tr("ROI绘制"), this);
    QVBoxLayout* groupLayout = new QVBoxLayout(group);

    // 绘制模式选择
    QLabel* modeLabel = new QLabel(tr("绘制模式:"), this);
    groupLayout->addWidget(modeLabel);

    drawModeGroup_ = new QButtonGroup(this);

    noneDrawRadio_ = new QRadioButton(tr("无（平移/缩放）"), this);
    noneDrawRadio_->setChecked(true);
    drawModeGroup_->addButton(noneDrawRadio_, None);
    groupLayout->addWidget(noneDrawRadio_);

    templateDrawRadio_ = new QRadioButton(tr("绘制定位模板ROI（绿色）"), this);
    drawModeGroup_->addButton(templateDrawRadio_, DrawTemplate);
    groupLayout->addWidget(templateDrawRadio_);

    codeDrawRadio_ = new QRadioButton(tr("绘制读码区域ROI（蓝色）"), this);
    drawModeGroup_->addButton(codeDrawRadio_, DrawCodeROI);
    groupLayout->addWidget(codeDrawRadio_);

    connect(drawModeGroup_, &QButtonGroup::idClicked,
            this, &CodeReadToolDialog::onDrawModeChanged);

    groupLayout->addSpacing(10);

    // ROI信息显示
    QFormLayout* roiInfoLayout = new QFormLayout();

    templateROILabel_ = new QLabel(tr("未设置"), this);
    templateROILabel_->setStyleSheet("color: green; font-weight: bold;");
    roiInfoLayout->addRow(tr("定位模板ROI:"), templateROILabel_);

    codeROILabel_ = new QLabel(tr("未设置"), this);
    codeROILabel_->setStyleSheet("color: blue; font-weight: bold;");
    roiInfoLayout->addRow(tr("读码区域ROI:"), codeROILabel_);

    groupLayout->addLayout(roiInfoLayout);

    // 清除按钮
    QHBoxLayout* clearLayout = new QHBoxLayout();

    clearTemplateBtn_ = new QPushButton(tr("清除定位ROI"), this);
    connect(clearTemplateBtn_, &QPushButton::clicked, this, &CodeReadToolDialog::onClearTemplateROI);
    clearLayout->addWidget(clearTemplateBtn_);

    clearCodeBtn_ = new QPushButton(tr("清除读码ROI"), this);
    connect(clearCodeBtn_, &QPushButton::clicked, this, &CodeReadToolDialog::onClearCodeROI);
    clearLayout->addWidget(clearCodeBtn_);

    groupLayout->addLayout(clearLayout);

    layout->addWidget(group);
}

void CodeReadToolDialog::createTrainGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox(tr("模型训练"), this);
    QVBoxLayout* groupLayout = new QVBoxLayout(group);

    QLabel* hintLabel = new QLabel(tr(
        "训练步骤：\n"
        "1. 加载参考图像\n"
        "2. 绘制定位模板ROI（用于粗定位）\n"
        "3. 绘制读码区域ROI\n"
        "4. 点击训练按钮"), this);
    hintLabel->setWordWrap(true);
    hintLabel->setStyleSheet("color: gray;");
    groupLayout->addWidget(hintLabel);

    trainButton_ = new QPushButton(tr("训练模型"), this);
    trainButton_->setMinimumHeight(40);
    trainButton_->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; font-weight: bold; }");
    connect(trainButton_, &QPushButton::clicked, this, &CodeReadToolDialog::onTrainClicked);
    groupLayout->addWidget(trainButton_);

    trainStatusLabel_ = new QLabel(tr("状态: 未训练"), this);
    trainStatusLabel_->setStyleSheet("font-weight: bold;");
    groupLayout->addWidget(trainStatusLabel_);

    layout->addWidget(group);
}

void CodeReadToolDialog::createTestGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox(tr("测试"), this);
    QVBoxLayout* groupLayout = new QVBoxLayout(group);

    QHBoxLayout* btnLayout = new QHBoxLayout();

    testButton_ = new QPushButton(tr("单张测试"), this);
    testButton_->setMinimumHeight(35);
    connect(testButton_, &QPushButton::clicked, this, &CodeReadToolDialog::onTestClicked);
    btnLayout->addWidget(testButton_);

    batchTestButton_ = new QPushButton(tr("批量测试"), this);
    batchTestButton_->setMinimumHeight(35);
    connect(batchTestButton_, &QPushButton::clicked, this, &CodeReadToolDialog::onBatchTestClicked);
    btnLayout->addWidget(batchTestButton_);

    groupLayout->addLayout(btnLayout);

    // 结果显示
    resultLabel_ = new QLabel(tr("等待测试..."), this);
    resultLabel_->setWordWrap(true);
    resultLabel_->setStyleSheet("font-size: 14px; padding: 5px; background-color: #f0f0f0; border-radius: 3px;");
    resultLabel_->setMinimumHeight(60);
    groupLayout->addWidget(resultLabel_);

    // 日志区域
    QLabel* logLabel = new QLabel(tr("日志:"), this);
    groupLayout->addWidget(logLabel);

    logTextEdit_ = new QTextEdit(this);
    logTextEdit_->setReadOnly(true);
    logTextEdit_->setMaximumHeight(150);
    logTextEdit_->setStyleSheet("font-family: Consolas, monospace; font-size: 11px;");
    groupLayout->addWidget(logTextEdit_);

    layout->addWidget(group);
}

void CodeReadToolDialog::createButtonGroup(QVBoxLayout* layout)
{
    QHBoxLayout* btnLayout = new QHBoxLayout();

    applyButton_ = new QPushButton(tr("应用"), this);
    applyButton_->setMinimumWidth(100);
    connect(applyButton_, &QPushButton::clicked, this, &CodeReadToolDialog::onApplyClicked);
    btnLayout->addWidget(applyButton_);

    btnLayout->addStretch();

    closeButton_ = new QPushButton(tr("关闭"), this);
    closeButton_->setMinimumWidth(100);
    connect(closeButton_, &QPushButton::clicked, this, &CodeReadToolDialog::onCloseClicked);
    btnLayout->addWidget(closeButton_);

    layout->addLayout(btnLayout);
}

void CodeReadToolDialog::updateUi()
{
    if (!tool_) return;

    // 更新码制选择
    int codeType = tool_->getCodeType();
    for (int i = 0; i < codeTypeCombo_->count(); ++i) {
        if (codeTypeCombo_->itemData(i).toInt() == codeType) {
            codeTypeCombo_->setCurrentIndex(i);
            break;
        }
    }

    // 更新超时时间
    timeoutSpin_->setValue(tool_->getTimeout());

    // 更新训练状态
    // 这里可以检查tool是否已经训练过
    updateROIDisplay();
}

void CodeReadToolDialog::updateROIDisplay()
{
    if (templateROI_.isValid()) {
        templateROILabel_->setText(QString("(%1,%2)-(%3,%4)")
            .arg(templateROI_.left()).arg(templateROI_.top())
            .arg(templateROI_.right()).arg(templateROI_.bottom()));
    } else {
        templateROILabel_->setText(tr("未设置"));
    }

    if (codeROI_.isValid()) {
        codeROILabel_->setText(QString("(%1,%2)-(%3,%4)")
            .arg(codeROI_.left()).arg(codeROI_.top())
            .arg(codeROI_.right()).arg(codeROI_.bottom()));
    } else {
        codeROILabel_->setText(tr("未设置"));
    }
}

void CodeReadToolDialog::updateImageNavigation()
{
    int count = imageFiles_.size();
    bool hasImages = count > 0;

    prevImageBtn_->setEnabled(hasImages && currentImageIndex_ > 0);
    nextImageBtn_->setEnabled(hasImages && currentImageIndex_ < count - 1);

    if (hasImages) {
        imageIndexLabel_->setText(QString("%1/%2").arg(currentImageIndex_ + 1).arg(count));
    } else {
        imageIndexLabel_->setText("0/0");
    }
}

void CodeReadToolDialog::loadImageAt(int index)
{
    if (index < 0 || index >= imageFiles_.size()) return;

    QString filePath = imageFiles_[index];

    // 使用Qt读取文件（支持中文路径）
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        appendLog(QString("无法打开文件: %1").arg(filePath), true);
        return;
    }

    QByteArray fileData = file.readAll();
    file.close();

    std::vector<uchar> buffer(fileData.begin(), fileData.end());
    cv::Mat mat = cv::imdecode(buffer, cv::IMREAD_COLOR);

    if (mat.empty()) {
        appendLog(QString("无法解码图像: %1").arg(filePath), true);
        return;
    }

    // 创建ImageData
    currentImage_ = Base::ImageMemoryPool::instance().allocate(mat.cols, mat.rows, mat.type());
    if (currentImage_) {
        mat.copyTo(currentImage_->mat());
        imageViewer_->setImage(currentImage_);
        currentImageIndex_ = index;
        updateImageNavigation();
        appendLog(QString("已加载: %1").arg(QFileInfo(filePath).fileName()));
    }
}

void CodeReadToolDialog::appendLog(const QString& message, bool isError)
{
    QString timestamp = QDateTime::currentDateTime().toString("HH:mm:ss");
    QString color = isError ? "red" : "black";
    logTextEdit_->append(QString("<span style='color:%1'>[%2] %3</span>")
        .arg(color).arg(timestamp).arg(message));
}

// ========== 槽函数实现 ==========

void CodeReadToolDialog::onLoadImageClicked()
{
    QString filePath = QFileDialog::getOpenFileName(this,
        tr("选择图像"),
        QString(),
        tr("图像文件 (*.png *.jpg *.jpeg *.bmp *.tif *.tiff);;所有文件 (*)"));

    if (filePath.isEmpty()) return;

    imageFiles_.clear();
    imageFiles_.append(filePath);
    loadImageAt(0);
}

void CodeReadToolDialog::onLoadFolderClicked()
{
    QString dirPath = QFileDialog::getExistingDirectory(this,
        tr("选择图像文件夹"),
        QString(),
        QFileDialog::ShowDirsOnly);

    if (dirPath.isEmpty()) return;

    QDir dir(dirPath);
    QStringList filters;
    filters << "*.png" << "*.jpg" << "*.jpeg" << "*.bmp" << "*.tif" << "*.tiff";

    imageFiles_ = dir.entryList(filters, QDir::Files, QDir::Name);
    for (int i = 0; i < imageFiles_.size(); ++i) {
        imageFiles_[i] = dir.absoluteFilePath(imageFiles_[i]);
    }

    if (imageFiles_.isEmpty()) {
        appendLog(tr("文件夹中没有找到图像文件"), true);
        return;
    }

    appendLog(QString(tr("已加载 %1 张图像")).arg(imageFiles_.size()));
    loadImageAt(0);
}

void CodeReadToolDialog::onPrevImageClicked()
{
    if (currentImageIndex_ > 0) {
        loadImageAt(currentImageIndex_ - 1);
    }
}

void CodeReadToolDialog::onNextImageClicked()
{
    if (currentImageIndex_ < imageFiles_.size() - 1) {
        loadImageAt(currentImageIndex_ + 1);
    }
}

void CodeReadToolDialog::onDrawModeChanged(int mode)
{
    currentDrawMode_ = static_cast<DrawMode>(mode);

    switch (currentDrawMode_) {
    case None:
        imageViewer_->setInteractionMode(ImageViewer::PanMode);
        break;
    case DrawTemplate:
        imageViewer_->setInteractionMode(ImageViewer::DrawRectangle);
        imageViewer_->setROIColor(Qt::green);
        break;
    case DrawCodeROI:
        imageViewer_->setInteractionMode(ImageViewer::DrawRectangle);
        imageViewer_->setROIColor(Qt::blue);
        break;
    }
}

void CodeReadToolDialog::onClearTemplateROI()
{
    templateROI_ = QRect();
    updateROIDisplay();

    // 从ImageViewer中移除绿色ROI
    auto rois = imageViewer_->getROIs();
    for (auto& roi : rois) {
        if (roi && roi->getColor() == Qt::green) {
            imageViewer_->removeROI(roi);
        }
    }

    isTrained_ = false;
    trainStatusLabel_->setText(tr("状态: 未训练"));
    trainStatusLabel_->setStyleSheet("color: black; font-weight: bold;");
    appendLog(tr("已清除定位模板ROI"));
}

void CodeReadToolDialog::onClearCodeROI()
{
    codeROI_ = QRect();
    updateROIDisplay();

    // 从ImageViewer中移除蓝色ROI
    auto rois = imageViewer_->getROIs();
    for (auto& roi : rois) {
        if (roi && roi->getColor() == Qt::blue) {
            imageViewer_->removeROI(roi);
        }
    }

    isTrained_ = false;
    trainStatusLabel_->setText(tr("状态: 未训练"));
    trainStatusLabel_->setStyleSheet("color: black; font-weight: bold;");
    appendLog(tr("已清除读码区域ROI"));
}

void CodeReadToolDialog::onCodeTypeChanged(int index)
{
    if (!tool_) return;

    int codeType = codeTypeCombo_->itemData(index).toInt();
    if (codeType < 0) {
        // 分隔行，恢复之前的选择
        return;
    }

    tool_->setCodeType(static_cast<Algorithm::CodeReadTool::CodeType>(codeType));
    appendLog(QString(tr("码制已设置为: %1")).arg(codeTypeCombo_->currentText()));
}

void CodeReadToolDialog::onTimeoutChanged(int value)
{
    if (!tool_) return;
    tool_->setTimeout(value);
}

void CodeReadToolDialog::onTrainClicked()
{
    if (!tool_) {
        QMessageBox::warning(this, tr("错误"), tr("工具未初始化"));
        return;
    }

    if (!currentImage_) {
        QMessageBox::warning(this, tr("错误"), tr("请先加载图像"));
        return;
    }

    if (!templateROI_.isValid()) {
        QMessageBox::warning(this, tr("错误"), tr("请先绘制定位模板ROI"));
        return;
    }

    if (!codeROI_.isValid()) {
        QMessageBox::warning(this, tr("错误"), tr("请先绘制读码区域ROI"));
        return;
    }

    appendLog(tr("开始训练模型..."));

    // 调用工具的训练方法
    bool success = tool_->train(currentImage_, templateROI_, codeROI_);

    if (success) {
        isTrained_ = true;
        trainStatusLabel_->setText(tr("状态: 已训练"));
        trainStatusLabel_->setStyleSheet("color: green; font-weight: bold;");
        appendLog(tr("模型训练成功!"));
        emit trainModelRequested();
    } else {
        trainStatusLabel_->setText(tr("状态: 训练失败"));
        trainStatusLabel_->setStyleSheet("color: red; font-weight: bold;");
        appendLog(tr("模型训练失败"), true);
    }
}

void CodeReadToolDialog::onTestClicked()
{
    if (!tool_) {
        resultLabel_->setText(tr("工具未初始化"));
        return;
    }

    if (!currentImage_) {
        resultLabel_->setText(tr("请先加载图像"));
        return;
    }

    appendLog(tr("开始测试..."));

    Algorithm::ToolResult result;
    bool success = tool_->process(currentImage_, result);

    if (success && result.success) {
        QString code = result.getValue<QString>("code", "");
        QString type = result.getValue<QString>("type", "");

        resultLabel_->setText(QString(tr("解码成功!\n类型: %1\n内容: %2")).arg(type).arg(code));
        resultLabel_->setStyleSheet("font-size: 14px; padding: 5px; background-color: #c8e6c9; border-radius: 3px;");
        appendLog(QString(tr("解码成功: %1")).arg(code));

        if (result.outputImage) {
            imageViewer_->setImage(result.outputImage);
        }
    } else {
        resultLabel_->setText(QString(tr("解码失败\n%1")).arg(result.errorMessage));
        resultLabel_->setStyleSheet("font-size: 14px; padding: 5px; background-color: #ffcdd2; border-radius: 3px;");
        appendLog(QString(tr("解码失败: %1")).arg(result.errorMessage), true);

        if (result.outputImage) {
            imageViewer_->setImage(result.outputImage);
        }
    }
}

void CodeReadToolDialog::onBatchTestClicked()
{
    if (!tool_) {
        QMessageBox::warning(this, tr("错误"), tr("工具未初始化"));
        return;
    }

    if (imageFiles_.isEmpty()) {
        QMessageBox::warning(this, tr("错误"), tr("请先加载图像文件夹"));
        return;
    }

    appendLog(QString(tr("开始批量测试 %1 张图像...")).arg(imageFiles_.size()));

    int successCount = 0;
    int failCount = 0;

    for (int i = 0; i < imageFiles_.size(); ++i) {
        loadImageAt(i);
        QApplication::processEvents();

        if (!currentImage_) {
            failCount++;
            continue;
        }

        Algorithm::ToolResult result;
        bool success = tool_->process(currentImage_, result);

        if (success && result.success) {
            successCount++;
            QString code = result.getValue<QString>("code", "");
            appendLog(QString("[%1] %2: %3").arg(i + 1).arg(QFileInfo(imageFiles_[i]).fileName()).arg(code));
        } else {
            failCount++;
            appendLog(QString("[%1] %2: 失败 - %3")
                .arg(i + 1).arg(QFileInfo(imageFiles_[i]).fileName()).arg(result.errorMessage), true);
        }
    }

    QString summary = QString(tr("批量测试完成\n成功: %1, 失败: %2\n成功率: %3%"))
        .arg(successCount)
        .arg(failCount)
        .arg(imageFiles_.size() > 0 ? (100.0 * successCount / imageFiles_.size()) : 0, 0, 'f', 1);

    resultLabel_->setText(summary);
    appendLog(summary);
}

void CodeReadToolDialog::onROICreated(ROIShapePtr roi)
{
    if (!roi) return;

    QRectF rectF = roi->boundingRect();
    QRect rect(static_cast<int>(rectF.left()),
               static_cast<int>(rectF.top()),
               static_cast<int>(rectF.width()),
               static_cast<int>(rectF.height()));

    switch (currentDrawMode_) {
    case DrawTemplate:
        // 先清除旧的模板ROI
        {
            auto rois = imageViewer_->getROIs();
            for (auto& r : rois) {
                if (r && r != roi && r->getColor() == Qt::green) {
                    imageViewer_->removeROI(r);
                }
            }
        }
        templateROI_ = rect;
        appendLog(QString(tr("定位模板ROI: (%1,%2)-(%3,%4)"))
            .arg(rect.left()).arg(rect.top()).arg(rect.right()).arg(rect.bottom()));
        break;

    case DrawCodeROI:
        // 先清除旧的读码ROI
        {
            auto rois = imageViewer_->getROIs();
            for (auto& r : rois) {
                if (r && r != roi && r->getColor() == Qt::blue) {
                    imageViewer_->removeROI(r);
                }
            }
        }
        codeROI_ = rect;
        appendLog(QString(tr("读码区域ROI: (%1,%2)-(%3,%4)"))
            .arg(rect.left()).arg(rect.top()).arg(rect.right()).arg(rect.bottom()));
        break;

    default:
        break;
    }

    updateROIDisplay();

    // 绘制完成后自动切换回平移模式
    noneDrawRadio_->setChecked(true);
    onDrawModeChanged(None);
}

void CodeReadToolDialog::onROISelectionChanged(ROIShapePtr roi)
{
    // 可以在这里实现选中ROI的高亮等功能
    Q_UNUSED(roi);
}

void CodeReadToolDialog::onApplyClicked()
{
    // 保存参数
    appendLog(tr("参数已应用"));
    accept();
}

void CodeReadToolDialog::onCloseClicked()
{
    reject();
}

void CodeReadToolDialog::setCamera(HAL::ICamera* camera)
{
    camera_ = camera;
    if (cameraGrabBtn_) {
        cameraGrabBtn_->setEnabled(camera_ != nullptr && camera_->isOpen());
    }
}

void CodeReadToolDialog::onCameraGrabClicked()
{
    if (!camera_ || !camera_->isOpen()) {
        QMessageBox::warning(this, tr("警告"), tr("相机未连接或未打开"));
        return;
    }

    // 启动采集
    bool wasGrabbing = camera_->isGrabbing();
    if (!wasGrabbing) {
        if (!camera_->startGrabbing()) {
            QMessageBox::warning(this, tr("警告"), tr("启动采集失败"));
            return;
        }
    }

    // 获取图像
    auto image = camera_->grabImage(1000);  // 1秒超时

    // 如果之前没有在采集，停止采集
    if (!wasGrabbing) {
        camera_->stopGrabbing();
    }

    if (image) {
        // 应用图像变换（旋转、镜像）
        HAL::ICamera::Config config = camera_->getConfig();
        if (config.rotationAngle != 0 || config.flipHorizontal || config.flipVertical) {
            cv::Mat mat = image->mat();
            if (!mat.empty()) {
                cv::Mat result = mat.clone();

                // 1. 旋转
                if (config.rotationAngle != 0) {
                    int rotateCode = -1;
                    switch (config.rotationAngle) {
                        case 90:
                            rotateCode = cv::ROTATE_90_CLOCKWISE;
                            break;
                        case 180:
                            rotateCode = cv::ROTATE_180;
                            break;
                        case 270:
                            rotateCode = cv::ROTATE_90_COUNTERCLOCKWISE;
                            break;
                    }
                    if (rotateCode >= 0) {
                        cv::Mat rotated;
                        cv::rotate(result, rotated, rotateCode);
                        result = rotated;
                    }
                }

                // 2. 镜像
                if (config.flipHorizontal && config.flipVertical) {
                    cv::Mat flipped;
                    cv::flip(result, flipped, -1);
                    result = flipped;
                } else if (config.flipHorizontal) {
                    cv::Mat flipped;
                    cv::flip(result, flipped, 1);
                    result = flipped;
                } else if (config.flipVertical) {
                    cv::Mat flipped;
                    cv::flip(result, flipped, 0);
                    result = flipped;
                }

                image = std::make_shared<Base::ImageData>(result);
            }
        }

        currentImage_ = image;
        imageViewer_->setImage(image);
        appendLog(tr("已从相机采集图像"));
    } else {
        QMessageBox::warning(this, tr("警告"), tr("采集图像失败"));
    }
}

} // namespace UI
} // namespace VisionForge
