// src/MainWindow.cpp
#include "MainWindow.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QFrame>
#include <QStringList>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
  setupUi();
  setupConnections();
}

void MainWindow::setupUi() {
  auto* central = new QWidget(this);
  auto* root = new QVBoxLayout(central);
  root->setContentsMargins(16,16,16,16);
  root->setSpacing(12);

  auto* header = new QHBoxLayout();
  header->setSpacing(8);
  auto* title = new QLabel("Catálogo");
  title->setProperty("role", "title");  
  search_ = new QLineEdit();
  search_->setPlaceholderText("Pesquisar...");

  header->addWidget(title, 0);
  header->addStretch(1);
  header->addWidget(search_, 0);

  auto* line = new QFrame();
  line->setObjectName("line");
  line->setFrameShape(QFrame::NoFrame);

  list_ = new QListWidget();
  list_->addItems(QStringList{
      "Produto A", "Produto B", "Produto C", "Produto D", "Produto E"
  });

  auto* actions = new QHBoxLayout();
  actions->setSpacing(8);
  addBtn_ = new QPushButton("Novo");
  removeBtn_ = new QPushButton("Remover");
  removeBtn_->setEnabled(false);
  actions->addStretch(1);
  actions->addWidget(addBtn_);
  actions->addWidget(removeBtn_);

  root->addLayout(header);
  root->addWidget(line);
  root->addWidget(list_, 1);
  root->addLayout(actions);

  setCentralWidget(central);
  setWindowTitle("DevTools Manager");
  resize(720, 480);
}

void MainWindow::setupConnections() {
  connect(search_, &QLineEdit::textChanged, this, &MainWindow::filterList);
  connect(list_, &QListWidget::itemSelectionChanged, this, [this]{
    removeBtn_->setEnabled(!list_->selectedItems().isEmpty());
  });
  connect(addBtn_, &QPushButton::clicked, this, [this]{
    list_->addItem(QString("Novo item %1").arg(list_->count()+1));
  });
  connect(removeBtn_, &QPushButton::clicked, this, [this]{
    qDeleteAll(list_->selectedItems());
  });
}

void MainWindow::filterList(const QString& text) {
  for (int i = 0; i < list_->count(); ++i) {
    auto* item = list_->item(i);
    item->setHidden(!item->text().contains(text, Qt::CaseInsensitive));
  }
}
