#include "mainwindow.h"
#include "sidebarwidget.h"
// ...existing code...
#include <QPushButton>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QWidget>
#include <QFrame>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    QWidget *central = new QWidget(this);

    // Layout principal (linha)
    QHBoxLayout *mainLayout = new QHBoxLayout(central);

    // Sidebar
    sidebar = new SidebarWidget(this);
    sidebar->setFixedWidth(280);  // largura semelhante ao design

    // Área direita principal
    QVBoxLayout *mainAreaLayout = new QVBoxLayout;

    // Top bar
    QHBoxLayout *topBarLayout = new QHBoxLayout;
    QLabel *logo = new QLabel("SANCAK");
    logo->setStyleSheet("font-weight: bold; font-size: 24px;");

    // Dashboard/Inventário
    QPushButton *dashboardBtn = new QPushButton("Dashboard");
    QPushButton *inventoryBtn = new QPushButton("Inventário");
    dashboardBtn->setCheckable(true);
    inventoryBtn->setCheckable(true);
    inventoryBtn->setChecked(true);
    dashboardBtn->setStyleSheet("background-color: #fff; border-radius: 16px; padding: 4px 24px;");
    inventoryBtn->setStyleSheet("background-color: #222; color: #fff; border-radius: 16px; padding: 4px 24px;");

    QWidget *menuBar = new QWidget;
    QHBoxLayout *menuBarLayout = new QHBoxLayout(menuBar);
    menuBarLayout->addWidget(dashboardBtn);
    menuBarLayout->addWidget(inventoryBtn);
    menuBarLayout->setSpacing(8);
    menuBarLayout->setContentsMargins(0,0,0,0);

    // Botões no topo direito
    QPushButton *settingsBtn = new QPushButton("⚙");
    QPushButton *accountBtn = new QPushButton("A");
    settingsBtn->setFixedSize(40, 40);
    accountBtn->setFixedSize(40, 40);
    settingsBtn->setStyleSheet("border-radius: 20px; background: #eee;");
    accountBtn->setStyleSheet("border-radius: 20px; background: #eee; font-weight: bold;");
    topBarLayout->addWidget(logo);
    topBarLayout->addStretch();
    topBarLayout->addWidget(menuBar);
    topBarLayout->addStretch();
    topBarLayout->addWidget(settingsBtn);
    topBarLayout->addWidget(accountBtn);
    topBarLayout->setSpacing(12);

    // Search bar e botão de adicionar
    QHBoxLayout *searchRowLayout = new QHBoxLayout;
    searchBar = new QLineEdit;
    searchBar->setPlaceholderText("Pesquisar produto...");
    searchBar->setStyleSheet("border-radius: 8px; padding: 8px; border: 1px solid #ddd; background: #fff;");
    QPushButton *filterBtn = new QPushButton("🔍");
    filterBtn->setFixedSize(32,32);
    addProductButton = new QPushButton("Adicionar Produto");
    addProductButton->setStyleSheet("background-color: #222; color:#fff; border-radius: 8px; padding: 8px 24px;");
    searchRowLayout->addWidget(searchBar, 1);
    searchRowLayout->addWidget(filterBtn);
    searchRowLayout->addStretch();
    searchRowLayout->addWidget(addProductButton);

    // Lista dos itens
    // ...existing code...

    mainAreaLayout->addLayout(topBarLayout);
    mainAreaLayout->addSpacing(10);
    mainAreaLayout->addLayout(searchRowLayout);
    // ...existing code...

    mainLayout->addWidget(sidebar);
    mainLayout->addLayout(mainAreaLayout);

    central->setLayout(mainLayout);
    setCentralWidget(central);
    setStyleSheet("QMainWindow { background: #F5F5F5; }");
}
