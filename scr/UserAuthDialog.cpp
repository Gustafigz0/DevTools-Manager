#include "UserAuthDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include "moc_UserAuthDialog.cpp"

UserAuthDialog::UserAuthDialog(Mode mode, QWidget* parent) : QDialog(parent), mode_(mode) {
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
    oldPassEdit_->setPlaceholderText("Old Password");
    oldPassEdit_->setEchoMode(QLineEdit::Password);
    newPassEdit_ = new QLineEdit;
    newPassEdit_->setPlaceholderText("New Password");
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
    layout->addWidget(new QLabel("Old Password:"));
    layout->addWidget(oldPassEdit_);
    layout->addWidget(new QLabel("New Password:"));
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
}

QString UserAuthDialog::username() const { return userEdit_->text().trimmed(); }
QString UserAuthDialog::password() const { return passEdit_->text(); }
QString UserAuthDialog::oldPassword() const { return oldPassEdit_->text(); }
QString UserAuthDialog::newPassword() const { return newPassEdit_->text(); }

void UserAuthDialog::onAcceptClicked() {
    // Just closes for now. Actual validation logic will go in main UI.
    if ((mode_ == Login || mode_ == Register) && (username().isEmpty() || password().isEmpty())) {
        QMessageBox::warning(this, "Invalid", "Username and password required!");
        return;
    }
    if (mode_ == ChangePassword && (oldPassword().isEmpty() || newPassword().isEmpty())) {
        QMessageBox::warning(this, "Invalid", "Old and new passwords required!");
        return;
    }
    emit authSuccess(username());
    accept();
}

void UserAuthDialog::onSwitchModeClicked() {
    if (mode_ == Login) setMode(Register);
    else setMode(Login);
}
