// include/MainWindow.h
#pragma once
#include <QMainWindow>
class QLineEdit;
class QComboBox;
class QLabel;
class QWidget;
class QVBoxLayout;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    QLineEdit* search_;
    QComboBox* filter_;
    QWidget* sidebarBox_;   // Label arredondada p/ filtros/pesquisa
    QWidget* userInfoLabel_; // Label arredondada p/ usuário/configurações
    QWidget* productsBg_;   // Label arredondada área dos produtos
    QVBoxLayout* productsLayout_;
    void setupUi();
    void setupConnections();
};
