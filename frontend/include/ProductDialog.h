#ifndef PRODUCTDIALOG_H
#define PRODUCTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>

struct Product; // Forward declaration

class ProductDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProductDialog(QWidget *parent = nullptr);
    explicit ProductDialog(const Product &product, QWidget *parent = nullptr);
    
    Product getProduct() const;
    void setProduct(const Product &product);
    
private:
    void setupUI();
    void setupConnections();
    bool validateInput();
    
private slots:
    void onSaveClicked();
    void onCancelClicked();
    
private:
    QLineEdit *nameEdit;
    QTextEdit *descriptionEdit;
    QDoubleSpinBox *priceSpinBox;
    QSpinBox *stockSpinBox;
    QComboBox *statusCombo;
    
    QPushButton *saveButton;
    QPushButton *cancelButton;
    
    bool editMode;
    int productId;
};

#endif // PRODUCTDIALOG_H
