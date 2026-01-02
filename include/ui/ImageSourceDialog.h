/**
 * @file ImageSourceDialog.h
 * @brief 图像数据源对话框
 * @author VisionForge Team
 * @date 2025-12-18
 */

#pragma once

#include <QDialog>
#include <QRadioButton>
#include <QButtonGroup>
#include <QGroupBox>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>

namespace VisionForge {
namespace UI {

/**
 * @class ImageSourceDialog
 * @brief 图像数据源选择对话框
 *
 * 用于模拟运行时选择图像数据源文件夹
 */
class ImageSourceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ImageSourceDialog(QWidget* parent = nullptr);
    ~ImageSourceDialog() override = default;

    // 获取选中的场景
    int getSelectedScene() const;

    // 获取各位置的图像路径
    QStringList getImagePaths() const;

private slots:
    void onSelectPath(int positionIndex);
    void onConfirm();
    void onCancel();

private:
    void setupUI();
    void updatePathTable();

private:
    // 场景选择
    QGroupBox* sceneGroup_ = nullptr;
    QButtonGroup* sceneButtonGroup_ = nullptr;
    QList<QRadioButton*> sceneRadios_;

    // 路径选择表格
    QGroupBox* pathGroup_ = nullptr;
    QTableWidget* pathTable_ = nullptr;

    // 路径数据
    QStringList imagePaths_;

    // 底部按钮
    QPushButton* confirmBtn_ = nullptr;
    QPushButton* cancelBtn_ = nullptr;
};

} // namespace UI
} // namespace VisionForge
