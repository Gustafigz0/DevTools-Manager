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
#include <QFrame>

AddProductDialog::AddProductDialog(const QStringList& categoriasUnicas, QWidget* parent)
    : QDialog(parent), isEditMode_(false) {
    setupUi(categoriasUnicas);
    setWindowTitle("Adicionar Novo Produto");
}

AddProductDialog::AddProductDialog(const Product& product, const QStringList& categoriasUnicas, QWidget* parent)
    : QDialog(parent), isEditMode_(true), originalProduct_(product) {
    setupUi(categoriasUnicas);
    populateFields(product);
    setWindowTitle("Editar Produto");
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
            background: #232428;
        }
        QLabel#TitleLabel {
            font-size: 24px;
            font-weight: 700;
            color: #58abfa;
            margin-bottom: 11px;
            letter-spacing: 0.5px;
        }
        QLabel#FormLabel {
            color: #fff;
            font-size: 15px;
            font-weight: 700;
            border-radius: 9px;
            padding: 8px 12px 8px 16px;
            min-width: 110px;
        }
        QLineEdit, QComboBox, QSpinBox, QDoubleSpinBox {
            background: #232428;
            color: #fff;
            border: 2px solid #353a40;
            border-radius: 12px;
            padding: 13px 18px;
            font-size: 15px;
        }
        QLineEdit:focus, QComboBox:focus, QSpinBox:focus, QDoubleSpinBox:focus {
            border: 2px solid #58abfa;
        }
        QComboBox QAbstractItemView {
            background: #232428;
            color: #fff;
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
            background: #58abfa;
            color: #fff;
            border: none;
            border-radius: 16px;
            padding: 14px 28px;
            font-size: 17px;
            font-weight: 600;
            min-width: 140px;
        }
        QPushButton:hover {
            background: #7bd0fc;
        }
        QPushButton:pressed {
            background: #43a0e4;
        }
        QPushButton#cancelButton {
            background: #313136;
            color: #ededed;
        }
        QPushButton#cancelButton:hover {
            background: #43434d;
        }
        QLabel#EditChip {
            background: #ffbc42;
            color: #232428;
            border-radius: 9px;
            font-weight: 800;
            font-size: 13px;
            padding: 4px 16px;
            margin-bottom: 11px;
            letter-spacing: 1px;
            min-width: 95px;
        }
        QLabel#AddChip {
            background: #65ff97;
            color: #232428;
            border-radius: 9px;
            font-weight: 800;
            font-size: 13px;
            padding: 4px 16px;
            margin-bottom: 11px;
            letter-spacing: 1px;
            min-width: 95px;
        }
        QFrame#stockAdjustGroup {
            background: #181b20;
            border: 2px solid #58abfa;
            border-radius: 18px;
        }
        QLabel#stockLabel {
            font-size: 15px;
            color: #58abfa;
            font-weight: 700;
            margin-left: 10px;
        }
    )");

    // Chip definido com base no modo
    if (isEditMode_) {
        QLabel* editModeChip = new QLabel("Modo Edição");
        editModeChip->setObjectName("EditChip");
        editModeChip->setAlignment(Qt::AlignCenter);
        mainLayout->addWidget(editModeChip, 0, Qt::AlignCenter);
    } else {
        QLabel* addChip = new QLabel("Novo Produto");
        addChip->setObjectName("AddChip");
        addChip->setAlignment(Qt::AlignCenter);
        mainLayout->addWidget(addChip, 0, Qt::AlignCenter);
    }

    // Cabeçalho visual moderno
    auto* headerWidget = new QWidget;
    headerWidget->setStyleSheet("background: #181b20; border-radius: 18px;");
    auto* headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->setContentsMargins(20, 8, 20, 8);
    headerLayout->setSpacing(10);

    QLabel* iconLabel = new QLabel(isEditMode_ ? "✏️" : "➕");
    iconLabel->setStyleSheet(isEditMode_
        ? "font-size: 27px; color: #58abfa; font-weight: bold; background: transparent;"
        : "font-size: 27px; color: #65ff97; font-weight: bold; background: transparent;");
    QLabel* titleLabel = new QLabel(isEditMode_ ? "Editar Produto" : "Adicionar Produto");
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
    nameEdit_->setPlaceholderText("Ex: Mouse Gamer RGB");
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
    quantitySpinBox_->setSuffix(" unidades");
    formRow("Quantidade:", quantitySpinBox_);

    // Ajuste de stock só modo edição
    stockAdjustSpinBox_ = nullptr;
    if (isEditMode_) {
        auto* stockGroup = new QFrame;
        stockGroup->setObjectName("stockAdjustGroup");
        auto* stockGroupLayout = new QHBoxLayout(stockGroup);
        stockGroupLayout->setContentsMargins(16, 8, 16, 8);
        stockGroupLayout->setSpacing(10);
        QLabel* stockIconLabel = new QLabel("🔄");
        stockIconLabel->setStyleSheet("font-size: 21px; color: #ffbc42; font-weight: bold;");
        stockAdjustSpinBox_ = new QSpinBox;
        stockAdjustSpinBox_->setRange(-999999, 999999);
        stockAdjustSpinBox_->setValue(0);
        stockAdjustSpinBox_->setSuffix(" unidades");
        QLabel* stockLabel = new QLabel("Ajustar Stock");
        stockLabel->setObjectName("stockLabel");
        stockGroupLayout->addWidget(stockIconLabel, 0, Qt::AlignLeft);
        stockGroupLayout->addWidget(stockAdjustSpinBox_, 0, Qt::AlignLeft);
        stockGroupLayout->addWidget(stockLabel, 0, Qt::AlignLeft);
        stockGroupLayout->addStretch(1);
        formLayout->addWidget(stockGroup);

        connect(stockAdjustSpinBox_, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](int value) {
            QString msg;
            if (value > 0) msg = QString("Adiciona %1 ao stock").arg(value);
            if (value < 0) msg = QString("Remove %1 do stock").arg(-value);
            if (value == 0) msg = QString("Nenhuma alteração de stock");
            stockAdjustSpinBox_->setToolTip(msg);
        });
    }

    mainLayout->addLayout(formLayout);
    mainLayout->addSpacing(15);

    // Botões principais
    auto* buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch();
    btnCancel_ = new QPushButton("Cancelar");
    btnCancel_->setObjectName("cancelButton");
    connect(btnCancel_, &QPushButton::clicked, this, &AddProductDialog::onCancelClicked);
    buttonsLayout->addWidget(btnCancel_);

    btnSave_ = new QPushButton(isEditMode_ ? "Salvar Alterações" : "Adicionar Produto");
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
}

void AddProductDialog::validateFields() {
    bool valid = !idEdit_->text().trimmed().isEmpty() && !nameEdit_->text().trimmed().isEmpty();
    btnSave_->setEnabled(valid);
}

void AddProductDialog::onSaveClicked() {
    if (idEdit_->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Erro", "O ID do produto é obrigatório!");
        idEdit_->setFocus();
        return;
    }
    if (nameEdit_->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Erro", "O nome do produto é obrigatório!");
        nameEdit_->setFocus();
        return;
    }
    if (priceSpinBox_->value() <= 0) {
        QMessageBox::warning(this, "Erro", "O preço deve ser maior que zero!");
        priceSpinBox_->setFocus();
        return;
    }

    if (isEditMode_ && stockAdjustSpinBox_) {
        int ajuste = stockAdjustSpinBox_->value();
        int novoStock = quantitySpinBox_->value() + ajuste;
        if (novoStock < 0) {
            QMessageBox::warning(this, "Erro", "O stock não pode ser negativo após ajuste!");
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
    return Product(
        idEdit_->text().trimmed(),
        nameEdit_->text().trimmed(),
        categoryCombo_->currentText().trimmed(),
        priceSpinBox_->value(),
        quantitySpinBox_->value()
    );
}
