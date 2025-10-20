
#pragma once
#include <QWidget>
#include "product.h"
#include <QVector>
#include <QVBoxLayout>

class ProductListWidget : public QWidget {
    Q_OBJECT
public:
    explicit ProductListWidget(QWidget *parent = nullptr);

public slots:
    void addProduct(Product prod);
    void removeProduct(const Product &prod);

private:
    QVector<Product> products;
    QVBoxLayout *listLayout;
};
