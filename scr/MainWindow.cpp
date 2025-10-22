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

    sidebarBox_ = new QWidget;
    sidebarBox_->setStyleSheet(R"(
        background: #232323;
        border-radius: 24px;
        min-width: 220px;
        max-width: 240px;
        margin-top: 8px;
        padding: 36px 24px;
    )");

    productsBg_ = new QWidget;
    productsBg_->setStyleSheet(R"(
        background: #242526;
        border-radius: 28px;
        padding: 36px 24px;
        margin-top: 8px;
        box-shadow: 0px 4px 24px #990c0c22;
    )");
    productsLayout_ = new QVBoxLayout(productsBg_);
    productsLayout_->setSpacing(18);
    productsLayout_->setContentsMargins(10,10,10,18);

    // Adiciona as labels de fundo (sidebar + produtos)
    root->addWidget(sidebarBox_,0);
    root->addWidget(productsBg_,1);

    setCentralWidget(central);
    setWindowTitle("DevTools Manager");
    resize(1050,650);
}
