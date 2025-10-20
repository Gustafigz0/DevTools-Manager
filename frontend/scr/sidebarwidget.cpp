#include "sidebarwidget.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFrame>

SidebarWidget::SidebarWidget(QWidget *parent): QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(12);

    QLabel *title = new QLabel("Malzeme"); // Nome da seção
    title->setStyleSheet("font-weight: bold; font-size: 22px;");

    QLabel *countLabel = new QLabel("6000");
    countLabel->setStyleSheet("border-radius: 8px; background-color: #eee; padding: 4px 12px; font-weight: bold;");

    layout->addWidget(title);
    layout->addWidget(countLabel);

    // Filtros
    QLabel *filtersTitle = new QLabel("MALZEME DURUMU");
    filtersTitle->setStyleSheet("font-weight: bold; margin-top: 18px;");
    layout->addWidget(filtersTitle);

    QPushButton *btnAll = new QPushButton("Tudo");
    QPushButton *btnActive = new QPushButton("Ativo");
    QPushButton *btnInactive = new QPushButton("Inativo");
    btnAll->setStyleSheet("border-radius: 8px; padding: 4px 12px; background: #eee;");
    btnActive->setStyleSheet("border-radius: 8px; padding: 4px 12px; background: #eee;");
    btnInactive->setStyleSheet("border-radius: 8px; padding: 4px 12px; background: #eee;");
    layout->addWidget(btnAll);
    layout->addWidget(btnActive);
    layout->addWidget(btnInactive);

    layout->addStretch();
}
