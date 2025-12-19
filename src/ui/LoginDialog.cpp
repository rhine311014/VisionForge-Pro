/**
 * @file LoginDialog.cpp
 * @brief 用户登录对话框实现
 * @author VisionForge Team
 * @date 2025-12-18
 */

#include "ui/LoginDialog.h"
#include "base/PermissionManager.h"
#include "base/ConfigManager.h"
#include "base/Logger.h"

#include <QFormLayout>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QApplication>
#include <QStyle>

namespace VisionForge {
namespace UI {

LoginDialog::LoginDialog(QWidget* parent)
    : QDialog(parent)
    , logoLabel_(nullptr)
    , titleLabel_(nullptr)
    , usernameEdit_(nullptr)
    , passwordEdit_(nullptr)
    , rememberCheck_(nullptr)
    , loginBtn_(nullptr)
    , cancelBtn_(nullptr)
    , errorLabel_(nullptr)
    , versionLabel_(nullptr)
{
    setWindowTitle(tr("用户登录"));
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setFixedSize(360, 380);

    createUI();
    connectSignals();
    loadSavedCredentials();
}

LoginDialog::~LoginDialog() = default;

QString LoginDialog::username() const
{
    return usernameEdit_->text().trimmed();
}

QString LoginDialog::password() const
{
    return passwordEdit_->text();
}

bool LoginDialog::rememberPassword() const
{
    return rememberCheck_->isChecked();
}

void LoginDialog::setUsername(const QString& username)
{
    usernameEdit_->setText(username);
}

void LoginDialog::setErrorMessage(const QString& message)
{
    errorLabel_->setText(message);
    errorLabel_->setVisible(true);
}

void LoginDialog::clearError()
{
    errorLabel_->clear();
    errorLabel_->setVisible(false);
}

void LoginDialog::showEvent(QShowEvent* event)
{
    QDialog::showEvent(event);

    // 聚焦到第一个空白输入框
    if (usernameEdit_->text().isEmpty()) {
        usernameEdit_->setFocus();
    } else {
        passwordEdit_->setFocus();
    }

    clearError();
}

void LoginDialog::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        if (loginBtn_->isEnabled()) {
            onLoginClicked();
        }
        return;
    }
    QDialog::keyPressEvent(event);
}

void LoginDialog::createUI()
{
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(40, 30, 40, 30);

    // Logo和标题
    logoLabel_ = new QLabel(this);
    logoLabel_->setPixmap(QApplication::style()->standardPixmap(QStyle::SP_ComputerIcon).scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    logoLabel_->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(logoLabel_);

    titleLabel_ = new QLabel(tr("VisionForge Pro"), this);
    titleLabel_->setAlignment(Qt::AlignCenter);
    titleLabel_->setStyleSheet("font-size: 18px; font-weight: bold; color: #2196F3;");
    mainLayout->addWidget(titleLabel_);

    mainLayout->addSpacing(10);

    // 输入表单
    auto* formLayout = new QFormLayout();
    formLayout->setSpacing(12);

    usernameEdit_ = new QLineEdit(this);
    usernameEdit_->setPlaceholderText(tr("请输入用户名"));
    usernameEdit_->setMinimumHeight(35);
    formLayout->addRow(tr("用户名:"), usernameEdit_);

    passwordEdit_ = new QLineEdit(this);
    passwordEdit_->setPlaceholderText(tr("请输入密码"));
    passwordEdit_->setEchoMode(QLineEdit::Password);
    passwordEdit_->setMinimumHeight(35);
    formLayout->addRow(tr("密码:"), passwordEdit_);

    mainLayout->addLayout(formLayout);

    // 记住密码
    rememberCheck_ = new QCheckBox(tr("记住密码"), this);
    mainLayout->addWidget(rememberCheck_);

    // 错误信息
    errorLabel_ = new QLabel(this);
    errorLabel_->setStyleSheet("color: #f44336; font-size: 12px;");
    errorLabel_->setAlignment(Qt::AlignCenter);
    errorLabel_->setVisible(false);
    mainLayout->addWidget(errorLabel_);

    // 按钮
    auto* buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(15);

    loginBtn_ = new QPushButton(tr("登录"), this);
    loginBtn_->setMinimumHeight(40);
    loginBtn_->setDefault(true);
    loginBtn_->setStyleSheet(
        "QPushButton {"
        "  background-color: #2196F3;"
        "  color: white;"
        "  border: none;"
        "  border-radius: 5px;"
        "  font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #1976D2;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #0D47A1;"
        "}"
        "QPushButton:disabled {"
        "  background-color: #BDBDBD;"
        "}"
    );

    cancelBtn_ = new QPushButton(tr("取消"), this);
    cancelBtn_->setMinimumHeight(40);
    cancelBtn_->setStyleSheet(
        "QPushButton {"
        "  background-color: #EEEEEE;"
        "  color: #424242;"
        "  border: 1px solid #BDBDBD;"
        "  border-radius: 5px;"
        "  font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #E0E0E0;"
        "}"
    );

    buttonLayout->addWidget(loginBtn_);
    buttonLayout->addWidget(cancelBtn_);
    mainLayout->addLayout(buttonLayout);

    mainLayout->addStretch();

    // 版本信息
    versionLabel_ = new QLabel(tr("版本 %1").arg(VISIONFORGE_VERSION), this);
    versionLabel_->setAlignment(Qt::AlignCenter);
    versionLabel_->setStyleSheet("color: #9E9E9E; font-size: 10px;");
    mainLayout->addWidget(versionLabel_);
}

void LoginDialog::connectSignals()
{
    connect(loginBtn_, &QPushButton::clicked, this, &LoginDialog::onLoginClicked);
    connect(cancelBtn_, &QPushButton::clicked, this, &LoginDialog::onCancelClicked);
    connect(usernameEdit_, &QLineEdit::textChanged, this, &LoginDialog::onInputChanged);
    connect(passwordEdit_, &QLineEdit::textChanged, this, &LoginDialog::onInputChanged);
}

void LoginDialog::onLoginClicked()
{
    QString user = username();
    QString pass = password();

    if (user.isEmpty()) {
        setErrorMessage(tr("请输入用户名"));
        usernameEdit_->setFocus();
        return;
    }

    if (pass.isEmpty()) {
        setErrorMessage(tr("请输入密码"));
        passwordEdit_->setFocus();
        return;
    }

    // 尝试登录
    if (Base::PermissionManager::instance().login(user, pass)) {
        // 保存凭据
        if (rememberPassword()) {
            saveCredentials();
        } else {
            // 清除保存的凭据
            auto& config = Base::ConfigManager::instance();
            config.setValue("Login/Username", "");
            config.setValue("Login/Remember", false);
        }
        accept();
    } else {
        setErrorMessage(tr("用户名或密码错误"));
        passwordEdit_->selectAll();
        passwordEdit_->setFocus();
    }
}

void LoginDialog::onCancelClicked()
{
    reject();
}

void LoginDialog::onInputChanged()
{
    clearError();
    loginBtn_->setEnabled(!usernameEdit_->text().isEmpty());
}

void LoginDialog::loadSavedCredentials()
{
    auto& config = Base::ConfigManager::instance();

    bool remember = config.getValue("Login/Remember", false).toBool();
    rememberCheck_->setChecked(remember);

    if (remember) {
        QString savedUser = config.getValue("Login/Username", "").toString();
        if (!savedUser.isEmpty()) {
            usernameEdit_->setText(savedUser);
        }
    }
}

void LoginDialog::saveCredentials()
{
    auto& config = Base::ConfigManager::instance();
    config.setValue("Login/Username", username());
    config.setValue("Login/Remember", true);
}

} // namespace UI
} // namespace VisionForge
