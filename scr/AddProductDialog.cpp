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
    idEdit_->setEnabled(false);
}

void AddProductDialog::setupUi() {
    setModal(true);
    setMinimumWidth(540);

    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(26);
    mainLayout->setContentsMargins(36, 36, 36, 36);

    setStyleSheet(R"(
        QDialog {
            background: #222428;
            border-radius: 28px;
        }
        QLabel[role="titleText"] {
            color: #64b6fa;
            font-size: 27px;
            font-weight: 800;
            margin-bottom: 18px;
            letter-spacing: 2px;
        }
        .FieldLabel {
            color: #64b6fa;
            font-size: 17px;
            font-weight: 900;
            letter-spacing
