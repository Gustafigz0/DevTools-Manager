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

AddProductDialog::AddProductDialog(const QStringList& categoriasUnicas, QWidget* parent)
    : QDialog(parent), isEditMode_(false) {
    setupUi(categoriasUnicas);
    setWindowTitle("Add New Product");
}

AddProductDialog::AddProductDialog(const Product& product, const QStringList& categoriasUnicas, QWidget* parent)
    : QDialog(parent), isEditMode_(true), originalProduct_(product) {
    setupUi(categoriasUnicas);
    populateFields(product);
    setWindowTitle("Edit Product");
    idEdit_->setEnabled(false);
}

void AddProductDialog::setupUi(const QStringList& categoriasUnicas) {
    setModal(true);
    setMinimumWidth(540);

    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(24);
    mainLayout->setContentsMargins(36, 26, 36, 26);

    setStyleSheet(R"(
        QDialog {
            background: #1a1a1a;
        }
        QLabel#TitleLabel {
            font-size: 24px;
            font-weight: 700;
            color: #ffffff;
            margin-bottom: 11px;
            letter-spacing: 0.5px;
        }
        QLabel#FormLabel {
            color: #ffffff;
            font-size: 15px;
            font-weight: 700;
            border-radius: 9px;
            padding: 8px 12px 8px 16px;
            min-width: 110px;
        }
        QLineEdit, QComboBox, QSpinBox, QDoubleSpinBox {
            background: #2a2a2a;
            color: #ffffff;
            border: 2px solid #444444;
            border-radius: 12px;
            padding: 13px 18px;
            font-size: 15px;
        }
        QLineEdit:focus, QComboBox:focus, QSpinBox:focus, QDoubleSpinBox:focus {
            border: 2px solid #666666;
        }
        QComboBox QAbstractItemView {
            background: #2a2a2a;
            color: #ffffff;
            border-radius: 9px;
            font-size: 15px;
        }
        QComboBox::drop-down { border: none; width: 0px; }
        QComboBox::down-arrow { image: none; width: 0; }
        QSpinBox::up-button, QSpinBox::down-button,
        QDoubleSpinBox::up-button, QDoubleSpinBox::down-button {
            background: transparent;
            border: none;
            width: 0px;
        }
        QSpinBox::up-arrow, QDoubleSpinBox::up-arrow,
        QSpinBox::down-arrow, QDoubleSpinBox::down-arrow {
            image: none; width: 0; height: 0;
        }
        QPushButton {
            background: #333333;
            color: #ffffff;
            border: none;
            border-radius: 16px;
            padding: 14px 28px;
            font-size: 17px;
            font-weight: 600;
            min-width: 140px;
        }
        QPushButton:hover {
            background: #444444;
        }
        QPushButton:pressed {
            background: #555555;
        }
        QPushButton#cancelButton {
            background: #2a2a2a;
            color: #cccccc;
        }
        QPushButton#cancelButton:hover {
            background: #3a3a3a;
        }
        QLabel#EditChip {
            background: #666666;
            color: #ffffff;
            border-radius: 9px;
            font-weight: 800;
            font-size: 13px;
            padding: 4px 16px;
            margin-bottom: 11px;
            letter-spacing: 1px;
            min-width: 95px;
        }
        QLabel#AddChip {
            background: #444444;
            color: #ffffff;
            border-radius: 9px;
            font-weight: 800;
            font-size: 13px;
            padding: 4px 16px;
            margin-bottom: 11px;
            letter-spacing: 1px;
            min-width: 95px;
        }
        QFrame#stockAdjustGroup {
            background: #2a2a2a;
            border: 2px solid #666666;
            border-radius: 18px;
        }
        QLabel#stockLabel {
            font-size: 15px;
            color: #cccccc;
            font-weight: 700;
            margin-left: 10px;
        }
    )");

    // Chip definido com base no modo
    if (isEditMode_) {
        QLabel* editModeChip = new QLabel("Edit Mode");
        editModeChip->setObjectName("EditChip");
        editModeChip->setAlignment(Qt::AlignCenter);
        mainLayout->addWidget(editModeChip, 0, Qt::AlignCenter);
    } else {
        QLabel* addChip = new QLabel("New Product");
        addChip->setObjectName("AddChip");
        addChip->setAlignment(Qt::AlignCenter);
        mainLayout->addWidget(addChip, 0, Qt::AlignCenter);
    }

    // Cabeçalho visual moderno
    auto* headerWidget = new QWidget;
    headerWidget->setStyleSheet("background: #2a2a2a; border-radius: 18px;");
    auto* headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->setContentsMargins(20, 8, 20, 8);
    headerLayout->setSpacing(10);

    QLabel* iconLabel = new QLabel(isEditMode_ ? "✏" : "+");
    iconLabel->setStyleSheet(isEditMode_
        ? "font-size: 27px; color: #cccccc; font-weight: bold; background: transparent;"
        : "font-size: 27px; color: #ffffff; font-weight: bold; background: transparent;");
    QLabel* titleLabel = new QLabel(isEditMode_ ? "Edit Product" : "Add Product");
    titleLabel->setObjectName("TitleLabel");
    titleLabel->setStyleSheet("font-size: 22px; font-weight: 800; color: #fff; letter-spacing: 1px; background: transparent;");
    headerLayout->addWidget(iconLabel, 0, Qt::AlignVCenter);
    headerLayout->addSpacing(7);
    headerLayout->addWidget(titleLabel, 0, Qt::AlignVCenter);
    headerLayout->addStretch(1);
    mainLayout->addWidget(headerWidget);

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
    formRow("Nome:", nameEdit_);

    categoryCombo_ = new QComboBox;
    categoryCombo_->addItems(categoriasUnicas);
    categoryCombo_->setEditable(true);
    formRow("Categoria:", categoryCombo_);

    priceSpinBox_ = new QDoubleSpinBox;
    priceSpinBox_->setPrefix("€ ");
    priceSpinBox_->setRange(0.01, 999999.99);
    priceSpinBox_->setDecimals(2);
    priceSpinBox_->setValue(0.01);
    formRow("Preço:", priceSpinBox_);

    quantitySpinBox_ = new QSpinBox;
    quantitySpinBox_->setRange(0, 999999);
    quantitySpinBox_->setValue(0);
    quantitySpinBox_->setSuffix(" units");
    formRow("Quantidade:", quantitySpinBox_);

    // Image upload section
    auto* imageRow = new QHBoxLayout;
    imageRow->setSpacing(13);
    QLabel* imageLabel = new QLabel("Imagem:");
    imageLabel->setObjectName("FormLabel");
    imageRow->addWidget(imageLabel, 0, Qt::AlignVCenter);
    
    auto* imageContainer = new QVBoxLayout;
    imageContainer->setSpacing(8);
    
    btnSelectImage_ = new QPushButton("📷 Select Image");
    btnSelectImage_->setStyleSheet(
        "QPushButton { background: #444444; color: #ffffff; border: 2px solid #666666; border-radius: 12px; padding: 10px 16px; font-size: 14px; font-weight: 600; }"
        "QPushButton:hover { background: #555555; border: 2px solid #777777; }"
    );
    connect(btnSelectImage_, &QPushButton::clicked, this, &AddProductDialog::onSelectImageClicked);
    imageContainer->addWidget(btnSelectImage_);
    
    imagePreviewLabel_ = new QLabel("No image selected");
    imagePreviewLabel_->setStyleSheet(
        "QLabel { background: #2a2a2a; border: 2px dashed #444444; border-radius: 12px; padding: 20px; color: #888888; font-size: 14px; min-height: 100px; }"
    );
    imagePreviewLabel_->setAlignment(Qt::AlignCenter);
    imagePreviewLabel_->setScaledContents(true);
    imagePreviewLabel_->setMaximumHeight(120);
    imageContainer->addWidget(imagePreviewLabel_);
    
    imageRow->addLayout(imageContainer, 1);
    formLayout->addLayout(imageRow);

    // Ajuste de stock só modo edição
    stockAdjustSpinBox_ = nullptr;
    if (isEditMode_) {
        auto* stockGroup = new QFrame;
        stockGroup->setObjectName("stockAdjustGroup");
        auto* stockGroupLayout = new QHBoxLayout(stockGroup);
        stockGroupLayout->setContentsMargins(16, 8, 16, 8);
        stockGroupLayout->setSpacing(10);
        QLabel* stockIconLabel = new QLabel("↻");
        stockIconLabel->setStyleSheet("font-size: 21px; color: #cccccc; font-weight: bold;");
        stockAdjustSpinBox_ = new QSpinBox;
        stockAdjustSpinBox_->setRange(-999999, 999999);
        stockAdjustSpinBox_->setValue(0);
        stockAdjustSpinBox_->setSuffix(" units");
        QLabel* stockLabel = new QLabel("Adjust Stock");
        stockLabel->setObjectName("stockLabel");
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
    mainLayout->addSpacing(15);

    // Botões principais
    auto* buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch();
    btnCancel_ = new QPushButton("Cancel");
    btnCancel_->setObjectName("cancelButton");
    connect(btnCancel_, &QPushButton::clicked, this, &AddProductDialog::onCancelClicked);
    buttonsLayout->addWidget(btnCancel_);

    btnSave_ = new QPushButton(isEditMode_ ? "Save Changes" : "Add Product");
    connect(btnSave_, &QPushButton::clicked, this, &AddProductDialog::onSaveClicked);
    buttonsLayout->addWidget(btnSave_);
    mainLayout->addLayout(buttonsLayout);

    connect(nameEdit_, &QLineEdit::textChanged, this, &AddProductDialog::validateFields);
    connect(idEdit_, &QLineEdit::textChanged, this, &AddProductDialog::validateFields);

    validateFields();
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
            btnSelectImage_->setText("🔄 Change Image");
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
            btnSelectImage_->setText("🔄 Change Image");
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
        int ajuste = stockAdjustSpinBox_->value();
        int novoStock = quantitySpinBox_->value() + ajuste;
        if (novoStock < 0) {
            QMessageBox::warning(this, "Error", "Stock cannot be negative after adjustment!");
            stockAdjustSpinBox_->setFocus();
            return;
        }
        quantitySpinBox_->setValue(novoStock);
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

void AddProductDialog::setInitialId(const QString& id) {
    if (!isEditMode_ && idEdit_ && idEdit_->text().trimmed().isEmpty()) {
        idEdit_->setText(id);
    }
}
