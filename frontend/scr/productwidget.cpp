
#include "productwidget.h"
#include "productcard.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QScrollArea>
#include <QLabel>
#include <QFrame>

#include "AnimationHelper.h"

ProductWidget::ProductWidget(QWidget *parent)
    : QWidget(parent)
{
    mainLayout = new QVBoxLayout(this);
    createProductGrid();
    setStyleSheet("background: #fff; border-radius: 12px; margin-bottom: 8px;");
}

void ProductWidget::createProductGrid()
{
    QWidget *gridWidget = new QWidget();
    productGrid = new QGridLayout(gridWidget);
    productGrid->setSpacing(20);
    
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidget(gridWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setObjectName("contentCard");
    
    mainLayout->addWidget(scrollArea);
}

void ProductWidget::updateGrid()
{
    // Clear existing grid
    QLayoutItem *item;
    while ((item = productGrid->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    
    // Add cards
    int row = 0, col = 0;
    const int maxColumns = 3;
    
    for (const auto &product : filteredProducts) {
        ProductCard *card = new ProductCard(product);
        
        connect(card, &ProductCard::editClicked, [this](int id) {
            onEditProduct(id);
        });
        
        connect(card, &ProductCard::deleteClicked, [this](int id) {
            onDeleteProduct(id);
        });
        
        productGrid->addWidget(card, row, col);
        
        col++;
        if (col >= maxColumns) {
            col = 0;
            row++;
        }
        
        // Add animation
        AnimationHelper::fadeIn(card, 200);
    }
    
    // Add flexible space at the end
    productGrid->setRowStretch(row + 1, 1);
}

void ProductWidget::onEditProduct(int id)
{
    // Implement edit functionality
    // Aqui você pode buscar o Product pelo id e emitir editRequested
    // Exemplo: emit editRequested(product);
}

void ProductWidget::onDeleteProduct(int id)
{
    // Implement delete functionality
    // Aqui você pode buscar o Product pelo id e emitir removeRequested
    // Exemplo: emit removeRequested(product);
}
