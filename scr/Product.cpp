#include "Product.h"

Product::Product() 
    : id_(""), name_(""), category_(""), price_(0.0), quantity_(0),
      createdAt_(QDateTime::currentDateTime()) {
}

Product::Product(QString id, QString name, QString category, 
                 double price, int quantity)
    : id_(id), name_(name), category_(category), 
      price_(price), quantity_(quantity),
      createdAt_(QDateTime::currentDateTime()) {
}

QString Product::getPriceFormatted() const {
    return QString("R$ %1").arg(price_, 0, 'f', 2);
}

QString Product::getQuantityText() const {
    return QString("Qtd: %1").arg(quantity_);
}
