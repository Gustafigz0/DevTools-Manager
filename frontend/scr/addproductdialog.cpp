#include "addproductdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QIntValidator>

AddProductDialog::AddProductDialog(QWidget *parent) : QDialog(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);
    title = new QLineEdit; category = new QLineEdit; location = new QLineEdit; stock = new QLineEdit; date = new QLineEdit;
    stock->setValidator(new QIntValidator(0, 100000, this));
    layout->addWidget(new QLabel("Nome:")); layout->addWidget(title);
    layout->addWidget(new QLabel("Categoria:")); layout->addWidget(category);
    layout->addWidget(new QLabel("Depósito:")); layout->addWidget(location);
    layout->addWidget(new QLabel("Estoque:")); layout->addWidget(stock);
    layout->addWidget(new QLabel("Último Uso:")); layout->addWidget(date);

    QHBoxLayout *btns = new QHBoxLayout;
    QPushButton *ok = new QPushButton("Adicionar"), *cancel = new QPushButton("Cancelar");
    connect(ok, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancel, &QPushButton::clicked, this, &QDialog::reject);
    btns->addWidget(ok); btns->addWidget(cancel);
    layout->addLayout(btns);
}

Product AddProductDialog::product() const {
    Product p;
    p.name = title->text();
    p.category = category->text();
    p.location = location->text();
    p.image = "";
    p.stock = stock->text().toInt();
    p.lastUse = date->text();
    // Adicione outros campos conforme necessário
    return p;
}
