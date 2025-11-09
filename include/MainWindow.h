#pragma once

#include <QMainWindow>
#include <QSet>
#include <QMap>
#include <QList>
#include "Product.h"
#include <QComboBox>
#include <QEvent>
#include <QColor>
class QPushButton;

// Forward declarations
class QVBoxLayout;
class QHBoxLayout;
class QLineEdit;
class QWidget;
class QLabel;
class QScrollArea;
class ToastNotification;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(const QString& username = QString(), QWidget* parent = nullptr);

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
    void onLogoutClicked();

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
    int lastProductCardClickedIndex_ = -1;

    // Custom title bar
    QWidget* titleBar_ = nullptr;
    QLabel* titleLabel_ = nullptr;
    QPushButton* btnWinMin_ = nullptr;
    QPushButton* btnWinMax_ = nullptr;
    QPushButton* btnWinClose_ = nullptr;
    bool draggingWindow_ = false;
    QPoint dragOffset_;

    // Window resize handling
    bool resizing_ = false;
    QPoint resizeOffset_;
    Qt::Edges resizeEdge_ = Qt::Edges();

    // Window rounded container (card)
    QWidget* windowCard_ = nullptr;

    QWidget* sidebarBox_;
    QVBoxLayout* sidebarLayout_;
    QPushButton* btnProducts_;
    QPushButton* btnShowFavorites_;
    QPushButton* btnDeleteSelected_;
    QPushButton* btnExport_;
    QPushButton* btnImport_;
    QPushButton* btnLogout_ = nullptr;
    QWidget* productsBg_;
    QVBoxLayout* productsLayout_;
    QWidget* productsContainer_;
    QVBoxLayout* productsCardsLayout_;
    QLineEdit* searchBar_;
    QComboBox* filterCategoryBox_;
    QComboBox* filterStockBox_;
    ToastNotification* toastNotifier_;
    QLabel* userLabel_ = nullptr;
    QString currentUsername_;
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
    
    // Helper methods for window resize
    Qt::Edges getResizeEdge(const QPoint& pos);
    void updateCursorShape(Qt::Edges edges);

    protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    void resizeEvent(QResizeEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

};
