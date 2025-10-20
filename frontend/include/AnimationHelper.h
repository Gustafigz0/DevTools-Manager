
#ifndef ANIMATIONHELPER_H
#define ANIMATIONHELPER_H

#include <QWidget>

class AnimationHelper {
public:
    static void fadeIn(QWidget *widget, int duration);
    static void fadeOut(QWidget *widget, int duration);
    static void slideInFromLeft(QWidget *widget, int distance, int duration);
    static void slideInFromRight(QWidget *widget, int distance, int duration);
    static void bounce(QWidget *widget, int height, int duration);
    static void pulse(QWidget *widget, double scale, int duration);
};

#endif // ANIMATIONHELPER_H
