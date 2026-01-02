/**
 * @file CalibrationWizard.h
 * @brief 多位置标定向导
 *
 * 分步引导完成多位置标定，防止遗漏
 * 支持多工位多相机多位置的统一标定流程
 */

#ifndef VISIONFORGE_UI_CALIBRATION_WIZARD_H
#define VISIONFORGE_UI_CALIBRATION_WIZARD_H

#include <QWizard>
#include <QWizardPage>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QTableWidget>
#include <QProgressBar>
#include <QListWidget>
#include <QGroupBox>
#include <QVector>
#include <memory>

namespace VisionForge {

namespace Core {
struct StationConfig;
struct PositionCameraBinding;
}

namespace Algorithm {
class CalibrationResult;
}

namespace UI {

class HalconImageViewer;

/**
 * @brief 标定点数据
 */
struct CalibPointData {
    int index = -1;             ///< 点索引 (0-8 for 9点标定)
    double imageX = 0.0;        ///< 图像X坐标
    double imageY = 0.0;        ///< 图像Y坐标
    double worldX = 0.0;        ///< 物理X坐标
    double worldY = 0.0;        ///< 物理Y坐标
    bool captured = false;      ///< 是否已采集
};

/**
 * @brief 位置标定状态
 */
struct PositionCalibStatus {
    QString positionId;         ///< 位置ID
    QString positionName;       ///< 位置名称
    bool enabled = true;        ///< 是否启用
    bool completed = false;     ///< 是否完成
    int completedPoints = 0;    ///< 已完成点数
    int totalPoints = 9;        ///< 总点数
    QVector<CalibPointData> points; ///< 标定点数据
};

/**
 * @brief 多位置标定向导
 *
 * 功能：
 * - 向导式流程引导
 * - 支持选择要标定的位置
 * - 逐位置标定
 * - 完成总结和保存
 */
class CalibrationWizard : public QWizard {
    Q_OBJECT

public:
    /**
     * @brief 向导页面ID
     */
    enum PageId {
        Page_Intro = 0,         ///< 介绍页
        Page_SelectPositions,   ///< 选择位置页
        Page_Calibration,       ///< 标定页 (动态创建)
        Page_Summary = 100      ///< 总结页
    };

    /**
     * @brief 构造函数
     */
    explicit CalibrationWizard(Core::StationConfig* station, QWidget* parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~CalibrationWizard() override;

    /**
     * @brief 获取工位配置
     */
    Core::StationConfig* station() const { return station_; }

    /**
     * @brief 获取位置标定状态列表
     */
    QVector<PositionCalibStatus>& calibrationStatus() { return calibStatus_; }

    /**
     * @brief 获取当前标定位置索引
     */
    int currentPositionIndex() const { return currentPosIndex_; }

    /**
     * @brief 设置当前标定位置索引
     */
    void setCurrentPositionIndex(int index);

    /**
     * @brief 获取选中的位置索引列表
     */
    QVector<int> selectedPositionIndices() const { return selectedPositions_; }

    /**
     * @brief 设置选中的位置索引列表
     */
    void setSelectedPositionIndices(const QVector<int>& indices);

signals:
    /**
     * @brief 请求移动到位置
     */
    void moveToPositionRequested(const QString& positionId);

    /**
     * @brief 请求拍照
     */
    void captureRequested(const QString& positionId);

    /**
     * @brief 标定完成
     */
    void calibrationCompleted(const QString& stationId);

    /**
     * @brief 单个位置标定完成
     */
    void positionCalibrated(const QString& positionId);

protected:
    void initializePage(int id) override;
    bool validateCurrentPage() override;
    int nextId() const override;

private:
    /**
     * @brief 创建向导页面
     */
    void setupPages();

    /**
     * @brief 创建介绍页
     */
    QWizardPage* createIntroPage();

    /**
     * @brief 创建位置选择页
     */
    QWizardPage* createPositionSelectPage();

    /**
     * @brief 创建标定页
     */
    QWizardPage* createCalibrationPage();

    /**
     * @brief 创建总结页
     */
    QWizardPage* createSummaryPage();

    /**
     * @brief 初始化标定状态
     */
    void initCalibrationStatus();

    // ========== 数据成员 ==========
    Core::StationConfig* station_;
    QVector<PositionCalibStatus> calibStatus_;
    QVector<int> selectedPositions_;
    int currentPosIndex_ = 0;

    // 页面引用
    QWizardPage* introPage_ = nullptr;
    QWizardPage* selectPage_ = nullptr;
    QWizardPage* calibPage_ = nullptr;
    QWizardPage* summaryPage_ = nullptr;
};

// ============================================================
// 向导页面类
// ============================================================

/**
 * @brief 介绍页
 */
class CalibIntroPage : public QWizardPage {
    Q_OBJECT

public:
    explicit CalibIntroPage(CalibrationWizard* wizard, QWidget* parent = nullptr);

private:
    void setupUI();
    CalibrationWizard* wizard_;
    QLabel* labelIntro_;
    QLabel* labelStationInfo_;
};

/**
 * @brief 位置选择页
 */
class CalibPositionSelectPage : public QWizardPage {
    Q_OBJECT

public:
    explicit CalibPositionSelectPage(CalibrationWizard* wizard, QWidget* parent = nullptr);

    void initializePage() override;
    bool validatePage() override;

private slots:
    void onSelectAll();
    void onDeselectAll();
    void onItemChanged(QListWidgetItem* item);

private:
    void setupUI();
    void updateSelectedCount();

    CalibrationWizard* wizard_;
    QListWidget* listPositions_;
    QPushButton* btnSelectAll_;
    QPushButton* btnDeselectAll_;
    QLabel* labelSelectedCount_;
};

/**
 * @brief 单位置标定页
 */
class CalibPositionPage : public QWizardPage {
    Q_OBJECT

public:
    explicit CalibPositionPage(CalibrationWizard* wizard, QWidget* parent = nullptr);

    void initializePage() override;
    bool isComplete() const override;
    bool validatePage() override;

signals:
    void pointCaptured(int pointIndex, double imageX, double imageY);

private slots:
    void onMoveToPosition();
    void onCapture();
    void onGridPointClicked(int index);
    void onExecuteCalibration();
    void onResetPoints();
    void onImageClicked(int x, int y);

private:
    void setupUI();
    void updateGridButtons();
    void updateProgress();
    void updatePositionInfo();

    CalibrationWizard* wizard_;

    // 位置信息
    QLabel* labelPositionName_;
    QLabel* labelCameraInfo_;
    QPushButton* btnMoveToPos_;

    // 图像显示
    HalconImageViewer* imageViewer_;

    // 3x3 网格按钮
    QPushButton* gridButtons_[9];
    int currentPointIndex_ = -1;

    // 采集控制
    QPushButton* btnCapture_;
    QPushButton* btnReset_;
    QPushButton* btnExecuteCalib_;

    // 进度
    QProgressBar* progressBar_;
    QLabel* labelProgress_;

    // 标定参数
    QLabel* labelGridSpacing_;

    // 完成确认
    QCheckBox* checkCompleted_;
};

/**
 * @brief 总结页
 */
class CalibSummaryPage : public QWizardPage {
    Q_OBJECT

public:
    explicit CalibSummaryPage(CalibrationWizard* wizard, QWidget* parent = nullptr);

    void initializePage() override;

private slots:
    void onSaveResults();
    void onExportReport();

private:
    void setupUI();
    void updateSummary();

    CalibrationWizard* wizard_;
    QTableWidget* tableSummary_;
    QLabel* labelOverallStatus_;
    QPushButton* btnSaveResults_;
    QPushButton* btnExportReport_;
};

} // namespace UI
} // namespace VisionForge

#endif // VISIONFORGE_UI_CALIBRATION_WIZARD_H
