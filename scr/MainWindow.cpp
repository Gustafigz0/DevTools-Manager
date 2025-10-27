#include "MainWindow.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QScrollArea>
#include <QMessageBox>
#include "AddProductDialog.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setupUi();
    loadSampleProducts();
    displayProducts();
}

void MainWindow::setupUi()
{
    auto* central = new QWidget(this);
    auto* root = new QHBoxLayout(central);
    root->setContentsMargins(16, 16, 16, 16);
    root->setSpacing(24);

    // SIDEBAR
    sidebarBox_ = new QWidget;
    sidebarBox_->setStyleSheet(R"(
        background: #232323;
        border-radius: 24px;
        min-width: 220px;
        max-width: 240px;
    )");
    sidebarLayout_ = new QVBoxLayout(sidebarBox_);
    sidebarLayout_->setContentsMargins(20, 26, 20, 30);
    sidebarLayout_->setSpacing(14);

    QLabel* logoLabel = new QLabel("<b>DevTools</b>\n Manager");
    logoLabel->setStyleSheet("color: #58abfa; font-size: 20px; font-weight: 700; border: none;");
    logoLabel->setAlignment(Qt::AlignCenter);
    sidebarLayout_->addWidget(logoLabel);
    sidebarLayout_->addSpacing(28);

    // Produtos (sem botão de seleção global)
    btnProducts_ = new QPushButton("📦 Produtos");
    btnProducts_->setStyleSheet(R"(
        QPushButton {
            background: #58abfa;
            color: #fff;
            border: none;
            border-radius: 12px;
            padding: 12px 16px;
            font-size: 15px;
            font-weight: 600;
            margin-bottom: 3px;
            text-align: left;
            letter-spacing: 1px;
        }
        QPushButton:hover, QPushButton:checked {
            background: #4a9de8;
        }
    )");
    btnProducts_->setCheckable(true);
    btnProducts_->setChecked(true);
    sidebarLayout_->addWidget(btnProducts_);

    btnDeleteSelected_ = new QPushButton("🗑️ Apagar Selecionados");
    btnDeleteSelected_->setEnabled(false);
    btnDeleteSelected_->setStyleSheet(R"(
        QPushButton {
            background: #e74c3c;
            color: #fff;
            border: none;
            border-radius: 12px;
            padding: 12px 16px;
            font-size: 15px;
            font-weight: 600;
            margin-bottom: 15px;
            text-align: left;
        }
        QPushButton:disabled {
            background: #373737;
            color: #aaa;
        }
        QPushButton:hover {
            background: #c0392b;
        }
    )");
    connect(btnDeleteSelected_, &QPushButton::clicked, this, &MainWindow::onDeleteSelectedProductsClicked);
    sidebarLayout_->addWidget(btnDeleteSelected_);
    sidebarLayout_->addSpacing(25);
    sidebarLayout_->addStretch(1);

    // MAIN AREA
    productsBg_ = new QWidget;
    productsBg_->setStyleSheet(R"(
        background: #242526;
        border-radius: 28px;
        padding: 24px;
    )");
    productsLayout_ = new QVBoxLayout(productsBg_);
    productsLayout_->setSpacing(18);
    productsLayout_->setContentsMargins(24, 24, 24, 24);

    // Header com usuário e add
    QWidget* userInfoLabel = new QWidget;
    userInfoLabel->setStyleSheet(R"(
        background: #222;
        border-radius: 18px;
        padding: 20px 28px;
    )");
    auto* headerLayout = new QHBoxLayout(userInfoLabel);
    headerLayout->setContentsMargins(0,0,0,0);

    QLabel* userLabel = new QLabel("👤 Utilizador: <b>Guts</b>");
    userLabel->setStyleSheet("color: #58abfa; font-size: 16px; font-weight: 600; background: transparent; border: none;");
    headerLayout->addWidget(userLabel);
    headerLayout->addStretch();

    QPushButton* btnAddProduct = new QPushButton("➕ Adicionar Produto");
    btnAddProduct->setStyleSheet(R"(
        QPushButton {
            background: #58abfa;
            color: #fff;
            border: none;
            border-radius: 12px;
            padding: 10px 20px;
            font-size: 14px;
            font-weight: 600;
        }
        QPushButton:hover {
            background: #4a9de8;
        }
    )");
    connect(btnAddProduct, &QPushButton::clicked, this, &MainWindow::onAddProductClicked);
    headerLayout->addWidget(btnAddProduct);

    productsLayout_->addWidget(userInfoLabel);

    // Busca
    searchBar_ = new QLineEdit;
    searchBar_->setPlaceholderText("🔍 Buscar produtos...");
    searchBar_->setStyleSheet(R"(
        QLineEdit {
            background: #2d2d2d;
            color: #fff;
            border: 2px solid #3a3a3a;
            border-radius: 14px;
            padding: 12px 18px;
            font-size: 15px;
        }
        QLineEdit:focus {
            border: 2px solid #58abfa;
        }
    )");
    connect(searchBar_, &QLineEdit::textChanged, this, &MainWindow::onSearchTextChanged);
    productsLayout_->addWidget(searchBar_);

    // Cards com scroll
    QScrollArea* scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setStyleSheet(R"(
        QScrollArea {
            background: transparent;
            border: none;
        }
    )");
    productsContainer_ = new QWidget;
    productsCardsLayout_ = new QVBoxLayout(productsContainer_);
    productsCardsLayout_->setSpacing(12);
    productsCardsLayout_->setContentsMargins(0,0,0,0);
    scrollArea->setWidget(productsContainer_);
    productsLayout_->addWidget(scrollArea, 1);

    root->addWidget(sidebarBox_, 0);
    root->addWidget(productsBg_, 1);
    setCentralWidget(central);
    setWindowTitle("DevTools Manager");
    resize(1100, 700);
}

void MainWindow::loadSampleProducts()
{
    allProducts_.append(Product("001", "Mouse Gamer RGB", "Periféricos", 49.90, 25));
    allProducts_.append(Product("002", "Teclado Mecânico", "Periféricos", 99.90, 15));
    allProducts_.append(Product("003", "Monitor 27\" 144Hz", "Monitores", 299.90, 8));
    allProducts_.append(Product("004", "Headset Wireless", "Áudio", 99.90, 12));
    allProducts_.append(Product("005", "Webcam Full HD", "Câmeras", 49.90, 20));
    allProducts_.append(Product("006", "SSD 1TB NVMe", "Armazenamento", 99.90, 30));
    allProducts_.append(Product("007", "Mousepad Grande", "Acessórios", 49.90, 50));
    filteredProducts_ = allProducts_;
}

void MainWindow::displayProducts()
{
    QLayoutItem* item;
    while ((item = productsCardsLayout_->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    if (filteredProducts_.isEmpty()) {
        QLabel* emptyLabel = new QLabel("🔍 Nenhum produto encontrado");
        emptyLabel->setStyleSheet("color: #888; font-size: 16px; padding: 40px;");
        emptyLabel->setAlignment(Qt::AlignCenter);
        productsCardsLayout_->addWidget(emptyLabel);
        productsCardsLayout_->addStretch();
        return;
    }
    for (const Product& product : filteredProducts_)
        createProductCard(product);
    productsCardsLayout_->addStretch();
}

// Novo botão minimalista para selecionar produto!
void MainWindow::createProductCard(const Product& product)
{
    QWidget* card = new QWidget;
    card->setStyleSheet(R"(
        QWidget {
            background: #232323;
            border-radius: 16px;
            margin-bottom: 6px;
            padding: 0px 0px;
        }
        QWidget:hover {
            background: #293043;
        }
    )");
    auto* cardLayout = new QHBoxLayout(card);
    cardLayout->setContentsMargins(24, 14, 24, 14);
    cardLayout->setSpacing(18);

    // Botão minimalista de seleção
    QPushButton* selectButton = new QPushButton;
    selectButton->setCheckable(true);
    selectButton->setChecked(selectedProductIds_.contains(product.getId()));
    selectButton->setStyleSheet(R"(
        QPushButton {
            background: #232323;
            border: 2px solid #58abfa;
            border-radius: 10px;
            min-width: 20px; min-height: 20px;
            max-width: 20px; max-height: 20px;
            padding: 0px;
        }
        QPushButton:checked {
            background: #58abfa;
            border: 2px solid #58abfa;
        }
        QPushButton:checked:hover {
            background: #58abfa;
            border: 2px solid #aee4ff;
        }
        QPushButton:hover:!checked {
            border-color: #88d1fc;
            background: #282e39;
        }
    )");

    cardLayout->addWidget(selectButton, 0, Qt::AlignVCenter);
    connect(selectButton, &QPushButton::toggled, this, [this, product](bool checked) {
        onSelectProductToggled(product.getId(), checked);
    });


    QLabel* icon = new QLabel("📦");
    icon->setStyleSheet(R"(
        background: #2d2d2d;
        border-radius: 12px;
        font-size: 28px;
        min-width: 48px; min-height: 48px; max-width: 48px; max-height: 48px;
        margin-left: 0px; margin-right: 8px; padding: 0px;
    )");
    icon->setAlignment(Qt::AlignCenter);
    cardLayout->addWidget(icon, 0, Qt::AlignVCenter);

    QLabel* nameLabel = new QLabel(QString("<b>%1</b>").arg(product.getName()));
    nameLabel->setStyleSheet("color: #fff; font-size: 17px; min-width:160px; background: transparent; border: none; margin-right:12px;");
    cardLayout->addWidget(nameLabel, 1, Qt::AlignVCenter);

    QLabel* categoryLabel = new QLabel(product.getCategory());
    categoryLabel->setStyleSheet("color: #aaa; font-size: 14px; background: transparent; min-width:110px;");
    cardLayout->addWidget(categoryLabel, 0, Qt::AlignVCenter);

    QLabel* idLabel = new QLabel(QString("ID: %1").arg(product.getId()));
    idLabel->setStyleSheet("color: #58abfa; font-size: 13px; background: transparent; min-width:70px;");
    cardLayout->addWidget(idLabel, 0, Qt::AlignVCenter);

    QLabel* priceLabel = new QLabel(product.getPriceFormatted());
    priceLabel->setStyleSheet("color: #2ecc71; font-size: 16px; font-weight: bold; background: transparent; min-width:95px;");
    cardLayout->addWidget(priceLabel, 0, Qt::AlignVCenter);

    QLabel* qtyLabel = new QLabel(product.getQuantityText());
    qtyLabel->setStyleSheet("color: #fff; font-size: 14px; background: transparent; min-width:80px;");
    cardLayout->addWidget(qtyLabel, 0, Qt::AlignVCenter);

    QLabel* stockStatus = new QLabel(product.isInStock() ? "✓ Em estoque" : "✗ Sem estoque");
    stockStatus->setStyleSheet(product.isInStock() ? "color: #2ecc71; font-size: 13px; background: transparent; min-width:108px;"
                                                  : "color: #e74c3c; font-size: 13px; background: transparent; min-width:108px;");
    cardLayout->addWidget(stockStatus, 0, Qt::AlignVCenter);

    cardLayout->addStretch(1);
    productsCardsLayout_->addWidget(card);
}

void MainWindow::onSelectProductToggled(const QString& productId, bool checked)
{
    if (checked)
        selectedProductIds_.insert(productId);
    else
        selectedProductIds_.remove(productId);
    updateDeleteSelectedButtonState();
}

void MainWindow::updateDeleteSelectedButtonState()
{
    btnDeleteSelected_->setEnabled(!selectedProductIds_.isEmpty());
}

void MainWindow::onDeleteSelectedProductsClicked()
{
    if (selectedProductIds_.isEmpty())
        return;
    int ret = QMessageBox::question(this, "Confirmar Exclusão",
        QString("Tem certeza que deseja apagar %1 produtos selecionados?").arg(selectedProductIds_.size()),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (ret != QMessageBox::Yes)
        return;
    for (const QString& id : selectedProductIds_) {
        for (int i = 0; i < allProducts_.size(); ++i) {
            if (allProducts_[i].getId() == id) {
                allProducts_.removeAt(i);
                break;
            }
        }
        for (int i = 0; i < filteredProducts_.size(); ++i) {
            if (filteredProducts_[i].getId() == id) {
                filteredProducts_.removeAt(i);
                break;
            }
        }
    }
    selectedProductIds_.clear();
    updateDeleteSelectedButtonState();
    displayProducts();
    QMessageBox::information(this, "Sucesso", "Produtos selecionados apagados!");
}

void MainWindow::onAddProductClicked()
{
    // Coleta todas as categorias únicas dos produtos já cadastrados
    QStringList categorias;
    for (const Product& p : allProducts_)
        categorias << p.getCategory();
    categorias.removeDuplicates();

    // Cria o dialog passando a lista de categorias únicas
    AddProductDialog dialog(categorias, this);

    // Resto do seu código (não muda)
    if (dialog.exec() == QDialog::Accepted) {
        Product newProduct = dialog.getProduct();
        for (const Product& p : allProducts_) {
            if (p.getId() == newProduct.getId()) {
                QMessageBox::warning(this, "Erro",
                    QString("Já existe um produto com o ID '%1'!").arg(newProduct.getId()));
                return;
            }
        }
        allProducts_.append(newProduct);
        filteredProducts_.append(newProduct);
        displayProducts();
        QMessageBox::information(this, "Sucesso",
            QString("Produto '%1' adicionado com sucesso!").arg(newProduct.getName()));
    }
}


void MainWindow::onSearchTextChanged(const QString& text)
{
    filteredProducts_.clear();
    QString searchText = text.toLower();
    for (const Product& product : allProducts_) {
        if (product.getName().toLower().contains(searchText) ||
            product.getCategory().toLower().contains(searchText) ||
            product.getId().toLower().contains(searchText)) {
            filteredProducts_.append(product);
        }
    }
    displayProducts();
}
