#pragma once

#include <QMainWindow>

class SidebarWidget;
// ...existing code...
class QPushButton;
class QLineEdit;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

private:
    SidebarWidget *sidebar;
    // ...existing code...
    QPushButton *addProductButton;
    QLineEdit *searchBar;
};
