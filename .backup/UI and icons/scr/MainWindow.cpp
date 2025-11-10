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
#include <QMouseEvent>
#include <QResizeEvent>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QPainter>
#include <cmath>

MainWindow::MainWindow(const QString& username, QWidget* parent)
    : QMainWindow(parent), currentUsername_(username)
{
    // Remove native title bar for custom styling
    setWindowFlag(Qt::FramelessWindowHint, true);
    // Enable translucent background for rounded corners
    setAttribute(Qt::WA_TranslucentBackground, true);
    setupUi();
    loadSampleProducts();
    filterAndSortProducts();
    displayProducts();
}

// Helper function to make icon white
static QIcon makeWhiteIcon(const QString& path, const QSize& size = QSize(24, 24)) {
    QPixmap pixmap(path);
    if (pixmap.isNull()) return QIcon();
    QImage img = pixmap.toImage().convertToFormat(QImage::Format_ARGB32);
    for (int y = 0; y < img.height(); ++y) {
        for (int x = 0; x < img.width(); ++x) {
            QColor c = img.pixelColor(x, y);
            if (c.alpha() > 0) {
                c.setRed(255); c.setGreen(255); c.setBlue(255);
                img.setPixelColor(x, y, c);
            }
        }
    }
    
    // Create QIcon with both normal and disabled states
    QIcon icon;
    icon.addPixmap(QPixmap::fromImage(img), QIcon::Normal);
    
    // Create dimmed version for disabled state
    QImage dimImg = img;
    for (int y = 0; y < dimImg.height(); ++y) {
        for (int x = 0; x < dimImg.width(); ++x) {
            QColor c = dimImg.pixelColor(x, y);
            if (c.alpha() > 0) {
                c.setRed(72); c.setGreen(79); c.setBlue(88); // #484f58
                dimImg.setPixelColor(x, y, c);
            }
        }
    }
    icon.addPixmap(QPixmap::fromImage(dimImg), QIcon::Disabled);
    
    return icon;
}

void MainWindow::setupUi()
{
    qDebug() << "setupUi: root";
    auto* central = new QWidget(this);
    central->setObjectName("centralRoot");
    // Transparent root for rounded corners
    central->setStyleSheet("#centralRoot { background: transparent; }");
    globalVBox_ = new QVBoxLayout(central);
    globalVBox_->setContentsMargins(10,10,10,10);
    globalVBox_->setSpacing(0);

    // Main container with rounded corners and shadow effect
    windowCard_ = new QWidget(central);
    windowCard_->setObjectName("WindowCard");
    windowCard_->setStyleSheet(R"(
        #WindowCard {
            background: #0d1117;
            border: 1px solid #30363d;
            border-radius: 12px;
        }
    )");
    auto* cardVBox = new QVBoxLayout(windowCard_);
    cardVBox->setContentsMargins(0,0,0,0);
    cardVBox->setSpacing(0);

    // Invisible resize border overlay for visual feedback
    resizeBorder_ = new QWidget(windowCard_);
    resizeBorder_->setObjectName("ResizeBorder");
    resizeBorder_->setStyleSheet(R"(
        #ResizeBorder {
            background: transparent;
            border: 2px solid transparent;
            border-radius: 12px;
        }
        #ResizeBorder:hover {
            border: 2px solid rgba(88, 166, 255, 0.3);
        }
    )");
    resizeBorder_->setAttribute(Qt::WA_TransparentForMouseEvents, false);
    resizeBorder_->raise();

    // ---------------- Custom Title Bar -----------------
    titleBar_ = new QWidget(windowCard_);
    titleBar_->setObjectName("TitleBar");
    titleBar_->setFixedHeight(48);
    titleBar_->setStyleSheet(R"(
        #TitleBar {
            background: #161b22;
            border-bottom: 1px solid #21262d;
            border-top-left-radius: 12px;
            border-top-right-radius: 12px;
        }
        #TitleBar QLabel#WindowTitleLabel {
            color: #c9d1d9;
            font-size: 14px;
            font-weight: 600;
            padding-left: 16px;
        }
        #TitleBar QPushButton {
            background: transparent;
            border: none;
            color: #8b949e;
            font-size: 16px;
            font-weight: 400;
            min-width: 46px;
            max-width: 46px;
            min-height: 48px;
            max-height: 48px;
            padding: 0px;
            border-radius: 0px;
        }
        #TitleBar QPushButton:hover { 
            background: rgba(255, 255, 255, 0.08); 
            color: #c9d1d9; 
        }
        #TitleBar QPushButton#MaxBtn {
            border-top-right-radius: 0px;
        }
        #TitleBar QPushButton#CloseBtn { 
            border-top-right-radius: 12px;
        }
        #TitleBar QPushButton#CloseBtn:hover { 
            background: #da3633; 
            color: #ffffff; 
        }
    )");
    auto* titleLayout = new QHBoxLayout(titleBar_);
    titleLayout->setContentsMargins(0,0,0,0);
    titleLayout->setSpacing(0);
    titleLabel_ = new QLabel("DevTools Manager", titleBar_);
    titleLabel_->setObjectName("WindowTitleLabel");
    titleLayout->addWidget(titleLabel_, 0, Qt::AlignVCenter);
    titleLayout->addStretch(1);
    
    // Modern window control buttons with proper symbols
    btnWinMin_ = new QPushButton("−", titleBar_); 
    btnWinMin_->setToolTip("Minimize");
    
    btnWinMax_ = new QPushButton(isMaximized() ? "❐" : "□", titleBar_); 
    btnWinMax_->setObjectName("MaxBtn");
    btnWinMax_->setToolTip("Maximize/Restore");
    
    btnWinClose_ = new QPushButton("✕", titleBar_); 
    btnWinClose_->setObjectName("CloseBtn"); 
    btnWinClose_->setToolTip("Close");
    
    titleLayout->addWidget(btnWinMin_);
    titleLayout->addWidget(btnWinMax_);
    titleLayout->addWidget(btnWinClose_);

    connect(btnWinMin_, &QPushButton::clicked, this, []{ QApplication::activeWindow()->showMinimized(); });
    connect(btnWinMax_, &QPushButton::clicked, this, [this]{
        if (isMaximized()) { 
            showNormal(); 
            btnWinMax_->setText("□"); 
            updateWindowStyle(false);
        }
        else { 
            showMaximized(); 
            btnWinMax_->setText("❐"); 
            updateWindowStyle(true);
        }
    });
    connect(btnWinClose_, &QPushButton::clicked, this, [this]{ close(); });

    // Mouse events for drag
    titleBar_->installEventFilter(this);

    // ---------------- Body Layout (Sidebar + Content) -----------------
    auto* bodyWrapper = new QWidget(windowCard_);
    auto* root = new QHBoxLayout(bodyWrapper);
    root->setContentsMargins(0,0,0,0);
    root->setSpacing(0);

    qDebug() << "setupUi: sidebarBox/layout";
    sidebarBox_ = new QWidget;
    sidebarBox_->setStyleSheet(R"(
        QWidget {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #161b22, stop:1 #0d1117);
            border-right: 1px solid #21262d;
            min-width: 280px;
            max-width: 280px;
        }
    )");
    sidebarLayout_ = new QVBoxLayout(sidebarBox_);
    sidebarLayout_->setContentsMargins(20, 28, 20, 28);
    sidebarLayout_->setSpacing(6);

    qDebug() << "setupUi: logo";
    // Logo container with icon + text
    QWidget* logoContainer = new QWidget;
    logoContainer->setStyleSheet("background: transparent; border: none;");
    auto* logoLayout = new QHBoxLayout(logoContainer);
    logoLayout->setContentsMargins(12, 12, 12, 12);
    logoLayout->setSpacing(12);
    
    // Icon box
    QLabel* iconLabel = new QLabel;
    iconLabel->setStyleSheet(R"(
        background: #0d1117;
        border: 2px solid #1f6feb;
        border-radius: 8px;
        padding: 0px;
        min-width: 40px;
        min-height: 40px;
        max-width: 40px;
        max-height: 40px;
    )");
    iconLabel->setAlignment(Qt::AlignCenter);
    {
        QPixmap logoPixmap(":/symbols/app-logo.png");
        if (!logoPixmap.isNull()) {
            // Convert to white
            QImage img = logoPixmap.toImage().convertToFormat(QImage::Format_ARGB32);
            for (int y = 0; y < img.height(); ++y) {
                for (int x = 0; x < img.width(); ++x) {
                    QColor c = img.pixelColor(x, y);
                    if (c.alpha() > 0) {
                        c.setRed(255); c.setGreen(255); c.setBlue(255);
                        img.setPixelColor(x, y, c);
                    }
                }
            }
            iconLabel->setPixmap(QPixmap::fromImage(img).scaled(36, 36, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
    }
    logoLayout->addWidget(iconLabel);
    
    QLabel* logoLabel = new QLabel("<span style='font-size: 16px; font-weight: 700; color: #c9d1d9;'>DevTools<br/><span style='font-size: 12px; color: #8b949e; font-weight: 500;'>Manager</span></span>");
    logoLabel->setStyleSheet("color: #c9d1d9; border: none; background: transparent;");
    logoLayout->addWidget(logoLabel, 1);
    
    sidebarLayout_->addWidget(logoContainer);
    sidebarLayout_->addSpacing(8);
    
    // Separator line with gradient effect
    QFrame* line1 = new QFrame();
    line1->setFrameShape(QFrame::HLine);
    line1->setStyleSheet(R"(
        background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
            stop:0 transparent, stop:0.5 #30363d, stop:1 transparent);
        max-height: 1px;
        border: none;
    )");
    sidebarLayout_->addWidget(line1);
    sidebarLayout_->addSpacing(12);

    // Navigation section label
    QLabel* navLabel = new QLabel("NAVIGATION");
    navLabel->setStyleSheet(R"(
        color: #6e7681;
        font-size: 11px;
        font-weight: 700;
        letter-spacing: 1px;
        background: transparent;
        border: none;
        padding: 8px 12px 4px 12px;
    )");
    sidebarLayout_->addWidget(navLabel);
    sidebarLayout_->addSpacing(4);

    qDebug() << "setupUi: btnProducts";
    btnProducts_ = new QPushButton("Products");
    {
        QPixmap boxPm(":/symbols/icons8-cardboard-box-48.png");
        if (!boxPm.isNull()) {
            QImage img = boxPm.toImage().convertToFormat(QImage::Format_ARGB32);
            for (int y = 0; y < img.height(); ++y) {
                for (int x = 0; x < img.width(); ++x) {
                    QColor c = img.pixelColor(x, y);
                    if (c.alpha() > 0) {
                        c.setRed(255); c.setGreen(255); c.setBlue(255);
                        img.setPixelColor(x, y, c);
                    }
                }
            }
            btnProducts_->setIcon(QIcon(QPixmap::fromImage(img)));
        }
    }
    btnProducts_->setIconSize(QSize(18,18));
    btnProducts_->setCheckable(true);
    btnProducts_->setChecked(true);
    btnProducts_->setStyleSheet(R"(
        QPushButton {
            background: transparent;
            color: #8b949e;
            border: none;
            border-radius: 8px;
            padding: 10px 14px;
            font-size: 14px;
            font-weight: 600;
            text-align: left;
        }
        QPushButton:checked {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #1f6feb, stop:1 #0d1117);
            color: #ffffff;
            font-weight: 700;
            padding-left: 10px;
            border-radius: 8px;
        }
        QPushButton:hover:!checked {
            background: #21262d;
            color: #c9d1d9;
        }
    )");
    sidebarLayout_->addWidget(btnProducts_);

    qDebug() << "setupUi: btnShowFavorites";
    btnShowFavorites_ = new QPushButton("Favorites");
    // Draw a white star icon for the favorites button
    {
        QPixmap starPm(18,18);
        starPm.fill(Qt::transparent);
        QPainter p(&starPm);
        p.setRenderHint(QPainter::Antialiasing, true);
        p.setBrush(QColor("#ffffff"));
        p.setPen(Qt::NoPen);
        QPointF pts[10];
        const double PI = 3.14159265358979323846;
        double cx=9, cy=9, r1=8, r2=3.8;
        for(int i=0;i<10;++i){
            double ang = PI/2 + i * PI/5;
            double r = (i%2==0)?r1:r2;
            pts[i] = QPointF(cx + r*std::cos(ang), cy - r*std::sin(ang));
        }
        p.drawPolygon(pts,10);
        p.end();
        btnShowFavorites_->setIcon(QIcon(starPm));
    }
    btnShowFavorites_->setIconSize(QSize(18,18));
    btnShowFavorites_->setCheckable(true);
    btnShowFavorites_->setChecked(false);
    btnShowFavorites_->setStyleSheet(R"(
        QPushButton {
            background: transparent;
            color: #8b949e;
            border: none;
            border-radius: 8px;
            padding: 10px 14px;
            font-size: 14px;
            font-weight: 600;
            text-align: left;
        }
        QPushButton:checked {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #1f6feb, stop:1 #0d1117);
            color: #ffffff;
            font-weight: 700;
            padding-left: 10px;
            border-radius: 8px;
        }
        QPushButton:hover:!checked {
            background: #21262d;
            color: #c9d1d9;
        }
    )");
    sidebarLayout_->addWidget(btnShowFavorites_);

    connect(btnProducts_, &QPushButton::clicked, this, &MainWindow::onBtnProducts);
    connect(btnShowFavorites_, &QPushButton::clicked, this, &MainWindow::onBtnFavorites);

    sidebarLayout_->addSpacing(8);

    // Actions section label
    QLabel* actionsLabel = new QLabel("ACTIONS");
    actionsLabel->setStyleSheet(R"(
        color: #6e7681;
        font-size: 11px;
        font-weight: 700;
        letter-spacing: 1px;
        background: transparent;
        border: none;
        padding: 8px 12px 4px 12px;
    )");
    sidebarLayout_->addWidget(actionsLabel);
    sidebarLayout_->addSpacing(4);

    qDebug() << "setupUi: btnDeleteSelected";
    btnDeleteSelected_ = new QPushButton("Delete Selected");
    // Trash icon is actually SVG, use QIcon directly or draw it
    // Since SVG support might not work, let's draw a simple trash icon
    {
        QPixmap trashPm(16, 16);
        trashPm.fill(Qt::transparent);
        QPainter p(&trashPm);
        p.setRenderHint(QPainter::Antialiasing, true);
        p.setPen(QPen(QColor("#ffffff"), 1.5));
        p.setBrush(Qt::NoBrush);
        // Draw trash can
        p.drawRect(4, 6, 8, 8);
        p.drawLine(3, 5, 13, 5);
        p.drawLine(6, 3, 10, 3);
        p.drawLine(6, 3, 6, 5);
        p.drawLine(10, 3, 10, 5);
        p.drawLine(6, 8, 6, 12);
        p.drawLine(8, 8, 8, 12);
        p.drawLine(10, 8, 10, 12);
        p.end();
        btnDeleteSelected_->setIcon(QIcon(trashPm));
    }
    btnDeleteSelected_->setIconSize(QSize(16,16));
    btnDeleteSelected_->setEnabled(false);
    // Ensure icon shows even when disabled
    btnDeleteSelected_->setProperty("iconVisibleInDisabledState", true);
    btnDeleteSelected_->setStyleSheet(R"(
        QPushButton {
            background: transparent;
            color: #f85149;
            border: 1px solid #da3633;
            border-radius: 8px;
            padding: 10px 14px;
            font-size: 13px;
            font-weight: 600;
            text-align: left;
            icon-size: 16px;
        }
        QPushButton:disabled {
            background: transparent;
            color: #484f58;
            border: 1px solid #30363d;
            opacity: 1.0;
        }
        QPushButton:hover:enabled {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #da3633, stop:1 #8b0000);
            color: #ffffff;
            border: 1px solid #f85149;
        }
    )");
    connect(btnDeleteSelected_, &QPushButton::clicked, this, &MainWindow::onDeleteSelectedProductsClicked);
    sidebarLayout_->addWidget(btnDeleteSelected_);
    sidebarLayout_->addSpacing(12);
    
    // Separator line with gradient
    QFrame* line2 = new QFrame();
    line2->setFrameShape(QFrame::HLine);
    line2->setStyleSheet(R"(
        background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
            stop:0 transparent, stop:0.5 #30363d, stop:1 transparent);
        max-height: 1px;
        border: none;
    )");
    sidebarLayout_->addWidget(line2);
    sidebarLayout_->addSpacing(12);

    sidebarLayout_->addStretch(1);

    // Tools section label
    QLabel* toolsLabel = new QLabel("TOOLS");
    toolsLabel->setStyleSheet(R"(
        color: #6e7681;
        font-size: 11px;
        font-weight: 700;
        letter-spacing: 1px;
        background: transparent;
        border: none;
        padding: 8px 12px 4px 12px;
    )");
    sidebarLayout_->addWidget(toolsLabel);
    sidebarLayout_->addSpacing(4);

    qDebug() << "setupUi: btnExport / btnImport";
    btnExport_ = new QPushButton("Export");
    {
        QPixmap trayPm(":/symbols/icons8-tray-30.png");
        if (!trayPm.isNull()) {
            QImage img = trayPm.toImage().convertToFormat(QImage::Format_ARGB32);
            for (int y = 0; y < img.height(); ++y) {
                for (int x = 0; x < img.width(); ++x) {
                    QColor c = img.pixelColor(x, y);
                    if (c.alpha() > 0) {
                        c.setRed(255); c.setGreen(255); c.setBlue(255);
                        img.setPixelColor(x, y, c);
                    }
                }
            }
            btnExport_->setIcon(QIcon(QPixmap::fromImage(img)));
        }
    }
    btnExport_->setIconSize(QSize(16,16));
    btnExport_->setStyleSheet(R"(
        QPushButton {
            background: transparent;
            color: #8b949e;
            border: none;
            border-radius: 8px;
            padding: 10px 14px;
            font-size: 13px;
            font-weight: 600;
            text-align: left;
        }
        QPushButton:hover {
            background: #21262d;
            color: #58a6ff;
        }
    )");
    btnExport_->setToolTip("Export catalog to a JSON file");
    connect(btnExport_, &QPushButton::clicked, this, &MainWindow::onExportProductsClicked);
    sidebarLayout_->addWidget(btnExport_);

    btnImport_ = new QPushButton("Import");
    {
        QPixmap trayPm(":/symbols/icons8-tray-30.png");
        if (!trayPm.isNull()) {
            QImage img = trayPm.toImage().convertToFormat(QImage::Format_ARGB32);
            for (int y = 0; y < img.height(); ++y) {
                for (int x = 0; x < img.width(); ++x) {
                    QColor c = img.pixelColor(x, y);
                    if (c.alpha() > 0) {
                        c.setRed(255); c.setGreen(255); c.setBlue(255);
                        img.setPixelColor(x, y, c);
                    }
                }
            }
            btnImport_->setIcon(QIcon(QPixmap::fromImage(img)));
        }
    }
    btnImport_->setIconSize(QSize(16,16));
    btnImport_->setStyleSheet(R"(
        QPushButton {
            background: transparent;
            color: #8b949e;
            border: none;
            border-radius: 8px;
            padding: 10px 14px;
            font-size: 13px;
            font-weight: 600;
            text-align: left;
        }
        QPushButton:hover {
            background: #21262d;
            color: #58a6ff;
        }
    )");
    btnImport_->setToolTip("Import products from a JSON file");
    connect(btnImport_, &QPushButton::clicked, this, &MainWindow::onImportProductsClicked);
    sidebarLayout_->addWidget(btnImport_);

    // User info card at bottom of sidebar
    sidebarLayout_->addSpacing(16);
    QWidget* sidebarUserCard = new QWidget;
    sidebarUserCard->setStyleSheet(R"(
        QWidget {
            background: #0d1117;
            border: 1px solid #21262d;
            border-radius: 8px;
        }
    )");
    auto* userCardLayout = new QVBoxLayout(sidebarUserCard);
    userCardLayout->setContentsMargins(12, 12, 12, 12);
    userCardLayout->setSpacing(6);
    
    QLabel* userIconLabel = new QLabel;
    userIconLabel->setStyleSheet("background: transparent; border: none;");
    userIconLabel->setAlignment(Qt::AlignCenter);
    {
        QPixmap userPm(":/symbols/icons8-admin-settings-male-48.png");
        if (!userPm.isNull()) {
            QImage img = userPm.toImage().convertToFormat(QImage::Format_ARGB32);
            for (int y = 0; y < img.height(); ++y) {
                for (int x = 0; x < img.width(); ++x) {
                    QColor c = img.pixelColor(x, y);
                    if (c.alpha() > 0) {
                        c.setRed(255); c.setGreen(255); c.setBlue(255);
                        img.setPixelColor(x, y, c);
                    }
                }
            }
            userIconLabel->setPixmap(QPixmap::fromImage(img).scaled(32,32, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
    }
    
    QLabel* sidebarUserLabel = new QLabel(currentUsername_.isEmpty() ? "User" : currentUsername_);
    sidebarUserLabel->setStyleSheet(R"(
        color: #c9d1d9;
        font-size: 13px;
        font-weight: 600;
        background: transparent;
        border: none;
    )");
    sidebarUserLabel->setAlignment(Qt::AlignCenter);
    
    userCardLayout->addWidget(userIconLabel);
    userCardLayout->addWidget(sidebarUserLabel);
    sidebarLayout_->addWidget(sidebarUserCard);

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
    btnAddProduct->setToolTip("Add a new product");
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
    cardVBox->addWidget(titleBar_);
    cardVBox->addWidget(bodyWrapper, 1);
    globalVBox_->addWidget(windowCard_, 1);
    setCentralWidget(central);
    setWindowTitle("DevTools Manager"); // logical title
    
    // Set proper initial size
    resize(1280, 800);
    setMinimumSize(960, 600);
    
    // Apply GitHub-like window styling
    setStyleSheet(styleSheet() + R"(
        QMainWindow { background: transparent; }
        QMessageBox { background: #161b22; color: #c9d1d9; }
        QMessageBox QPushButton {
            background: #21262d; color: #c9d1d9; border: 1px solid #30363d; border-radius: 6px; padding: 5px 16px;
        }
        QMessageBox QPushButton:hover { background: #30363d; }
    )");
    
    // Add subtle drop shadow effect
    if (windowCard_) {
        QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(this);
        shadow->setBlurRadius(30);
        shadow->setXOffset(0);
        shadow->setYOffset(4);
        shadow->setColor(QColor(0, 0, 0, 120));
        windowCard_->setGraphicsEffect(shadow);
    }
    
    qDebug() << "setupUi: FIM";
}

void MainWindow::updateWindowStyle(bool maximized)
{
    if (maximized) {
        // Remove rounded corners when maximized
        globalVBox_->setContentsMargins(0,0,0,0);
        windowCard_->setStyleSheet(R"(
            #WindowCard {
                background: #0d1117;
                border: 1px solid #30363d;
                border-radius: 0px;
            }
        )");
        titleBar_->setStyleSheet(R"(
            #TitleBar {
                background: #161b22;
                border-bottom: 1px solid #21262d;
                border-top-left-radius: 0px;
                border-top-right-radius: 0px;
            }
            #TitleBar QLabel#WindowTitleLabel {
                color: #c9d1d9;
                font-size: 14px;
                font-weight: 600;
                padding-left: 16px;
            }
            #TitleBar QPushButton {
                background: transparent;
                border: none;
                color: #8b949e;
                font-size: 16px;
                font-weight: 400;
                min-width: 46px;
                max-width: 46px;
                min-height: 48px;
                max-height: 48px;
                padding: 0px;
                border-radius: 0px;
            }
            #TitleBar QPushButton:hover { 
                background: rgba(255, 255, 255, 0.08); 
                color: #c9d1d9; 
            }
            #TitleBar QPushButton#MaxBtn {
                border-top-right-radius: 0px;
            }
            #TitleBar QPushButton#CloseBtn { 
                border-top-right-radius: 0px;
            }
            #TitleBar QPushButton#CloseBtn:hover { 
                background: #da3633; 
                color: #ffffff; 
            }
        )");
        if (resizeBorder_) {
            resizeBorder_->setStyleSheet(R"(
                #ResizeBorder {
                    background: transparent;
                    border: 2px solid transparent;
                    border-radius: 0px;
                }
            )");
        }
    } else {
        // Restore rounded corners when normal
        globalVBox_->setContentsMargins(10,10,10,10);
        windowCard_->setStyleSheet(R"(
            #WindowCard {
                background: #0d1117;
                border: 1px solid #30363d;
                border-radius: 12px;
            }
        )");
        titleBar_->setStyleSheet(R"(
            #TitleBar {
                background: #161b22;
                border-bottom: 1px solid #21262d;
                border-top-left-radius: 12px;
                border-top-right-radius: 12px;
            }
            #TitleBar QLabel#WindowTitleLabel {
                color: #c9d1d9;
                font-size: 14px;
                font-weight: 600;
                padding-left: 16px;
            }
            #TitleBar QPushButton {
                background: transparent;
                border: none;
                color: #8b949e;
                font-size: 16px;
                font-weight: 400;
                min-width: 46px;
                max-width: 46px;
                min-height: 48px;
                max-height: 48px;
                padding: 0px;
                border-radius: 0px;
            }
            #TitleBar QPushButton:hover { 
                background: rgba(255, 255, 255, 0.08); 
                color: #c9d1d9; 
            }
            #TitleBar QPushButton#MaxBtn {
                border-top-right-radius: 0px;
            }
            #TitleBar QPushButton#CloseBtn { 
                border-top-right-radius: 12px;
            }
            #TitleBar QPushButton#CloseBtn:hover { 
                background: #da3633; 
                color: #ffffff; 
            }
        )");
        if (resizeBorder_) {
            resizeBorder_->setStyleSheet(R"(
                #ResizeBorder {
                    background: transparent;
                    border: 2px solid transparent;
                    border-radius: 12px;
                }
            )");
        }
    }
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
        if (stockStatus == "In stock")
            matchesStock = product.getQuantity() > 5;
        else if (stockStatus == "Low stock")
            matchesStock = product.getQuantity() > 0 && product.getQuantity() <= 5;
        else if (stockStatus == "Out of stock")
            matchesStock = product.getQuantity() == 0;

        // Toggle logic for Favorites/Products:
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
        filterCategoryBox_->addItem("All Categories");
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

    QPushButton* favBtn = new QPushButton;
    favBtn->setCheckable(true);
    favBtn->setChecked(favoriteProductIds_.contains(product.getId()));
    favBtn->setCursor(Qt::PointingHandCursor);
    favBtn->setToolTip(favBtn->isChecked() ? "Unfavorite" : "Favorite");
    favBtn->setStyleSheet(R"(
        QPushButton {
            background: transparent;
            border: none;
            min-width: 24px;
            min-height: 24px;
            padding: 0px;
        }
        QPushButton:hover { opacity: 0.8; }
    )");
    auto updateFavIcon = [favBtn]() {
        // Draw star programmatically in yellow (checked) or dim white (unchecked)
        QPixmap starPm(20,20);
        starPm.fill(Qt::transparent);
        QPainter p(&starPm);
        p.setRenderHint(QPainter::Antialiasing, true);
        QColor fill = favBtn->isChecked() ? QColor("#f1e05a") : QColor(255,255,255,80); // white dim when unchecked
        p.setBrush(fill);
        p.setPen(Qt::NoPen);
        // Draw 5-point star
        QPointF pts[10];
        const double PI = 3.14159265358979323846;
        double cx=10, cy=10, r1=9, r2=4.2;
        for(int i=0;i<10;++i){
            double ang = PI/2 + i * PI/5;
            double r = (i%2==0)?r1:r2;
            pts[i] = QPointF(cx + r*std::cos(ang), cy - r*std::sin(ang));
        }
        p.drawPolygon(pts,10);
        p.end();
        favBtn->setIcon(QIcon(starPm));
        favBtn->setIconSize(QSize(20,20));
    };
    updateFavIcon();
    connect(favBtn, &QPushButton::clicked, this, [this, product, favBtn, updateFavIcon]() {
        bool favorited = favBtn->isChecked();
        if (favorited) favoriteProductIds_.insert(product.getId());
        else favoriteProductIds_.remove(product.getId());
        favBtn->setToolTip(favorited ? "Unfavorite" : "Favorite");
        updateFavIcon();
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
        font-size: 24px;
    )");
    icon->setAlignment(Qt::AlignCenter);
    
    // Load product image if it exists
    QString imagePath = product.getImagePath();
    if (!imagePath.isEmpty() && QFile::exists(imagePath)) {
        QPixmap pixmap(imagePath);
        if (!pixmap.isNull()) {
            icon->setPixmap(pixmap.scaled(48, 48, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            icon->setScaledContents(true);
        } else {
            QPixmap boxPm(":/symbols/icons8-cardboard-box-48.png");
            if (!boxPm.isNull()) {
                QImage img = boxPm.toImage().convertToFormat(QImage::Format_ARGB32);
                for (int y = 0; y < img.height(); ++y) {
                    for (int x = 0; x < img.width(); ++x) {
                        QColor c = img.pixelColor(x, y);
                        if (c.alpha() > 0) {
                            c.setRed(255); c.setGreen(255); c.setBlue(255);
                            img.setPixelColor(x, y, c);
                        }
                    }
                }
                icon->setPixmap(QPixmap::fromImage(img).scaled(32,32, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            }
        }
    } else {
        QPixmap boxPm(":/symbols/icons8-cardboard-box-48.png");
        if (!boxPm.isNull()) {
            QImage img = boxPm.toImage().convertToFormat(QImage::Format_ARGB32);
            for (int y = 0; y < img.height(); ++y) {
                for (int x = 0; x < img.width(); ++x) {
                    QColor c = img.pixelColor(x, y);
                    if (c.alpha() > 0) {
                        c.setRed(255); c.setGreen(255); c.setBlue(255);
                        img.setPixelColor(x, y, c);
                    }
                }
            }
            icon->setPixmap(QPixmap::fromImage(img).scaled(32,32, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
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
    // Title bar drag & double click
    if (obj == titleBar_) {
        if (event->type() == QEvent::MouseButtonPress) {
            auto* me = static_cast<QMouseEvent*>(event);
            if (me->button() == Qt::LeftButton) {
                draggingWindow_ = true;
                dragOffset_ = me->globalPosition().toPoint() - frameGeometry().topLeft();
                return true;
            }
        } else if (event->type() == QEvent::MouseMove && draggingWindow_) {
            auto* me = static_cast<QMouseEvent*>(event);
            if (me->buttons() & Qt::LeftButton) {
                move(me->globalPosition().toPoint() - dragOffset_);
                return true;
            }
        } else if (event->type() == QEvent::MouseButtonRelease) {
            draggingWindow_ = false;
        } else if (event->type() == QEvent::MouseButtonDblClick) {
            if (isMaximized()) {
                showNormal();
                if (btnWinMax_) btnWinMax_->setText("□");
                updateWindowStyle(false);
            } else {
                showMaximized();
                if (btnWinMax_) btnWinMax_->setText("❐");
                updateWindowStyle(true);
            }
            return true;
        }
    }

    // Keep popup width equal to combo width
    if ((obj == filterCategoryBox_ || obj == filterStockBox_) &&
        (event->type() == QEvent::Resize || event->type() == QEvent::Show || event->type() == QEvent::ShowToParent)) {
        if (auto* combo = qobject_cast<QComboBox*>(obj); combo && combo->view()) {
            combo->view()->setFixedWidth(combo->width());
        }
    }

    // Ctrl+A select all
    if (event->type() == QEvent::KeyPress) {
        auto* keyEvent = static_cast<QKeyEvent*>(event);
#ifdef Q_OS_MAC
        bool isCommandA = keyEvent->key() == Qt::Key_A && (keyEvent->modifiers() & Qt::MetaModifier);
#else
        bool isCommandA = keyEvent->key() == Qt::Key_A && (keyEvent->modifiers() & Qt::ControlModifier);
#endif
        if (isCommandA) {
            selectedProductIds_.clear();
            for (const Product &p : filteredProducts_) selectedProductIds_.insert(p.getId());
            updateDeleteSelectedButtonState();
            displayProducts();
            return true;
        }
    }

    // Product card double-click edit
    if (event->type() == QEvent::MouseButtonDblClick) {
        if (auto* card = qobject_cast<QWidget*>(obj); card && card->property("editId").isValid()) {
            onEditProductRequested(card->property("editId").toString());
            return true;
        }
    }

    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    // Update resize border to match window size
    if (resizeBorder_ && windowCard_) {
        resizeBorder_->setGeometry(windowCard_->rect());
    }
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        resizeEdge_ = getResizeEdge(event->pos());
        if (resizeEdge_ != Qt::Edges()) {
            resizing_ = true;
            resizeOffset_ = event->globalPosition().toPoint();
            event->accept();
            return;
        }
    }
    QMainWindow::mousePressEvent(event);
}

void MainWindow::mouseMoveEvent(QMouseEvent* event)
{
    if (resizing_ && resizeEdge_ != Qt::Edges()) {
        QPoint delta = event->globalPosition().toPoint() - resizeOffset_;
        resizeOffset_ = event->globalPosition().toPoint();
        
        QRect geo = geometry();
        
        if (resizeEdge_ & Qt::LeftEdge) {
            geo.setLeft(geo.left() + delta.x());
        }
        if (resizeEdge_ & Qt::RightEdge) {
            geo.setRight(geo.right() + delta.x());
        }
        if (resizeEdge_ & Qt::TopEdge) {
            geo.setTop(geo.top() + delta.y());
        }
        if (resizeEdge_ & Qt::BottomEdge) {
            geo.setBottom(geo.bottom() + delta.y());
        }
        
        // Apply minimum size constraints
        if (geo.width() < minimumWidth()) {
            if (resizeEdge_ & Qt::LeftEdge) {
                geo.setLeft(geo.right() - minimumWidth());
            } else {
                geo.setRight(geo.left() + minimumWidth());
            }
        }
        if (geo.height() < minimumHeight()) {
            if (resizeEdge_ & Qt::TopEdge) {
                geo.setTop(geo.bottom() - minimumHeight());
            } else {
                geo.setBottom(geo.top() + minimumHeight());
            }
        }
        
        setGeometry(geo);
        event->accept();
        return;
    }
    
    // Update cursor shape when hovering over edges
    if (!resizing_) {
        Qt::Edges edges = getResizeEdge(event->pos());
        updateCursorShape(edges);
    }
    
    QMainWindow::mouseMoveEvent(event);
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton && resizing_) {
        resizing_ = false;
        resizeEdge_ = Qt::Edges();
        setCursor(Qt::ArrowCursor);
        event->accept();
        return;
    }
    QMainWindow::mouseReleaseEvent(event);
}

Qt::Edges MainWindow::getResizeEdge(const QPoint& pos)
{
    const int edgeMargin = 10; // increased for better usability
    Qt::Edges edges;
    
    if (pos.x() <= edgeMargin) edges |= Qt::LeftEdge;
    if (pos.x() >= width() - edgeMargin) edges |= Qt::RightEdge;
    if (pos.y() <= edgeMargin) edges |= Qt::TopEdge;
    if (pos.y() >= height() - edgeMargin) edges |= Qt::BottomEdge;
    
    return edges;
}

void MainWindow::updateCursorShape(Qt::Edges edges)
{
    // Update resize border visual feedback
    if (resizeBorder_) {
        if (edges != Qt::Edges()) {
            resizeBorder_->setStyleSheet(R"(
                #ResizeBorder {
                    background: transparent;
                    border: 2px solid rgba(88, 166, 255, 0.5);
                    border-radius: 12px;
                }
            )");
        } else {
            resizeBorder_->setStyleSheet(R"(
                #ResizeBorder {
                    background: transparent;
                    border: 2px solid transparent;
                    border-radius: 12px;
                }
            )");
        }
    }
    
    // Update cursor
    if (edges == (Qt::LeftEdge | Qt::TopEdge) || edges == (Qt::RightEdge | Qt::BottomEdge)) {
        setCursor(Qt::SizeFDiagCursor);
    } else if (edges == (Qt::RightEdge | Qt::TopEdge) || edges == (Qt::LeftEdge | Qt::BottomEdge)) {
        setCursor(Qt::SizeBDiagCursor);
    } else if (edges & (Qt::LeftEdge | Qt::RightEdge)) {
        setCursor(Qt::SizeHorCursor);
    } else if (edges & (Qt::TopEdge | Qt::BottomEdge)) {
        setCursor(Qt::SizeVerCursor);
    } else {
        setCursor(Qt::ArrowCursor);
    }
}

void MainWindow::onEditProductRequested(const QString& productId)
{
    Product* toEdit = nullptr;
    for (Product& p : allProducts_)
        if (p.getId() == productId) { toEdit = &p; break; }
    if (!toEdit) return;

    QStringList categories;
    for (const Product& prod : allProducts_)
        categories << prod.getCategory();
    categories.removeDuplicates();

    AddProductDialog dialog(*toEdit, categories, this);
    if (dialog.exec() == QDialog::Accepted) {
        Product updated = dialog.getProduct();
        *toEdit = updated;
        showToast("Product updated!");
        logOperation("Edited", updated);
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
                logOperation("Deleted", allProducts_[i]);
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
    QStringList categories;
    for (const Product& p : allProducts_)
        categories << p.getCategory();
    categories.removeDuplicates();

    AddProductDialog dialog(categories, this);

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
        logOperation("Added", newProduct);
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
