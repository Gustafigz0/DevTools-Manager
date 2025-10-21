// scr/MainWindow.cpp
#include "MainWindow.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QFrame>
#include <QStringList>

MainWindow::MainWindow(QWidget* parent): QMainWindow(parent) {
    setupUi();
    setupConnections();
}

void MainWindow::setupUi() {
    auto* central = new QWidget(this);
    auto* root = new QHBoxLayout(central);
    root->setContentsMargins(16,16,16,16);
    root->setSpacing(24);

    // Sidebar minimalista e arredondada
    sidebarBox_ = new QWidget;
    sidebarBox_->setStyleSheet(R"(
        background: #232323;
        border-radius: 24px;
        padding: 28px 12px;
        min-width: 220px;
        max-width: 240px;
    )");
    auto* sidebarLayout = new QVBoxLayout(sidebarBox_);
    sidebarLayout->setContentsMargins(0,0,0,0);
    sidebarLayout->setSpacing(22);

    auto* filterLabel = new QLabel("Filtros e Pesquisa");
    filterLabel->setStyleSheet("font-weight:600;font-size:17px;color:#fff;");

    search_ = new QLineEdit();
    search_->setPlaceholderText("Pesquisar...");
    search_->setStyleSheet(R"(
        border-radius: 12px;
        background: #222;
        color: #fff;
        font-size:16px;
        padding: 8px 16px;
        border: none;
    )");

    filter_ = new QComboBox();
    filter_->addItems({"Estoque", "Categoria", "Preço"});
    filter_->setStyleSheet(R"(
        border-radius:10px;
        background:#222;
        color:#fff;
        font-size:15px;
        padding:6px;
        border:none;
    )");

    sidebarLayout->addWidget(filterLabel);
    sidebarLayout->addWidget(search_);
    sidebarLayout->addWidget(filter_);

    sidebarLayout->addStretch(1);

    // --- User info label (arredondada, destacada no topo da área de produtos)
    userInfoLabel_ = new QWidget;
    userInfoLabel_->setStyleSheet(R"(
        background: #222;
        border-radius: 18px;
        padding: 16px 32px;
        margin-bottom:18px;
        box-shadow: 0px 0px 8px #0001;
    )");
    auto* userInfoLayout = new QHBoxLayout(userInfoLabel_);
    userInfoLayout->setSpacing(0);
    userInfoLayout->setContentsMargins(0,0,0,0);

    QLabel* userLabel = new QLabel("Utilizador: <b>Guts</b>");
    userLabel->setStyleSheet("color: #58abfa; font-size: 17px; font-weight: 600; letter-spacing: 0.5px;");
    userInfoLayout->addWidget(userLabel, 0, Qt::AlignLeft);

    userInfoLayout->addStretch(1);

    QLabel* configsLabel = new QLabel("⚙ <b>Configurações</b>");
    configsLabel->setStyleSheet("color:#58abfa;font-size:17px;font-weight:500; letter-spacing: 0.4px;");
    userInfoLayout->addWidget(configsLabel, 0, Qt::AlignRight);

    // --- Products area (arredondado e visual refinado)
    productsBg_ = new QWidget;
    productsBg_->setStyleSheet(R"(
        background: #242526;
        border-radius: 28px;
        padding: 36px 24px;
        margin-top:8px;
        box-shadow: 0px 4px 24px #0002;
    )");
    productsLayout_ = new QVBoxLayout(productsBg_);
    productsLayout_->setSpacing(18);
    productsLayout_->setContentsMargins(10,10,10,18);

    // Adiciona info usuário na área de produtos
    productsLayout_->addWidget(userInfoLabel_);

    // Cards/labels de produtos
    QStringList products = {"Produto A", "Produto B", "Produto C", "Produto D", "Produto E"};
    for(const auto& prod : products) {
        QWidget* itemCard = new QWidget;
        itemCard->setStyleSheet(R"(
            background: #232323;
            border-radius:18px;
            padding:22px 24px;
            margin-bottom:10px;
            box-shadow: 0 2px 9px #0001;
        )");
        auto* cardLayout = new QHBoxLayout(itemCard);
        cardLayout->setContentsMargins(0,0,0,0);
        cardLayout->setSpacing(22);

        QLabel* icon = new QLabel;
        icon->setFixedSize(44,44);
        icon->setStyleSheet("background:#111;border-radius:12px;");

        QLabel* name = new QLabel(QString("<b>%1</b>").arg(prod));
        name->setStyleSheet("color:#fff;font-size:17px;min-width:135px;");
        QLabel* cat = new QLabel("Categoria");
        cat->setStyleSheet("color:#ccc;font-size:15px;min-width:100px;");
        QLabel* id = new QLabel("ID123");
        id->setStyleSheet("color:#58abfa;min-width:57px;");
        QLabel* price = new QLabel("$00,00");
        price->setStyleSheet("color:#fff;min-width:75px;");
        QLabel* qty = new QLabel("Qtd: 0");
        qty->setStyleSheet("color:#fff;min-width:60px;");

        // Centraliza todos na vertical
        cardLayout->addWidget(icon, 0, Qt::AlignVCenter);
        cardLayout->addWidget(name, 0, Qt::AlignVCenter);
        cardLayout->addWidget(cat, 0, Qt::AlignVCenter);
        cardLayout->addWidget(id, 0, Qt::AlignVCenter);
        cardLayout->addWidget(price, 0, Qt::AlignVCenter);
        cardLayout->addWidget(qty, 0, Qt::AlignVCenter);
        cardLayout->addStretch();

        productsLayout_->addWidget(itemCard);
    }
    productsLayout_->addStretch();

    root->addWidget(sidebarBox_,0);
    root->addWidget(productsBg_,1);

    setCentralWidget(central);
    setWindowTitle("DevTools Manager");
    resize(1050,650);
}

void MainWindow::setupConnections() {}
