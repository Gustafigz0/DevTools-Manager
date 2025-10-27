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

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setupUi();
    loadSampleProducts();
    displayProducts();
}

void MainWindow::setupUi() {
    auto* central = new QWidget(this);
    auto* root = new QHBoxLayout(central);
    root->setContentsMargins(16, 16, 16, 16);
    root->setSpacing(24);

    // ========== SIDEBAR ==========
    sidebarBox_ = new QWidget;
    sidebarBox_->setStyleSheet(R"(
        background: #232323;
        border-radius: 24px;
        min-width: 220px;
        max-width: 240px;
    )");
    
    auto* sidebarLayout = new QVBoxLayout(sidebarBox_);
    sidebarLayout->setContentsMargins(20, 30, 20, 30);
    sidebarLayout->setSpacing(12);
    
    // Logo/Título
    QLabel* logoLabel = new QLabel("<b>DevTools</b><br>Manager");
    logoLabel->setStyleSheet("color: #58abfa; font-size: 20px; font-weight: 700; border: none;");
    logoLabel->setAlignment(Qt::AlignCenter);
    sidebarLayout->addWidget(logoLabel);
    sidebarLayout->addSpacing(30);
    
    // Botão Produtos (ativo)
    QPushButton* btnProducts = new QPushButton("📦 Produtos");
    btnProducts->setStyleSheet(R"(
        QPushButton {
            background: #58abfa;
            color: #fff;
            border: none;
            border-radius: 12px;
            padding: 12px 16px;
            font-size: 15px;
            font-weight: 600;
            text-align: left;
        }
        QPushButton:hover {
            background: #4a9de8;
        }
    )");
    sidebarLayout->addWidget(btnProducts);
    
    // Botões inativos (futuro)
    QPushButton* btnSuppliers = new QPushButton("🏭 Fornecedores");
    btnSuppliers->setEnabled(false);
    btnSuppliers->setStyleSheet(R"(
        background: transparent;
        color: #888;
        border: none;
        border-radius: 12px;
        padding: 12px 16px;
        font-size: 15px;
        text-align: left;
    )");
    sidebarLayout->addWidget(btnSuppliers);
    
    QPushButton* btnCustomers = new QPushButton("👥 Clientes");
    btnCustomers->setEnabled(false);
    btnCustomers->setStyleSheet(btnSuppliers->styleSheet());
    sidebarLayout->addWidget(btnCustomers);
    
    QPushButton* btnReports = new QPushButton("📊 Relatórios");
    btnReports->setEnabled(false);
    btnReports->setStyleSheet(btnSuppliers->styleSheet());
    sidebarLayout->addWidget(btnReports);
    
    sidebarLayout->addStretch();

    // ========== ÁREA PRINCIPAL ==========
    productsBg_ = new QWidget;
    productsBg_->setStyleSheet(R"(
        background: #242526;
        border-radius: 28px;
        padding: 24px;
    )");
    
    productsLayout_ = new QVBoxLayout(productsBg_);
    productsLayout_->setSpacing(18);
    productsLayout_->setContentsMargins(24, 24, 24, 24);

    // ========== HEADER (Usuário + Botão Adicionar) ==========
    userInfoLabel_ = new QWidget;
    userInfoLabel_->setStyleSheet(R"(
        background: #222;
        border-radius: 18px;
        padding: 20px 28px;
    )");
    
    auto* headerLayout = new QHBoxLayout(userInfoLabel_);
    headerLayout->setContentsMargins(0, 0, 0, 0);
    
    QLabel* userLabel = new QLabel("👤 Utilizador: <b>Guts</b>");
    userLabel->setStyleSheet("color: #58abfa; font-size: 16px; font-weight: 600; background: transparent; border: none;");
    headerLayout->addWidget(userLabel);
    
    headerLayout->addStretch();
    
    // Botão Adicionar Produto
    btnAddProduct_ = new QPushButton("➕ Adicionar Produto");
    btnAddProduct_->setStyleSheet(R"(
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
    connect(btnAddProduct_, &QPushButton::clicked, this, &MainWindow::onAddProductClicked);
    headerLayout->addWidget(btnAddProduct_);
    
    productsLayout_->addWidget(userInfoLabel_);

    // ========== BARRA DE BUSCA ==========
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

    // ========== ÁREA DE CARDS (com scroll) ==========
    QScrollArea* scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setStyleSheet(R"(
        QScrollArea {
            background: transparent;
            border: none;
        }
        QScrollBar:vertical {
            background: #2d2d2d;
            width: 10px;
            border-radius: 5px;
        }
        QScrollBar::handle:vertical {
            background: #58abfa;
            border-radius: 5px;
        }
    )");
    
    productsContainer_ = new QWidget;
    productsContainer_->setStyleSheet("background: transparent;");
    productsCardsLayout_ = new QVBoxLayout(productsContainer_);
    productsCardsLayout_->setSpacing(12);
    productsCardsLayout_->setContentsMargins(0, 0, 0, 0);
    
    scrollArea->setWidget(productsContainer_);
    productsLayout_->addWidget(scrollArea, 1);

    // ========== ADICIONA TUDO ==========
    root->addWidget(sidebarBox_, 0);
    root->addWidget(productsBg_, 1);

    setCentralWidget(central);
    setWindowTitle("DevTools Manager");
    resize(1100, 700);
}

void MainWindow::loadSampleProducts() {
    // Carrega produtos de exemplo
    allProducts_.append(Product("001", "Mouse Gamer RGB", "Periféricos", 149.90, 25));
    allProducts_.append(Product("002", "Teclado Mecânico", "Periféricos", 299.90, 15));
    allProducts_.append(Product("003", "Monitor 27\" 144Hz", "Monitores", 1299.90, 8));
    allProducts_.append(Product("004", "Headset Wireless", "Áudio", 399.90, 12));
    allProducts_.append(Product("005", "Webcam Full HD", "Câmeras", 249.90, 20));
    allProducts_.append(Product("006", "SSD 1TB NVMe", "Armazenamento", 499.90, 30));
    allProducts_.append(Product("007", "Mousepad Grande", "Acessórios", 79.90, 50));
    
    filteredProducts_ = allProducts_;
}

void MainWindow::displayProducts() {
    // Limpa cards anteriores
    QLayoutItem* item;
    while ((item = productsCardsLayout_->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    
    // Se não houver produtos
    if (filteredProducts_.isEmpty()) {
        QLabel* emptyLabel = new QLabel("🔍 Nenhum produto encontrado");
        emptyLabel->setStyleSheet("color: #888; font-size: 16px; padding: 40px;");
        emptyLabel->setAlignment(Qt::AlignCenter);
        productsCardsLayout_->addWidget(emptyLabel);
        productsCardsLayout_->addStretch();
        return;
    }
    
    // Cria card para cada produto
    for (const Product& product : filteredProducts_) {
        createProductCard(product);
    }
    
    productsCardsLayout_->addStretch();
}

void MainWindow::createProductCard(const Product& product) {
    QWidget* card = new QWidget;
    card->setStyleSheet(R"(
        QWidget {
            background: #232323;
            border-radius: 16px;
            padding: 18px 22px;
        }
        QWidget:hover {
            background: #2a2a2a;
        }
    )");
    
    auto* cardLayout = new QHBoxLayout(card);
    cardLayout->setContentsMargins(10, 10, 10, 10);
    cardLayout->setSpacing(16);
    
    // Ícone
    QLabel* icon = new QLabel("📦");
    icon->setFixedSize(50, 50);
    icon->setStyleSheet(R"(
        background: #2d2d2d;
        border-radius: 12px;
        font-size: 28px;
        padding: 0px;
    )");
    icon->setAlignment(Qt::AlignCenter);
    cardLayout->addWidget(icon);
    
    // Nome
    QLabel* nameLabel = new QLabel(QString("<b>%1</b>").arg(product.getName()));
    nameLabel->setStyleSheet("color: #fff; font-size: 16px; background: transparent; border: none;");
    nameLabel->setMinimumWidth(200);
    cardLayout->addWidget(nameLabel);
    
    // Categoria
    QLabel* categoryLabel = new QLabel(product.getCategory());
    categoryLabel->setStyleSheet("color: #aaa; font-size: 14px; background: transparent;");
    categoryLabel->setMinimumWidth(120);
    cardLayout->addWidget(categoryLabel);
    
    // ID
    QLabel* idLabel = new QLabel(QString("ID: %1").arg(product.getId()));
    idLabel->setStyleSheet("color: #58abfa; font-size: 13px; background: transparent;");
    idLabel->setMinimumWidth(80);
    cardLayout->addWidget(idLabel);
    
    // Preço
    QLabel* priceLabel = new QLabel(product.getPriceFormatted());
    priceLabel->setStyleSheet("color: #2ecc71; font-size: 16px; font-weight: bold; background: transparent;");
    priceLabel->setMinimumWidth(100);
    cardLayout->addWidget(priceLabel);
    
    // Quantidade
    QLabel* qtyLabel = new QLabel(product.getQuantityText());
    qtyLabel->setStyleSheet("color: #fff; font-size: 14px; background: transparent;");
    qtyLabel->setMinimumWidth(90);
    cardLayout->addWidget(qtyLabel);
    
    // Status estoque
    QLabel* stockStatus = new QLabel(product.isInStock() ? "✓ Em estoque" : "✗ Sem estoque");
    stockStatus->setStyleSheet(product.isInStock() ? 
        "color: #2ecc71; font-size: 13px; background: transparent;" : 
        "color: #e74c3c; font-size: 13px; background: transparent;");
    stockStatus->setMinimumWidth(100);
    cardLayout->addWidget(stockStatus);
    
    cardLayout->addStretch();
    
    // ========== BOTÕES DE AÇÃO (NOVO) ==========
    
    // Botão Editar
    QPushButton* btnEdit = new QPushButton("✏️");
    btnEdit->setFixedSize(40, 40);
    btnEdit->setToolTip("Editar produto");
    btnEdit->setStyleSheet(R"(
        QPushButton {
            background: #3498db;
            color: #fff;
            border: none;
            border-radius: 8px;
            font-size: 18px;
        }
        QPushButton:hover {
            background: #2980b9;
        }
        QPushButton:pressed {
            background: #1f6ba6;
        }
    )");
    connect(btnEdit, &QPushButton::clicked, this, [this, product]() {
        onEditProduct(product.getId());
    });
    cardLayout->addWidget(btnEdit);
    
    // Botão Deletar
    QPushButton* btnDelete = new QPushButton("🗑️");
    btnDelete->setFixedSize(40, 40);
    btnDelete->setToolTip("Deletar produto");
    btnDelete->setStyleSheet(R"(
        QPushButton {
            background: #e74c3c;
            color: #fff;
            border: none;
            border-radius: 8px;
            font-size: 18px;
        }
        QPushButton:hover {
            background: #c0392b;
        }
        QPushButton:pressed {
            background: #a93226;
        }
    )");
    connect(btnDelete, &QPushButton::clicked, this, [this, product]() {
        onDeleteProduct(product.getId());
    });
    cardLayout->addWidget(btnDelete);
    
    productsCardsLayout_->addWidget(card);
}

void MainWindow::onAddProductClicked() {
    AddProductDialog dialog(this);
    
    if (dialog.exec() == QDialog::Accepted) {
        Product newProduct = dialog.getProduct();
        
        // Verifica se o ID já existe
        for (const Product& p : allProducts_) {
            if (p.getId() == newProduct.getId()) {
                QMessageBox::warning(this, "Erro", 
                    QString("Já existe um produto com o ID '%1'!").arg(newProduct.getId()));
                return;
            }
        }
        
        // Adiciona o produto
        allProducts_.append(newProduct);
        filteredProducts_.append(newProduct);
        
        // Atualiza a exibição
        displayProducts();
        
        QMessageBox::information(this, "Sucesso", 
            QString("Produto '%1' adicionado com sucesso!").arg(newProduct.getName()));
    }
}


void MainWindow::onSearchTextChanged(const QString& text) {
    // Filtra produtos baseado no texto de busca
    filteredProducts_.clear();
    QString searchText = text.toLower();
    
    for (const Product& product : allProducts_) {
        if (product.getName().toLower().contains(searchText) ||
            product.getCategory().toLower().contains(searchText) ||
            product.getId().toLower().contains(searchText)) {
            filteredProducts_.append(product);
        }
    }
    // Atualiza a exibição com os resultados filtrados
    displayProducts();
}

Product* MainWindow::findProductById(const QString& id) {
    for (int i = 0; i < allProducts_.size(); ++i) {
        if (allProducts_[i].getId() == id) {
            return &allProducts_[i];
        }
    }
    return nullptr;
}

void MainWindow::onEditProduct(const QString& productId) {
    Product* product = findProductById(productId);
    if (!product) {
        QMessageBox::warning(this, "Erro", "Produto não encontrado!");
        return;
    }
    
    AddProductDialog dialog(*product, this);
    
    if (dialog.exec() == QDialog::Accepted) {
        Product updatedProduct = dialog.getProduct();
        
        // Atualiza o produto
        *product = updatedProduct;
        
        // Atualiza filteredProducts_ também
        for (int i = 0; i < filteredProducts_.size(); ++i) {
            if (filteredProducts_[i].getId() == productId) {
                filteredProducts_[i] = updatedProduct;
                break;
            }
        }
        
        // Atualiza a exibição
        displayProducts();
        
        QMessageBox::information(this, "Sucesso", 
            QString("Produto '%1' atualizado com sucesso!").arg(updatedProduct.getName()));
    }
}

void MainWindow::onDeleteProduct(const QString& productId) {
    Product* product = findProductById(productId);
    if (!product) {
        QMessageBox::warning(this, "Erro", "Produto não encontrado!");
        return;
    }
    
    // Confirmação
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Confirmar Exclusão",
        QString("Tem certeza que deseja deletar o produto '%1'?\n\nEsta ação não pode ser desfeita!")
            .arg(product->getName()),
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No
    );
    
    if (reply == QMessageBox::Yes) {
        QString productName = product->getName();
        
        // Remove de allProducts_
        for (int i = 0; i < allProducts_.size(); ++i) {
            if (allProducts_[i].getId() == productId) {
                allProducts_.removeAt(i);
                break;
            }
        }
        
        // Remove de filteredProducts_
        for (int i = 0; i < filteredProducts_.size(); ++i) {
            if (filteredProducts_[i].getId() == productId) {
                filteredProducts_.removeAt(i);
                break;
            }
        }
        
        // Atualiza a exibição
        displayProducts();
        
        QMessageBox::information(this, "Sucesso", 
            QString("Produto '%1' deletado com sucesso!").arg(productName));
    }
}

