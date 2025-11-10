#include "DashboardWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QDateTime>
#include <QStyle>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QChart>
#include <QtCharts/QLegend>
#include <QGraphicsDropShadowEffect>
#include <algorithm>

DashboardWidget::DashboardWidget(QWidget* parent)
    : QWidget(parent)
    , avgPrice_(0.0)
    , avgStock_(0.0)
    , updateTimer_(new QTimer(this))
{
    updateTimer_->setSingleShot(true);
    connect(updateTimer_, &QTimer::timeout, this, &DashboardWidget::performUpdate);
    
    setupUi();
    applyStyles();
    lastUpdateTime_ = QDateTime::currentDateTime();
}

DashboardWidget::~DashboardWidget() {
    // Qt handles cleanup of child widgets
}

void DashboardWidget::setupUi() {
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(24, 24, 24, 24);
    mainLayout->setSpacing(20);
    
    // Create components
    createHeader();
    createStatisticsCards();
    createChartSection();
    
    mainLayout->addStretch();
    setLayout(mainLayout);
}

void DashboardWidget::createHeader() {
    auto* headerWidget = new QWidget(this);
    auto* headerLayout = new QVBoxLayout(headerWidget);
    headerLayout->setContentsMargins(0, 0, 0, 0);
    headerLayout->setSpacing(4);
    
    // Title row with icon and last updated
    auto* titleRow = new QHBoxLayout();
    
    titleLabel_ = new QLabel("📊 Dashboard", this);
    titleLabel_->setObjectName("dashboardTitle");
    titleLabel_->setStyleSheet("font-size: 18px; font-weight: 700; color: #c9d1d9;");
    
    titleRow->addWidget(titleLabel_);
    titleRow->addStretch();
    
    lastUpdatedLabel_ = new QLabel("Last updated: Just now", this);
    lastUpdatedLabel_->setObjectName("lastUpdated");
    lastUpdatedLabel_->setStyleSheet("font-size: 11px; color: #6e7681;");
    titleRow->addWidget(lastUpdatedLabel_);
    
    // Subtitle
    subtitleLabel_ = new QLabel("Product Analytics & Insights", this);
    subtitleLabel_->setObjectName("dashboardSubtitle");
    subtitleLabel_->setStyleSheet("font-size: 12px; color: #8b949e;");
    
    headerLayout->addLayout(titleRow);
    headerLayout->addWidget(subtitleLabel_);
    
    layout()->addWidget(headerWidget);
}

void DashboardWidget::createStatisticsCards() {
    auto* cardsLayout = new QHBoxLayout();
    cardsLayout->setSpacing(16);
    
    // Card 1: Average Price
    avgPriceCard_ = new QWidget(this);
    avgPriceCard_->setObjectName("statCard");
    auto* priceCardLayout = new QVBoxLayout(avgPriceCard_);
    priceCardLayout->setContentsMargins(20, 20, 20, 24);
    priceCardLayout->setSpacing(8);
    
    auto* priceIcon = new QLabel("💰", this);
    priceIcon->setStyleSheet("font-size: 32px;");
    
    auto* priceLabel = new QLabel("AVERAGE PRICE", this);
    priceLabel->setStyleSheet("font-size: 12px; color: #8b949e; letter-spacing: 1px;");
    
    avgPriceValueLabel_ = new QLabel("€ 0.00", this);
    avgPriceValueLabel_->setObjectName("statValue");
    avgPriceValueLabel_->setStyleSheet("font-size: 28px; font-weight: 700; color: #58a6ff;");
    
    avgPriceTrendLabel_ = new QLabel("No trend data", this);
    avgPriceTrendLabel_->setStyleSheet("font-size: 11px; color: #8b949e;");
    
    priceCardLayout->addWidget(priceIcon);
    priceCardLayout->addWidget(priceLabel);
    priceCardLayout->addWidget(avgPriceValueLabel_);
    priceCardLayout->addWidget(avgPriceTrendLabel_);
    priceCardLayout->addStretch();
    
    // Card 2: Average Stock
    avgStockCard_ = new QWidget(this);
    avgStockCard_->setObjectName("statCard");
    auto* stockCardLayout = new QVBoxLayout(avgStockCard_);
    stockCardLayout->setContentsMargins(20, 20, 20, 24);
    stockCardLayout->setSpacing(8);
    
    auto* stockIcon = new QLabel("📦", this);
    stockIcon->setStyleSheet("font-size: 32px;");
    
    auto* stockLabel = new QLabel("AVERAGE STOCK", this);
    stockLabel->setStyleSheet("font-size: 12px; color: #8b949e; letter-spacing: 1px;");
    
    avgStockValueLabel_ = new QLabel("0 units", this);
    avgStockValueLabel_->setObjectName("statValue");
    avgStockValueLabel_->setStyleSheet("font-size: 28px; font-weight: 700; color: #a371f7;");
    
    stockStatusBadge_ = new QLabel("● No Data", this);
    stockStatusBadge_->setStyleSheet("font-size: 11px; color: #8b949e;");
    
    stockCardLayout->addWidget(stockIcon);
    stockCardLayout->addWidget(stockLabel);
    stockCardLayout->addWidget(avgStockValueLabel_);
    stockCardLayout->addWidget(stockStatusBadge_);
    stockCardLayout->addStretch();
    
    cardsLayout->addWidget(avgPriceCard_);
    cardsLayout->addWidget(avgStockCard_);
    
    layout()->addItem(cardsLayout);
}

void DashboardWidget::createChartSection() {
    chartCard_ = new QWidget(this);
    chartCard_->setObjectName("chartCard");
    auto* chartLayout = new QVBoxLayout(chartCard_);
    chartLayout->setContentsMargins(24, 24, 24, 24);
    chartLayout->setSpacing(16);
    
    // Create chart
    chart_ = new QChart();
    chart_->setTheme(QChart::ChartThemeDark);
    chart_->setBackgroundVisible(false);
    chart_->setBackgroundBrush(Qt::transparent);
    chart_->setMargins(QMargins(0, 0, 0, 0));
    chart_->setPlotAreaBackgroundVisible(false);
    chart_->legend()->setAlignment(Qt::AlignRight);
    chart_->legend()->setFont(QFont("Segoe UI", 14));
    chart_->legend()->setLabelColor(QColor("#8b949e"));
    chart_->legend()->setBackgroundVisible(false);
    chart_->legend()->setBorderColor(Qt::transparent);
    
    pieSeries_ = new QPieSeries();
    chart_->addSeries(pieSeries_);
    
    chartView_ = new QChartView(chart_, this);
    chartView_->setRenderHint(QPainter::Antialiasing);
    chartView_->setMinimumHeight(400);
    chartView_->setStyleSheet("border: none; background: transparent;");
    
    // Empty state label
    emptyStateLabel_ = new QLabel("📊\n\nNo products available\n\nAdd products to see stock distribution", this);
    emptyStateLabel_->setAlignment(Qt::AlignCenter);
    emptyStateLabel_->setStyleSheet("font-size: 14px; color: #6e7681; line-height: 1.6;");
    emptyStateLabel_->setVisible(true);
    
    chartLayout->addWidget(chartView_);
    chartLayout->addWidget(emptyStateLabel_);
    
    layout()->addWidget(chartCard_);
}

void DashboardWidget::applyStyles() {
    // Main widget background
    setStyleSheet(R"(
        DashboardWidget {
            background-color: #0d1117;
        }
        
        QWidget#statCard {
            background-color: transparent;
            border: none;
            min-height: 160px;
        }
        
        QWidget#chartCard {
            background-color: #161b22;
            border: 1px solid #21262d;
            border-radius: 8px;
            min-height: 400px;
        }
    )");
}

void DashboardWidget::updateData(const QVector<Product>& products) {
    products_ = products;
    
    // Debounce updates: restart timer
    updateTimer_->stop();
    updateTimer_->start(DEBOUNCE_MS);
}

void DashboardWidget::performUpdate() {
    updateStatistics();
    updateChart();
    updateLastRefreshTime();
}

void DashboardWidget::updateStatistics() {
    if (products_.isEmpty()) {
        avgPrice_ = 0.0;
        avgStock_ = 0.0;
        avgPriceValueLabel_->setText("€ 0.00");
        avgStockValueLabel_->setText("0 units");
        avgPriceTrendLabel_->setText("No products");
        stockStatusBadge_->setText("● No Data");
        stockStatusBadge_->setStyleSheet("font-size: 11px; color: #8b949e;");
        return;
    }
    
    // Calculate average price
    double totalPrice = 0.0;
    int totalStock = 0;
    
    for (const Product& product : products_) {
        totalPrice += product.getPrice();
        totalStock += product.getQuantity();
    }
    
    avgPrice_ = totalPrice / products_.size();
    avgStock_ = static_cast<double>(totalStock) / products_.size();
    
    // Update UI
    avgPriceValueLabel_->setText(formatPrice(avgPrice_));
    avgStockValueLabel_->setText(QString("%1 units").arg(static_cast<int>(avgStock_)));
    
    // Trend indicator (placeholder - no historical data yet)
    avgPriceTrendLabel_->setText("Historical data not available");
    avgPriceTrendLabel_->setStyleSheet("font-size: 11px; color: #8b949e;");
    
    // Stock status badge
    QString status = getStockStatus(avgStock_);
    QColor statusColor = getStockStatusColor(avgStock_);
    stockStatusBadge_->setText("● " + status);
    stockStatusBadge_->setStyleSheet(QString("font-size: 11px; color: %1; font-weight: 600;")
                                     .arg(statusColor.name()));
}

void DashboardWidget::updateChart() {
    updatePieChart();
}

void DashboardWidget::updatePieChart() {
    if (!pieSeries_) return;
    
    // Clear existing chart
    pieSeries_->clear();
    
    if (products_.isEmpty()) {
        emptyStateLabel_->setVisible(true);
        chartView_->setVisible(false);
        return;
    }
    
    emptyStateLabel_->setVisible(false);
    chartView_->setVisible(true);
    
    // Create a copy and sort by quantity (descending)
    QVector<Product> sortedProducts = products_;
    std::sort(sortedProducts.begin(), sortedProducts.end(), 
              [](const Product& a, const Product& b) {
                  return a.getQuantity() > b.getQuantity();
              });
    
    // Color palette (Blue tones only)
    QList<QColor> colors = {
        QColor("#58a6ff"), // Bright blue
        QColor("#1f6feb"), // Medium blue
        QColor("#0969da"), // Deep blue
        QColor("#0550ae"), // Darker blue
        QColor("#033d8b"), // Navy blue
        QColor("#6cb6ff"), // Light blue
        QColor("#4493f8"), // Sky blue
        QColor("#2f81f7"), // Vibrant blue
    };
    
    int totalQuantity = 0;
    for (const Product& p : products_) {
        totalQuantity += p.getQuantity();
    }
    
    if (totalQuantity == 0) {
        // All products have 0 stock
        auto* slice = new QPieSlice("No stock available", 1);
        slice->setColor(QColor("#6e7681"));
        slice->setLabelVisible(false);
        pieSeries_->append(slice);
        return;
    }
    
    // Add top products (up to MAX_PIE_SLICES)
    int othersQuantity = 0;
    int sliceCount = 0;
    
    for (int i = 0; i < sortedProducts.size(); ++i) {
        const Product& product = sortedProducts[i];
        
        if (sliceCount < MAX_PIE_SLICES) {
            double percentage = (static_cast<double>(product.getQuantity()) / totalQuantity) * 100.0;
            QString label = QString("%1: %2 units (%3%)")
                .arg(product.getName())
                .arg(product.getQuantity())
                .arg(percentage, 0, 'f', 1);
            
            auto* slice = new QPieSlice(label, product.getQuantity());
            
            // Apply color with cycling
            QColor color = colors[sliceCount % colors.size()];
            slice->setColor(color);
            slice->setLabelVisible(false);  // Hide labels around pie
            slice->setLabelColor(QColor("#c9d1d9"));
            
            // Hover effect
            connect(slice, &QPieSlice::hovered, [slice](bool hovered) {
                slice->setExploded(hovered);
            });
            
            pieSeries_->append(slice);
            sliceCount++;
        } else {
            othersQuantity += product.getQuantity();
        }
    }
    
    // Add "Others" slice if there are more products
    if (othersQuantity > 0) {
        double percentage = (static_cast<double>(othersQuantity) / totalQuantity) * 100.0;
        QString label = QString("Others: %1 units (%2%)")
            .arg(othersQuantity)
            .arg(percentage, 0, 'f', 1);
        
        auto* slice = new QPieSlice(label, othersQuantity);
        slice->setColor(QColor("#0d419d")); // Dark blue for "Others"
        slice->setLabelVisible(false);  // Hide labels around pie
        slice->setLabelColor(QColor("#c9d1d9"));
        
        pieSeries_->append(slice);
    }
    
    chart_->setTitle("");
}

void DashboardWidget::updateLastRefreshTime() {
    lastUpdateTime_ = QDateTime::currentDateTime();
    QString timeStr = getRelativeTimeString();
    lastUpdatedLabel_->setText("Last updated: " + timeStr);
}

QString DashboardWidget::formatPrice(double price) const {
    return QString("€ %1").arg(price, 0, 'f', 2);
}

QString DashboardWidget::formatPercentage(double value) const {
    QString sign = value >= 0 ? "+" : "";
    return QString("%1%2%").arg(sign).arg(value, 0, 'f', 1);
}

QString DashboardWidget::getStockStatus(double avgStock) const {
    if (avgStock < STOCK_CRITICAL) {
        return "Critical";
    } else if (avgStock < STOCK_LOW) {
        return "Low";
    } else {
        return "Healthy";
    }
}

QColor DashboardWidget::getStockStatusColor(double avgStock) const {
    if (avgStock < STOCK_CRITICAL) {
        return QColor("#f85149"); // Red
    } else if (avgStock < STOCK_LOW) {
        return QColor("#d29922"); // Yellow
    } else {
        return QColor("#3fb950"); // Green
    }
}

QString DashboardWidget::getRelativeTimeString() const {
    qint64 secondsAgo = lastUpdateTime_.secsTo(QDateTime::currentDateTime());
    
    if (secondsAgo < 60) {
        return "Just now";
    } else if (secondsAgo < 3600) {
        int minutes = secondsAgo / 60;
        return QString("%1m ago").arg(minutes);
    } else if (secondsAgo < 86400) {
        int hours = secondsAgo / 3600;
        return QString("%1h ago").arg(hours);
    } else {
        return lastUpdateTime_.toString("MMM dd, hh:mm");
    }
}
