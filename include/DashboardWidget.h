#pragma once

#include <QWidget>
#include <QVector>
#include <QTimer>
#include <QDateTime>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>
#include "Product.h"

// Forward declarations
class QVBoxLayout;
class QHBoxLayout;
class QLabel;
class QPushButton;

/**
 * @brief Dashboard widget displaying analytics and statistics
 * 
 * Features:
 * - Average product price with trend indicator (placeholder)
 * - Average stock level with status badge
 * - Stock distribution pie chart
 * - Real-time updates when product data changes
 */
class DashboardWidget : public QWidget {
    Q_OBJECT

public:
    explicit DashboardWidget(QWidget* parent = nullptr);
    ~DashboardWidget();

    /**
     * @brief Update dashboard with new product data
     * @param products List of all products
     * 
     * Triggers recalculation of statistics and chart update.
     * Uses debouncing (500ms) to prevent excessive updates.
     */
    void updateData(const QVector<Product>& products);

private slots:
    void performUpdate();

private:
    void setupUi();
    void createHeader();
    void createStatisticsCards();
    void createChartSection();
    void applyStyles();
    
    void updateStatistics();
    void updateChart();
    void updatePieChart();
    void updateLastRefreshTime();
    
    // Helper methods
    QString formatPrice(double price) const;
    QString formatPercentage(double value) const;
    QString getStockStatus(double avgStock) const;
    QColor getStockStatusColor(double avgStock) const;
    QString getRelativeTimeString() const;
    quint64 computeProductsSignature(const QVector<Product>& products) const;

    // UI Components - Header
    QLabel* titleLabel_;
    QLabel* subtitleLabel_;
    QLabel* lastUpdatedLabel_;
    
    // UI Components - Statistics Cards
    QWidget* avgPriceCard_;
    QWidget* avgStockCard_;
    QLabel* avgPriceValueLabel_;
    QLabel* avgStockValueLabel_;
    QLabel* avgPriceTrendLabel_;
    QLabel* stockStatusBadge_;
    
    // UI Components - Chart
    QWidget* chartCard_;
    QChartView* chartView_;
    QPieSeries* pieSeries_;
    QChart* chart_;
    QLabel* emptyStateLabel_;
    
    // Data
    QVector<Product> products_;
    double avgPrice_;
    double avgStock_;
    QTimer* updateTimer_;
    QDateTime lastUpdateTime_;
    quint64 lastProductsSignature_ = 0;
    
    // Constants
    static constexpr int DEBOUNCE_MS = 500;
    static constexpr int MAX_PIE_SLICES = 10;
    static constexpr double STOCK_CRITICAL = 5.0;
    static constexpr double STOCK_LOW = 20.0;
};
