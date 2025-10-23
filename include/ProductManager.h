#pragma once
#include <QObject>
#include <QList>
#include <QString>
#include "Product.h"

class ProductManager : public QObject {
    Q_OBJECT
    
public:
    explicit ProductManager(QObject* parent = nullptr);
    
    // CRUD Operations
    bool addProduct(const Product& product);
    bool updateProduct(const Product& product);
    bool deleteProduct(const QString& productId);
    Product* findProductById(const QString& id);
    
    // Getters
    QList<Product> getAllProducts() const { return products_; }
    QList<Product> searchProducts(const QString& query) const;
    QList<Product> filterByCategory(const QString& category) const;
    QList<Product> getOutOfStock() const;
    
    // Statistics
    int getTotalProducts() const { return products_.size(); }
    int getTotalStock() const;
    double getTotalValue() const;
    QStringList getAllCategories() const;
    
    // Persistence
    bool saveToFile(const QString& filePath);
    bool loadFromFile(const QString& filePath);
    
    // Clear all
    void clear() { products_.clear(); emit productsChanged(); }
    
signals:
    void productAdded(const Product& product);
    void productUpdated(const Product& product);
    void productDeleted(const QString& productId);
    void productsChanged();
    
private:
    QList<Product> products_;
    bool productExists(const QString& id) const;
};
