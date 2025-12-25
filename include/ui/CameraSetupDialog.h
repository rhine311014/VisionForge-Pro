/**
 * @file CameraSetupDialog.h
 * @brief 相机设置对话框
 * @details 首次运行软件时配置相机通道和位置映射
 */

#ifndef VISIONFORGE_UI_CAMERA_SETUP_DIALOG_H
#define VISIONFORGE_UI_CAMERA_SETUP_DIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QLabel>
#include <QSlider>
#include <QPushButton>
#include <QGroupBox>
#include <QLineEdit>
#include <QTableWidget>
#include <QTimer>

namespace VisionForge {
namespace HAL { class ICamera; }  // 前向声明
namespace UI {

/**
 * @brief 相机通道配置结构
 */
struct CameraChannelConfig {
    QString channelId;          ///< 通道ID (CH1, CH2, ...)
    QString originalCameraId;   ///< 原相机ID
    QString mappedCameraId;     ///< 映射的相机ID
};

/**
 * @brief 位置相机映射结构
 */
struct PositionCameraMapping {
    QString positionId;         ///< 位置ID
    QString originalChannel;    ///< 原相机通道
    QString mappedChannel;      ///< 新相机通道
};

/**
 * @class CameraSetupDialog
 * @brief 相机设置对话框
 * @details 提供相机通道配置、位置映射、采集测试等功能
 */
class CameraSetupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CameraSetupDialog(QWidget* parent = nullptr);
    ~CameraSetupDialog();

    /**
     * @brief 检查是否有已保存的相机配置
     * @return true 有配置，false 无配置
     */
    static bool hasExistingConfig();

    /**
     * @brief 获取配置文件路径
     */
    static QString configFilePath();

    /**
     * @brief 是否保存并退出
     */
    bool isSaveAndExit() const { return saveAndExit_; }

    /**
     * @brief 是否跳过配置
     */
    bool isSkipped() const { return skipped_; }

signals:
    void configurationSaved();
    void configurationSkipped();

private slots:
    void onCameraIdChanged(int index);
    void onCaptureClicked();
    void onExposureChanged(int value);
    void onGainChanged(int value);
    void onStationButtonClicked();
    void onConfirmClicked();
    void onCancelClicked();
    void onSkipClicked();
    void refreshCameraList();
    void onChannelMappingChanged();
    void onPositionMappingChanged(int row, int column);
    void onCaptureTimerTimeout();       ///< 连续采集定时器槽

private:
    void setupUI();
    void createCameraTestGroup();
    void createChannelSettingsGroup();
    void createPositionSettingsGroup();
    void createSideButtons();
    void loadExistingConfig();
    bool saveConfig();
    void updatePreview(const QImage& image);
    void populateCameraList();
    void updateChannelMappings();
    void updatePositionMappings();
    HAL::ICamera* ensureCameraReady(const QString& cameraId);  ///< 确保相机准备好
    void startCapture();                ///< 开始连续采集
    void stopCapture();                 ///< 停止连续采集

private:
    // 相机采集测试组件
    QGroupBox* cameraTestGroup_;
    QLabel* previewLabel_;
    QComboBox* cameraIdCombo_;
    QPushButton* captureButton_;
    QSlider* exposureSlider_;
    QSlider* gainSlider_;
    QLabel* exposureValueLabel_;
    QLabel* gainValueLabel_;

    // 相机通道设置组件
    QGroupBox* channelSettingsGroup_;
    QTableWidget* channelTable_;

    // 位置信息设置组件
    QGroupBox* positionSettingsGroup_;
    QTableWidget* positionTable_;

    // 侧边按钮
    QPushButton* stationButton_;
    QPushButton* confirmButton_;
    QPushButton* cancelButton_;
    QPushButton* skipButton_;

    // 状态栏
    QLabel* statusLabel_;

    // 数据
    QStringList availableCameras_;
    QVector<CameraChannelConfig> channelConfigs_;
    QVector<PositionCameraMapping> positionMappings_;
    int currentStationIndex_ = 0;
    int channelCount_ = 1;
    int positionCount_ = 4;

    // 状态标志
    bool saveAndExit_ = false;
    bool skipped_ = false;
    bool isCapturing_ = false;          ///< 是否正在连续采集
    QString currentCameraId_;           ///< 当前采集的相机ID

    // 定时器（用于预览刷新）
    QTimer* previewTimer_;
};

} // namespace UI
} // namespace VisionForge

#endif // VISIONFORGE_UI_CAMERA_SETUP_DIALOG_H
