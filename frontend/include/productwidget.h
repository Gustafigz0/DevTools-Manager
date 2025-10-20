#pragma once
#include <QWidget>
#include <QString>
class QLabel;

class ProductWidget : public QWidget {
    Q_OBJECT
public:
    ProductWidget(const QString &nome, const QString &categoria, int qtd, const QString &data, const QString &deposito, QWidget *parent = nullptr);
};
