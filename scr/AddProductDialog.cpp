#include "AddProductDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QPushButton>
#include <QMessageBox>

AddProductDialog::AddProductDialog(QWidget* parent) 
    : QDialog(parent), isEditMode_(false) {
    setupUi();
    setWindowTitle("Adicionar Novo Produto");
}

AddProductDialog::AddProductDialog(const Product& product, QWidget* parent)
    : QDialog(parent), isEditMode_(true), originalProduct_(product) {
    setupUi();
    populateFields(product);
    setWindowTitle("Editar Produto");
    idEdit_->setEnabled(false); // ID não pode ser editado
}

void AddProductDialog::setupUi() {
    setModal(true);
    setMinimumWidth(500);
    
    // Layout principal
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    
    // Estilo do diálogo
    setStyleSheet(R"(
        QDialog {
            background: #242526;
        }
        QLabel {
            color: #fff;
            font-size: 14px;
            font-weight: 600;
        }
        QLineEdit, QComboBox, QSpinBox, QDoubleSpinBox {
            background: #2d2d2d;
            color: #fff;
            border: 2px solid #3a3a3a;
            border-radius: 10px;
            padding: 10px 14px;
            font-size: 14px;
        }
        QLineEdit:focus, QComboBox:focus, QSpinBox:focus, QDoubleSpinBox:focus {
            border: 2px solid #58abfa;
        }
        QComboBox::drop-down {
            border: none;
            width: 30px;
        }
        QComboBox::down-arrow {
            image: none;
            border-left: 5px solid transparent;
            border-right: 5px solid transparent;
            border-top: 7px solid #58abfa;
            margin-right: 10px;
        }
        QPushButton {
            background: #58abfa;
            color: #fff;
            border: none;
            border-radius: 12px;
            padding: 12px 24px;
            font-size: 15px;
            font-weight: 600;
            min-width: 120px;
        }
        QPushButton:hover {
            background: #4a9de8;
        }
        QPushButton:pressed {
            background: #3a8dd8;
        }
        QPushButton#cancelButton {
            background: #3a3a3a;
        }
        QPushButton#cancelButton:hover {
            background: #4a4a4a;
        }
    )");
    
    // Título
    QLabel* titleLabel = new QLabel(isEditMode_ ? "✏️ Editar Produto" : "➕ Novo Produto");
    titleLabel->setStyleSheet("font-size: 22px; font-weight: 700; color: #58abfa; margin-bottom: 10px;");
    mainLayout->addWidget(titleLabel);
    
    // Formulário
    auto* formLayout = new QFormLayout;
    formLayout->setSpacing(15);
    formLayout->setLabelAlignment(Qt::AlignRight);
    
    // ID
    idEdit_ = new QLineEdit;
    idEdit_->setPlaceholderText("Ex: 001");
    formLayout->addRow("ID:", idEdit_);
    
    // Nome
    nameEdit_ = new QLineEdit;
    nameEdit_->setPlaceholderText("Ex: Mouse Gamer RGB");
    formLayout->addRow("Nome:", nameEdit_);
    
    // Categoria
    categoryCombo_ = new QComboBox;
    categoryCombo_->addItems({
        "Periféricos",
        "Monitores",
        "Áudio",
        "Câmeras",
        "Armazenamento",
        "Acessórios",
        "Componentes",
        "Notebooks",
        "Outros"
    });
    categoryCombo_->setEditable(true);
    formLayout->addRow("Categoria:", categoryCombo_);
    
    // Preço
    priceSpinBox_ = new QDoubleSpinBox;
    priceSpinBox_->setPrefix("R$ ");
    priceSpinBox_->setRange(0.01, 999999.99);
    priceSpinBox_->setDecimals(2);
    priceSpinBox_->setValue(0.00);
    formLayout->addRow("Preço:", priceSpinBox_);
    
    // Quantidade
    quantitySpinBox_ = new QSpinBox;
    quantitySpinBox_->setRange(0, 999999);
    quantitySpinBox_->setValue(0);
    quantitySpinBox_->setSuffix(" unidades");
    formLayout->addRow("Quantidade:", quantitySpinBox_);
    
    mainLayout->addLayout(formLayout);
    mainLayout->addSpacing(10);
    
    // Botões
    auto* buttonsLayout = new QHBoxLayout;
    buttonsLayout->setSpacing(12);
    buttonsLayout->addStretch();
    
    btnCancel_ = new QPushButton("Cancelar");
    btnCancel_->setObjectName("cancelButton");
    connect(btnCancel_, &QPushButton::clicked, this, &AddProductDialog::onCancelClicked);
    buttonsLayout->addWidget(btnCancel_);
    
    btnSave_ = new QPushButton(isEditMode_ ? "Salvar Alterações" : "Adicionar Produto");
    connect(btnSave_, &QPushButton::clicked, this, &AddProductDialog::onSaveClicked);
    buttonsLayout->addWidget(btnSave_);
    
    mainLayout->addLayout(buttonsLayout);
    
    // Conectar validação
    connect(nameEdit_, &QLineEdit::textChanged, this, &AddProductDialog::validateFields);
    connect(idEdit_, &QLineEdit::textChanged, this, &AddProductDialog::validateFields);
    
    validateFields();
}

void AddProductDialog::populateFields(const Product& product) {
    idEdit_->setText(product.getId());
    nameEdit_->setText(product.getName());
    
    int index = categoryCombo_->findText(product.getCategory());
    if (index >= 0) {
        categoryCombo_->setCurrentIndex(index);
    } else {
        categoryCombo_->setEditText(product.getCategory());
    }
    
    priceSpinBox_->setValue(product.getPrice());
    quantitySpinBox_->setValue(product.getQuantity());
}

void AddProductDialog::validateFields() {
    bool valid = !idEdit_->text().trimmed().isEmpty() && 
                 !nameEdit_->text().trimmed().isEmpty();
    btnSave_->setEnabled(valid);
}

void AddProductDialog::onSaveClicked() {
    // Validação adicional
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
    
    accept(); // Fecha o diálogo com sucesso
}

void AddProductDialog::onCancelClicked() {
    reject(); // Fecha o diálogo sem salvar
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
