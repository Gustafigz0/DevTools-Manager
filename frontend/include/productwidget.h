#pragma once
#include <QWidget>
#include <QGridLayout>
#include "product.h"

class ProductWidget : public QWidget {
    Q_OBJECT
public:
    explicit ProductWidget(QWidget *parent = nullptr);
    void updateGrid();
    void createProductGrid();

signals:
    void editRequested(Product prod);
    void removeRequested(Product prod);

private:
    QGridLayout *productGrid;
    QVBoxLayout *mainLayout;
    QList<Product> filteredProducts;

private slots:
    void onEditProduct(int id);
    void onDeleteProduct(int id);
};
