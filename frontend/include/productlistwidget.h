#pragma once

#include <QWidget>

class ProductListWidget : public QWidget {
    Q_OBJECT
public:
    explicit ProductListWidget(QWidget *parent = nullptr) : QWidget(parent) {}
};
