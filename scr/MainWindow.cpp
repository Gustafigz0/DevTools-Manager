#include "MainWindow.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setupUi();
}

void MainWindow::setupUi() {
    auto* central = new QWidget(this);
    auto* root = new QHBoxLayout(central);
    root->setContentsMargins(16,16,16,16);
    root->setSpacing(24);

    // Label lateral de fundo
    sidebarBox_ = new QWidget;
    sidebarBox_->setStyleSheet(R"(
        background: #232323;
        border-radius: 24px;
        min-width: 220px;
        max-width: 240px;
    )");

    // Label topo área usuário/configs
    userInfoLabel_ = new QWidget;
    userInfoLabel_->setStyleSheet(R"(
        background: #222;
        border-radius: 18px;
        padding: 24px 32px;
        margin-bottom:18px;
        box-shadow: 0px 0px 8px #0001;
    )");
    auto* userLayout = new QHBoxLayout(userInfoLabel_);
    QLabel* userLabel = new QLabel("Usuário: Admin");
    userLabel->setStyleSheet("color: #5a6269ff; font-size: 17px; font-weight: 600;");
    userLayout->addWidget(userLabel);

    // Label de fundo dos produtos
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

    // Adiciona área topo info usuário
    productsLayout_->addWidget(userInfoLabel_);

    // Adiciona apenas UM produto/label
    QWidget* itemCard = new QWidget;
    itemCard->setStyleSheet(R"(
        background: #232323;
        border-radius:18px;
        padding:22px 24px;
        margin-bottom:10px;
        box-shadow: 0 2px 9px #0001;
    )");
    auto* cardLayout = new QHBoxLayout(itemCard);
    QLabel* icon = new QLabel;
    icon->setFixedSize(44,44);
    icon->setStyleSheet("background:#111;border-radius:12px;");
    QLabel* name = new QLabel("<b>Produto Único</b>");
    name->setStyleSheet("color:#fff;font-size:17px;");
    QLabel* cat = new QLabel("Categoria");
    cat->setStyleSheet("color:#ccc;font-size:15px;");
    QLabel* id = new QLabel("ID123");
    id->setStyleSheet("color:#58abfa;");
    QLabel* price = new QLabel("$00,00");
    price->setStyleSheet("color:#fff;");
    QLabel* qty = new QLabel("Qtd: 0");
    qty->setStyleSheet("color:#fff;");
    cardLayout->addWidget(icon);
    cardLayout->addWidget(name);
    cardLayout->addWidget(cat);
    cardLayout->addWidget(id);
    cardLayout->addWidget(price);
    cardLayout->addWidget(qty);
    cardLayout->addStretch();
    productsLayout_->addWidget(itemCard);

    productsLayout_->addStretch();

    // Adiciona as labels de fundo (sidebar + produtos)
    root->addWidget(sidebarBox_,0);
    root->addWidget(productsBg_,1);

    setCentralWidget(central);
    setWindowTitle("DevTools Manager");
    resize(1050,650);
}
