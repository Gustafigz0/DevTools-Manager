#include "UserAuthDialog.h"
#include "UserAuthManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include "moc_UserAuthDialog.cpp"

UserAuthDialog::UserAuthDialog(Mode mode, const QString& currentUsername, QWidget* parent) 
    : QDialog(parent), mode_(mode), currentUsername_(currentUsername) {
    setupUi();
    setMode(mode_);
}

void UserAuthDialog::setupUi() {
    setModal(true);
    setMinimumWidth(350);
    auto *layout = new QVBoxLayout(this);

    userEdit_ = new QLineEdit;
    userEdit_->setPlaceholderText("Username");
    passEdit_ = new QLineEdit;
    passEdit_->setPlaceholderText("Password");
    passEdit_->setEchoMode(QLineEdit::Password);
    oldPassEdit_ = new QLineEdit;
    // oldPassEdit_->setPlaceholderText("Old Password");
    oldPassEdit_->setEchoMode(QLineEdit::Password);
    newPassEdit_ = new QLineEdit;
    // newPassEdit_->setPlaceholderText("New Password");
    newPassEdit_->setEchoMode(QLineEdit::Password);

    acceptBtn_ = new QPushButton("OK");
    cancelBtn_ = new QPushButton("Cancel");
    switchBtn_ = new QPushButton;
    connect(acceptBtn_, &QPushButton::clicked, this, &UserAuthDialog::onAcceptClicked);
    connect(cancelBtn_, &QPushButton::clicked, this, &UserAuthDialog::reject);
    connect(switchBtn_, &QPushButton::clicked, this, &UserAuthDialog::onSwitchModeClicked);

    layout->addWidget(new QLabel("Username:"));
    layout->addWidget(userEdit_);
    layout->addWidget(new QLabel("Password:"));
    layout->addWidget(passEdit_);
    // layout->addWidget(new QLabel("Old Password:"));
    layout->addWidget(oldPassEdit_);
    // layout->addWidget(new QLabel("New Password:"));
    layout->addWidget(newPassEdit_);
    auto bRow = new QHBoxLayout;
    bRow->addWidget(acceptBtn_);
    bRow->addWidget(cancelBtn_);
    bRow->addWidget(switchBtn_);
    layout->addLayout(bRow);
}

void UserAuthDialog::setMode(Mode mode) {
    mode_ = mode;
    bool isLogin = (mode == Login);
    bool isRegister = (mode == Register);
    bool isChange = (mode == ChangePassword);

    userEdit_->setVisible(isLogin || isRegister);
    passEdit_->setVisible(isLogin || isRegister);
    oldPassEdit_->setVisible(isChange);
    newPassEdit_->setVisible(isChange);
    switchBtn_->setText(isLogin ? "Create account" : (isRegister ? "Login" : "Login"));

    setWindowTitle(isLogin ? "Login" : (isRegister ? "Register" : "Change Password"));
    
    // In change password mode, show current username as read-only
    if (isChange && !currentUsername_.isEmpty()) {
        userEdit_->setText(currentUsername_);
        userEdit_->setReadOnly(true);
        userEdit_->setVisible(true);
    } else {
        userEdit_->setReadOnly(false);
    }
}

void UserAuthDialog::setCurrentUsername(const QString& username) {
    currentUsername_ = username;
}

QString UserAuthDialog::username() const { return userEdit_->text().trimmed(); }
QString UserAuthDialog::password() const { return passEdit_->text(); }
QString UserAuthDialog::oldPassword() const { return oldPassEdit_->text(); }
QString UserAuthDialog::newPassword() const { return newPassEdit_->text(); }

void UserAuthDialog::onAcceptClicked() {
    QString errorMsg;
    UserAuthManager& authMgr = UserAuthManager::instance();
    
    if (mode_ == Login) {
        if (username().isEmpty() || password().isEmpty()) {
            QMessageBox::warning(this, "Invalid", "Username and password required!");
            return;
        }
        
        if (!authMgr.login(username(), password(), errorMsg)) {
            QMessageBox::warning(this, "Login Failed", errorMsg);
            return;
        }
        
        emit authSuccess(username());
        accept();
    }
    else if (mode_ == Register) {
        if (username().isEmpty() || password().isEmpty()) {
            QMessageBox::warning(this, "Invalid", "Username and password required!");
            return;
        }
        
        if (password().length() < 4) {
            QMessageBox::warning(this, "Invalid", "Password must be at least 4 characters long!");
            return;
        }
        
        if (!authMgr.registerUser(username(), password(), errorMsg)) {
            QMessageBox::warning(this, "Registration Failed", errorMsg);
            return;
        }
        
        QMessageBox::information(this, "Success", "Account created successfully! You can now login.");
        setMode(Login);
        userEdit_->setText(username());
        passEdit_->clear();
    }
    else if (mode_ == ChangePassword) {
        if (oldPassword().isEmpty() || newPassword().isEmpty()) {
            QMessageBox::warning(this, "Invalid", "Old and new passwords required!");
            return;
        }
        
        if (newPassword().length() < 4) {
            QMessageBox::warning(this, "Invalid", "New password must be at least 4 characters long!");
            return;
        }
        
        QString currentUser = currentUsername_.isEmpty() ? username() : currentUsername_;
        if (currentUser.isEmpty()) {
            QMessageBox::warning(this, "Error", "No username available for password change!");
            return;
        }
        if (!authMgr.changePassword(currentUser, oldPassword(), newPassword(), errorMsg)) {
            QMessageBox::warning(this, "Password Change Failed", errorMsg);
            return;
        }
        
        QMessageBox::information(this, "Success", "Password changed successfully!");
        emit authSuccess(currentUser);
        accept();
    }
}

void UserAuthDialog::onSwitchModeClicked() {
    if (mode_ == Login) setMode(Register);
    else setMode(Login);
}
