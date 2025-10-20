
#include "mainwindow.h"
#include "sidebarwidget.h"
#include "productlistwidget.h"
#include <QPushButton>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QMessageBox>
#include "animationhelper.h"
#include "productdialog.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    QWidget *central = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout;
    sidebar = new SidebarWidget(this);
    productList = new ProductListWidget(this);
    addProductButton = new QPushButton("Adicionar Produto");
    searchBar = new QLineEdit;
    searchBar->setPlaceholderText("Pesquisar produto...");

    // Adicionar animação de fade in ao iniciar
    AnimationHelper::fadeIn(productList, 300);

    QVBoxLayout *rightSide = new QVBoxLayout;
    QHBoxLayout *headerLayout = new QHBoxLayout;
    headerLayout->addWidget(searchBar);
    headerLayout->addWidget(addProductButton);
    rightSide->addLayout(headerLayout);
    rightSide->addWidget(productList);

    // Conectar o botão com animação e dialog
    connect(addProductButton, &QPushButton::clicked, [this]() {
        AnimationHelper::pulse(addProductButton, 1.05, 150);
        
        ProductDialog dialog(this);
        if (dialog.exec() == QDialog::Accepted) {
            Product newProduct = dialog.getProduct();
            productList->addProduct(newProduct);
            QMessageBox::information(this, "Sucesso", "Produto adicionado com sucesso!");
        }
    });

    mainLayout->addWidget(sidebar);
    mainLayout->addLayout(rightSide);
    central->setLayout(mainLayout);
    setCentralWidget(central);
    connect(addProductButton, &QPushButton::clicked, this, &MainWindow::showAddDialog);
}

void MainWindow::showAddDialog() {
    // Implemente a lógica do diálogo de adicionar produto aqui
}
