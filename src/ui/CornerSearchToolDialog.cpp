/**
 * @file CornerSearchToolDialog.cpp
 * @brief 角点搜索工具对话框实现
 * @author VisionForge Team
 * @date 2025-12-30
 */

#include "ui/CornerSearchToolDialog.h"
#include "ui/HalconImageViewer.h"
#include "algorithm/CornerSearchTool.h"
#include "base/Logger.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <opencv2/opencv.hpp>

namespace VisionForge {
namespace UI {

CornerSearchToolDialog::CornerSearchToolDialog(Algorithm::CornerSearchTool* tool, QWidget* parent)
    : SearchToolDialog(tool, parent)
    , cornerTool_(tool)
{
    connectSpecificSignals();
    updateSpecificUI();
}

CornerSearchToolDialog::~CornerSearchToolDialog()
{
}

QWidget* CornerSearchToolDialog::createParameterPanel(QWidget* parent)
{
    QWidget* panel = new QWidget(parent);
    QVBoxLayout* layout = new QVBoxLayout(panel);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setSpacing(8);

    createMethodGroup(layout);
    createHarrisParamGroup(layout);
    createShiTomasiParamGroup(layout);
    createFASTParamGroup(layout);
    createFilterGroup(layout);

    layout->addStretch();

    // 初始显示Harris参数
    updateParameterVisibility();

    return panel;
}

void CornerSearchToolDialog::createMethodGroup(QVBoxLayout* layout)
{
    methodGroup_ = new QGroupBox(tr("检测方法"), this);
    QVBoxLayout* groupLayout = new QVBoxLayout(methodGroup_);

    QHBoxLayout* comboLayout = new QHBoxLayout();
    comboLayout->addWidget(new QLabel(tr("方法:"), this));
    methodCombo_ = new QComboBox(this);
    methodCombo_->addItem(tr("Harris角点"), Harris);
    methodCombo_->addItem(tr("Shi-Tomasi角点"), ShiTomasi);
    methodCombo_->addItem(tr("FAST特征点"), FAST);
    methodCombo_->addItem(tr("ORB特征点"), ORB);
#ifdef USE_HALCON
    methodCombo_->addItem(tr("Halcon兴趣点"), HalconPoints);
#endif
    comboLayout->addWidget(methodCombo_, 1);
    groupLayout->addLayout(comboLayout);

    methodDescLabel_ = new QLabel(this);
    methodDescLabel_->setWordWrap(true);
    methodDescLabel_->setStyleSheet("color: #666; font-size: 11px;");
    methodDescLabel_->setText(tr("Harris: 经典的角点检测算法，适合检测边缘交叉点"));
    groupLayout->addWidget(methodDescLabel_);

    layout->addWidget(methodGroup_);
}

void CornerSearchToolDialog::createHarrisParamGroup(QVBoxLayout* layout)
{
    harrisGroup_ = new QGroupBox(tr("Harris参数"), this);
    QGridLayout* groupLayout = new QGridLayout(harrisGroup_);
    groupLayout->setSpacing(6);

    int row = 0;

    // 邻域大小
    groupLayout->addWidget(new QLabel(tr("邻域大小:"), this), row, 0);
    harrisBlockSizeSpin_ = new QSpinBox(this);
    harrisBlockSizeSpin_->setRange(2, 31);
    harrisBlockSizeSpin_->setValue(2);
    harrisBlockSizeSpin_->setSingleStep(1);
    harrisBlockSizeSpin_->setToolTip(tr("角点检测的邻域大小"));
    groupLayout->addWidget(harrisBlockSizeSpin_, row, 1);
    row++;

    // Sobel算子大小
    groupLayout->addWidget(new QLabel(tr("Sobel大小:"), this), row, 0);
    harrisKSizeSpin_ = new QSpinBox(this);
    harrisKSizeSpin_->setRange(1, 31);
    harrisKSizeSpin_->setSingleStep(2);  // 只能是奇数
    harrisKSizeSpin_->setValue(3);
    harrisKSizeSpin_->setToolTip(tr("Sobel算子的孔径大小（奇数）"));
    groupLayout->addWidget(harrisKSizeSpin_, row, 1);
    row++;

    // Harris参数k
    groupLayout->addWidget(new QLabel(tr("Harris K:"), this), row, 0);
    harrisKSpin_ = new QDoubleSpinBox(this);
    harrisKSpin_->setRange(0.01, 0.2);
    harrisKSpin_->setSingleStep(0.01);
    harrisKSpin_->setValue(0.04);
    harrisKSpin_->setToolTip(tr("Harris角点检测的自由参数"));
    groupLayout->addWidget(harrisKSpin_, row, 1);
    row++;

    // 阈值
    groupLayout->addWidget(new QLabel(tr("阈值:"), this), row, 0);
    QHBoxLayout* threshLayout = new QHBoxLayout();
    harrisThresholdSpin_ = new QDoubleSpinBox(this);
    harrisThresholdSpin_->setRange(0.0, 1.0);
    harrisThresholdSpin_->setSingleStep(0.01);
    harrisThresholdSpin_->setValue(0.01);
    harrisThresholdSlider_ = new QSlider(Qt::Horizontal, this);
    harrisThresholdSlider_->setRange(0, 100);
    harrisThresholdSlider_->setValue(1);
    threshLayout->addWidget(harrisThresholdSpin_);
    threshLayout->addWidget(harrisThresholdSlider_);
    groupLayout->addLayout(threshLayout, row, 1);

    layout->addWidget(harrisGroup_);
}

void CornerSearchToolDialog::createShiTomasiParamGroup(QVBoxLayout* layout)
{
    shiTomasiGroup_ = new QGroupBox(tr("Shi-Tomasi参数"), this);
    QGridLayout* groupLayout = new QGridLayout(shiTomasiGroup_);
    groupLayout->setSpacing(6);

    int row = 0;

    // 最大角点数
    groupLayout->addWidget(new QLabel(tr("最大角点数:"), this), row, 0);
    shiTomasiMaxCornersSpin_ = new QSpinBox(this);
    shiTomasiMaxCornersSpin_->setRange(1, 10000);
    shiTomasiMaxCornersSpin_->setValue(100);
    groupLayout->addWidget(shiTomasiMaxCornersSpin_, row, 1);
    row++;

    // 质量水平
    groupLayout->addWidget(new QLabel(tr("质量水平:"), this), row, 0);
    shiTomasiQualitySpin_ = new QDoubleSpinBox(this);
    shiTomasiQualitySpin_->setRange(0.001, 1.0);
    shiTomasiQualitySpin_->setSingleStep(0.01);
    shiTomasiQualitySpin_->setValue(0.01);
    shiTomasiQualitySpin_->setToolTip(tr("角点的最小可接受质量"));
    groupLayout->addWidget(shiTomasiQualitySpin_, row, 1);
    row++;

    // 最小距离
    groupLayout->addWidget(new QLabel(tr("最小距离:"), this), row, 0);
    shiTomasiMinDistSpin_ = new QDoubleSpinBox(this);
    shiTomasiMinDistSpin_->setRange(1.0, 100.0);
    shiTomasiMinDistSpin_->setSingleStep(1.0);
    shiTomasiMinDistSpin_->setValue(10.0);
    shiTomasiMinDistSpin_->setSuffix(" px");
    groupLayout->addWidget(shiTomasiMinDistSpin_, row, 1);
    row++;

    // 邻域大小
    groupLayout->addWidget(new QLabel(tr("邻域大小:"), this), row, 0);
    shiTomasiBlockSizeSpin_ = new QSpinBox(this);
    shiTomasiBlockSizeSpin_->setRange(2, 31);
    shiTomasiBlockSizeSpin_->setValue(3);
    groupLayout->addWidget(shiTomasiBlockSizeSpin_, row, 1);

    layout->addWidget(shiTomasiGroup_);
}

void CornerSearchToolDialog::createFASTParamGroup(QVBoxLayout* layout)
{
    fastGroup_ = new QGroupBox(tr("FAST参数"), this);
    QGridLayout* groupLayout = new QGridLayout(fastGroup_);
    groupLayout->setSpacing(6);

    int row = 0;

    // 阈值
    groupLayout->addWidget(new QLabel(tr("阈值:"), this), row, 0);
    fastThresholdSpin_ = new QSpinBox(this);
    fastThresholdSpin_->setRange(1, 255);
    fastThresholdSpin_->setValue(10);
    fastThresholdSpin_->setToolTip(tr("中心像素与周围像素的亮度差阈值"));
    groupLayout->addWidget(fastThresholdSpin_, row, 1);
    row++;

    // 非极大值抑制
    groupLayout->addWidget(new QLabel(tr("非极大值抑制:"), this), row, 0);
    fastNMSCheck_ = new QCheckBox(tr("启用"), this);
    fastNMSCheck_->setChecked(true);
    groupLayout->addWidget(fastNMSCheck_, row, 1);
    row++;

    // 检测类型
    groupLayout->addWidget(new QLabel(tr("检测类型:"), this), row, 0);
    fastTypeCombo_ = new QComboBox(this);
    fastTypeCombo_->addItem("TYPE_9_16", cv::FastFeatureDetector::TYPE_9_16);
    fastTypeCombo_->addItem("TYPE_7_12", cv::FastFeatureDetector::TYPE_7_12);
    fastTypeCombo_->addItem("TYPE_5_8", cv::FastFeatureDetector::TYPE_5_8);
    groupLayout->addWidget(fastTypeCombo_, row, 1);

    layout->addWidget(fastGroup_);
}

void CornerSearchToolDialog::createFilterGroup(QVBoxLayout* layout)
{
    filterGroup_ = new QGroupBox(tr("结果过滤"), this);
    QGridLayout* groupLayout = new QGridLayout(filterGroup_);
    groupLayout->setSpacing(6);

    int row = 0;

    // 最大角点数
    groupLayout->addWidget(new QLabel(tr("最大角点数:"), this), row, 0);
    maxCornersSpin_ = new QSpinBox(this);
    maxCornersSpin_->setRange(1, 10000);
    maxCornersSpin_->setValue(500);
    groupLayout->addWidget(maxCornersSpin_, row, 1);
    row++;

    // 最小距离
    groupLayout->addWidget(new QLabel(tr("最小距离:"), this), row, 0);
    minDistanceSpin_ = new QDoubleSpinBox(this);
    minDistanceSpin_->setRange(0.0, 100.0);
    minDistanceSpin_->setSingleStep(1.0);
    minDistanceSpin_->setValue(5.0);
    minDistanceSpin_->setSuffix(" px");
    groupLayout->addWidget(minDistanceSpin_, row, 1);
    row++;

    // 亚像素精度
    useSubPixelCheck_ = new QCheckBox(tr("亚像素精度"), this);
    useSubPixelCheck_->setChecked(true);
    groupLayout->addWidget(useSubPixelCheck_, row, 0);

    subPixelWinSizeSpin_ = new QSpinBox(this);
    subPixelWinSizeSpin_->setRange(3, 31);
    subPixelWinSizeSpin_->setSingleStep(2);
    subPixelWinSizeSpin_->setValue(5);
    subPixelWinSizeSpin_->setToolTip(tr("亚像素精化窗口大小"));
    groupLayout->addWidget(subPixelWinSizeSpin_, row, 1);

    layout->addWidget(filterGroup_);
}

void CornerSearchToolDialog::connectSpecificSignals()
{
    // 方法切换
    connect(methodCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &CornerSearchToolDialog::onMethodChanged);

    // Harris阈值同步
    connect(harrisThresholdSlider_, &QSlider::valueChanged, [this](int value) {
        harrisThresholdSpin_->blockSignals(true);
        harrisThresholdSpin_->setValue(value / 100.0);
        harrisThresholdSpin_->blockSignals(false);
        if (previewHelper_) previewHelper_->requestPreview();
    });
    connect(harrisThresholdSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [this](double value) {
        harrisThresholdSlider_->blockSignals(true);
        harrisThresholdSlider_->setValue(static_cast<int>(value * 100));
        harrisThresholdSlider_->blockSignals(false);
        if (previewHelper_) previewHelper_->requestPreview();
    });

    // 亚像素启用控制
    connect(useSubPixelCheck_, &QCheckBox::toggled, subPixelWinSizeSpin_, &QSpinBox::setEnabled);

    // 参数变化触发预览
    auto triggerPreview = [this]() {
        if (previewHelper_) previewHelper_->requestPreview();
    };

    connect(harrisBlockSizeSpin_, QOverload<int>::of(&QSpinBox::valueChanged), triggerPreview);
    connect(harrisKSizeSpin_, QOverload<int>::of(&QSpinBox::valueChanged), triggerPreview);
    connect(harrisKSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged), triggerPreview);
    connect(shiTomasiMaxCornersSpin_, QOverload<int>::of(&QSpinBox::valueChanged), triggerPreview);
    connect(shiTomasiQualitySpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged), triggerPreview);
    connect(shiTomasiMinDistSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged), triggerPreview);
    connect(fastThresholdSpin_, QOverload<int>::of(&QSpinBox::valueChanged), triggerPreview);
    connect(fastNMSCheck_, &QCheckBox::toggled, triggerPreview);
}

void CornerSearchToolDialog::updateParameterVisibility()
{
    CornerMethod method = static_cast<CornerMethod>(methodCombo_->currentData().toInt());

    harrisGroup_->setVisible(method == Harris);
    shiTomasiGroup_->setVisible(method == ShiTomasi);
    fastGroup_->setVisible(method == FAST || method == ORB);
}

void CornerSearchToolDialog::onMethodChanged(int index)
{
    Q_UNUSED(index);
    updateParameterVisibility();

    CornerMethod method = static_cast<CornerMethod>(methodCombo_->currentData().toInt());
    switch (method) {
        case Harris:
            methodDescLabel_->setText(tr("Harris: 经典的角点检测算法，适合检测边缘交叉点"));
            break;
        case ShiTomasi:
            methodDescLabel_->setText(tr("Shi-Tomasi: 改进的Harris角点检测，特征点更稳定"));
            break;
        case FAST:
            methodDescLabel_->setText(tr("FAST: 快速角点检测，适合实时应用"));
            break;
        case ORB:
            methodDescLabel_->setText(tr("ORB: 带方向的FAST特征点，具有旋转不变性"));
            break;
        case HalconPoints:
            methodDescLabel_->setText(tr("Halcon兴趣点: 使用Halcon的点检测算法"));
            break;
    }

    if (previewHelper_) {
        previewHelper_->requestPreview();
    }
}

bool CornerSearchToolDialog::executeSearch(std::vector<SearchResult>& results)
{
    if (!currentImage_) {
        LOG_WARNING("执行角点搜索失败: 未加载图像");
        return false;
    }

    try {
        cv::Mat mat = currentImage_->mat();
        cv::Mat grayMat;
        if (mat.channels() == 3) {
            cv::cvtColor(mat, grayMat, cv::COLOR_BGR2GRAY);
        } else {
            grayMat = mat.clone();
        }

        CornerMethod method = static_cast<CornerMethod>(methodCombo_->currentData().toInt());
        std::vector<cv::Point2f> corners;

        switch (method) {
            case Harris: {
                cv::Mat harrisResponse;
                cv::cornerHarris(grayMat, harrisResponse,
                    harrisBlockSizeSpin_->value(),
                    harrisKSizeSpin_->value(),
                    harrisKSpin_->value());

                // 归一化
                cv::Mat normalized;
                cv::normalize(harrisResponse, normalized, 0, 255, cv::NORM_MINMAX, CV_8U);

                // 阈值化提取角点
                double threshold = harrisThresholdSpin_->value() * 255;
                for (int y = 1; y < normalized.rows - 1; ++y) {
                    for (int x = 1; x < normalized.cols - 1; ++x) {
                        if (normalized.at<uchar>(y, x) > threshold) {
                            // 局部极大值检测
                            uchar val = normalized.at<uchar>(y, x);
                            bool isMax = true;
                            for (int dy = -1; dy <= 1 && isMax; ++dy) {
                                for (int dx = -1; dx <= 1 && isMax; ++dx) {
                                    if (dx == 0 && dy == 0) continue;
                                    if (normalized.at<uchar>(y + dy, x + dx) >= val) {
                                        isMax = false;
                                    }
                                }
                            }
                            if (isMax) {
                                corners.push_back(cv::Point2f(static_cast<float>(x), static_cast<float>(y)));
                            }
                        }
                    }
                }
                break;
            }

            case ShiTomasi: {
                cv::goodFeaturesToTrack(grayMat, corners,
                    shiTomasiMaxCornersSpin_->value(),
                    shiTomasiQualitySpin_->value(),
                    shiTomasiMinDistSpin_->value(),
                    cv::noArray(),
                    shiTomasiBlockSizeSpin_->value());
                break;
            }

            case FAST:
            case ORB: {
                std::vector<cv::KeyPoint> keypoints;
                if (method == FAST) {
                    cv::FAST(grayMat, keypoints,
                        fastThresholdSpin_->value(),
                        fastNMSCheck_->isChecked(),
                        static_cast<cv::FastFeatureDetector::DetectorType>(fastTypeCombo_->currentData().toInt()));
                } else {
                    auto orb = cv::ORB::create(maxCornersSpin_->value());
                    orb->detect(grayMat, keypoints);
                }
                for (const auto& kp : keypoints) {
                    corners.push_back(kp.pt);
                }
                break;
            }

            case HalconPoints:
#ifdef USE_HALCON
                // TODO: 使用Halcon的点检测
#endif
                break;
        }

        // 亚像素精化
        if (useSubPixelCheck_->isChecked() && !corners.empty()) {
            cv::Size winSize(subPixelWinSizeSpin_->value(), subPixelWinSizeSpin_->value());
            cv::TermCriteria criteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 30, 0.01);
            cv::cornerSubPix(grayMat, corners, winSize, cv::Size(-1, -1), criteria);
        }

        // 限制数量
        int maxCount = maxCornersSpin_->value();
        if (static_cast<int>(corners.size()) > maxCount) {
            corners.resize(maxCount);
        }

        // 转换为结果
        results.clear();
        for (size_t i = 0; i < corners.size(); ++i) {
            SearchResult r;
            r.index = static_cast<int>(i) + 1;
            r.x = corners[i].x;
            r.y = corners[i].y;
            r.angle = 0;
            r.scale = 1.0;
            r.score = 1.0;  // 角点没有分数
            results.push_back(r);
        }

        LOG_INFO(QString("角点搜索完成，找到 %1 个角点").arg(results.size()));
        return true;

    } catch (const cv::Exception& e) {
        LOG_ERROR(QString("角点搜索异常: %1").arg(e.what()));
        return false;
    }
}

void CornerSearchToolDialog::applySpecificParameters()
{
    if (!cornerTool_) return;

    // TODO: 将UI参数应用到工具

    LOG_DEBUG("角点搜索参数已应用");
}

void CornerSearchToolDialog::updateSpecificUI()
{
    if (!cornerTool_) return;

    // TODO: 从工具读取参数更新UI
}

} // namespace UI
} // namespace VisionForge
