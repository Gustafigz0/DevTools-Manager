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
    // Novo construtor para criar (passa lista de categorias!)
    explicit AddProductDialog(const QStringList& categorias, QWidget* parent = nullptr);

    // Novo construtor para editar (produto e lista de categorias)
    AddProductDialog(const Product& product, const QStringList& categorias, QWidget* parent = nullptr);

    Product getProduct() const;

private slots:
    void onSaveClicked();
    void onCancelClicked();
    void validateFields();

private:
    void setupUi(const QStringList& categorias); // setupUi agora recebe lista
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
