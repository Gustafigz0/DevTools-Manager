#pragma once
#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QColor>

class ToastNotification : public QWidget
{
    Q_OBJECT
public:
    explicit ToastNotification(QWidget* parent = nullptr);

    void setText(const QString& text);
    void setColor(const QColor& color);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QLabel* label_;
    QTimer fadeTimer_;
    QColor bgColor_;
    float opacity_;
    void startFadeOut();

private slots:
    void onFadeStep();
};
