#pragma once
#include <QMainWindow>
class QLabel;
class QWidget;
class QVBoxLayout;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
private:
    QWidget* sidebarBox_;    // Label lateral (fundo à esquerda)
    QWidget* userInfoLabel_; // Label topo (usuário/configs)
    QWidget* productsBg_;    // Label área dos produtos
    QVBoxLayout* productsLayout_;
    void setupUi();
};
