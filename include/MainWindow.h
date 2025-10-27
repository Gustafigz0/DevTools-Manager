#pragma once

#include <QMainWindow>
#include <QSet>
#include <QMap>
#include <QList>
#include "Product.h"
#include <QComboBox>
#include <QEvent>

// Forward declarations
class QVBoxLayout;
class QHBoxLayout;
class QLineEdit;
class QPushButton;
class QWidget;
class QLabel;
class QScrollArea;
class ToastNotification;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

signals:
    void productListChanged();

private slots:
    void onAddProductClicked();
    void onEditProductRequested(const QString& productId);
    void onFavoriteProductToggled(const QString& productId, bool favorited);
    void onSelectProductToggled(const QString& productId, bool checked);
    void onDeleteSelectedProductsClicked();
    void onSearchTextChanged(const QString&);
    void onFilterChanged();
    void onExportProductsClicked();
    void onImportProductsClicked();

private:
    void setupUi();
    void loadSampleProducts();
    void displayProducts();
    void createProductCard(const Product& product);
    void updateDeleteSelectedButtonState();
    void atualizarListaProdutosUI();

    // Novos helpers
    void showToast(const QString& message, QColor color = QColor("#323e54"));
    void logOperation(const QString& op, const Product& p);
    void showHistoryDialog();

    // UI State
private:
    bool filterRunning_ = false;

    QWidget* sidebarBox_;
    QVBoxLayout* sidebarLayout_;
    QPushButton* btnProducts_;
    QPushButton* btnShowFavorites_;
    QPushButton* btnDeleteSelected_;
    QPushButton* btnExport_;
    QPushButton* btnImport_;
    QWidget* productsBg_;
    QVBoxLayout* productsLayout_;
    QWidget* productsContainer_;
    QVBoxLayout* productsCardsLayout_;
    QLineEdit* searchBar_;
    QComboBox* filterCategoryBox_;
    QComboBox* filterStockBox_;
    ToastNotification* toastNotifier_;
    // e outros membros...


    // Dados
    QList<Product> allProducts_;
    QList<Product> filteredProducts_;
    QSet<QString> selectedProductIds_;
    QSet<QString> favoriteProductIds_;
    QList<QPair<QString, Product>> operationLog_; // op, produto (add, edit, del)

    void filterAndSortProducts();
    void onBtnProducts();
    void onBtnFavorites();

    protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

};
