#include "productcard.h"
#include "productwidget.h"
#include "animationhelper.h"

ProductCard::ProductCard(const Product &prod, QWidget *parent)
    : QWidget(parent), product(prod)
{
    setupUI();
}

void ProductCard::setupUI()
{
    setObjectName("contentCard");
    setCursor(Qt::PointingHandCursor);
    setMinimumHeight(150);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    
    // Nome
    nameLabel = new QLabel(product.name);
    nameLabel->setObjectName("cardTitle");
    nameLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #2c3e50;");
    
    // Descrição
    descriptionLabel = new QLabel(product.description);
    descriptionLabel->setWordWrap(true);
    descriptionLabel->setStyleSheet("color: #7f8c8d; font-size: 13px;");
    descriptionLabel->setMaximumHeight(40);
    
    // Layout horizontal para preço e estoque
    QHBoxLayout *infoLayout = new QHBoxLayout();
    
    priceLabel = new QLabel(QString("R$ %1").arg(product.price, 0, 'f', 2));
    priceLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #27ae60;");
    
    stockLabel = new QLabel(QString("Estoque: %1").arg(product.stock));
    stockLabel->setStyleSheet("font-size: 13px; color: #95a5a6;");
    
    infoLayout->addWidget(priceLabel);
    infoLayout->addStretch();
    infoLayout->addWidget(stockLabel);
    
    // Status badge
    statusBadge = new QLabel(product.status == "active" ? "Ativo" : "Inativo");
    statusBadge->setObjectName("statusBadge");
    statusBadge->setProperty("status", product.status);
    statusBadge->setAlignment(Qt::AlignCenter);
    statusBadge->setMaximumWidth(80);
    
    // Botões de ação
    QHBoxLayout *actionLayout = new QHBoxLayout();
    
    editButton = new QPushButton("Editar");
    editButton->setObjectName("secondaryButton");
    editButton->setCursor(Qt::PointingHandCursor);
    
    deleteButton = new QPushButton("Excluir");
    deleteButton->setObjectName("deleteButton");
    deleteButton->setCursor(Qt::PointingHandCursor);
    
    actionLayout->addWidget(statusBadge);
    actionLayout->addStretch();
    actionLayout->addWidget(editButton);
    actionLayout->addWidget(deleteButton);
    
    // Adicionar tudo ao layout principal
    mainLayout->addWidget(nameLabel);
    mainLayout->addWidget(descriptionLabel);
    mainLayout->addLayout(infoLayout);
    mainLayout->addLayout(actionLayout);
    
    // Connections
    connect(editButton, &QPushButton::clicked, [this]() {
        emit editClicked(product.id);
    });
    
    connect(deleteButton, &QPushButton::clicked, [this]() {
        emit deleteClicked(product.id);
    });
}

void ProductCard::enterEvent(QEnterEvent *event)
{
    QWidget::enterEvent(event);
    // Adicionar sombra ou efeito hover
    setStyleSheet("#contentCard { border: 2px solid #1abc9c; }");
}

void ProductCard::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);
    setStyleSheet("");
}

void ProductCard::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    emit cardClicked(product.id);
}
