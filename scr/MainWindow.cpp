#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QScrollArea>
#include <QMessageBox>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QTimer>
#include <QDebug>
#include "AddProductDialog.h"
#include "ToastNotification.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    qDebug() << "MainWindow início";
    setupUi();
    qDebug() << "MainWindow: depois de setupUi";
    loadSampleProducts();
    qDebug() << "MainWindow: depois de loadSampleProducts";
    qDebug() << "ANTES filterAndSortProducts";
    filterAndSortProducts();
    qDebug() << "DEPOIS filterAndSortProducts";
    displayProducts();
    qDebug() << "MainWindow fim/OK";
}

void MainWindow::setupUi()
{
    qDebug() << "setupUi: widgets-raiz";
    auto* central = new QWidget(this);
    auto* root = new QHBoxLayout(central);
    root->setContentsMargins(16, 16, 16, 16);
    root->setSpacing(24);

    qDebug() << "setupUi: sidebarBox/layout";
    sidebarBox_ = new QWidget;
    sidebarBox_->setStyleSheet("background: #232323; border-radius: 24px; min-width: 220px; max-width: 240px;");
    sidebarLayout_ = new QVBoxLayout(sidebarBox_);
    sidebarLayout_->setContentsMargins(20, 26, 20, 30);
    sidebarLayout_->setSpacing(14);

    qDebug() << "setupUi: logo";
    QLabel* logoLabel = new QLabel("<b>DevTools</b>\n Manager");
    logoLabel->setStyleSheet("color: #58abfa; font-size: 20px; font-weight: 700; border: none;");
    logoLabel->setAlignment(Qt::AlignCenter);
    sidebarLayout_->addWidget(logoLabel);
    sidebarLayout_->addSpacing(28);

    qDebug() << "setupUi: btnProducts";
    btnProducts_ = new QPushButton("📦 Produtos");
    btnProducts_->setCheckable(true);
    btnProducts_->setChecked(true);
    btnProducts_->setStyleSheet(
        "QPushButton { background: #58abfa; color: #fff; border: none; border-radius: 12px; padding: 12px 16px; font-size: 15px; font-weight: 600; margin-bottom: 3px; text-align: left; letter-spacing: 1px; }"
        "QPushButton:checked { background: #2492fa; }"
        "QPushButton:hover { background: #4a9de8; }"
    );
    sidebarLayout_->addWidget(btnProducts_);

    qDebug() << "setupUi: btnShowFavorites";
    btnShowFavorites_ = new QPushButton("⭐ Favoritos");
    btnShowFavorites_->setCheckable(true);
    btnShowFavorites_->setChecked(false);
    btnShowFavorites_->setStyleSheet(
        "QPushButton { background: #293043; color: #ffec8b; border: none; border-radius: 12px; padding: 12px 16px; font-size: 15px; font-weight: 600; margin-bottom: 6px; text-align: left; }"
        "QPushButton:checked { background: #ffe066; color: #232323; }"
        "QPushButton:hover { background: #3b3f53; }"
    );
    sidebarLayout_->addWidget(btnShowFavorites_);

    // Troca: conectando para slots exclusivos (não aos filtros diretos)
    connect(btnProducts_, &QPushButton::clicked, this, &MainWindow::onBtnProducts);
    connect(btnShowFavorites_, &QPushButton::clicked, this, &MainWindow::onBtnFavorites);

    qDebug() << "setupUi: btnDeleteSelected";
    btnDeleteSelected_ = new QPushButton("🗑️ Apagar Selecionados");
    btnDeleteSelected_->setEnabled(false);
    btnDeleteSelected_->setStyleSheet("QPushButton {background: #e74c3c;color: #fff;border: none;border-radius: 12px;padding: 12px 16px;font-size: 15px;font-weight: 600;margin-bottom: 15px;text-align: left;}QPushButton:disabled {background: #373737; color: #aaa;} QPushButton:hover {background: #c0392b;}");
    connect(btnDeleteSelected_, &QPushButton::clicked, this, &MainWindow::onDeleteSelectedProductsClicked);
    sidebarLayout_->addWidget(btnDeleteSelected_);
    sidebarLayout_->addSpacing(25);

    qDebug() << "setupUi: btnExport / btnImport";
    btnExport_ = new QPushButton("⬆ Exportar Catálogo");
    btnExport_->setToolTip("Exporta o catálogo para um arquivo JSON");
    connect(btnExport_, &QPushButton::clicked, this, &MainWindow::onExportProductsClicked);
    sidebarLayout_->addWidget(btnExport_);

    btnImport_ = new QPushButton("⬇ Importar Catálogo");
    btnImport_->setToolTip("Importa produtos de um arquivo JSON");
    connect(btnImport_, &QPushButton::clicked, this, &MainWindow::onImportProductsClicked);
    sidebarLayout_->addWidget(btnImport_);

    sidebarLayout_->addStretch(1);

    qDebug() << "setupUi: productsBg";
    productsBg_ = new QWidget;
    productsBg_->setStyleSheet("background: #242526; border-radius: 28px; padding: 24px;");
    productsLayout_ = new QVBoxLayout(productsBg_);
    productsLayout_->setSpacing(18);
    productsLayout_->setContentsMargins(24, 24, 24, 24);

    qDebug() << "setupUi: userInfoLabel/header";
    QWidget* userInfoLabel = new QWidget;
    userInfoLabel->setStyleSheet("background: #222; border-radius: 18px; padding: 20px 28px;");
    auto* headerLayout = new QHBoxLayout(userInfoLabel);
    headerLayout->setContentsMargins(0,0,0,0);

    QLabel* userLabel = new QLabel("👤 Utilizador: <b>Guts</b>");
    userLabel->setStyleSheet("color: #58abfa; font-size: 16px; font-weight: 600; background: transparent; border: none;");
    headerLayout->addWidget(userLabel);
    headerLayout->addStretch();

    qDebug() << "setupUi: btnAddProduct";
    QPushButton* btnAddProduct = new QPushButton("➕ Adicionar Produto");
    btnAddProduct->setStyleSheet("QPushButton { background: #58abfa; color: #fff; border: none; border-radius: 12px; padding: 10px 20px; font-size: 14px; font-weight: 600; } QPushButton:hover { background: #4a9de8; }");
    btnAddProduct->setToolTip("Adicionar um novo produto");
    connect(btnAddProduct, &QPushButton::clicked, this, &MainWindow::onAddProductClicked);
    headerLayout->addWidget(btnAddProduct);

    qDebug() << "setupUi: btnShowLog";
    QPushButton* btnShowLog = new QPushButton("📄 Histórico");
    btnShowLog->setToolTip("Visualizar histórico de operações");
    btnShowLog->setStyleSheet("QPushButton { background: #262727; color: #c1c5d1; border: none; border-radius: 10px; padding: 10px 20px; font-size: 13px; font-weight: 600; } QPushButton:hover { background: #444650; }");
    connect(btnShowLog, &QPushButton::clicked, this, &MainWindow::showHistoryDialog);
    headerLayout->addWidget(btnShowLog);

    productsLayout_->addWidget(userInfoLabel);

    qDebug() << "setupUi: filtros";
    auto* filterRow = new QHBoxLayout;
    searchBar_ = new QLineEdit;
    searchBar_->setPlaceholderText("🔍 Buscar produtos...");
    searchBar_->setStyleSheet("QLineEdit { background: #2d2d2d; color: #fff; border: 2px solid #3a3a3a; border-radius: 14px; padding: 12px 18px; font-size: 15px;} QLineEdit:focus { border: 2px solid #58abfa; }");
    connect(searchBar_, &QLineEdit::textChanged, this, &MainWindow::onSearchTextChanged);
    filterRow->addWidget(searchBar_, 5);

    filterCategoryBox_ = new QComboBox;
    filterCategoryBox_->setToolTip("Filtrar por Categoria");
    filterCategoryBox_->setStyleSheet("QComboBox { background: #242526; color: #fff; border: 2px solid #353a40; border-radius: 10px; font-size: 14px; padding: 6px 17px;} QComboBox::drop-down { border: none; width: 0px; }");
    filterRow->addWidget(filterCategoryBox_, 2);
    connect(filterCategoryBox_, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onFilterChanged);

    filterStockBox_ = new QComboBox;
    filterStockBox_->addItems({"Todos stock", "stock OK", "Baixo stock", "Sem stock"});
    filterStockBox_->setToolTip("Filtrar por situação de stock");
    filterStockBox_->setStyleSheet("QComboBox { background: #242526; color: #fff; border: 2px solid #353a40; border-radius: 10px; font-size: 14px; padding: 6px 17px;} QComboBox::drop-down { border: none; width: 0px; }");
    filterRow->addWidget(filterStockBox_, 2);
    connect(filterStockBox_, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onFilterChanged);

    productsLayout_->addLayout(filterRow);
    qDebug() << "setupUi: filtros OK";

    qDebug() << "setupUi: cards/scroll";
    QScrollArea* scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setStyleSheet("QScrollArea { background: transparent; border: none; }");
    productsContainer_ = new QWidget;
    productsCardsLayout_ = new QVBoxLayout(productsContainer_);
    productsCardsLayout_->setSpacing(16);
    productsCardsLayout_->setContentsMargins(0,0,0,0);
    scrollArea->setWidget(productsContainer_);
    productsLayout_->addWidget(scrollArea, 1);

    qDebug() << "setupUi: Toast";
    toastNotifier_ = new ToastNotification(productsBg_);
    toastNotifier_->hide();

    root->addWidget(sidebarBox_, 0);
    root->addWidget(productsBg_, 1);
    setCentralWidget(central);
    setWindowTitle("DevTools Manager");
    resize(1200, 770);
    qDebug() << "setupUi: FIM";
}



void MainWindow::loadSampleProducts()
{
    allProducts_.append(Product("001", "Mouse Gamer RGB", "Periféricos", 49.90, 25));
    allProducts_.append(Product("002", "Teclado Mecânico", "Periféricos", 99.90, 15));
    allProducts_.append(Product("003", "Monitor 27\" 144Hz", "Monitores", 299.90, 8));
    allProducts_.append(Product("004", "Headset Wireless", "Áudio", 99.90, 12));
    allProducts_.append(Product("005", "Webcam Full HD", "Câmeras", 49.90, 20));
    allProducts_.append(Product("006", "SSD 1TB NVMe", "Armazenamento", 399.90, 3));
    allProducts_.append(Product("007", "Mousepad Grande", "Acessórios", 15.90, 50));
    filteredProducts_ = allProducts_;
    operationLog_.clear();
}

void MainWindow::filterAndSortProducts()
{   
    if (filterRunning_) return; // evita recursão infinita
    filterRunning_ = true;

    filteredProducts_.clear();
    QString searchText = searchBar_->text().toLower().trimmed();
    QString selectedCategory = filterCategoryBox_->currentIndex() > 0 ? filterCategoryBox_->currentText() : "";
    QString stockStatus = filterStockBox_->currentText();

    bool mostrarFavoritos = btnShowFavorites_->isChecked();

    for (const Product& product : allProducts_) {
        bool matchesSearch =
            searchText.isEmpty() ||
            product.getName().toLower().contains(searchText) ||
            product.getCategory().toLower().contains(searchText) ||
            product.getId().toLower().contains(searchText);

        bool matchesCategory = selectedCategory.isEmpty() || product.getCategory() == selectedCategory;

        bool matchesStock = true;
        if (stockStatus == "stock OK")
            matchesStock = product.getQuantity() > 5;
        else if (stockStatus == "Baixo stock")
            matchesStock = product.getQuantity() > 0 && product.getQuantity() <= 5;
        else if (stockStatus == "Sem stock")
            matchesStock = product.getQuantity() == 0;

        // AQUI a lógica correta do toggle Favoritos/Produtos:
        bool matchesFav = !mostrarFavoritos || favoriteProductIds_.contains(product.getId());

        if (matchesSearch && matchesCategory && matchesStock && matchesFav)
            filteredProducts_.append(product);
    }

    std::sort(filteredProducts_.begin(), filteredProducts_.end(), [](const Product& a, const Product& b){
        return a.getName().toLower() < b.getName().toLower();
    });

    filterCategoryBox_->clear();
    filterCategoryBox_->addItem("Todas Categorias");
    QSet<QString> cats;
    for (const Product& p : allProducts_) cats.insert(p.getCategory());
    for (const QString& cat : cats) filterCategoryBox_->addItem(cat);

    filterRunning_ = false;
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
    cardLayout->setContentsMargins(22, 12, 22, 12);
    cardLayout->setSpacing(15);

    QPushButton* favBtn = new QPushButton(favoriteProductIds_.contains(product.getId()) ? "★" : "☆");
    favBtn->setCheckable(true);
    favBtn->setChecked(favoriteProductIds_.contains(product.getId()));
    favBtn->setCursor(Qt::PointingHandCursor);
    favBtn->setToolTip(favBtn->isChecked() ? "Desfavoritar" : "Favoritar");
    favBtn->setStyleSheet(R"(
        QPushButton {
            font-size: 22px;
            color: #ffec8b;
            background: transparent;
            border: none;
            min-width: 34px;
        }
        QPushButton:hover { color: #ffee12; }
    )");
    connect(favBtn, &QPushButton::clicked, this, [this, product, favBtn]() {
        bool favorited = favBtn->isChecked();
        if (favorited) favoriteProductIds_.insert(product.getId());
        else favoriteProductIds_.remove(product.getId());
        favBtn->setText(favorited ? "★" : "☆");
        showToast(favorited ? "Produto favoritado!" : "Produto desfavoritado!", favorited ? QColor("#ffe066") : QColor("#323e54"));
        filterAndSortProducts();
        displayProducts();
    });
    cardLayout->addWidget(favBtn, 0, Qt::AlignVCenter);

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
    selectButton->setToolTip(selectedProductIds_.contains(product.getId()) ? "Desmarcar" : "Selecionar");
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

    card->setCursor(Qt::PointingHandCursor);
    card->installEventFilter(this);
    card->setProperty("editId", product.getId());

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

    QString stockColor;
    if (product.isInStock() && product.getQuantity() <= 5)
        stockColor = "#f7b84b";
    else if (!product.isInStock())
        stockColor = "#e74c3c";
    else
        stockColor = "#2ecc71";
    QLabel* stockStatus = new QLabel(product.isInStock() ? (product.getQuantity() <= 5 ? "⚠ Baixo stock" : "✓ Em stock") : "✗ Sem stock");
    stockStatus->setStyleSheet(QString("color: %1; font-size: 13px; background: transparent; min-width:108px;").arg(stockColor));
    cardLayout->addWidget(stockStatus, 0, Qt::AlignVCenter);

    cardLayout->addStretch(1);
    productsCardsLayout_->addWidget(card);

    card->installEventFilter(this);
}

bool MainWindow::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        QWidget* card = qobject_cast<QWidget*>(obj);
        if (card && card->property("editId").isValid()) {
            QString editId = card->property("editId").toString();
            onEditProductRequested(editId);
            return true;
        }
    }
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::onEditProductRequested(const QString& productId)
{
    Product* toEdit = nullptr;
    for (Product& p : allProducts_)
        if (p.getId() == productId) { toEdit = &p; break; }
    if (!toEdit) return;

    QStringList categorias;
    for (const Product& prod : allProducts_)
        categorias << prod.getCategory();
    categorias.removeDuplicates();

    AddProductDialog dialog(*toEdit, categorias, this);
    if (dialog.exec() == QDialog::Accepted) {
        Product updated = dialog.getProduct();
        *toEdit = updated;
        showToast("Produto atualizado!");
        logOperation("Editado", updated);
        filterAndSortProducts();
        displayProducts();
    }
}

void MainWindow::onFavoriteProductToggled(const QString& productId, bool favorited)
{
    if (favorited) favoriteProductIds_.insert(productId);
    else favoriteProductIds_.remove(productId);
    showToast(favorited ? "Produto favoritado!" : "Produto desfavoritado!");
    filterAndSortProducts();
    displayProducts();
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
                logOperation("Deletado", allProducts_[i]);
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
    filterAndSortProducts();
    displayProducts();
    showToast("Produtos apagados!", QColor("#e74c3c"));
}

void MainWindow::onAddProductClicked()
{
    QStringList categorias;
    for (const Product& p : allProducts_)
        categorias << p.getCategory();
    categorias.removeDuplicates();

    AddProductDialog dialog(categorias, this);

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
        logOperation("Adicionado", newProduct);
        filterAndSortProducts();
        displayProducts();
        showToast(QString("Produto '%1' adicionado!").arg(newProduct.getName()), QColor("#2ecc71"));
    }
}

void MainWindow::onSearchTextChanged(const QString&)
{
    filterAndSortProducts();
    displayProducts();
}

void MainWindow::onFilterChanged()
{
    filterAndSortProducts();
    displayProducts();
}

void MainWindow::showToast(const QString& message, QColor color)
{
    if (!toastNotifier_) return;
    toastNotifier_->setText(message);
    toastNotifier_->setColor(color);
    toastNotifier_->show();
    QTimer::singleShot(1400, toastNotifier_, &ToastNotification::hide);
}

void MainWindow::logOperation(const QString& op, const Product& p)
{
    operationLog_.append(qMakePair(op, p));
    if (operationLog_.size() > 35)
        operationLog_.removeFirst();
}

void MainWindow::showHistoryDialog()
{
    QString logText;
    for (const auto& entry : operationLog_) {
        logText += QString("[%1] %2 - %3\n").arg(entry.first, entry.second.getId(), entry.second.getName());
    }
    QMessageBox::information(this, "Histórico do Sistema",
        logText.isEmpty() ? "Nenhuma operação registrada ainda..." : logText);
}

void MainWindow::onExportProductsClicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Exportar produtos", "", "JSON Files (*.json)");
    if (fileName.isEmpty()) return;

    QJsonArray prodArray;
    for (const Product& p : allProducts_) prodArray.append(p.toJsonObject());
    QJsonObject obj;
    obj["produtos"] = prodArray;
    QFile f(fileName);
    if (!f.open(QIODevice::WriteOnly)) {
        showToast("Falha ao salvar!", QColor("#e74c3c"));
        return;
    }
    f.write(QJsonDocument(obj).toJson());
    f.close();
    showToast("Catálogo exportado!", QColor("#2ecc71"));
}

void MainWindow::onImportProductsClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Importar produtos", "", "JSON Files (*.json)");
    if (fileName.isEmpty()) return;
    QFile f(fileName);
    if (!f.open(QIODevice::ReadOnly)) {
        showToast("Falha ao importar!", QColor("#e74c3c"));
        return;
    }
    QByteArray data = f.readAll();
    f.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) {
        showToast("Arquivo inválido!", QColor("#e74c3c"));
        return;
    }
    QJsonArray prodArray = doc.object().value("produtos").toArray();
    allProducts_.clear();
    for (const QJsonValue& v : prodArray)
        allProducts_.append(Product::fromJsonObject(v.toObject()));
    filterAndSortProducts();
    displayProducts();
    showToast("Produtos importados!", QColor("#2ecc71"));
}

void MainWindow::atualizarListaProdutosUI()
{
    filterAndSortProducts();
    displayProducts();
}

void MainWindow::onBtnProducts() {
    btnProducts_->setChecked(true);
    btnShowFavorites_->setChecked(false);
    filterAndSortProducts();
    displayProducts();
}

void MainWindow::onBtnFavorites() {
    btnProducts_->setChecked(false);
    btnShowFavorites_->setChecked(true);
    filterAndSortProducts();
    displayProducts();
}
