#include "sidebarwidget.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFrame>

SidebarWidget::SidebarWidget(QWidget *parent): QWidget(parent) {
    setObjectName("sidebar");
    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *title = new QLabel("<b>Malzeme</b>");
    title->setStyleSheet("font-size:22px;margin-top:12px;");
    QLabel *count = new QLabel("6000");
    count->setStyleSheet("padding:4px 15px;background:#eee;border-radius:8px;font-weight:600;");

    layout->addWidget(title);
    layout->addWidget(count);
    layout->addSpacing(20);

    // Exemplo de filtros
    QLabel *lblStatus = new QLabel("Status");
    lblStatus->setStyleSheet("font-weight:700;");
    layout->addWidget(lblStatus);
    layout->addWidget(new QPushButton("Tudo"));
    layout->addWidget(new QPushButton("Ativo"));
    layout->addWidget(new QPushButton("Inativo"));

    layout->addStretch();
}
