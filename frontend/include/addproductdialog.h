#pragma once
#include <QDialog>
#include "product.h"

class QLineEdit;

class AddProductDialog : public QDialog {
    Q_OBJECT
public:
    AddProductDialog(QWidget *parent=nullptr);

    Product product() const;
private:
    QLineEdit *title, *category, *location, *stock, *date;
};
