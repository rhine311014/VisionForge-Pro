/**
 * @file LoginDialog.h
 * @brief 用户登录对话框
 * @author VisionForge Team
 * @date 2025-12-18
 */

#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QVBoxLayout>

namespace VisionForge {
namespace UI {

/**
 * @class LoginDialog
 * @brief 用户登录对话框
 *
 * 功能：
 * - 用户名/密码输入
 * - 记住密码选项
 * - 登录验证
 */
class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget* parent = nullptr);
    ~LoginDialog() override;

    /**
     * @brief 获取用户名
     */
    QString username() const;

    /**
     * @brief 获取密码
     */
    QString password() const;

    /**
     * @brief 是否记住密码
     */
    bool rememberPassword() const;

    /**
     * @brief 设置用户名
     */
    void setUsername(const QString& username);

    /**
     * @brief 设置错误信息
     */
    void setErrorMessage(const QString& message);

    /**
     * @brief 清除错误信息
     */
    void clearError();

protected:
    void showEvent(QShowEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private slots:
    void onLoginClicked();
    void onCancelClicked();
    void onInputChanged();

private:
    void createUI();
    void connectSignals();
    void loadSavedCredentials();
    void saveCredentials();

private:
    // UI组件
    QLabel* logoLabel_;
    QLabel* titleLabel_;
    QLineEdit* usernameEdit_;
    QLineEdit* passwordEdit_;
    QCheckBox* rememberCheck_;
    QPushButton* loginBtn_;
    QPushButton* cancelBtn_;
    QLabel* errorLabel_;
    QLabel* versionLabel_;
};

} // namespace UI
} // namespace VisionForge
