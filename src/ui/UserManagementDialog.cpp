/**
 * @file UserManagementDialog.cpp
 * @brief 用户管理对话框实现
 */

#include "ui/UserManagementDialog.h"
#include "base/Logger.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QCheckBox>
#include <QFormLayout>

namespace VisionForge {
namespace UI {

// ==================== UserManagementDialog ====================

UserManagementDialog::UserManagementDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle(tr("用户管理"));
    setMinimumSize(600, 400);
    resize(700, 500);

    setupUI();
    refreshUserList();
    updateButtonStates();
}

void UserManagementDialog::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // 用户列表
    userTable_ = new QTableWidget(this);
    userTable_->setColumnCount(5);
    userTable_->setHorizontalHeaderLabels({
        tr("用户名"), tr("显示名称"), tr("角色"), tr("状态"), tr("最后登录")
    });
    userTable_->setSelectionBehavior(QAbstractItemView::SelectRows);
    userTable_->setSelectionMode(QAbstractItemView::SingleSelection);
    userTable_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    userTable_->horizontalHeader()->setStretchLastSection(true);
    userTable_->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    userTable_->verticalHeader()->setVisible(false);
    userTable_->setAlternatingRowColors(true);

    connect(userTable_, &QTableWidget::itemSelectionChanged,
            this, &UserManagementDialog::onUserSelectionChanged);
    connect(userTable_, &QTableWidget::cellDoubleClicked,
            this, &UserManagementDialog::onTableDoubleClicked);

    mainLayout->addWidget(userTable_);

    // 按钮区域
    QHBoxLayout* buttonLayout = new QHBoxLayout();

    addButton_ = new QPushButton(tr("添加用户(&A)"), this);
    editButton_ = new QPushButton(tr("编辑(&E)"), this);
    deleteButton_ = new QPushButton(tr("删除(&D)"), this);
    resetPasswordButton_ = new QPushButton(tr("重置密码(&R)"), this);
    closeButton_ = new QPushButton(tr("关闭(&C)"), this);

    connect(addButton_, &QPushButton::clicked, this, &UserManagementDialog::onAddUser);
    connect(editButton_, &QPushButton::clicked, this, &UserManagementDialog::onEditUser);
    connect(deleteButton_, &QPushButton::clicked, this, &UserManagementDialog::onDeleteUser);
    connect(resetPasswordButton_, &QPushButton::clicked, this, &UserManagementDialog::onResetPassword);
    connect(closeButton_, &QPushButton::clicked, this, &QDialog::accept);

    buttonLayout->addWidget(addButton_);
    buttonLayout->addWidget(editButton_);
    buttonLayout->addWidget(deleteButton_);
    buttonLayout->addWidget(resetPasswordButton_);
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeButton_);

    mainLayout->addLayout(buttonLayout);
}

void UserManagementDialog::refreshUserList()
{
    userTable_->setRowCount(0);

    QList<Base::UserInfo> users = Base::PermissionManager::instance().getAllUsers();

    for (const Base::UserInfo& user : users) {
        int row = userTable_->rowCount();
        userTable_->insertRow(row);

        userTable_->setItem(row, 0, new QTableWidgetItem(user.username));
        userTable_->setItem(row, 1, new QTableWidgetItem(user.displayName));
        userTable_->setItem(row, 2, new QTableWidgetItem(getRoleDisplayName(user.role)));
        userTable_->setItem(row, 3, new QTableWidgetItem(user.enabled ? tr("启用") : tr("禁用")));
        userTable_->setItem(row, 4, new QTableWidgetItem(user.lastLogin.isEmpty() ? tr("从未") : user.lastLogin));

        // 状态列颜色
        if (!user.enabled) {
            userTable_->item(row, 3)->setForeground(Qt::red);
        }
    }

    updateButtonStates();
}

void UserManagementDialog::updateButtonStates()
{
    bool hasSelection = !selectedUsername_.isEmpty();

    // 不能编辑或删除当前登录用户
    const Base::UserInfo* currentUser = Base::PermissionManager::instance().currentUser();
    bool isCurrentUser = currentUser && currentUser->username == selectedUsername_;

    // 不能删除admin用户
    bool isAdmin = (selectedUsername_ == "admin");

    editButton_->setEnabled(hasSelection);
    deleteButton_->setEnabled(hasSelection && !isCurrentUser && !isAdmin);
    resetPasswordButton_->setEnabled(hasSelection);
}

void UserManagementDialog::onUserSelectionChanged()
{
    QList<QTableWidgetItem*> items = userTable_->selectedItems();
    if (!items.isEmpty()) {
        int row = items.first()->row();
        QTableWidgetItem* item = userTable_->item(row, 0);
        if (item) {
            selectedUsername_ = item->text();
        }
    } else {
        selectedUsername_.clear();
    }
    updateButtonStates();
}

void UserManagementDialog::onTableDoubleClicked(int row, int column)
{
    Q_UNUSED(column);
    QTableWidgetItem* item = userTable_->item(row, 0);
    if (item) {
        selectedUsername_ = item->text();
        onEditUser();
    }
}

void UserManagementDialog::onAddUser()
{
    UserEditDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        Base::UserInfo user = dialog.getUserInfo();
        QString password = dialog.getPassword();

        if (Base::PermissionManager::instance().addUser(user, password)) {
            LOG_INFO(QString("添加用户: %1").arg(user.username));
            refreshUserList();
        } else {
            QMessageBox::warning(this, tr("添加失败"),
                tr("添加用户失败，用户名可能已存在。"));
        }
    }
}

void UserManagementDialog::onEditUser()
{
    if (selectedUsername_.isEmpty()) return;

    Base::UserInfo* user = Base::PermissionManager::instance().getUser(selectedUsername_);
    if (!user) return;

    UserEditDialog dialog(*user, this);
    if (dialog.exec() == QDialog::Accepted) {
        Base::UserInfo updatedUser = dialog.getUserInfo();

        // 更新用户信息（保留密码哈希）
        user->displayName = updatedUser.displayName;
        user->role = updatedUser.role;
        user->enabled = updatedUser.enabled;

        Base::PermissionManager::instance().saveConfig();
        LOG_INFO(QString("编辑用户: %1").arg(user->username));
        refreshUserList();
    }
}

void UserManagementDialog::onDeleteUser()
{
    if (selectedUsername_.isEmpty()) return;

    int ret = QMessageBox::question(this, tr("确认删除"),
        tr("确定要删除用户 \"%1\" 吗？\n此操作不可恢复。").arg(selectedUsername_),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        if (Base::PermissionManager::instance().removeUser(selectedUsername_)) {
            LOG_INFO(QString("删除用户: %1").arg(selectedUsername_));
            selectedUsername_.clear();
            refreshUserList();
        } else {
            QMessageBox::warning(this, tr("删除失败"),
                tr("删除用户失败。"));
        }
    }
}

void UserManagementDialog::onResetPassword()
{
    if (selectedUsername_.isEmpty()) return;

    PasswordResetDialog dialog(selectedUsername_, this);
    if (dialog.exec() == QDialog::Accepted) {
        QString newPassword = dialog.getNewPassword();

        // 使用changePassword方法（管理员可以跳过旧密码验证）
        // 这里简化处理：直接修改用户密码
        Base::UserInfo* user = Base::PermissionManager::instance().getUser(selectedUsername_);
        if (user) {
            // 重新计算密码哈希
            QByteArray hash = QCryptographicHash::hash(
                newPassword.toUtf8(), QCryptographicHash::Sha256);
            user->passwordHash = QString(hash.toHex());

            Base::PermissionManager::instance().saveConfig();
            LOG_INFO(QString("重置用户密码: %1").arg(selectedUsername_));

            QMessageBox::information(this, tr("密码重置"),
                tr("用户 \"%1\" 的密码已重置。").arg(selectedUsername_));
        }
    }
}

QString UserManagementDialog::getRoleDisplayName(Base::UserRole role) const
{
    switch (role) {
        case Base::UserRole::Guest: return tr("访客");
        case Base::UserRole::Operator: return tr("操作员");
        case Base::UserRole::Engineer: return tr("工程师");
        case Base::UserRole::Administrator: return tr("管理员");
        default: return tr("未知");
    }
}

Base::UserRole UserManagementDialog::getRoleFromIndex(int index) const
{
    switch (index) {
        case 0: return Base::UserRole::Guest;
        case 1: return Base::UserRole::Operator;
        case 2: return Base::UserRole::Engineer;
        case 3: return Base::UserRole::Administrator;
        default: return Base::UserRole::Guest;
    }
}

int UserManagementDialog::getRoleIndex(Base::UserRole role) const
{
    switch (role) {
        case Base::UserRole::Guest: return 0;
        case Base::UserRole::Operator: return 1;
        case Base::UserRole::Engineer: return 2;
        case Base::UserRole::Administrator: return 3;
        default: return 0;
    }
}

// ==================== UserEditDialog ====================

UserEditDialog::UserEditDialog(QWidget* parent)
    : QDialog(parent)
    , isNewUser_(true)
{
    setWindowTitle(tr("添加用户"));
    setupUI();
}

UserEditDialog::UserEditDialog(const Base::UserInfo& user, QWidget* parent)
    : QDialog(parent)
    , isNewUser_(false)
{
    setWindowTitle(tr("编辑用户"));
    setupUI();

    // 填充现有数据
    usernameEdit_->setText(user.username);
    usernameEdit_->setReadOnly(true);  // 不允许修改用户名
    displayNameEdit_->setText(user.displayName);

    // 设置角色
    int roleIndex = 0;
    switch (user.role) {
        case Base::UserRole::Guest: roleIndex = 0; break;
        case Base::UserRole::Operator: roleIndex = 1; break;
        case Base::UserRole::Engineer: roleIndex = 2; break;
        case Base::UserRole::Administrator: roleIndex = 3; break;
    }
    roleCombo_->setCurrentIndex(roleIndex);

    enabledCheck_->setChecked(user.enabled);

    // 编辑模式下隐藏密码字段
    passwordLabel_->hide();
    passwordEdit_->hide();
    confirmLabel_->hide();
    confirmPasswordEdit_->hide();
}

void UserEditDialog::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QFormLayout* formLayout = new QFormLayout();

    usernameEdit_ = new QLineEdit(this);
    usernameEdit_->setPlaceholderText(tr("输入用户名（字母数字下划线）"));
    formLayout->addRow(tr("用户名:"), usernameEdit_);

    displayNameEdit_ = new QLineEdit(this);
    displayNameEdit_->setPlaceholderText(tr("输入显示名称"));
    formLayout->addRow(tr("显示名称:"), displayNameEdit_);

    passwordLabel_ = new QLabel(tr("密码:"), this);
    passwordEdit_ = new QLineEdit(this);
    passwordEdit_->setEchoMode(QLineEdit::Password);
    passwordEdit_->setPlaceholderText(tr("输入密码（至少6位）"));
    formLayout->addRow(passwordLabel_, passwordEdit_);

    confirmLabel_ = new QLabel(tr("确认密码:"), this);
    confirmPasswordEdit_ = new QLineEdit(this);
    confirmPasswordEdit_->setEchoMode(QLineEdit::Password);
    confirmPasswordEdit_->setPlaceholderText(tr("再次输入密码"));
    formLayout->addRow(confirmLabel_, confirmPasswordEdit_);

    roleCombo_ = new QComboBox(this);
    roleCombo_->addItem(tr("访客"));
    roleCombo_->addItem(tr("操作员"));
    roleCombo_->addItem(tr("工程师"));
    roleCombo_->addItem(tr("管理员"));
    roleCombo_->setCurrentIndex(1);  // 默认操作员
    formLayout->addRow(tr("角色:"), roleCombo_);

    enabledCheck_ = new QCheckBox(tr("启用"), this);
    enabledCheck_->setChecked(true);
    formLayout->addRow(tr("状态:"), enabledCheck_);

    mainLayout->addLayout(formLayout);

    // 按钮
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    okButton_ = new QPushButton(tr("确定"), this);
    cancelButton_ = new QPushButton(tr("取消"), this);

    connect(okButton_, &QPushButton::clicked, this, &UserEditDialog::onAccept);
    connect(cancelButton_, &QPushButton::clicked, this, &QDialog::reject);

    // 输入验证
    connect(usernameEdit_, &QLineEdit::textChanged, this, &UserEditDialog::validateInput);
    connect(passwordEdit_, &QLineEdit::textChanged, this, &UserEditDialog::validateInput);
    connect(confirmPasswordEdit_, &QLineEdit::textChanged, this, &UserEditDialog::validateInput);

    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton_);
    buttonLayout->addWidget(cancelButton_);

    mainLayout->addLayout(buttonLayout);

    validateInput();
}

void UserEditDialog::validateInput()
{
    bool valid = true;

    // 用户名验证
    QString username = usernameEdit_->text().trimmed();
    if (username.isEmpty() || username.length() < 3) {
        valid = false;
    }

    // 新用户需要密码
    if (isNewUser_) {
        QString password = passwordEdit_->text();
        QString confirm = confirmPasswordEdit_->text();

        if (password.length() < 6) {
            valid = false;
        }
        if (password != confirm) {
            valid = false;
        }
    }

    okButton_->setEnabled(valid);
}

void UserEditDialog::onAccept()
{
    // 最终验证
    QString username = usernameEdit_->text().trimmed();

    if (isNewUser_) {
        // 检查用户名是否已存在
        if (Base::PermissionManager::instance().getUser(username)) {
            QMessageBox::warning(this, tr("用户名已存在"),
                tr("用户名 \"%1\" 已被使用，请选择其他用户名。").arg(username));
            return;
        }

        // 验证密码
        if (passwordEdit_->text() != confirmPasswordEdit_->text()) {
            QMessageBox::warning(this, tr("密码不匹配"),
                tr("两次输入的密码不一致。"));
            return;
        }
    }

    accept();
}

Base::UserInfo UserEditDialog::getUserInfo() const
{
    Base::UserInfo user;
    user.username = usernameEdit_->text().trimmed();
    user.displayName = displayNameEdit_->text().trimmed();
    if (user.displayName.isEmpty()) {
        user.displayName = user.username;
    }

    switch (roleCombo_->currentIndex()) {
        case 0: user.role = Base::UserRole::Guest; break;
        case 1: user.role = Base::UserRole::Operator; break;
        case 2: user.role = Base::UserRole::Engineer; break;
        case 3: user.role = Base::UserRole::Administrator; break;
        default: user.role = Base::UserRole::Operator; break;
    }

    user.enabled = enabledCheck_->isChecked();
    return user;
}

QString UserEditDialog::getPassword() const
{
    return passwordEdit_->text();
}

// ==================== PasswordResetDialog ====================

PasswordResetDialog::PasswordResetDialog(const QString& username, QWidget* parent)
    : QDialog(parent)
    , username_(username)
{
    setWindowTitle(tr("重置密码"));
    setupUI();
}

void PasswordResetDialog::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QLabel* infoLabel = new QLabel(
        tr("为用户 \"%1\" 设置新密码:").arg(username_), this);
    mainLayout->addWidget(infoLabel);

    QFormLayout* formLayout = new QFormLayout();

    newPasswordEdit_ = new QLineEdit(this);
    newPasswordEdit_->setEchoMode(QLineEdit::Password);
    newPasswordEdit_->setPlaceholderText(tr("输入新密码（至少6位）"));
    formLayout->addRow(tr("新密码:"), newPasswordEdit_);

    confirmPasswordEdit_ = new QLineEdit(this);
    confirmPasswordEdit_->setEchoMode(QLineEdit::Password);
    confirmPasswordEdit_->setPlaceholderText(tr("再次输入新密码"));
    formLayout->addRow(tr("确认密码:"), confirmPasswordEdit_);

    mainLayout->addLayout(formLayout);

    // 按钮
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    okButton_ = new QPushButton(tr("确定"), this);
    cancelButton_ = new QPushButton(tr("取消"), this);

    connect(okButton_, &QPushButton::clicked, this, &PasswordResetDialog::onAccept);
    connect(cancelButton_, &QPushButton::clicked, this, &QDialog::reject);
    connect(newPasswordEdit_, &QLineEdit::textChanged, this, &PasswordResetDialog::validateInput);
    connect(confirmPasswordEdit_, &QLineEdit::textChanged, this, &PasswordResetDialog::validateInput);

    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton_);
    buttonLayout->addWidget(cancelButton_);

    mainLayout->addLayout(buttonLayout);

    okButton_->setEnabled(false);
}

void PasswordResetDialog::validateInput()
{
    QString password = newPasswordEdit_->text();
    QString confirm = confirmPasswordEdit_->text();

    bool valid = (password.length() >= 6) && (password == confirm);
    okButton_->setEnabled(valid);
}

void PasswordResetDialog::onAccept()
{
    if (newPasswordEdit_->text() != confirmPasswordEdit_->text()) {
        QMessageBox::warning(this, tr("密码不匹配"),
            tr("两次输入的密码不一致。"));
        return;
    }
    accept();
}

QString PasswordResetDialog::getNewPassword() const
{
    return newPasswordEdit_->text();
}

} // namespace UI
} // namespace VisionForge
