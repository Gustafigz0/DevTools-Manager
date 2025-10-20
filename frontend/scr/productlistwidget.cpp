#include "productlistwidget.h"
#include "productwidget.h"
#include <QVBoxLayout>

ProductListWidget::ProductListWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->addWidget(new ProductWidget("Motor", "Categoria Motor", 120, "20 Out 2026", "Depo A"));
    layout->addWidget(new ProductWidget("Bujji", "Categoria Bujji", 500, "20 Out 2026", "Depo A"));
    // ...adicione demais produtos aqui

    layout->addStretch();
}
