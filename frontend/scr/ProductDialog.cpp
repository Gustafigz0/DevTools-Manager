#include "productdialog.h"
#include "productwidget.h"
#include <QLabel>
#include <QMessageBox>
#include <QGroupBox>

ProductDialog::ProductDialog(QWidget *parent)
    : QDialog(parent), editMode(false), productId(-1)
{
    setupUI();
    setupConnections();
    setWindowTitle("Adicionar Novo Produto");
}

ProductDialog::ProductDialog(const Product &product, QWidget *parent)
    : QDialog(parent), editMode(true), productId(product.id)
{
    setupUI();
    setupConnections();
    setProduct(product);
    setWindowTitle("Editar Produto");
}

void ProductDialog::setupUI()
{
    setModal(true);
    setMinimumWidth(600);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    
    // Título do dialog
    QLabel *titleLabel = new QLabel(editMode ? "Editar Produto" : "Novo Produto");
    titleLabel->setObjectName("dialogTitle");
    titleLabel->setStyleSheet("font-size: 22px; font-weight: bold; color: #2c3e50; margin-bottom: 10px;");
    
    // Formulário
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(15);
    formLayout->setLabelAlignment(Qt::AlignRight);
    
    // Nome do Produto
    QLabel *nameLabel = new QLabel("Nome do Produto:");
    nameLabel->setObjectName("fieldLabel");
    nameEdit = new QLineEdit();
    nameEdit->setPlaceholderText("Digite o nome do produto");
    formLayout->addRow(nameLabel, nameEdit);
    
    // Descrição
    QLabel *descLabel = new QLabel("Descrição:");
    descLabel->setObjectName("fieldLabel");
    descriptionEdit = new QTextEdit();
    descriptionEdit->setPlaceholderText("Digite uma descrição detalhada");
    descriptionEdit->setMaximumHeight(100);
    formLayout->addRow(descLabel, descriptionEdit);
    
    // Preço
    QLabel *priceLabel = new QLabel("Preço (R$):");
    priceLabel->setObjectName("fieldLabel");
    priceSpinBox = new QDoubleSpinBox();
    priceSpinBox->setRange(0.01, 999999.99);
    priceSpinBox->setDecimals(2);
    priceSpinBox->setPrefix("R$ ");
    priceSpinBox->setValue(0.00);
    formLayout->addRow(priceLabel, priceSpinBox);
    
    // Estoque
    QLabel *stockLabel = new QLabel("Quantidade em Estoque:");
    stockLabel->setObjectName("fieldLabel");
    stockSpinBox = new QSpinBox();
    stockSpinBox->setRange(0, 999999);
    stockSpinBox->setValue(0);
    formLayout->addRow(stockLabel, stockSpinBox);
    
    // Status
    QLabel *statusLabel = new QLabel("Status:");
    statusLabel->setObjectName("fieldLabel");
    statusCombo = new QComboBox();
    statusCombo->addItem("Ativo", "active");
    statusCombo->addItem("Inativo", "inactive");
    formLayout->addRow(statusLabel, statusCombo);
    
    // Botões de ação
    QWidget *buttonWidget = new QWidget();
    QHBoxLayout *buttonLayout = new QHBoxLayout(buttonWidget);
    buttonLayout->setContentsMargins(0, 20, 0, 0);
    
    cancelButton = new QPushButton("Cancelar");
    cancelButton->setObjectName("secondaryButton");
    cancelButton->setMinimumWidth(120);
    
    saveButton = new QPushButton(editMode ? "Salvar Alterações" : "Adicionar Produto");
    saveButton->setObjectName("primaryButton");
    saveButton->setMinimumWidth(150);
    
    buttonLayout->addStretch();
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(saveButton);
    
    // Adicionar tudo ao layout principal
    mainLayout->addWidget(titleLabel);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(buttonWidget);
    
    setLayout(mainLayout);
}

void ProductDialog::setupConnections()
{
    connect(saveButton, &QPushButton::clicked, this, &ProductDialog::onSaveClicked);
    connect(cancelButton, &QPushButton::clicked, this, &ProductDialog::onCancelClicked);
}

bool ProductDialog::validateInput()
{
    if (nameEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validação", 
                            "Por favor, insira o nome do produto.");
        nameEdit->setFocus();
        return false;
    }
    
    if (descriptionEdit->toPlainText().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validação", 
                            "Por favor, insira uma descrição para o produto.");
        descriptionEdit->setFocus();
        return false;
    }
    
    if (priceSpinBox->value() <= 0) {
        QMessageBox::warning(this, "Validação", 
                            "O preço deve ser maior que zero.");
        priceSpinBox->setFocus();
        return false;
    }
    
    return true;
}

Product ProductDialog::getProduct() const
{
    Product product;
    product.id = productId;
    product.name = nameEdit->text().trimmed();
    product.description = descriptionEdit->toPlainText().trimmed();
    product.price = priceSpinBox->value();
    product.stock = stockSpinBox->value();
    product.status = statusCombo->currentData().toString();
    
    return product;
}

void ProductDialog::setProduct(const Product &product)
{
    productId = product.id;
    nameEdit->setText(product.name);
    descriptionEdit->setPlainText(product.description);
    priceSpinBox->setValue(product.price);
    stockSpinBox->setValue(product.stock);
    
    int index = statusCombo->findData(product.status);
    if (index != -1) {
        statusCombo->setCurrentIndex(index);
    }
}

void ProductDialog::onSaveClicked()
{
    if (validateInput()) {
        accept();
    }
}

void ProductDialog::onCancelClicked()
{
    reject();
}
