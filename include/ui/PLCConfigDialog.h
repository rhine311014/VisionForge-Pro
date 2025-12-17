/**
 * @file PLCConfigDialog.h
 * @brief PLC配置对话框
 * @author VisionForge Team
 * @date 2025-12-17
 */

#pragma once

#include "comm/PLCDef.h"
#include <QDialog>
#include <QListWidget>
#include <QStackedWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QTableWidget>
#include <QTextEdit>

namespace VisionForge {

/**
 * @class PLCConfigDialog
 * @brief PLC配置对话框
 *
 * 提供PLC连接的配置和测试功能
 */
class PLCConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PLCConfigDialog(QWidget* parent = nullptr);
    ~PLCConfigDialog();

private slots:
    // 连接管理
    void onAddConnection();
    void onRemoveConnection();
    void onConnectionSelected(int row);

    // 配置操作
    void onProtocolChanged(int index);
    void onApplyConfig();
    void onTestConnection();

    // 读写测试
    void onReadRegister();
    void onWriteRegister();
    void onReadCoil();
    void onWriteCoil();

    // 保存/加载
    void onSaveConfig();
    void onLoadConfig();

private:
    void setupUI();
    void setupConnections();

    // 创建UI组件
    QWidget* createConnectionListPanel();
    QWidget* createConfigPanel();
    QWidget* createTestPanel();

    // 更新UI
    void updateConnectionList();
    void loadConnectionConfig(const QString& name);
    void clearConfigFields();
    void appendLog(const QString& message);

    // 获取当前配置
    Comm::PLCConfig getCurrentConfig() const;

private:
    // 连接列表
    QListWidget* connectionList_;
    QPushButton* addBtn_;
    QPushButton* removeBtn_;

    // 基本配置
    QLineEdit* nameEdit_;
    QComboBox* protocolCombo_;

    // 网络配置
    QGroupBox* networkGroup_;
    QLineEdit* ipEdit_;
    QSpinBox* portSpin_;

    // 串口配置
    QGroupBox* serialGroup_;
    QComboBox* portCombo_;
    QComboBox* baudRateCombo_;
    QComboBox* dataBitsCombo_;
    QComboBox* stopBitsCombo_;
    QComboBox* parityCombo_;

    // Modbus配置
    QGroupBox* modbusGroup_;
    QSpinBox* slaveIdSpin_;

    // 通用配置
    QSpinBox* timeoutSpin_;
    QSpinBox* retryCountSpin_;

    // 配置按钮
    QPushButton* applyBtn_;
    QPushButton* testBtn_;

    // 读写测试
    QSpinBox* testAddressSpin_;
    QSpinBox* testCountSpin_;
    QLineEdit* testValueEdit_;
    QPushButton* readRegBtn_;
    QPushButton* writeRegBtn_;
    QPushButton* readCoilBtn_;
    QPushButton* writeCoilBtn_;

    // 结果显示
    QTableWidget* resultTable_;
    QTextEdit* logText_;

    // 保存/加载按钮
    QPushButton* saveConfigBtn_;
    QPushButton* loadConfigBtn_;

    // 状态
    QString currentConnection_;
    QLabel* statusLabel_;
};

} // namespace VisionForge
