#include "animationhelper.h"
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QRect>
#include <QPoint>

void AnimationHelper::fadeIn(QWidget *widget, int duration)
{
    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(widget);
    widget->setGraphicsEffect(effect);
    QPropertyAnimation *animation = new QPropertyAnimation(effect, "opacity");
    animation->setDuration(duration);
    animation->setStartValue(0.0);
    animation->setEndValue(1.0);
    animation->setEasingCurve(QEasingCurve::InOutQuad);
    animation->start(QPropertyAnimation::DeleteWhenStopped);
}

void AnimationHelper::fadeOut(QWidget *widget, int duration)
{
    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(widget);
    widget->setGraphicsEffect(effect);
    QPropertyAnimation *animation = new QPropertyAnimation(effect, "opacity");
    animation->setDuration(duration);
    animation->setStartValue(1.0);
    animation->setEndValue(0.0);
    animation->setEasingCurve(QEasingCurve::InOutQuad);
    animation->start(QPropertyAnimation::DeleteWhenStopped);
}

void AnimationHelper::slideInFromLeft(QWidget *widget, int distance, int duration)
{
    QPoint originalPos = widget->pos();
    widget->move(originalPos.x() - distance, originalPos.y());
    QPropertyAnimation *animation = new QPropertyAnimation(widget, "pos");
    animation->setDuration(duration);
    animation->setStartValue(QPoint(originalPos.x() - distance, originalPos.y()));
    animation->setEndValue(originalPos);
    animation->setEasingCurve(QEasingCurve::OutCubic);
    animation->start(QPropertyAnimation::DeleteWhenStopped);
}

void AnimationHelper::slideInFromRight(QWidget *widget, int distance, int duration)
{
    QPoint originalPos = widget->pos();
    widget->move(originalPos.x() + distance, originalPos.y());
    QPropertyAnimation *animation = new QPropertyAnimation(widget, "pos");
    animation->setDuration(duration);
    animation->setStartValue(QPoint(originalPos.x() + distance, originalPos.y()));
    animation->setEndValue(originalPos);
    animation->setEasingCurve(QEasingCurve::OutCubic);
    animation->start(QPropertyAnimation::DeleteWhenStopped);
}

void AnimationHelper::bounce(QWidget *widget, int height, int duration)
{
    QPoint originalPos = widget->pos();
    QPropertyAnimation *animation = new QPropertyAnimation(widget, "pos");
    animation->setDuration(duration);
    animation->setKeyValueAt(0.0, originalPos);
    animation->setKeyValueAt(0.5, QPoint(originalPos.x(), originalPos.y() - height));
    animation->setKeyValueAt(1.0, originalPos);
    animation->setEasingCurve(QEasingCurve::OutBounce);
    animation->start(QPropertyAnimation::DeleteWhenStopped);
}

void AnimationHelper::pulse(QWidget *widget, double scale, int duration)
{
    QPropertyAnimation *animation = new QPropertyAnimation(widget, "geometry");
    animation->setDuration(duration);
    QRect originalGeometry = widget->geometry();
    int newWidth = originalGeometry.width() * scale;
    int newHeight = originalGeometry.height() * scale;
    int xOffset = (newWidth - originalGeometry.width()) / 2;
    int yOffset = (newHeight - originalGeometry.height()) / 2;
    QRect scaledGeometry(
        originalGeometry.x() - xOffset,
        originalGeometry.y() - yOffset,
        newWidth,
        newHeight
    );
    animation->setKeyValueAt(0.0, originalGeometry);
    animation->setKeyValueAt(0.5, scaledGeometry);
    animation->setKeyValueAt(1.0, originalGeometry);
    animation->setEasingCurve(QEasingCurve::InOutQuad);
    animation->start(QPropertyAnimation::DeleteWhenStopped);
}
