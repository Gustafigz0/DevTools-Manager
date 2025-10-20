#include "productwidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QFrame>

ProductWidget::ProductWidget(Product prod, QWidget *parent)
    : QWidget(parent), m_prod(prod)
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    QLabel *img = new QLabel;
    img->setFixedSize(50,50);
    img->setPixmap(QPixmap(":/resources/icons/motor.png").scaled(50,50));
    img->setStyleSheet("background: #fff; border-radius: 8px; border: 1px solid #eee;");

    QVBoxLayout *info = new QVBoxLayout;
    QLabel *top = new QLabel(prod.title);
    top->setStyleSheet("font-weight: bold; font-size: 16px;");
    QLabel *cat = new QLabel(prod.category + QString(" • Estoque: %1").arg(prod.stock));
    QLabel *date = new QLabel("Último uso: " + prod.lastUse);
    QLabel *loc = new QLabel("Depósito: " + prod.location);

    info->addWidget(top);
    info->addWidget(cat);
    info->addWidget(date);
    info->addWidget(loc);

    mainLayout->addWidget(img);
    mainLayout->addLayout(info);
    mainLayout->addStretch();
    setStyleSheet("background: #fff; border-radius: 12px; margin-bottom: 8px;");
}
