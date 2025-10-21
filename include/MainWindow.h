// include/MainWindow.h
#pragma once
#include <QMainWindow>

class QLineEdit;
class QListWidget;
class QPushButton;

class MainWindow : public QMainWindow {
  Q_OBJECT
public:
  explicit MainWindow(QWidget* parent = nullptr);

private:
  QLineEdit*   search_;
  QListWidget* list_;
  QPushButton* addBtn_;
  QPushButton* removeBtn_;

  void setupUi();
  void setupConnections();
  void filterList(const QString& text);
};
