#include "ToastNotification.h"
#include <QHBoxLayout>
#include <QPropertyAnimation>
#include <QPainter>
#include <QStyleOption>
#include <QGraphicsOpacityEffect>

ToastNotification::ToastNotification(QWidget* parent)
    : QWidget(parent), label_(nullptr), bgColor_("#323e54"), opacity_(1.0)
{
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint | Qt::ToolTip);
    setFixedHeight(46);
    setMinimumWidth(220);
    setMaximumWidth(500);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    auto* layout = new QHBoxLayout(this);
    layout->setContentsMargins(20, 8, 20, 8);
    label_ = new QLabel(this);
    label_->setStyleSheet("color: #fff; font-size: 16px; font-weight: 500;");
    label_->setAlignment(Qt::AlignCenter);
    layout->addWidget(label_, 1);

    hide();
    connect(&fadeTimer_, &QTimer::timeout, this, &ToastNotification::onFadeStep);
}

void ToastNotification::setText(const QString& text)
{
    label_->setText(text);
    opacity_ = 1.0;
    show();
    raise();
    startFadeOut();
}

void ToastNotification::setColor(const QColor& color)
{
    bgColor_ = color;
}

void ToastNotification::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setOpacity(opacity_);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(bgColor_);
    painter.setPen(Qt::NoPen);
    QRect r = rect();
    r.setHeight(height());
    r.setWidth(width());
    painter.drawRoundedRect(r, 16, 16);
}

void ToastNotification::startFadeOut()
{
    fadeTimer_.stop();
    opacity_ = 1.0;
    show();
    fadeTimer_.start(40); // fade step every 40ms
}

void ToastNotification::onFadeStep()
{
    opacity_ -= 0.025f; // fade out smoothly over ~2 seconds
    if (opacity_ <= 0.0f) {
        opacity_ = 0.0f;
        fadeTimer_.stop();
        hide();
    } else {
        update();
    }
}
