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
#include <QPixmap>
#include <QTimer>
#include <QDebug>
#include "AddProductDialog.h"
#include "ToastNotification.h"
#include <QGuiApplication>
#include <QApplication>
#include <QKeyEvent>
#include "UserAuthDialog.h"
#include <QCryptographicHash>
#include <QProcess>
#include <QListView>
#include <QSignalBlocker>
#include <QRegularExpression>

MainWindow::MainWindow(const QString& username, QWidget* parent)
    : QMainWindow(parent), currentUsername_(username)
{
    setupUi();
    loadSampleProducts();
    filterAndSortProducts();
    displayProducts();
    
    // Update user label will be done in setupUi after userLabel_ is created
}

void MainWindow::setupUi()
{
    qDebug() << "setupUi: widgets-raiz";
    auto* central = new QWidget(this);
    central->setStyleSheet("background: #0d1117;"); // GitHub dark background
    auto* root = new QHBoxLayout(central);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    qDebug() << "setupUi: sidebarBox/layout";
    sidebarBox_ = new QWidget;
    sidebarBox_->setStyleSheet(R"(
        QWidget {
            background: #161b22;
            border-right: 1px solid #21262d;
            min-width: 256px;
            max-width: 256px;
        }
    )");
    sidebarLayout_ = new QVBoxLayout(sidebarBox_);
    sidebarLayout_->setContentsMargins(16, 24, 16, 24);
    sidebarLayout_->setSpacing(8);

    qDebug() << "setupUi: logo";
    QLabel* logoLabel = new QLabel("<span style='font-size: 16px; font-weight: 600; color: #c9d1d9;'>DevTools Manager</span>");
    logoLabel->setStyleSheet(R"(
        color: #c9d1d9;
        border: none;
        background: transparent;
        padding: 8px 12px;
    )");
    logoLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    sidebarLayout_->addWidget(logoLabel);
    
    // Separator line
    QFrame* line1 = new QFrame();
    line1->setFrameShape(QFrame::HLine);
    line1->setStyleSheet("background: #21262d; max-height: 1px;");
    sidebarLayout_->addWidget(line1);
    sidebarLayout_->addSpacing(8);

    qDebug() << "setupUi: btnProducts";
    btnProducts_ = new QPushButton("Products");
    btnProducts_->setCheckable(true);
    btnProducts_->setChecked(true);
    btnProducts_->setStyleSheet(R"(
        QPushButton {
            background: transparent;
            color: #8b949e;
            border: none;
            border-radius: 6px;
            padding: 8px 12px;
            font-size: 14px;
            font-weight: 500;
            text-align: left;
        }
        QPushButton:checked {
            background: #21262d;
            color: #c9d1d9;
            font-weight: 600;
        }
        QPushButton:hover {
            background: #161b22;
            color: #c9d1d9;
        }
    )");
    sidebarLayout_->addWidget(btnProducts_);

    qDebug() << "setupUi: btnShowFavorites";
    btnShowFavorites_ = new QPushButton("Favorites");
    btnShowFavorites_->setCheckable(true);
    btnShowFavorites_->setChecked(false);
    btnShowFavorites_->setStyleSheet(R"(
        QPushButton {
            background: transparent;
            color: #8b949e;
            border: none;
            border-radius: 6px;
            padding: 8px 12px;
            font-size: 14px;
            font-weight: 500;
            text-align: left;
        }
        QPushButton:checked {
            background: #21262d;
            color: #c9d1d9;
            font-weight: 600;
        }
        QPushButton:hover {
            background: #161b22;
            color: #c9d1d9;
        }
    )");
    sidebarLayout_->addWidget(btnShowFavorites_);

    // Troca: conectando para slots exclusivos (não aos filtros diretos)
    connect(btnProducts_, &QPushButton::clicked, this, &MainWindow::onBtnProducts);
    connect(btnShowFavorites_, &QPushButton::clicked, this, &MainWindow::onBtnFavorites);

    qDebug() << "setupUi: btnDeleteSelected";
    btnDeleteSelected_ = new QPushButton("Delete Selected");
    btnDeleteSelected_->setEnabled(false);
    btnDeleteSelected_->setStyleSheet(R"(
        QPushButton {
            background: transparent;
            color: #f85149;
            border: 1px solid #da3633;
            border-radius: 6px;
            padding: 8px 12px;
            font-size: 14px;
            font-weight: 500;
            text-align: center;
        }
        QPushButton:disabled {
            background: transparent;
            color: #484f58;
            border: 1px solid #30363d;
        }
        QPushButton:hover:enabled {
            background: #da3633;
            color: #ffffff;
            border: 1px solid #da3633;
        }
    )");
    connect(btnDeleteSelected_, &QPushButton::clicked, this, &MainWindow::onDeleteSelectedProductsClicked);
    sidebarLayout_->addWidget(btnDeleteSelected_);
    sidebarLayout_->addSpacing(4);
    
    // Separator line
    QFrame* line2 = new QFrame();
    line2->setFrameShape(QFrame::HLine);
    line2->setStyleSheet("background: #21262d; max-height: 1px;");
    sidebarLayout_->addWidget(line2);
    sidebarLayout_->addSpacing(8);

    sidebarLayout_->addStretch(1);

    qDebug() << "setupUi: btnExport / btnImport";
    btnExport_ = new QPushButton("Export");
    btnExport_->setStyleSheet(R"(
        QPushButton {
            background: transparent;
            color: #8b949e;
            border: none;
            border-radius: 6px;
            padding: 8px 12px;
            font-size: 14px;
            font-weight: 500;
            text-align: left;
        }
        QPushButton:hover {
            background: #161b22;
            color: #c9d1d9;
        }
    )");
    btnExport_->setToolTip("Exporta o catálogo para um arquivo JSON");
    connect(btnExport_, &QPushButton::clicked, this, &MainWindow::onExportProductsClicked);
    sidebarLayout_->addWidget(btnExport_);

    btnImport_ = new QPushButton("Import");
    btnImport_->setStyleSheet(R"(
        QPushButton {
            background: transparent;
            color: #8b949e;
            border: none;
            border-radius: 6px;
            padding: 8px 12px;
            font-size: 14px;
            font-weight: 500;
            text-align: left;
        }
        QPushButton:hover {
            background: #161b22;
            color: #c9d1d9;
        }
    )");
    btnImport_->setToolTip("Importa produtos de um arquivo JSON");
    connect(btnImport_, &QPushButton::clicked, this, &MainWindow::onImportProductsClicked);
    sidebarLayout_->addWidget(btnImport_);

    qDebug() << "setupUi: productsBg";
    productsBg_ = new QWidget;
    productsBg_->setStyleSheet("background: #0d1117;");
    productsLayout_ = new QVBoxLayout(productsBg_);
    productsLayout_->setSpacing(16);
    productsLayout_->setContentsMargins(24, 24, 24, 24);

    qDebug() << "setupUi: userInfoLabel/header";
    QWidget* userInfoLabel = new QWidget;
    userInfoLabel->setStyleSheet(R"(
        QWidget {
            background: #161b22;
            border: 1px solid #21262d;
            border-radius: 6px;
        }
    )");
    auto* headerLayout = new QHBoxLayout(userInfoLabel);
    headerLayout->setContentsMargins(16, 12, 16, 12);
    headerLayout->setSpacing(12);

    userLabel_ = new QLabel();
    userLabel_->setText(currentUsername_.isEmpty() ? "User" : currentUsername_);
    userLabel_->setStyleSheet(R"(
        color: #c9d1d9;
        font-size: 14px;
        font-weight: 600;
        background: transparent;
        border: none;
    )");
    headerLayout->addWidget(userLabel_);
    headerLayout->addStretch();

    btnLogout_ = new QPushButton("Logout");
    btnLogout_->setStyleSheet(R"(
        QPushButton {
            background: transparent;
            color: #8b949e;
            border: 1px solid #30363d;
            border-radius: 6px;
            font-size: 12px;
            padding: 5px 12px;
            font-weight: 500;
        }
        QPushButton:hover {
            background: #21262d;
            border: 1px solid #30363d;
            color: #c9d1d9;
        }
    )");
    connect(btnLogout_, &QPushButton::clicked, this, &MainWindow::onLogoutClicked);
    headerLayout->addWidget(btnLogout_);

    qDebug() << "setupUi: btnAddProduct";
    QPushButton* btnAddProduct = new QPushButton("Add Product");
    btnAddProduct->setStyleSheet(R"(
        QPushButton {
            background: #238636;
            color: #ffffff;
            border: none;
            border-radius: 6px;
            padding: 5px 16px;
            font-size: 12px;
            font-weight: 600;
        }
        QPushButton:hover {
            background: #2ea043;
        }
    )");
    btnAddProduct->setToolTip("Adicionar um novo produto");
    connect(btnAddProduct, &QPushButton::clicked, this, &MainWindow::onAddProductClicked);
    headerLayout->addWidget(btnAddProduct);

    qDebug() << "setupUi: btnShowLog";
    QPushButton* btnShowLog = new QPushButton("History");
    btnShowLog->setToolTip("View operation history");
    btnShowLog->setStyleSheet(R"(
        QPushButton {
            background: transparent;
            color: #8b949e;
            border: 1px solid #30363d;
            border-radius: 6px;
            padding: 5px 12px;
            font-size: 12px;
            font-weight: 500;
        }
        QPushButton:hover {
            background: #21262d;
            border: 1px solid #30363d;
            color: #c9d1d9;
        }
    )");
    connect(btnShowLog, &QPushButton::clicked, this, &MainWindow::showHistoryDialog);
    headerLayout->addWidget(btnShowLog);

    productsLayout_->addWidget(userInfoLabel);

    qDebug() << "setupUi: filtros";
    auto* filterRow = new QHBoxLayout;
    filterRow->setSpacing(12);
    searchBar_ = new QLineEdit;
    searchBar_->setPlaceholderText("Search products...");
    searchBar_->setStyleSheet(R"(
        QLineEdit {
            background: #0d1117;
            color: #c9d1d9;
            border: 1px solid #30363d;
            border-radius: 6px;
            padding: 5px 12px;
            font-size: 14px;
        }
        QLineEdit:focus {
            background: #0d1117;
            border: 1px solid #58a6ff;
            outline: none;
        }
        QLineEdit::placeholder {
            color: #484f58;
        }
    )");
    connect(searchBar_, &QLineEdit::textChanged, this, &MainWindow::onSearchTextChanged);
    filterRow->addWidget(searchBar_, 5);

    filterCategoryBox_ = new QComboBox;
    filterCategoryBox_->setToolTip("Filtrar por Categoria");
    filterCategoryBox_->setStyleSheet(R"(
        QComboBox {
            background: #0d1117;
            color: #c9d1d9;
            border: 1px solid #30363d;
            border-radius: 6px;
            font-size: 14px;
            padding: 5px 12px;
            min-height: 20px;
        }
        QComboBox:focus {
            border: 1px solid #58a6ff;
        }
        QComboBox::drop-down {
            border: none;
            width: 20px;
        }
        QComboBox::down-arrow {
            image: none;
            border-left: 4px solid transparent;
            border-right: 4px solid transparent;
            border-top: 4px solid #8b949e;
            width: 0;
            height: 0;
        }
        QComboBox QAbstractItemView {
            background: #161b22;
            border: 1px solid #30363d;
            border-radius: 6px;
            outline: none;
            selection-background-color: #21262d;
        }
    )");
    filterCategoryBox_->setAttribute(Qt::WA_NoSystemBackground);
    filterCategoryBox_->setAttribute(Qt::WA_TranslucentBackground);
    filterCategoryBox_->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    filterCategoryBox_->setEditable(false);
    filterCategoryBox_->setInsertPolicy(QComboBox::NoInsert);
    filterCategoryBox_->setDuplicatesEnabled(false);
    filterCategoryBox_->setMaxVisibleItems(8);
    filterCategoryBox_->setFrame(false);
    // Modern popup view styling
    {
        auto* catView = new QListView(filterCategoryBox_);
        catView->setStyleSheet(R"(
            QListView {
                background: #161b22;
                color: #c9d1d9;
                border: none;
                padding: 4px;
                outline: 0;
            }
            QListView::viewport {
                background: #161b22;
                border: 1px solid #30363d;
                border-radius: 6px;
            }
            QListView::item {
                padding: 6px 12px;
                min-height: 28px;
                border-radius: 4px;
            }
            QListView::item:hover {
                background: #21262d;
            }
            QListView::item:selected {
                background: #1f6feb;
                color: #ffffff;
            }
            QScrollBar:vertical {
                background: #161b22;
                width: 8px;
                margin: 0;
            }
            QScrollBar::handle:vertical {
                background: #30363d;
                border-radius: 4px;
            }
            QScrollBar::handle:vertical:hover {
                background: #484f58;
            }
            QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
                height: 0;
            }
        )");
        catView->setFrameShape(QFrame::NoFrame);
        catView->setAutoFillBackground(false);
        // Let eventFilter keep popup width equal to button width
        catView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        catView->setUniformItemSizes(true);
        catView->setSpacing(2);
        catView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        catView->setTextElideMode(Qt::ElideRight);
        filterCategoryBox_->setView(catView);
        filterCategoryBox_->setMaxVisibleItems(8);
    }
    filterRow->addWidget(filterCategoryBox_, 2);
    // Use activated() so we react only to user selection from the popup (reduces extra redraws)
    connect(filterCategoryBox_, QOverload<int>::of(&QComboBox::activated), this, &MainWindow::onFilterChanged);

    filterStockBox_ = new QComboBox;
    filterStockBox_->addItems({"All stock", "In stock", "Low stock", "Out of stock"});
    filterStockBox_->setToolTip("Filter by stock status");
    filterStockBox_->setStyleSheet(R"(
        QComboBox {
            background: #0d1117;
            color: #c9d1d9;
            border: 1px solid #30363d;
            border-radius: 6px;
            font-size: 14px;
            padding: 5px 12px;
            min-height: 20px;
        }
        QComboBox:focus {
            border: 1px solid #58a6ff;
        }
        QComboBox::drop-down {
            border: none;
            width: 20px;
        }
        QComboBox::down-arrow {
            image: none;
            border-left: 4px solid transparent;
            border-right: 4px solid transparent;
            border-top: 4px solid #8b949e;
            width: 0;
            height: 0;
        }
        QComboBox QAbstractItemView {
            background: #161b22;
            border: 1px solid #30363d;
            border-radius: 6px;
            outline: none;
            selection-background-color: #21262d;
        }
    )");
    filterStockBox_->setAttribute(Qt::WA_NoSystemBackground);
    filterStockBox_->setAttribute(Qt::WA_TranslucentBackground);
    filterStockBox_->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    filterStockBox_->setEditable(false);
    filterStockBox_->setInsertPolicy(QComboBox::NoInsert);
    filterStockBox_->setDuplicatesEnabled(false);
    filterStockBox_->setMaxVisibleItems(8);
    filterStockBox_->setFrame(false);
    {
        auto* stockView = new QListView(filterStockBox_);
        stockView->setStyleSheet(R"(
            QListView {
                background: #161b22;
                color: #c9d1d9;
                border: none;
                padding: 4px;
                outline: 0;
            }
            QListView::viewport {
                background: #161b22;
                border: 1px solid #30363d;
                border-radius: 6px;
            }
            QListView::item {
                padding: 6px 12px;
                min-height: 28px;
                border-radius: 4px;
            }
            QListView::item:hover {
                background: #21262d;
            }
            QListView::item:selected {
                background: #1f6feb;
                color: #ffffff;
            }
            QScrollBar:vertical {
                background: #161b22;
                width: 8px;
                margin: 0;
            }
            QScrollBar::handle:vertical {
                background: #30363d;
                border-radius: 4px;
            }
            QScrollBar::handle:vertical:hover {
                background: #484f58;
            }
            QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
                height: 0;
            }
        )");
        stockView->setFrameShape(QFrame::NoFrame);
        stockView->setAutoFillBackground(false);
        // Let eventFilter keep popup width equal to button width
        stockView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        stockView->setUniformItemSizes(true);
        stockView->setSpacing(2);
        stockView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        stockView->setTextElideMode(Qt::ElideRight);
        filterStockBox_->setView(stockView);
        filterStockBox_->setMaxVisibleItems(8);
    }
    filterRow->addWidget(filterStockBox_, 2);
    // Use activated() so we react only to user selection from the popup (reduces extra redraws)
    connect(filterStockBox_, QOverload<int>::of(&QComboBox::activated), this, &MainWindow::onFilterChanged);

    // Normalize heights to match the search bar
    {
        int h = searchBar_->sizeHint().height();
        filterCategoryBox_->setFixedHeight(h);
        filterStockBox_->setFixedHeight(h);
        // Track size changes to keep popup width == button width
        filterCategoryBox_->installEventFilter(this);
        filterStockBox_->installEventFilter(this);
    }
    productsLayout_->addLayout(filterRow);
    qDebug() << "setupUi: filtros OK";

    qDebug() << "setupUi: cards/scroll";
    QScrollArea* scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setStyleSheet(R"(
        QScrollArea {
            background: transparent;
            border: none;
        }
        QScrollBar:vertical {
            background: #0d1117;
            width: 10px;
            margin: 0;
        }
        QScrollBar::handle:vertical {
            background: #30363d;
            border-radius: 5px;
            min-height: 30px;
        }
        QScrollBar::handle:vertical:hover {
            background: #484f58;
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0;
        }
        QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {
            background: none;
        }
    )");
    productsContainer_ = new QWidget;
    productsCardsLayout_ = new QVBoxLayout(productsContainer_);
    productsCardsLayout_->setSpacing(8);
    productsCardsLayout_->setContentsMargins(0,0,8,0);
    scrollArea->setWidget(productsContainer_);
    productsLayout_->addWidget(scrollArea, 1);

    qDebug() << "setupUi: Toast";
    toastNotifier_ = new ToastNotification(productsBg_);
    toastNotifier_->hide();

    root->addWidget(sidebarBox_, 0);
    root->addWidget(productsBg_, 1);
    setCentralWidget(central);
    setWindowTitle("DevTools Manager");
    resize(1280, 800);
    
    // Apply GitHub-like window styling
    setStyleSheet(R"(
        QMainWindow {
            background: #0d1117;
        }
        QMessageBox {
            background: #161b22;
            color: #c9d1d9;
        }
        QMessageBox QPushButton {
            background: #21262d;
            color: #c9d1d9;
            border: 1px solid #30363d;
            border-radius: 6px;
            padding: 5px 16px;
        }
        QMessageBox QPushButton:hover {
            background: #30363d;
        }
    )");
    
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

    {
        QSignalBlocker blocker(filterCategoryBox_);
        filterCategoryBox_->clear();
        filterCategoryBox_->addItem("Todas Categorias");
        QSet<QString> cats;
        for (const Product& p : allProducts_) cats.insert(p.getCategory());
        QStringList sortedCats = QStringList(cats.begin(), cats.end());
        std::sort(sortedCats.begin(), sortedCats.end(), [](const QString& a, const QString& b){ return a.toLower() < b.toLower(); });
        for (const QString& cat : sortedCats) filterCategoryBox_->addItem(cat);
        int idx = 0;
        if (!selectedCategory.isEmpty()) {
            for (int i = 0; i < filterCategoryBox_->count(); ++i) {
                if (filterCategoryBox_->itemText(i) == selectedCategory) { idx = i; break; }
            }
        }
        filterCategoryBox_->setCurrentIndex(idx);
    }

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
        QLabel* emptyLabel = new QLabel("No products found");
        emptyLabel->setStyleSheet(R"(
            color: #8b949e;
            font-size: 14px;
            font-weight: 400;
            padding: 48px 24px;
            background: transparent;
        )");
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
            background: #161b22;
            border: 1px solid #21262d;
            border-radius: 6px;
        }
        QWidget:hover {
            border: 1px solid #30363d;
            background: #161b22;
        }
    )");
    auto* cardLayout = new QHBoxLayout(card);
    cardLayout->setContentsMargins(16, 12, 16, 12);
    cardLayout->setSpacing(12);

    QPushButton* favBtn = new QPushButton(favoriteProductIds_.contains(product.getId()) ? "★" : "☆");
    favBtn->setCheckable(true);
    favBtn->setChecked(favoriteProductIds_.contains(product.getId()));
    favBtn->setCursor(Qt::PointingHandCursor);
    favBtn->setToolTip(favBtn->isChecked() ? "Unfavorite" : "Favorite");
    favBtn->setStyleSheet(R"(
        QPushButton {
            font-size: 18px;
            color: #8b949e;
            background: transparent;
            border: none;
            min-width: 24px;
            padding: 0px;
        }
        QPushButton:checked {
            color: #f1e05a;
        }
        QPushButton:hover {
            color: #f1e05a;
        }
    )");
    connect(favBtn, &QPushButton::clicked, this, [this, product, favBtn]() {
        bool favorited = favBtn->isChecked();
        if (favorited) favoriteProductIds_.insert(product.getId());
        else favoriteProductIds_.remove(product.getId());
        favBtn->setText(favorited ? "★" : "☆");
        filterAndSortProducts();
        displayProducts();
    });
    cardLayout->addWidget(favBtn, 0, Qt::AlignVCenter);

    int currentIndex = -1;
    for (int i = 0; i < filteredProducts_.size(); ++i) {
        if (filteredProducts_[i].getId() == product.getId()) {
            currentIndex = i;
            break;
        }
    }
    QPushButton* selectButton = new QPushButton;
    selectButton->setCheckable(true);
    selectButton->setChecked(selectedProductIds_.contains(product.getId()));
    selectButton->setStyleSheet(R"(
        QPushButton {
            background: #0d1117;
            border: 1px solid #30363d;
            border-radius: 4px;
            min-width: 16px;
            min-height: 16px;
            max-width: 16px;
            max-height: 16px;
            padding: 0px;
        }
        QPushButton:checked {
            background: #1f6feb;
            border: 1px solid #1f6feb;
        }
        QPushButton:hover {
            border-color: #58a6ff;
        }
    )");
    selectButton->setToolTip(selectedProductIds_.contains(product.getId()) ? "Unselect" : "Select");
    cardLayout->addWidget(selectButton, 0, Qt::AlignVCenter);
    connect(selectButton, &QPushButton::clicked, this, [this, product, currentIndex, selectButton](bool checked) {
        QGuiApplication::keyboardModifiers();
        auto modifiers = QGuiApplication::keyboardModifiers();
        if (modifiers & Qt::ShiftModifier && lastProductCardClickedIndex_ >= 0 && lastProductCardClickedIndex_ < filteredProducts_.size()) {
            int from = qMin(lastProductCardClickedIndex_, currentIndex);
            int to = qMax(lastProductCardClickedIndex_, currentIndex);
            for (int i = from; i <= to; ++i) {
                selectedProductIds_.insert(filteredProducts_[i].getId());
            }
            updateDeleteSelectedButtonState(); // Sure it's enabled after selection
            displayProducts();
            if (btnDeleteSelected_) btnDeleteSelected_->setChecked(true); /* For checkable, otherwise just enabled is enough */
        } else {
            onSelectProductToggled(product.getId(), checked);
        }
        lastProductCardClickedIndex_ = currentIndex;
    });

    QLabel* icon = new QLabel;
    icon->setStyleSheet(R"(
        background: #21262d;
        border: 1px solid #30363d;
        border-radius: 6px;
        min-width: 48px;
        min-height: 48px;
        max-width: 48px;
        max-height: 48px;
        padding: 0px;
    )");
    icon->setAlignment(Qt::AlignCenter);
    icon->setScaledContents(true);
    
    // Load product image if it exists
    QString imagePath = product.getImagePath();
    if (!imagePath.isEmpty() && QFile::exists(imagePath)) {
        QPixmap pixmap(imagePath);
        if (!pixmap.isNull()) {
            icon->setPixmap(pixmap.scaled(48, 48, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        } else {
            icon->setText("📦");
            icon->setStyleSheet(icon->styleSheet() + "font-size: 24px; color: #58a6ff;");
        }
    } else {
        icon->setText("📦");
        icon->setStyleSheet(icon->styleSheet() + "font-size: 24px; color: #58a6ff;");
    }
    
    cardLayout->addWidget(icon, 0, Qt::AlignVCenter);

    QLabel* nameLabel = new QLabel(product.getName());
    nameLabel->setStyleSheet(R"(
        color: #c9d1d9;
        font-size: 14px;
        font-weight: 600;
        min-width: 200px;
        background: transparent;
        border: none;
    )");
    cardLayout->addWidget(nameLabel, 1, Qt::AlignVCenter);

    card->setCursor(Qt::PointingHandCursor);
    card->installEventFilter(this);
    card->setProperty("editId", product.getId());

    QLabel* categoryLabel = new QLabel(product.getCategory());
    categoryLabel->setStyleSheet(R"(
        color: #8b949e;
        font-size: 12px;
        background: #21262d;
        border: 1px solid #30363d;
        border-radius: 12px;
        padding: 2px 8px;
        min-width: 80px;
        font-weight: 500;
    )");
    categoryLabel->setAlignment(Qt::AlignCenter);
    cardLayout->addWidget(categoryLabel, 0, Qt::AlignVCenter);

    QLabel* idLabel = new QLabel(QString("#%1").arg(product.getId()));
    idLabel->setStyleSheet(R"(
        color: #8b949e;
        font-size: 12px;
        background: transparent;
        min-width: 60px;
        font-weight: 400;
    )");
    cardLayout->addWidget(idLabel, 0, Qt::AlignVCenter);

    QLabel* priceLabel = new QLabel(product.getPriceFormatted());
    priceLabel->setStyleSheet(R"(
        color: #c9d1d9;
        font-size: 14px;
        font-weight: 600;
        background: transparent;
        min-width: 85px;
    )");
    cardLayout->addWidget(priceLabel, 0, Qt::AlignVCenter);

    QLabel* qtyLabel = new QLabel(QString("Qty: %1").arg(product.getQuantity()));
    qtyLabel->setStyleSheet(R"(
        color: #8b949e;
        font-size: 12px;
        background: transparent;
        min-width: 70px;
        font-weight: 400;
    )");
    cardLayout->addWidget(qtyLabel, 0, Qt::AlignVCenter);

    QString stockColor;
    QString stockBgColor;
    QString stockText;
    if (product.isInStock() && product.getQuantity() <= 5) {
        stockColor = "#d29922";
        stockBgColor = "#341a00";
        stockText = "Low stock";
    } else if (!product.isInStock()) {
        stockColor = "#f85149";
        stockBgColor = "#490202";
        stockText = "Out of stock";
    } else {
        stockColor = "#3fb950";
        stockBgColor = "#0f2414";
        stockText = "In stock";
    }
    
    QLabel* stockStatus = new QLabel(stockText);
    stockStatus->setStyleSheet(QString(R"(
        color: %1;
        font-size: 12px;
        background: %2;
        border-radius: 12px;
        padding: 2px 8px;
        min-width: 80px;
        font-weight: 500;
    )").arg(stockColor, stockBgColor));
    stockStatus->setAlignment(Qt::AlignCenter);
    cardLayout->addWidget(stockStatus, 0, Qt::AlignVCenter);

    cardLayout->addStretch(1);
    productsCardsLayout_->addWidget(card);

    card->installEventFilter(this);
}

bool MainWindow::eventFilter(QObject* obj, QEvent* event)
{
    // Keep popup width exactly equal to combo button width and handle select-all hotkey
    if ((obj == filterCategoryBox_ || obj == filterStockBox_) &&
        (event->type() == QEvent::Resize || event->type() == QEvent::Show || event->type() == QEvent::ShowToParent)) {
        QComboBox* combo = qobject_cast<QComboBox*>(obj);
        if (combo && combo->view()) {
            combo->view()->setFixedWidth(combo->width());
        }
    }
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
#ifdef Q_OS_MAC
        bool isCommandA = keyEvent->key() == Qt::Key_A && (keyEvent->modifiers() & Qt::MetaModifier);
#else
        bool isCommandA = keyEvent->key() == Qt::Key_A && (keyEvent->modifiers() & Qt::ControlModifier);
#endif
        if (isCommandA) {
            // Select all visible (filtered) products
            selectedProductIds_.clear();
            for (const Product &p : filteredProducts_) {
                selectedProductIds_.insert(p.getId());
            }
            updateDeleteSelectedButtonState(); // Make sure delete is enabled
            displayProducts(); // update UI for checked checkboxes
            if (btnDeleteSelected_) btnDeleteSelected_->setChecked(true); /* For checkable, otherwise just enabled is enough */
            return true;
        }
    }
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
        showToast("Product updated!");
        logOperation("Editado", updated);
        filterAndSortProducts();
        displayProducts();
    }
}

void MainWindow::onFavoriteProductToggled(const QString& productId, bool favorited)
{
    if (favorited) favoriteProductIds_.insert(productId);
    else favoriteProductIds_.remove(productId);
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
    int ret = QMessageBox::question(this, "Confirm Deletion",
        QString("Are you sure you want to delete %1 selected products?").arg(selectedProductIds_.size()),
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
}

void MainWindow::onAddProductClicked()
{
    QStringList categorias;
    for (const Product& p : allProducts_)
        categorias << p.getCategory();
    categorias.removeDuplicates();

    AddProductDialog dialog(categorias, this);

    // Auto-generate a unique numeric ID (user can still edit it)
    QSet<QString> existingIds;
    for (const Product& p : allProducts_) existingIds.insert(p.getId());
    auto generateUniqueId = [&](int minWidth){
        int maxNum = 0;
        QRegularExpression re("^\\d+$");
        for (const QString& id : existingIds) {
            if (re.match(id).hasMatch()) {
                bool ok = false; int n = id.toInt(&ok);
                if (ok) maxNum = qMax(maxNum, n);
            }
        }
        int candidate = maxNum + 1;
        QString s;
        do {
            s = QString("%1").arg(candidate, qMax(minWidth, (int)QString::number(candidate).size()), 10, QChar('0'));
            candidate++;
        } while (existingIds.contains(s));
        return s;
    };
    dialog.setInitialId(generateUniqueId(3));

    if (dialog.exec() == QDialog::Accepted) {
        Product newProduct = dialog.getProduct();
        for (const Product& p : allProducts_) {
            if (p.getId() == newProduct.getId()) {
                QMessageBox::warning(this, "Error",
                    QString("A product with ID '%1' already exists!").arg(newProduct.getId()));
                return;
            }
        }
        allProducts_.append(newProduct);
        logOperation("Adicionado", newProduct);
        filterAndSortProducts();
        displayProducts();
        showToast(QString("Product '%1' added!").arg(newProduct.getName()), QColor("#666666"));
    }
}

void MainWindow::onSearchTextChanged(const QString&)
{
    filterAndSortProducts();
    displayProducts();
}

void MainWindow::onFilterChanged()
{
    // Rebuild products list immediately without animations
    if (productsBg_) {
        productsBg_->setUpdatesEnabled(false);
        filterAndSortProducts();
        displayProducts();
        productsBg_->setUpdatesEnabled(true);
    }
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
    QMessageBox::information(this, "System History",
        logText.isEmpty() ? "No operations recorded yet..." : logText);
}

void MainWindow::onExportProductsClicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Export products", "", "JSON Files (*.json)");
    if (fileName.isEmpty()) return;

    QJsonArray prodArray;
    for (const Product& p : allProducts_) prodArray.append(p.toJsonObject());
    QJsonObject obj;
    obj["produtos"] = prodArray;
    QFile f(fileName);
    if (!f.open(QIODevice::WriteOnly)) {
        showToast("Failed to save!", QColor("#666666"));
        return;
    }
    f.write(QJsonDocument(obj).toJson());
    f.close();
    showToast("Catalog exported!", QColor("#666666"));
}

void MainWindow::onImportProductsClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Import products", "", "JSON Files (*.json)");
    if (fileName.isEmpty()) return;
    QFile f(fileName);
    if (!f.open(QIODevice::ReadOnly)) {
        showToast("Failed to import!", QColor("#666666"));
        return;
    }
    QByteArray data = f.readAll();
    f.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) {
        showToast("Invalid file!", QColor("#666666"));
        return;
    }
    QJsonArray prodArray = doc.object().value("produtos").toArray();
    allProducts_.clear();
    for (const QJsonValue& v : prodArray)
        allProducts_.append(Product::fromJsonObject(v.toObject()));
    filterAndSortProducts();
    displayProducts();
    showToast("Products imported!", QColor("#666666"));
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

void MainWindow::onLogoutClicked() {
    int ret = QMessageBox::question(this, "Logout", 
        "Are you sure you want to logout?",
        QMessageBox::Yes | QMessageBox::No);
    
    if (ret == QMessageBox::Yes) {
        this->close();
        QApplication::quit();
        // Restart the application (main will show login again)
        QProcess::startDetached(QApplication::arguments()[0], QApplication::arguments());
    }
}
