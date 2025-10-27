#pragma once
#include <QString>
#include <QJsonObject>

class Product {
public:
    Product();
    Product(const QString& id, const QString& name, const QString& category, double price, int quantity);

    QString getId() const;
    QString getName() const;
    QString getCategory() const;
    double getPrice() const;
    int getQuantity() const;
    QString getPriceFormatted() const;
    QString getQuantityText() const;
    bool isInStock() const;

    // Serialização/Deserialização
    QJsonObject toJsonObject() const;
    static Product fromJsonObject(const QJsonObject& obj);

    void setId(const QString&);
    void setName(const QString&);
    void setCategory(const QString&);
    void setPrice(double);
    void setQuantity(int);

private:
    QString id_;
    QString name_;
    QString category_;
    double price_;
    int quantity_;
};
