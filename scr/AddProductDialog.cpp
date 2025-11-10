#include "AddProductDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QPushButton>
#include <QMessageBox>
#include <QStringList>
#include <QFileDialog>
#include <QPixmap>
#include <QGraphicsDropShadowEffect>
#include <QMouseEvent>
#include <QApplication>

AddProductDialog::AddProductDialog(const QStringList& categoriasUnicas, QWidget* parent)
    : QDialog(parent), isEditMode_(false) {
    // Match application look & feel
    setWindowFlag(Qt::FramelessWindowHint, true);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setupUi(categoriasUnicas);
    setWindowTitle("Add New Product");
}

AddProductDialog::AddProductDialog(const Product& product, const QStringList& categoriasUnicas, QWidget* parent)
    : QDialog(parent), isEditMode_(true), originalProduct_(product) {
    // Match application look & feel
    setWindowFlag(Qt::FramelessWindowHint, true);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setupUi(categoriasUnicas);
    populateFields(product);
    setWindowTitle("Edit Product");
    idEdit_->setEnabled(false);
}

void AddProductDialog::setupUi(const QStringList& categoriasUnicas) {
    setModal(true);
    setMinimumWidth(560);

    // Root transparent layout
    auto* rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(12,12,12,12);
    rootLayout->setSpacing(0);

    // Card container (match MainWindow)
    card_ = new QWidget(this);
    card_->setObjectName("AddCard");
    card_->setStyleSheet(R"(
        #AddCard { background:#0d1117; border:1px solid #30363d; border-radius:12px; }
    )");
    auto* cardVBox = new QVBoxLayout(card_);
    cardVBox->setContentsMargins(0,0,0,0);
    cardVBox->setSpacing(0);

    // Title bar (drag + close)
    titleBar_ = new QWidget(card_);
    titleBar_->setObjectName("TitleBar");
    titleBar_->setFixedHeight(46);
    titleBar_->setStyleSheet(R"(
        #TitleBar { background:#161b22; border-bottom:1px solid #21262d; border-top-left-radius:12px; border-top-right-radius:12px; }
        #TitleBar QLabel#TitleLabel { color:#c9d1d9; font-size:14px; font-weight:600; padding-left:16px; }
        #TitleBar QPushButton { background:transparent; border:none; color:#8b949e; min-width:46px; max-width:46px; min-height:46px; max-height:46px; }
        #TitleBar QPushButton:hover { background:rgba(255,255,255,0.08); color:#c9d1d9; }
        #TitleBar QPushButton#CloseBtn:hover { background:#da3633; color:#fff; }
    )");
    auto* tbLayout = new QHBoxLayout(titleBar_);
    tbLayout->setContentsMargins(0,0,0,0);
    tbLayout->setSpacing(0);
    titleLabel_ = new QLabel(isEditMode_ ? "Edit Product" : "Add Product", titleBar_);
    titleLabel_->setObjectName("TitleLabel");
    tbLayout->addWidget(titleLabel_, 0, Qt::AlignVCenter);
    tbLayout->addStretch(1);
    auto* closeBtn = new QPushButton("✕", titleBar_);
    closeBtn->setObjectName("CloseBtn");
    QObject::connect(closeBtn, &QPushButton::clicked, this, &AddProductDialog::onCancelClicked);
    tbLayout->addWidget(closeBtn);
    titleBar_->installEventFilter(this);

    // Content area
    contentRoot_ = new QWidget(card_);
    auto* mainLayout = new QVBoxLayout(contentRoot_);
    mainLayout->setSpacing(16);
    mainLayout->setContentsMargins(24, 24, 24, 24);

    // Header chip (optional visual)
    QLabel* chip = new QLabel(isEditMode_ ? "EDIT MODE" : "NEW PRODUCT");
    chip->setStyleSheet(R"(
        QLabel { background:#161b22; border:1px solid #21262d; color:#8b949e; border-radius:12px; font-size:11px; font-weight:800; padding:4px 10px; letter-spacing:1px; }
    )");
    chip->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(chip, 0, Qt::AlignLeft);

    // Form styles
    auto labelStyle = QString("color:#8b949e; font-size:12px; font-weight:600; background:transparent; border:none; letter-spacing:0.4px;");
    auto fieldStyle = QString(R"(
        QLineEdit, QComboBox, QSpinBox, QDoubleSpinBox {
            background:#0d1117; color:#c9d1d9; border:1px solid #30363d; border-radius:6px; padding:8px 12px; font-size:14px;
        }
        QLineEdit:focus, QComboBox:focus, QSpinBox:focus, QDoubleSpinBox:focus { border:1px solid #58a6ff; }
        QComboBox::drop-down { width: 18px; border:none; }
        QComboBox::down-arrow { image: none; border-left:4px solid transparent; border-right:4px solid transparent; border-top:4px solid #8b949e; width:0; height:0; }
        QComboBox QAbstractItemView { background:#161b22; border:1px solid #30363d; border-radius:6px; outline:0; selection-background-color:#21262d; }
    )");
    setStyleSheet(styleSheet() + fieldStyle);

    // Formulário principal
    auto* formLayout = new QVBoxLayout;
    formLayout->setSpacing(18);

    auto formRow = [&](const QString& text, QWidget* field) {
        auto* row = new QHBoxLayout;
        row->setSpacing(13);
        QLabel* label = new QLabel(text);
        label->setObjectName("FormLabel");
        row->addWidget(label, 0, Qt::AlignVCenter);
        row->addWidget(field, 1, Qt::AlignVCenter);
        formLayout->addLayout(row);
    };

    idEdit_ = new QLineEdit;
    idEdit_->setPlaceholderText("Ex: 001");
    formRow("ID:", idEdit_);

    nameEdit_ = new QLineEdit;
    nameEdit_->setPlaceholderText("Ex: Gaming Mouse RGB");
    formRow("Name:", nameEdit_);

    categoryCombo_ = new QComboBox;
    categoryCombo_->addItems(categoriasUnicas);
    categoryCombo_->setEditable(true);
    formRow("Category:", categoryCombo_);

    priceSpinBox_ = new QDoubleSpinBox;
    priceSpinBox_->setPrefix("€ ");
    priceSpinBox_->setRange(0.01, 999999.99);
    priceSpinBox_->setDecimals(2);
    priceSpinBox_->setValue(0.01);
    formRow("Price:", priceSpinBox_);

    quantitySpinBox_ = new QSpinBox;
    quantitySpinBox_->setRange(0, 999999);
    quantitySpinBox_->setValue(0);
    quantitySpinBox_->setSuffix(" units");
    formRow("Quantity:", quantitySpinBox_);

    // Image upload section
    auto* imageRow = new QHBoxLayout;
    imageRow->setSpacing(13);
    QLabel* imageLabel = new QLabel("Image:");
    imageLabel->setObjectName("FormLabel");
    imageRow->addWidget(imageLabel, 0, Qt::AlignVCenter);
    
    auto* imageContainer = new QVBoxLayout;
    imageContainer->setSpacing(8);
    
    btnSelectImage_ = new QPushButton("Select Image");
    {
        QPixmap camPm(":/symbols/icons8-camera-50.png");
        if (!camPm.isNull()) {
            QImage img = camPm.toImage().convertToFormat(QImage::Format_ARGB32);
            for (int y = 0; y < img.height(); ++y) {
                for (int x = 0; x < img.width(); ++x) {
                    QColor c = img.pixelColor(x, y);
                    if (c.alpha() > 0) {
                        c.setRed(255); c.setGreen(255); c.setBlue(255);
                        img.setPixelColor(x, y, c);
                    }
                }
            }
            btnSelectImage_->setIcon(QIcon(QPixmap::fromImage(img)));
        }
    }
    btnSelectImage_->setIconSize(QSize(16,16));
    btnSelectImage_->setStyleSheet(R"(
        QPushButton { background:transparent; color:#8b949e; border:1px solid #30363d; border-radius:6px; padding:8px 12px; font-size:13px; font-weight:600; }
        QPushButton:hover { background:#21262d; color:#c9d1d9; }
    )");
    connect(btnSelectImage_, &QPushButton::clicked, this, &AddProductDialog::onSelectImageClicked);
    imageContainer->addWidget(btnSelectImage_);
    
    imagePreviewLabel_ = new QLabel("No image selected");
    imagePreviewLabel_->setStyleSheet(R"(
        QLabel { background:#161b22; border:1px dashed #30363d; border-radius:8px; padding:16px; color:#8b949e; font-size:13px; min-height:100px; }
    )");
    imagePreviewLabel_->setAlignment(Qt::AlignCenter);
    imagePreviewLabel_->setScaledContents(false);
    imagePreviewLabel_->setMaximumHeight(120);
    imageContainer->addWidget(imagePreviewLabel_);
    
    imageRow->addLayout(imageContainer, 1);
    formLayout->addLayout(imageRow);

    // Stock adjustment (edit mode only)
    stockAdjustSpinBox_ = nullptr;
    if (isEditMode_) {
        auto* stockGroup = new QFrame;
        stockGroup->setObjectName("stockAdjustGroup");
        auto* stockGroupLayout = new QHBoxLayout(stockGroup);
        stockGroupLayout->setContentsMargins(16, 8, 16, 8);
        stockGroupLayout->setSpacing(10);
    QLabel* stockIconLabel = new QLabel;
        stockIconLabel->setStyleSheet("background: transparent;");
        QPixmap rotPm(":/symbols/icons8-rotate-right-48.png");
        if (!rotPm.isNull()) {
            QImage img = rotPm.toImage().convertToFormat(QImage::Format_ARGB32);
            for (int y = 0; y < img.height(); ++y) {
                for (int x = 0; x < img.width(); ++x) {
                    QColor c = img.pixelColor(x, y);
                    if (c.alpha() > 0) {
                        c.setRed(255); c.setGreen(255); c.setBlue(255);
                        img.setPixelColor(x, y, c);
                    }
                }
            }
            stockIconLabel->setPixmap(QPixmap::fromImage(img).scaled(18,18, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
        stockAdjustSpinBox_ = new QSpinBox;
        stockAdjustSpinBox_->setRange(-999999, 999999);
        stockAdjustSpinBox_->setValue(0);
        stockAdjustSpinBox_->setSuffix(" units");
        QLabel* stockLabel = new QLabel("Adjust Stock");
        stockLabel->setObjectName("stockLabel");
        stockGroup->setStyleSheet(R"(
            QFrame#stockAdjustGroup { background:#161b22; border:1px solid #21262d; border-radius:8px; }
            QLabel#stockLabel { font-size: 12px; color:#8b949e; font-weight:600; margin-left:8px; }
        )");
        stockGroupLayout->addWidget(stockIconLabel, 0, Qt::AlignLeft);
        stockGroupLayout->addWidget(stockAdjustSpinBox_, 0, Qt::AlignLeft);
        stockGroupLayout->addWidget(stockLabel, 0, Qt::AlignLeft);
        stockGroupLayout->addStretch(1);
        formLayout->addWidget(stockGroup);

        connect(stockAdjustSpinBox_, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](int value) {
            QString msg;
            if (value > 0) msg = QString("Add %1 to stock").arg(value);
            if (value < 0) msg = QString("Remove %1 from stock").arg(-value);
            if (value == 0) msg = QString("No stock change");
            stockAdjustSpinBox_->setToolTip(msg);
        });
    }

    mainLayout->addLayout(formLayout);
    mainLayout->addSpacing(8);

    // Action buttons
    auto* buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch();
    btnCancel_ = new QPushButton("Cancel");
    btnCancel_->setStyleSheet(R"(
        QPushButton { background:transparent; color:#8b949e; border:1px solid #30363d; border-radius:6px; padding:8px 16px; font-size:13px; font-weight:500; }
        QPushButton:hover { background:#21262d; color:#c9d1d9; }
    )");
    QObject::connect(btnCancel_, &QPushButton::clicked, this, &AddProductDialog::onCancelClicked);
    buttonsLayout->addWidget(btnCancel_);

    btnSave_ = new QPushButton(isEditMode_ ? "Save Changes" : "Add Product");
    btnSave_->setStyleSheet(R"(
        QPushButton { background:#238636; color:#ffffff; border:none; border-radius:6px; padding:8px 16px; font-size:13px; font-weight:600; }
        QPushButton:hover { background:#2ea043; }
    )");
    QObject::connect(btnSave_, &QPushButton::clicked, this, &AddProductDialog::onSaveClicked);
    buttonsLayout->addWidget(btnSave_);
    mainLayout->addLayout(buttonsLayout);

    connect(nameEdit_, &QLineEdit::textChanged, this, &AddProductDialog::validateFields);
    connect(idEdit_, &QLineEdit::textChanged, this, &AddProductDialog::validateFields);

    validateFields();

    // Assemble card and shadow
    cardVBox->addWidget(titleBar_);
    cardVBox->addWidget(contentRoot_);
    rootLayout->addWidget(card_);
    auto* shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(28);
    shadow->setXOffset(0);
    shadow->setYOffset(4);
    shadow->setColor(QColor(0,0,0,120));
    card_->setGraphicsEffect(shadow);
}

void AddProductDialog::populateFields(const Product& product) {
    idEdit_->setText(product.getId());
    nameEdit_->setText(product.getName());
    int index = categoryCombo_->findText(product.getCategory());
    if (index >= 0)
        categoryCombo_->setCurrentIndex(index);
    else
        categoryCombo_->setEditText(product.getCategory());
    priceSpinBox_->setValue(product.getPrice());
    quantitySpinBox_->setValue(product.getQuantity());
    
    // Load image if exists
    imagePath_ = product.getImagePath();
    if (!imagePath_.isEmpty() && QFile::exists(imagePath_)) {
        QPixmap pixmap(imagePath_);
        if (!pixmap.isNull()) {
            imagePreviewLabel_->setPixmap(pixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            imagePreviewLabel_->setText("");
            btnSelectImage_->setText(" Change Image");
        }
    }
}

void AddProductDialog::validateFields() {
    bool valid = !idEdit_->text().trimmed().isEmpty() && !nameEdit_->text().trimmed().isEmpty();
    btnSave_->setEnabled(valid);
}

void AddProductDialog::onSelectImageClicked() {
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Select Product Image",
        "",
        "Image Files (*.png *.jpg *.jpeg *.bmp *.gif);;All Files (*)"
    );
    
    if (!fileName.isEmpty()) {
        QPixmap pixmap(fileName);
        if (!pixmap.isNull()) {
            imagePath_ = fileName;
            imagePreviewLabel_->setPixmap(pixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            imagePreviewLabel_->setText("");
            btnSelectImage_->setText(" Change Image");
        } else {
            QMessageBox::warning(this, "Error", "Could not load the selected image file!");
        }
    }
}

void AddProductDialog::onSaveClicked() {
    if (idEdit_->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Error", "Product ID is required!");
        idEdit_->setFocus();
        return;
    }
    if (nameEdit_->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Error", "Product name is required!");
        nameEdit_->setFocus();
        return;
    }
    if (priceSpinBox_->value() <= 0) {
        QMessageBox::warning(this, "Error", "Price must be greater than zero!");
        priceSpinBox_->setFocus();
        return;
    }

    if (isEditMode_ && stockAdjustSpinBox_) {
        int adjustment = stockAdjustSpinBox_->value();
        int newStock = quantitySpinBox_->value() + adjustment;
        if (newStock < 0) {
            QMessageBox::warning(this, "Error", "Stock cannot be negative after adjustment!");
            stockAdjustSpinBox_->setFocus();
            return;
        }
        quantitySpinBox_->setValue(newStock);
    }

    accept();
}

void AddProductDialog::onCancelClicked() {
    reject();
}

Product AddProductDialog::getProduct() const {
    Product product(
        idEdit_->text().trimmed(),
        nameEdit_->text().trimmed(),
        categoryCombo_->currentText().trimmed(),
        priceSpinBox_->value(),
        quantitySpinBox_->value()
    );
    product.setImagePath(imagePath_);
    return product;
}

// Drag handling for custom title bar
bool AddProductDialog::eventFilter(QObject* obj, QEvent* event) {
    if (obj == titleBar_) {
        if (event->type() == QEvent::MouseButtonPress) {
            auto* me = static_cast<QMouseEvent*>(event);
            if (me->button() == Qt::LeftButton) {
                dragging_ = true;
                dragOffset_ = me->globalPosition().toPoint() - frameGeometry().topLeft();
                return true;
            }
        } else if (event->type() == QEvent::MouseMove && dragging_) {
            auto* me = static_cast<QMouseEvent*>(event);
            if (me->buttons() & Qt::LeftButton) {
                move(me->globalPosition().toPoint() - dragOffset_);
                return true;
            }
        } else if (event->type() == QEvent::MouseButtonRelease) {
            dragging_ = false;
        }
    }
    return QDialog::eventFilter(obj, event);
}

void AddProductDialog::setInitialId(const QString& id) {
    if (!isEditMode_ && idEdit_ && idEdit_->text().trimmed().isEmpty()) {
        idEdit_->setText(id);
    }
}
