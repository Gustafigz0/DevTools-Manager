
#include "mainwindow.h"
#include "sidebarwidget.h"
#include "productlistwidget.h"
#include <QPushButton>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    QWidget *central = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout;
    sidebar = new SidebarWidget(this);
    productList = new ProductListWidget(this);
    addProductButton = new QPushButton("Adicionar Produto");
    searchBar = new QLineEdit;
    searchBar->setPlaceholderText("Pesquisar produto...");

    QVBoxLayout *rightSide = new QVBoxLayout;
    QHBoxLayout *headerLayout = new QHBoxLayout;
    headerLayout->addWidget(searchBar);
    headerLayout->addWidget(addProductButton);
    rightSide->addLayout(headerLayout);
    rightSide->addWidget(productList);

    mainLayout->addWidget(sidebar);
    mainLayout->addLayout(rightSide);
    central->setLayout(mainLayout);
    setCentralWidget(central);
    connect(addProductButton, &QPushButton::clicked, this, &MainWindow::showAddDialog);
}

void MainWindow::showAddDialog() {
    // Implemente a lógica do diálogo de adicionar produto aqui
}
