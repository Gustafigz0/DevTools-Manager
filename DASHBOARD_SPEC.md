# Dashboard System - Functional Specification

## Overview
A comprehensive analytics dashboard for the DevTools Manager application that provides real-time insights into product inventory, pricing, and stock distribution using GitHub's dark theme UI design patterns.

## Design Philosophy
- **GitHub Dark Theme**: Follow GitHub's modern dark UI aesthetic (#0d1117 background, #161b22 cards, #c9d1d9 text)
- **Clean & Minimal**: Focus on data clarity with subtle borders and gradients
- **Responsive Cards**: Modular card-based layout with rounded corners (8-12px radius)
- **Consistent Spacing**: 16-24px padding, 12-16px gaps between elements

---

## Components

### 1. Dashboard Header
**Location**: Top of main content area, below user info header

**Elements**:
- Title: "Dashboard" (18px, font-weight: 700, color: #c9d1d9)
- Subtitle: "Product Analytics & Insights" (12px, color: #8b949e)
- Last Updated: Timestamp showing last data refresh (11px, color: #6e7681)

**Layout**:
```
┌─────────────────────────────────────────────────────────┐
│  📊 Dashboard                        Last updated: 2m ago │
│  Product Analytics & Insights                            │
└─────────────────────────────────────────────────────────┘
```

---

### 2. Statistics Cards Row
**Layout**: Horizontal flexbox with 2 equal-width cards

#### Card 2.1: Average Product Price
**Visual Design**:
- Background: #161b22
- Border: 1px solid #21262d
- Border-radius: 8px
- Padding: 20px
- Min-height: 120px

**Content**:
- Icon: 💰 or price tag icon (top-left, 32x32px)
- Label: "Average Price" (12px, #8b949e, uppercase, letter-spacing: 1px)
- Value: €XX.XX (28px, font-weight: 700, color: #58a6ff - GitHub blue)
- Trend indicator: +X.X% from last month (11px, color: #3fb950 if positive, #f85149 if negative)

**Calculation**: 
```cpp
double totalPrice = 0;
for (const Product& p : allProducts_) {
    totalPrice += p.getPrice();
}
double avgPrice = allProducts_.isEmpty() ? 0 : totalPrice / allProducts_.size();
```

#### Card 2.2: Average Stock Level
**Visual Design**: Same as Card 2.1

**Content**:
- Icon: 📦 or box icon (top-left, 32x32px)
- Label: "Average Stock" (12px, #8b949e, uppercase)
- Value: XX units (28px, font-weight: 700, color: #a371f7 - GitHub purple)
- Status badge: "Healthy" / "Low" / "Critical" with appropriate colors

**Calculation**:
```cpp
int totalStock = 0;
for (const Product& p : allProducts_) {
    totalStock += p.getQuantity();
}
double avgStock = allProducts_.isEmpty() ? 0 : (double)totalStock / allProducts_.size();
```

**Status Logic**:
- Healthy (green #3fb950): avgStock > 20
- Low (yellow #d29922): avgStock 5-20
- Critical (red #f85149): avgStock < 5

---

### 3. Stock Distribution Chart
**Location**: Below statistics cards, full width

**Visual Design**:
- Background: #161b22
- Border: 1px solid #21262d
- Border-radius: 8px
- Padding: 24px
- Min-height: 400px

**Header**:
- Title: "Stock Distribution by Product" (14px, font-weight: 600, #c9d1d9)
- View toggle: Pie Chart / Bar Chart (right-aligned buttons)

**Pie Chart Specifications**:
- **Library**: Use Qt Charts (QPieSeries)
- **Size**: 300x300px center-aligned
- **Colors**: GitHub palette
  - Slice 1: #1f6feb (blue)
  - Slice 2: #a371f7 (purple)
  - Slice 3: #3fb950 (green)
  - Slice 4: #d29922 (yellow)
  - Slice 5: #f85149 (red)
  - Additional slices: cycle through palette with 20% opacity variation

**Data**:
- Each slice represents one product
- Slice size = product quantity
- Label format: "{ProductName}: {Quantity} units ({Percentage}%)"
- Show labels outside the pie with connecting lines

**Legend**:
- Position: Right side of chart
- Style: Vertical list with color squares (12x12px, border-radius: 2px)
- Font: 12px, color: #8b949e
- Hover effect: Highlight corresponding slice

**Empty State**:
- Show placeholder icon (📊) and message: "No products available"
- Style: Center-aligned, color: #6e7681

---

### 4. Additional Metrics Row (Optional Enhancement)
**Layout**: Horizontal flexbox with 3 cards

**Card 4.1: Total Products**
- Value: Total count of products
- Icon: 🏷️

**Card 4.2: Out of Stock**
- Value: Count of products with quantity = 0
- Icon: ⚠️
- Color: #f85149 (red)

**Card 4.3: Low Stock Alert**
- Value: Count of products with quantity 1-5
- Icon: 📉
- Color: #d29922 (yellow)

---

## Implementation Details

### Data Refresh
- **Real-time Updates**: Dashboard refreshes when products are added/edited/deleted
- **Method**: Connect to product list change signals
- **Debounce**: 500ms delay to prevent excessive recalculations

### Responsive Behavior
- **Desktop (>1200px)**: All cards in horizontal layout
- **Tablet (768-1200px)**: Statistics cards side-by-side, chart full width
- **Mobile (<768px)**: All cards stacked vertically

### Accessibility
- All cards keyboard navigable
- ARIA labels for screen readers
- Color-blind friendly palette (avoid red/green only differentiation)
- High contrast text (WCAG AA compliant)

### Performance Optimization
- Cache calculations until data changes
- Use QTimer for debounced updates
- Lazy load chart rendering
- Maximum 20 products in pie chart (group smaller ones as "Others")

---

## Technical Requirements

### Qt Modules
```cpp
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QChart>
```

### New Classes

#### DashboardWidget.h
```cpp
class DashboardWidget : public QWidget {
    Q_OBJECT
public:
    explicit DashboardWidget(QWidget* parent = nullptr);
    void updateData(const QVector<Product>& products);

private:
    void setupUi();
    void updateStatistics();
    void updateChart();
    
    // Statistics
    QLabel* avgPriceLabel_;
    QLabel* avgStockLabel_;
    QLabel* avgPriceTrend_;
    QLabel* stockStatusBadge_;
    
    // Chart
    QtCharts::QChartView* chartView_;
    QtCharts::QPieSeries* pieSeries_;
    
    // Data
    QVector<Product> products_;
    double avgPrice_;
    double avgStock_;
};
```

### Integration Points
1. Add "Dashboard" button to sidebar navigation (above "Products")
2. Create new DashboardWidget instance in MainWindow
3. Switch view when Dashboard button clicked
4. Connect product changes to `updateData()` slot

---

## Visual Mockup (ASCII)

```
┌─────────────────────────────────────────────────────────────────┐
│  📊 Dashboard                        Last updated: Just now      │
│  Product Analytics & Insights                                    │
├─────────────────────────────────────────────────────────────────┤
│                                                                   │
│  ┌─────────────────────────┐  ┌─────────────────────────┐      │
│  │ 💰                      │  │ 📦                      │      │
│  │ AVERAGE PRICE           │  │ AVERAGE STOCK           │      │
│  │                         │  │                         │      │
│  │ € 149.85                │  │ 19 units                │      │
│  │ +2.3% from last month   │  │ ● Healthy               │      │
│  └─────────────────────────┘  └─────────────────────────┘      │
│                                                                   │
│  ┌───────────────────────────────────────────────────────────┐  │
│  │ Stock Distribution by Product           [Pie] [Bar]       │  │
│  │                                                            │  │
│  │              ┌─────────┐                                  │  │
│  │             ╱           ╲     Legend:                     │  │
│  │            │             │    ■ Mouse Gamer: 25 (20%)    │  │
│  │            │    📊      │    ■ Teclado: 15 (12%)        │  │
│  │            │             │    ■ Monitor: 8 (7%)          │  │
│  │             ╲           ╱     ■ Headset: 12 (10%)       │  │
│  │              └─────────┘      ■ Others: 63 (51%)        │  │
│  │                                                            │  │
│  └───────────────────────────────────────────────────────────┘  │
│                                                                   │
└─────────────────────────────────────────────────────────────────┘
```

---

## Color Palette Reference

| Color Name | Hex Code | Usage |
|------------|----------|-------|
| Background | #0d1117 | Main background |
| Card BG | #161b22 | Card backgrounds |
| Border | #21262d | Card borders |
| Text Primary | #c9d1d9 | Main text |
| Text Secondary | #8b949e | Labels, secondary text |
| Text Tertiary | #6e7681 | Timestamps, meta info |
| Blue | #58a6ff | Links, primary accent |
| Purple | #a371f7 | Secondary accent |
| Green | #3fb950 | Success, positive |
| Yellow | #d29922 | Warning |
| Red | #f85149 | Error, critical |

---

## User Stories

1. **As a manager**, I want to see the average product price so I can understand my pricing strategy.
2. **As a warehouse supervisor**, I need to monitor average stock levels to plan inventory restocking.
3. **As an analyst**, I want to visualize stock distribution to identify which products dominate inventory.
4. **As a decision maker**, I need quick access to key metrics without navigating through product lists.

---

## Success Metrics

- Dashboard loads in < 500ms
- Data updates reflect immediately after product changes
- All calculations accurate to 2 decimal places
- Chart supports up to 100 products without performance degradation
- Zero UI blocking during data refresh

---

## Future Enhancements (Phase 2)

- Export dashboard as PDF report
- Time-series graphs showing trends over weeks/months
- Category-based filtering for statistics
- Comparison mode (current vs previous period)
- Animated transitions when data changes
- Dark/light theme toggle
- Customizable dashboard layout (drag & drop cards)

---

## Testing Checklist

- [ ] Empty product list shows placeholder correctly
- [ ] Single product calculates 100% in pie chart
- [ ] Large numbers format with proper separators (1,234.56)
- [ ] Trend indicators calculate correctly
- [ ] Chart colors consistent across sessions
- [ ] Responsive layout works on different window sizes
- [ ] Updates propagate from product CRUD operations
- [ ] No memory leaks on repeated data refresh
- [ ] Pie chart labels readable and non-overlapping
- [ ] Legend items clickable to toggle slice visibility

---

**Document Version**: 1.0  
**Last Updated**: November 10, 2025  
**Status**: Ready for Implementation
