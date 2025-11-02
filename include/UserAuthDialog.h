#pragma once
#include <QDialog>
#include <QString>

class QLineEdit;
class QPushButton;

class UserAuthDialog : public QDialog {
    Q_OBJECT
public:
    enum Mode { Login, Register, ChangePassword };
    UserAuthDialog(Mode mode, const QString& currentUsername = QString(), QWidget* parent = nullptr);

    void setMode(Mode mode);
    void setCurrentUsername(const QString& username);
    QString username() const;
    QString password() const;
    QString oldPassword() const;
    QString newPassword() const;

signals:
    void authSuccess(const QString& username);
    void authCancelled();

private slots:
    void onAcceptClicked();
    void onSwitchModeClicked();

private:
    void setupUi();
    Mode mode_;
    QString currentUsername_;  // For change password mode
    QLineEdit *userEdit_, *passEdit_, *oldPassEdit_, *newPassEdit_;
    QPushButton *acceptBtn_, *cancelBtn_, *switchBtn_;
};
