/**
 * @file ImageSourceDialog.cpp
 * @brief 图像数据源对话框实现
 */

#include "ui/ImageSourceDialog.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QFileDialog>
#include <QLabel>

namespace VisionForge {
namespace UI {

ImageSourceDialog::ImageSourceDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("图像数据源");
    setMinimumSize(600, 400);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    // 初始化路径列表（默认1个位置）
    imagePaths_ << "";

    setupUI();
}

void ImageSourceDialog::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(15, 15, 15, 15);

    // ===== 场景选择 =====
    sceneGroup_ = new QGroupBox("场景选择", this);
    QHBoxLayout* sceneLayout = new QHBoxLayout(sceneGroup_);

    sceneButtonGroup_ = new QButtonGroup(this);

    // 创建场景单选按钮（根据实际场景数量动态创建）
    QRadioButton* scene1Radio = new QRadioButton("场景一", this);
    scene1Radio->setChecked(true);
    sceneButtonGroup_->addButton(scene1Radio, 0);
    sceneRadios_.append(scene1Radio);
    sceneLayout->addWidget(scene1Radio);

    sceneLayout->addStretch();

    mainLayout->addWidget(sceneGroup_);

    // ===== 位置图像路径选择 =====
    pathGroup_ = new QGroupBox("位置图像路径选择", this);
    QVBoxLayout* pathLayout = new QVBoxLayout(pathGroup_);

    // 创建表格
    pathTable_ = new QTableWidget(this);
    pathTable_->setColumnCount(3);
    pathTable_->setHorizontalHeaderLabels({"序号", "图像路径", "操作"});

    // 设置表格样式
    pathTable_->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    pathTable_->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    pathTable_->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
    pathTable_->setColumnWidth(0, 60);
    pathTable_->setColumnWidth(2, 100);
    pathTable_->verticalHeader()->setVisible(false);
    pathTable_->setSelectionBehavior(QAbstractItemView::SelectRows);
    pathTable_->setEditTriggers(QAbstractItemView::NoEditTriggers);

    updatePathTable();

    pathLayout->addWidget(pathTable_);

    mainLayout->addWidget(pathGroup_, 1);

    // ===== 底部按钮 =====
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();

    confirmBtn_ = new QPushButton("确认", this);
    confirmBtn_->setFixedSize(80, 30);
    connect(confirmBtn_, &QPushButton::clicked, this, &ImageSourceDialog::onConfirm);
    buttonLayout->addWidget(confirmBtn_);

    cancelBtn_ = new QPushButton("取消", this);
    cancelBtn_->setFixedSize(80, 30);
    connect(cancelBtn_, &QPushButton::clicked, this, &ImageSourceDialog::onCancel);
    buttonLayout->addWidget(cancelBtn_);

    buttonLayout->addStretch();

    mainLayout->addLayout(buttonLayout);
}

void ImageSourceDialog::updatePathTable()
{
    pathTable_->setRowCount(imagePaths_.size());

    for (int i = 0; i < imagePaths_.size(); ++i) {
        // 序号列
        QTableWidgetItem* indexItem = new QTableWidgetItem(QString("位置%1").arg(i + 1));
        indexItem->setTextAlignment(Qt::AlignCenter);
        pathTable_->setItem(i, 0, indexItem);

        // 路径列 - 使用QLineEdit
        QLineEdit* pathEdit = new QLineEdit(this);
        pathEdit->setText(imagePaths_[i]);
        pathEdit->setReadOnly(true);
        pathTable_->setCellWidget(i, 1, pathEdit);

        // 操作列 - 选择路径按钮
        QPushButton* selectBtn = new QPushButton(QString("选择路径-%1").arg(i + 1), this);
        connect(selectBtn, &QPushButton::clicked, this, [this, i]() {
            onSelectPath(i);
        });
        pathTable_->setCellWidget(i, 2, selectBtn);

        pathTable_->setRowHeight(i, 35);
    }
}

void ImageSourceDialog::onSelectPath(int positionIndex)
{
    QString dir = QFileDialog::getExistingDirectory(this,
        QString("选择位置%1的图像文件夹").arg(positionIndex + 1),
        imagePaths_[positionIndex].isEmpty() ? QDir::homePath() : imagePaths_[positionIndex],
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (!dir.isEmpty()) {
        imagePaths_[positionIndex] = dir;

        // 更新表格中的路径显示
        QLineEdit* pathEdit = qobject_cast<QLineEdit*>(pathTable_->cellWidget(positionIndex, 1));
        if (pathEdit) {
            pathEdit->setText(dir);
        }
    }
}

int ImageSourceDialog::getSelectedScene() const
{
    return sceneButtonGroup_->checkedId();
}

QStringList ImageSourceDialog::getImagePaths() const
{
    return imagePaths_;
}

void ImageSourceDialog::onConfirm()
{
    accept();
}

void ImageSourceDialog::onCancel()
{
    reject();
}

} // namespace UI
} // namespace VisionForge
