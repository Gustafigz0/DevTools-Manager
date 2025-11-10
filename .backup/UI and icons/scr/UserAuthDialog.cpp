// Updated styling to match MainWindow (GitHub-like dark theme) while preserving logic.
#include "UserAuthDialog.h"
#include "UserAuthManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QMouseEvent>
#include <QGraphicsDropShadowEffect>
#include <QApplication>
#include <QTimer>
#include <QDebug>
#include "moc_UserAuthDialog.cpp"

UserAuthDialog::UserAuthDialog(Mode mode, const QString& currentUsername, QWidget* parent) 
    : QDialog(parent), mode_(mode), currentUsername_(currentUsername) {
    // Custom frameless dialog to match main window aesthetics
    setWindowFlag(Qt::FramelessWindowHint, true);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setupUi();
    setMode(mode_);
}

void UserAuthDialog::setupUi() {
    setModal(true);
    setMinimumWidth(420);

    // Root layout with transparent background (rounded container inside)
    auto *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(12,12,12,12);
    rootLayout->setSpacing(0);

    // Card container replicating MainWindow windowCard style
    card_ = new QWidget(this);
    card_->setObjectName("AuthCard");
    card_->setStyleSheet(R"(
        #AuthCard {
            background: #0d1117;
            border: 1px solid #30363d;
            border-radius: 12px;
        }
    )");
    auto *cardVBox = new QVBoxLayout(card_);
    cardVBox->setContentsMargins(0,0,0,0);
    cardVBox->setSpacing(0);

    // Custom title bar (drag area)
    titleBar_ = new QWidget(card_);
    titleBar_->setObjectName("AuthTitleBar");
    titleBar_->setFixedHeight(46);
    titleBar_->setStyleSheet(R"(
        #AuthTitleBar {
            background: #161b22;
            border-bottom: 1px solid #21262d;
            border-top-left-radius: 12px;
            border-top-right-radius: 12px;
        }
        #AuthTitleBar QLabel#TitleLabel {
            color: #c9d1d9;
            font-size: 14px;
            font-weight: 600;
            padding-left: 16px;
        }
        #AuthTitleBar QPushButton {
            background: transparent;
            border: none;
            color: #8b949e;
            font-size: 14px;
            min-width: 46px; max-width: 46px; min-height: 46px; max-height: 46px;
        }
        #AuthTitleBar QPushButton:hover { background: rgba(255,255,255,0.08); color: #c9d1d9; }
        #AuthTitleBar QPushButton#CloseBtn:hover { background: #da3633; color: #fff; }
    )");
    auto *titleLayout = new QHBoxLayout(titleBar_);
    titleLayout->setContentsMargins(0,0,0,0);
    titleLayout->setSpacing(0);
    titleLabel_ = new QLabel("Authentication", titleBar_);
    titleLabel_->setObjectName("TitleLabel");
    titleLayout->addWidget(titleLabel_, 0, Qt::AlignVCenter);
    titleLayout->addStretch(1);
    QPushButton *closeBtn = new QPushButton("✕", titleBar_);
    closeBtn->setObjectName("CloseBtn");
    connect(closeBtn, &QPushButton::clicked, this, &UserAuthDialog::reject);
    titleLayout->addWidget(closeBtn);
    titleBar_->installEventFilter(this); // for dragging

    // Content area
    content_ = new QWidget(card_);
    auto *contentLayout = new QVBoxLayout(content_);
    contentLayout->setContentsMargins(24,24,24,24);
    contentLayout->setSpacing(16);

    // Intro header
    QLabel *headerLabel = new QLabel("<span style='color:#c9d1d9;font-size:18px;font-weight:700;'>Welcome</span><br/><span style='color:#8b949e;font-size:12px;'>Enter your credentials below</span>");
    headerLabel->setAlignment(Qt::AlignLeft);
    headerLabel->setStyleSheet("background:transparent; border:none;");
    contentLayout->addWidget(headerLabel);

    // Form container with subtle background
    QWidget *formContainer = new QWidget;
    formContainer->setStyleSheet(R"(
        QWidget {
            background: #161b22;
            border: 1px solid #21262d;
            border-radius: 8px;
        }
    )");
    auto *formLayout = new QVBoxLayout(formContainer);
    formLayout->setContentsMargins(16,16,16,16);
    formLayout->setSpacing(12);

    auto labelStyle = QString("color:#8b949e;font-size:12px;font-weight:600;background:transparent;border:none;letter-spacing:0.5px;");
    auto lineEditStyle = QString(R"(
        QLineEdit {
            background:#0d1117; color:#c9d1d9; border:1px solid #30363d; border-radius:6px; padding:6px 10px; font-size:14px;
        }
        QLineEdit:focus { border:1px solid #58a6ff; }
        QLineEdit::placeholder { color:#484f58; }
    )");

    userEdit_ = new QLineEdit; userEdit_->setPlaceholderText("Username"); userEdit_->setStyleSheet(lineEditStyle);
    passEdit_ = new QLineEdit; passEdit_->setPlaceholderText("Password"); passEdit_->setEchoMode(QLineEdit::Password); passEdit_->setStyleSheet(lineEditStyle);
    oldPassEdit_ = new QLineEdit; oldPassEdit_->setEchoMode(QLineEdit::Password); oldPassEdit_->setPlaceholderText("Old Password"); oldPassEdit_->setStyleSheet(lineEditStyle);
    newPassEdit_ = new QLineEdit; newPassEdit_->setEchoMode(QLineEdit::Password); newPassEdit_->setPlaceholderText("New Password"); newPassEdit_->setStyleSheet(lineEditStyle);

    // Field rows (label + input) for easier show/hide per mode
    rowUser_ = new QWidget; {
        auto *vl = new QVBoxLayout(rowUser_);
        vl->setContentsMargins(0,0,0,0); vl->setSpacing(6);
        vl->addWidget(new QLabel("<span style='" + labelStyle + "'>USERNAME</span>"));
        vl->addWidget(userEdit_);
    }
    rowPass_ = new QWidget; {
        auto *vl = new QVBoxLayout(rowPass_);
        vl->setContentsMargins(0,0,0,0); vl->setSpacing(6);
        vl->addWidget(new QLabel("<span style='" + labelStyle + "'>PASSWORD</span>"));
        vl->addWidget(passEdit_);
    }
    rowOldPass_ = new QWidget; {
        auto *vl = new QVBoxLayout(rowOldPass_);
        vl->setContentsMargins(0,0,0,0); vl->setSpacing(6);
        vl->addWidget(new QLabel("<span style='" + labelStyle + "'>OLD PASSWORD</span>"));
        vl->addWidget(oldPassEdit_);
    }
    rowNewPass_ = new QWidget; {
        auto *vl = new QVBoxLayout(rowNewPass_);
        vl->setContentsMargins(0,0,0,0); vl->setSpacing(6);
        vl->addWidget(new QLabel("<span style='" + labelStyle + "'>NEW PASSWORD</span>"));
        vl->addWidget(newPassEdit_);
    }

    formLayout->addWidget(rowUser_);
    formLayout->addWidget(rowPass_);
    formLayout->addWidget(rowOldPass_);
    formLayout->addWidget(rowNewPass_);

    contentLayout->addWidget(formContainer);

    // Button row styled similarly to MainWindow buttons
    auto *buttonsRow = new QHBoxLayout;
    buttonsRow->setSpacing(12);
    acceptBtn_ = new QPushButton("OK");
    acceptBtn_->setStyleSheet(R"(
        QPushButton { background:#238636; color:#ffffff; border:none; border-radius:6px; padding:8px 18px; font-size:13px; font-weight:600; }
        QPushButton:hover { background:#2ea043; }
    )");
    cancelBtn_ = new QPushButton("Cancel");
    cancelBtn_->setStyleSheet(R"(
        QPushButton { background:transparent; color:#8b949e; border:1px solid #30363d; border-radius:6px; padding:8px 18px; font-size:13px; font-weight:500; }
        QPushButton:hover { background:#21262d; color:#c9d1d9; }
    )");
    switchBtn_ = new QPushButton;
    switchBtn_->setStyleSheet(R"(
        QPushButton { background:transparent; color:#58a6ff; border:none; border-radius:6px; padding:8px 10px; font-size:13px; font-weight:600; }
        QPushButton:hover { color:#79c0ff; text-decoration:underline; }
    )");
    connect(acceptBtn_, &QPushButton::clicked, this, &UserAuthDialog::onAcceptClicked);
    connect(cancelBtn_, &QPushButton::clicked, this, &UserAuthDialog::reject);
    connect(switchBtn_, &QPushButton::clicked, this, &UserAuthDialog::onSwitchModeClicked);
    buttonsRow->addWidget(acceptBtn_);
    buttonsRow->addWidget(cancelBtn_);
    buttonsRow->addStretch(1);
    buttonsRow->addWidget(switchBtn_);
    contentLayout->addLayout(buttonsRow);

    // Footer info
    QLabel *footerLabel = new QLabel("<span style='color:#484f58;font-size:11px;'>Passwords are stored securely.</span>");
    footerLabel->setAlignment(Qt::AlignLeft);
    footerLabel->setStyleSheet("background:transparent; border:none;");
    contentLayout->addWidget(footerLabel);

    // Assemble card
    cardVBox->addWidget(titleBar_);
    cardVBox->addWidget(content_);
    rootLayout->addWidget(card_, 1);

    // Shadow effect
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(28);
    shadow->setXOffset(0);
    shadow->setYOffset(4);
    shadow->setColor(QColor(0,0,0,120));
    card_->setGraphicsEffect(shadow);

    resize(480, 520);
}

void UserAuthDialog::setMode(Mode mode) {
    mode_ = mode;
    bool isLogin = (mode == Login);
    bool isRegister = (mode == Register);
    bool isChange = (mode == ChangePassword);

    if (rowUser_) rowUser_->setVisible(isLogin || isRegister || isChange); // show user always for change password (read-only)
    if (rowPass_) rowPass_->setVisible(isLogin || isRegister);
    if (rowOldPass_) rowOldPass_->setVisible(isChange);
    if (rowNewPass_) rowNewPass_->setVisible(isChange);
    passEdit_->setVisible(isLogin || isRegister);
    oldPassEdit_->setVisible(isChange);
    newPassEdit_->setVisible(isChange);
    switchBtn_->setText(isLogin ? "Create account" : (isRegister ? "Login" : "Login"));

    setWindowTitle(isLogin ? "Login" : (isRegister ? "Register" : "Change Password"));
    if (titleLabel_) {
        titleLabel_->setText(isLogin ? "Login" : (isRegister ? "Create Account" : "Change Password"));
    }
    if (acceptBtn_) {
        acceptBtn_->setText(isLogin ? "Sign in" : (isRegister ? "Create account" : "Change password"));
    }
    
    // In change password mode, show current username as read-only
    if (isChange && !currentUsername_.isEmpty()) {
        userEdit_->setText(currentUsername_);
        userEdit_->setReadOnly(true);
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

// ---- Event Handling for drag (similar to MainWindow) ----
bool UserAuthDialog::eventFilter(QObject *obj, QEvent *event) {
    if (obj == titleBar_) {
        if (event->type() == QEvent::MouseButtonPress) {
            auto *me = static_cast<QMouseEvent*>(event);
            if (me->button() == Qt::LeftButton) {
                dragging_ = true;
                dragOffset_ = me->globalPosition().toPoint() - frameGeometry().topLeft();
                return true;
            }
        } else if (event->type() == QEvent::MouseMove && dragging_) {
            auto *me = static_cast<QMouseEvent*>(event);
            if (me->buttons() & Qt::LeftButton) {
                move(me->globalPosition().toPoint() - dragOffset_);
                return true;
            }
        } else if (event->type() == QEvent::MouseButtonRelease) {
            dragging_ = false;
        } else if (event->type() == QEvent::MouseButtonDblClick) {
            // No maximize for dialog, ignore
            return true;
        }
    }
    return QDialog::eventFilter(obj, event);
}
