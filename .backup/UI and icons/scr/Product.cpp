#include "Product.h"

Product::Product()
    : id_(""), name_(""), category_(""), price_(0.0), quantity_(0), imagePath_("") {}

Product::Product(const QString& id, const QString& name, const QString& category, double price, int quantity)
    : id_(id), name_(name), category_(category), price_(price), quantity_(quantity), imagePath_("") {}

QString Product::getId() const { return id_; }
QString Product::getName() const { return name_; }
QString Product::getCategory() const { return category_; }
double Product::getPrice() const { return price_; }
int Product::getQuantity() const { return quantity_; }
bool Product::isInStock() const { return quantity_ > 0; }

QString Product::getPriceFormatted() const {
    // Usar € para exemplo europeu ou "R$" se preferir
    return QString("€ %1").arg(QString::number(price_, 'f', 2));
}

QString Product::getQuantityText() const {
    return QString("Qtd: %1").arg(quantity_);
}

QString Product::getImagePath() const { return imagePath_; }

// Serialização para JSON
QJsonObject Product::toJsonObject() const {
    QJsonObject obj;
    obj["id"] = id_;
    obj["name"] = name_;
    obj["category"] = category_;
    obj["price"] = price_;
    obj["imagePath"] = imagePath_;
    return obj;
}

Product Product::fromJsonObject(const QJsonObject& obj) {
    Product product(
        obj.value("id").toString(),
        obj.value("name").toString(),
        obj.value("category").toString(),
        obj.value("price").toDouble(),
        obj.value("quantity").toInt()
    );
    product.setImagePath(obj.value("imagePath").toString());
    return product;
}

// Setters
void Product::setId(const QString& id) { id_ = id; }
void Product::setName(const QString& n) { name_ = n; }
void Product::setCategory(const QString& c) { category_ = c; }
void Product::setPrice(double p) { price_ = p; }
void Product::setImagePath(const QString& path) { imagePath_ = path; }
