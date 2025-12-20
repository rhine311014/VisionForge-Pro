/**
 * @file UserManagementDialog.h
 * @brief 用户管理对话框
 * @author VisionForge Team
 * @date 2025-12-19
 */

#pragma once

#include "base/PermissionManager.h"
#include <QDialog>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QGroupBox>
#include <QCheckBox>

namespace VisionForge {
namespace UI {

/**
 * @class UserManagementDialog
 * @brief 用户管理对话框
 *
 * 功能：
 * - 查看所有用户列表
 * - 添加/删除/编辑用户
 * - 修改用户角色
 * - 重置用户密码
 */
class UserManagementDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UserManagementDialog(QWidget* parent = nullptr);
    ~UserManagementDialog() override = default;

private slots:
    void onAddUser();
    void onEditUser();
    void onDeleteUser();
    void onResetPassword();
    void onUserSelectionChanged();
    void onTableDoubleClicked(int row, int column);

private:
    void setupUI();
    void refreshUserList();
    void updateButtonStates();

    // 辅助方法
    QString getRoleDisplayName(Base::UserRole role) const;
    Base::UserRole getRoleFromIndex(int index) const;
    int getRoleIndex(Base::UserRole role) const;

private:
    // 用户列表
    QTableWidget* userTable_;

    // 按钮
    QPushButton* addButton_;
    QPushButton* editButton_;
    QPushButton* deleteButton_;
    QPushButton* resetPasswordButton_;
    QPushButton* closeButton_;

    // 当前选中的用户
    QString selectedUsername_;
};

/**
 * @class UserEditDialog
 * @brief 用户编辑对话框（添加/编辑用户）
 */
class UserEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UserEditDialog(QWidget* parent = nullptr);
    explicit UserEditDialog(const Base::UserInfo& user, QWidget* parent = nullptr);
    ~UserEditDialog() override = default;

    Base::UserInfo getUserInfo() const;
    QString getPassword() const;
    bool isNewUser() const { return isNewUser_; }

private slots:
    void onAccept();
    void validateInput();

private:
    void setupUI();

private:
    QLineEdit* usernameEdit_;
    QLineEdit* displayNameEdit_;
    QLineEdit* passwordEdit_;
    QLineEdit* confirmPasswordEdit_;
    QComboBox* roleCombo_;
    QCheckBox* enabledCheck_;

    QPushButton* okButton_;
    QPushButton* cancelButton_;

    QLabel* passwordLabel_;
    QLabel* confirmLabel_;

    bool isNewUser_;
};

/**
 * @class PasswordResetDialog
 * @brief 密码重置对话框
 */
class PasswordResetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PasswordResetDialog(const QString& username, QWidget* parent = nullptr);
    ~PasswordResetDialog() override = default;

    QString getNewPassword() const;

private slots:
    void onAccept();
    void validateInput();

private:
    void setupUI();

private:
    QString username_;
    QLineEdit* newPasswordEdit_;
    QLineEdit* confirmPasswordEdit_;
    QPushButton* okButton_;
    QPushButton* cancelButton_;
};

} // namespace UI
} // namespace VisionForge
