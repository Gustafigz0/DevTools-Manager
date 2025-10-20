#include "productlistwidget.h"
#include "productwidget.h"
#include <QVBoxLayout>

ProductListWidget::ProductListWidget(QWidget *parent): QWidget(parent) {
    listLayout = new QVBoxLayout(this);
    listLayout->setSpacing(12);
    listLayout->addStretch();
}

void ProductListWidget::addProduct(Product prod) {
    ProductWidget *w = new ProductWidget(this);
    connect(w, &ProductWidget::removeRequested, this, [this](Product p){ removeProduct(p); });
    // Aqui pode conectar editar também
    listLayout->insertWidget(listLayout->count()-1, w);
    products.append(prod);
}

void ProductListWidget::removeProduct(const Product &prod) {
    // Simples: remove último, ou implemente busca por igualdade
    if(QWidget *w = listLayout->itemAt(listLayout->count()-2)->widget()) {
        w->deleteLater();
    }
    if(!products.isEmpty()) products.removeLast();
}
