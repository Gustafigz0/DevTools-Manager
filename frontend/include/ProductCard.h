
#ifndef PRODUCTCARD_H
#define PRODUCTCARD_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include "product.h"

class ProductCard : public QWidget
{
    Q_OBJECT

public:
    explicit ProductCard(const Product &product, QWidget *parent = nullptr);
    
signals:
    void editClicked(int productId);
    void deleteClicked(int productId);
    void cardClicked(int productId);

protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    void setupUI();
    
private:
    Product product;
    QLabel *nameLabel;
    QLabel *descriptionLabel;
    QLabel *priceLabel;
    QLabel *stockLabel;
    QLabel *statusBadge;
    QPushButton *editButton;
    QPushButton *deleteButton;
};

#endif // PRODUCTCARD_H
