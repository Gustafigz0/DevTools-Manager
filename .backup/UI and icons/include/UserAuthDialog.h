#pragma once
#include <QDialog>
#include <QString>
#include <QPoint>

class QLineEdit;
class QPushButton;
class QWidget;
class QLabel;

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
    bool eventFilter(QObject* obj, QEvent* event) override;
    Mode mode_;
    QString currentUsername_;  // For change password mode
    QLineEdit *userEdit_, *passEdit_, *oldPassEdit_, *newPassEdit_;
    QPushButton *acceptBtn_, *cancelBtn_, *switchBtn_;
    // Styled UI elements
    QWidget *card_ = nullptr;
    QWidget *titleBar_ = nullptr;
    QWidget *content_ = nullptr;
    QLabel *titleLabel_ = nullptr;
    QWidget *rowUser_ = nullptr;
    QWidget *rowPass_ = nullptr;
    QWidget *rowOldPass_ = nullptr;
    QWidget *rowNewPass_ = nullptr;
    // Drag helpers
    bool dragging_ = false;
    QPoint dragOffset_;
};
