#pragma once
#include <QDialog>
#include "Product.h"

class QLineEdit;
class QComboBox;
class QDoubleSpinBox;
class QSpinBox;
class QPushButton;

class AddProductDialog : public QDialog {
    Q_OBJECT
public:
    // Para adicionar produto (recebe lista dinâmica de categorias)
    explicit AddProductDialog(const QStringList& categorias, QWidget* parent = nullptr);
    // Para editar produto (recebe produto e lista dinâmica de categorias)
    AddProductDialog(const Product& product, const QStringList& categorias, QWidget* parent = nullptr);

    Product getProduct() const;

private slots:
    void onSaveClicked();
    void onCancelClicked();
    void validateFields();

private:
    void setupUi(const QStringList& categorias); // agora recebe a lista dinâmica
    void populateFields(const Product& product);

    QLineEdit* idEdit_;
    QLineEdit* nameEdit_;
    QComboBox* categoryCombo_;
    QDoubleSpinBox* priceSpinBox_;
    QSpinBox* quantitySpinBox_;
    QPushButton* btnSave_;
    QPushButton* btnCancel_;
    bool isEditMode_;
    Product originalProduct_;
};
