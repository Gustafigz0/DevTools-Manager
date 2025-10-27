#pragma once
#include <QMainWindow>
#include <QList>
#include <QSet>
#include "Product.h"

class QLabel;
class QWidget;
class QVBoxLayout;
class QPushButton;
class QLineEdit;
class QCheckBox;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void onAddProductClicked();
    void onSearchTextChanged(const QString& text);
    void onSelectProductToggled(const QString& productId, bool checked);
    void onDeleteSelectedProductsClicked();
    void updateDeleteSelectedButtonState();

private:
    void setupUi();
    void loadSampleProducts();
    void displayProducts();
    void createProductCard(const Product& product);

    QWidget* sidebarBox_;
    QVBoxLayout* sidebarLayout_;
    QPushButton* btnProducts_;
    QPushButton* btnDeleteSelected_;

    QWidget* productsBg_;
    QVBoxLayout* productsLayout_;
    QLineEdit* searchBar_;
    QWidget* productsContainer_;
    QVBoxLayout* productsCardsLayout_;

    // Dados
    QList<Product> allProducts_;
    QList<Product> filteredProducts_;
    QSet<QString> selectedProductIds_;
};
