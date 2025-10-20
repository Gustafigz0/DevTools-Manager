#pragma once
#include <QMainWindow>
class SidebarWidget;
class ProductListWidget;
class QPushButton;
class QLineEdit;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void showAddDialog();

private:
    SidebarWidget *sidebar;
    ProductListWidget *productList;
    QPushButton *addProductButton;
    QLineEdit *searchBar;
};
