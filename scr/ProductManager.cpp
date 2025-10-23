#include "ProductManager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

ProductManager::ProductManager(QObject* parent) : QObject(parent) {
}

bool ProductManager::productExists(const QString& id) const {
    for (const Product& p : products_) {
        if (p.getId() == id) {
            return true;
        }
    }
    return false;
}

bool ProductManager::addProduct(const Product& product) {
    if (productExists(product.getId())) {
        qWarning() << "Produto com ID" << product.getId() << "já existe!";
        return false;
    }
    
    products_.append(product);
    emit productAdded(product);
    emit productsChanged();
    return true;
}

bool ProductManager::updateProduct(const Product& product) {
    for (int i = 0; i < products_.size(); ++i) {
        if (products_[i].getId() == product.getId()) {
            products_[i] = product;
            emit productUpdated(product);
            emit productsChanged();
            return true;
        }
    }
    
    qWarning() << "Produto com ID" << product.getId() << "não encontrado!";
    return false;
}

bool ProductManager::deleteProduct(const QString& productId) {
    for (int i = 0; i < products_.size(); ++i) {
        if (products_[i].getId() == productId) {
            products_.removeAt(i);
            emit productDeleted(productId);
            emit productsChanged();
            return true;
        }
    }
    
    qWarning() << "Produto com ID" << productId << "não encontrado!";
    return false;
}

Product* ProductManager::findProductById(const QString& id) {
    for (int i = 0; i < products_.size(); ++i) {
        if (products_[i].getId() == id) {
            return &products_[i];
        }
    }
    return nullptr;
}

QList<Product> ProductManager::searchProducts(const QString& query) const {
    if (query.isEmpty()) {
        return products_;
    }
    
    QList<Product> results;
    QString lowerQuery = query.toLower();
    
    for (const Product& p : products_) {
        if (p.getName().toLower().contains(lowerQuery) ||
            p.getCategory().toLower().contains(lowerQuery) ||
            p.getId().toLower().contains(lowerQuery)) {
            results.append(p);
        }
    }
    
    return results;
}

QList<Product> ProductManager::filterByCategory(const QString& category) const {
    QList<Product> results;
    
    for (const Product& p : products_) {
        if (p.getCategory() == category) {
            results.append(p);
        }
    }
    
    return results;
}

QList<Product> ProductManager::getOutOfStock() const {
    QList<Product> results;
    
    for (const Product& p : products_) {
        if (!p.isInStock()) {
            results.append(p);
        }
    }
    
    return results;
}

int ProductManager::getTotalStock() const {
    int total = 0;
    for (const Product& p : products_) {
        total += p.getQuantity();
    }
    return total;
}

double ProductManager::getTotalValue() const {
    double total = 0;
    for (const Product& p : products_) {
        total += p.getPrice() * p.getQuantity();
    }
    return total;
}
