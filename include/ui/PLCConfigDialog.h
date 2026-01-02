/**
 * @file PLCConfigDialog.h
 * @brief PLC通信配置对话框
 * @author VisionForge Team
 * @date 2025-12-17
 */

#pragma once

#include "comm/PLCDef.h"
#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QTableWidget>
#include <QTextEdit>
#include <QRadioButton>
#include <QButtonGroup>

namespace VisionForge {

/**
 * @class PLCConfigDialog
 * @brief PLC通信配置对话框
 *
 * 仿VisionASM风格的通信配置界面
 */
class PLCConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PLCConfigDialog(QWidget* parent = nullptr);
    ~PLCConfigDialog();

private slots:
    // 网络操作
    void onOpenConnection();
    void onMoreSettings();

    // 数据操作
    void onSendData();
    void onClearSendData();
    void onClearRecvData();

    // 读写测试
    void onReadData();
    void onWriteData();

    // 对话框按钮
    void onConfirm();
    void onCancel();

private:
    void setupUI();
    void setupConnections();
    void setupStyles();

    // 创建UI区域
    QWidget* createLeftPanel();      // 左侧：通信参数、指令、数据、设置
    QWidget* createMiddlePanel();    // 中间：网络设置、读写测试
    QWidget* createRightPanel();     // 右侧：发送/接收数据、按钮

    // 更新状态
    void updateConnectionStatus(bool connected, const QString& message = QString());
    void appendLog(const QString& message);
    void updateDataTable();

private:
    // ===== 左侧：通信参数 =====
    QComboBox* commTypeCombo_;       // 通信类型
    QComboBox* commProtocolCombo_;   // 通信协议
    QComboBox* commDataCombo_;       // 通信数据
    QComboBox* commStorageCombo_;    // 通信存储

    // ===== 左侧：通信指令 =====
    QLineEdit* plcCmdAddrEdit_;      // PLC指令基地址
    QSpinBox* plcCmdLenSpin_;        // PLC指令长度
    QLineEdit* visionCmdAddrEdit_;   // 视觉指令基地址

    // ===== 左侧：通信数据 =====
    QLineEdit* plcDataAddrEdit_;     // PLC数据基地址
    QSpinBox* plcDataLenSpin_;       // PLC数据长度
    QLineEdit* visionDataAddrEdit_;  // 视觉数据基地址

    // ===== 左侧：通信设置 =====
    QSpinBox* recvDelaySpin_;        // 接收延时
    QSpinBox* sendDelaySpin_;        // 发送延时
    QSpinBox* commTimeoutSpin_;      // 通信超时
    QSpinBox* retryCountSpin_;       // 重发次数
    QSpinBox* xUnitPowerSpin_;       // X单位幂
    QSpinBox* yUnitPowerSpin_;       // Y单位幂
    QSpinBox* thetaUnitPowerSpin_;   // θ单位幂
    QComboBox* dataLenTypeCombo_;    // 数据长度类型

    // ===== 中间：网络设置 =====
    QRadioButton* serverRadio_;      // 服务端
    QRadioButton* clientRadio_;      // 客户端
    QButtonGroup* modeGroup_;        // 模式选择组
    QLineEdit* ipEdit1_;             // IP地址段1
    QLineEdit* ipEdit2_;             // IP地址段2
    QLineEdit* ipEdit3_;             // IP地址段3
    QLineEdit* ipEdit4_;             // IP地址段4
    QSpinBox* portSpin_;             // 端口
    QComboBox* cmdTermCombo_;        // 命令终止符
    QComboBox* commTermCombo_;       // 通讯终止符
    QPushButton* moreSettingsBtn_;   // 更多设置
    QPushButton* openBtn_;           // 打开按钮

    // ===== 中间：读写测试 =====
    QComboBox* testDataTypeCombo_;   // 通信数据类型
    QLineEdit* testAddrEdit_;        // 数据地址
    QPushButton* readBtn_;           // 读按钮
    QPushButton* writeBtn_;          // 写按钮
    QTableWidget* dataTable_;        // 数据表格

    // ===== 右侧：发送/接收数据 =====
    QTextEdit* sendDataEdit_;        // 发送数据
    QPushButton* sendBtn_;           // 发送按钮
    QPushButton* clearSendBtn_;      // 清空发送
    QTextEdit* recvDataEdit_;        // 接收数据
    QPushButton* clearRecvBtn_;      // 清空接收

    // ===== 右上角图标 =====
    QPushButton* iconBtn_;           // 边检图标按钮

    // ===== 底部 =====
    QLabel* statusLabel_;            // 状态标签
    QPushButton* confirmBtn_;        // 确定按钮
    QPushButton* cancelBtn_;         // 取消按钮

    // 样式
    QString groupBoxStyle_;
    QString buttonStyle_;
    QString largeButtonStyle_;
};

} // namespace VisionForge
