#pragma once
#include <QWidget>
#include "product.h"

class ProductWidget : public QWidget {
    Q_OBJECT
public:
    explicit ProductWidget(Product prod, QWidget *parent = nullptr);

signals:
    void editRequested(Product prod);
    void removeRequested(Product prod);

private:
    Product m_prod;
};
