#pragma once
#include <QMainWindow>
#include <QList>
#include "Product.h"

class QLabel;
class QWidget;
class QVBoxLayout;
class QPushButton;
class QLineEdit;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    
private slots:
    void onAddProductClicked();
    void onSearchTextChanged(const QString& text);
    void onEditProduct(const QString& productId);      // ← NOVO
    void onDeleteProduct(const QString& productId);    // ← NOVO
    
private:
    void setupUi();
    void loadSampleProducts();
    void displayProducts();
    void createProductCard(const Product& product);
    Product* findProductById(const QString& id);       // ← NOVO
    
    // Widgets
    QWidget* sidebarBox_;
    QWidget* userInfoLabel_;
    QWidget* productsBg_;
    QVBoxLayout* productsLayout_;
    QLineEdit* searchBar_;
    QPushButton* btnAddProduct_;
    
    // Área onde os cards serão exibidos
    QWidget* productsContainer_;
    QVBoxLayout* productsCardsLayout_;
    
    // Dados
    QList<Product> allProducts_;
    QList<Product> filteredProducts_;
};
