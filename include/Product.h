#pragma once
#include <QString>
#include <QDateTime>

class Product {
public:
    // Construtores
    Product();
    Product(QString id, QString name, QString category, 
            double price, int quantity);
    
    // Getters
    QString getId() const { return id_; }
    QString getName() const { return name_; }
    QString getCategory() const { return category_; }
    double getPrice() const { return price_; }
    int getQuantity() const { return quantity_; }
    QDateTime getCreatedAt() const { return createdAt_; }
    
    // Setters
    void setId(const QString& id) { id_ = id; }
    void setName(const QString& name) { name_ = name; }
    void setCategory(const QString& category) { category_ = category; }
    void setPrice(double price) { price_ = price; }
    void setQuantity(int quantity) { quantity_ = quantity; }
    
    // Métodos auxiliares
    QString getPriceFormatted() const;
    QString getQuantityText() const;
    bool isInStock() const { return quantity_ > 0; }
    
private:
    QString id_;
    QString name_;
    QString category_;
    double price_;
    int quantity_;
    QDateTime createdAt_;
};