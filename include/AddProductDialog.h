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
    explicit AddProductDialog(QWidget* parent = nullptr);
    AddProductDialog(const Product& product, QWidget* parent = nullptr);
    
    Product getProduct() const;
    
private slots:
    void onSaveClicked();
    void onCancelClicked();
    void validateFields();
    
private:
    void setupUi();
    void populateFields(const Product& product);
    
    // Campos do formulário
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
