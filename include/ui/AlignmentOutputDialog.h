/**
 * @file AlignmentOutputDialog.h
 * @brief 对位输出配置对话框
 * @details 配置PLC/TCP/串口输出参数
 * @author VisionForge Team
 * @date 2025-12-19
 */

#pragma once

#include <QDialog>
#include "algorithm/AlignmentOutputTool.h"

class QTabWidget;
class QComboBox;
class QPushButton;
class QSpinBox;
class QDoubleSpinBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QCheckBox;
class QTextEdit;
class QRadioButton;

namespace VisionForge {
namespace UI {

/**
 * @class AlignmentOutputDialog
 * @brief 对位输出配置对话框
 *
 * 功能：
 * - PLC输出配置（Modbus、三菱MC、基恩士、欧姆龙）
 * - TCP输出配置（JSON、CSV、二进制、自定义格式）
 * - 串口输出配置
 * - 测试输出功能
 */
class AlignmentOutputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AlignmentOutputDialog(QWidget* parent = nullptr);
    ~AlignmentOutputDialog() override;

    /**
     * @brief 设置输出工具
     */
    void setOutputTool(Algorithm::AlignmentOutputTool* tool);

    /**
     * @brief 获取输出工具
     */
    Algorithm::AlignmentOutputTool* getOutputTool() const;

signals:
    /**
     * @brief 配置已更改信号
     */
    void configurationChanged();

private slots:
    // PLC配置
    void onPLCProtocolChanged(int index);
    void onConnectPLC();
    void onDisconnectPLC();
    void onTestPLCOutput();

    // TCP配置
    void onTCPFormatChanged(int index);
    void onConnectTCP();
    void onDisconnectTCP();
    void onTestTCPOutput();

    // 串口配置
    void onRefreshSerialPorts();
    void onOpenSerial();
    void onCloseSerial();
    void onTestSerialOutput();

    // 输出方式
    void onOutputMethodChanged();

    // 配置
    void onApply();

private:
    void setupUI();
    void createOutputMethodGroup();
    void createPLCTab();
    void createTCPTab();
    void createSerialTab();
    void createButtonGroup();

    void updateFromTool();
    void applyToTool();
    void updateConnectionStatus();

private:
    Algorithm::AlignmentOutputTool* outputTool_;

    // 输出方式选择
    QGroupBox* outputMethodGroup_;
    QCheckBox* plcEnabledCheck_;
    QCheckBox* tcpEnabledCheck_;
    QCheckBox* serialEnabledCheck_;

    // 标签页
    QTabWidget* tabWidget_;

    // PLC配置页
    QWidget* plcTab_;
    QComboBox* plcProtocolCombo_;
    QLineEdit* plcIPEdit_;
    QSpinBox* plcPortSpin_;

    // PLC寄存器配置
    QSpinBox* regOffsetXSpin_;
    QSpinBox* regOffsetYSpin_;
    QSpinBox* regOffsetThetaSpin_;
    QSpinBox* regResultSpin_;
    QSpinBox* regTriggerSpin_;

    // PLC数据格式
    QDoubleSpinBox* plcScaleFactorSpin_;
    QCheckBox* plcSignedCheck_;
    QComboBox* plcDataTypeCombo_;

    // PLC连接
    QPushButton* plcConnectBtn_;
    QPushButton* plcDisconnectBtn_;
    QPushButton* plcTestBtn_;
    QLabel* plcStatusLabel_;

    // TCP配置页
    QWidget* tcpTab_;
    QLineEdit* tcpServerEdit_;
    QSpinBox* tcpPortSpin_;
    QComboBox* tcpFormatCombo_;
    QLineEdit* tcpCustomFormatEdit_;
    QLineEdit* tcpDelimiterEdit_;
    QCheckBox* tcpChecksumCheck_;
    QLineEdit* tcpEndMarkerEdit_;
    QSpinBox* tcpTimeoutSpin_;

    // TCP连接
    QPushButton* tcpConnectBtn_;
    QPushButton* tcpDisconnectBtn_;
    QPushButton* tcpTestBtn_;
    QLabel* tcpStatusLabel_;

    // 预览
    QTextEdit* tcpPreviewEdit_;

    // 串口配置页
    QWidget* serialTab_;
    QComboBox* serialPortCombo_;
    QComboBox* serialBaudRateCombo_;
    QComboBox* serialDataBitsCombo_;
    QComboBox* serialStopBitsCombo_;
    QComboBox* serialParityCombo_;
    QComboBox* serialFormatCombo_;
    QLineEdit* serialCustomFormatEdit_;
    QLineEdit* serialEndMarkerEdit_;
    QSpinBox* serialTimeoutSpin_;

    // 串口连接
    QPushButton* serialRefreshBtn_;
    QPushButton* serialOpenBtn_;
    QPushButton* serialCloseBtn_;
    QPushButton* serialTestBtn_;
    QLabel* serialStatusLabel_;

    // 底部按钮
    QPushButton* applyBtn_;
    QPushButton* okBtn_;
    QPushButton* cancelBtn_;
};

} // namespace UI
} // namespace VisionForge
