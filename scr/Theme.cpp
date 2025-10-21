// src/Theme.cpp
#include "Theme.h"
#include <QPalette>
#include <QColor>
#include <QFont>

void Theme::apply(QApplication& app) {
  app.setStyle("Fusion"); // estilo consistente entre plataformas
  QPalette pal;
  pal.setColor(QPalette::Window, QColor("#f7f7f8"));
  pal.setColor(QPalette::WindowText, QColor("#111111"));
  pal.setColor(QPalette::Base, QColor("#ffffff"));
  pal.setColor(QPalette::AlternateBase, QColor("#f0f0f1"));
  pal.setColor(QPalette::Text, QColor("#111111"));
  pal.setColor(QPalette::Button, QColor("#111111"));
  pal.setColor(QPalette::ButtonText, QColor("#ffffff"));
  pal.setColor(QPalette::Highlight, QColor("#3b82f6"));
  pal.setColor(QPalette::HighlightedText, QColor("#ffffff"));
  app.setPalette(pal);

  QFont font;
  font.setPointSize(12);
  font.setFamily("Segoe UI");
  app.setFont(font);

  const char* qss = R"(
    QWidget {
      background: #f7f7f8;
      color: #111111;
      font-size: 13px;
    }
    QLineEdit {
      border: 1px solid #e5e7eb;
      border-radius: 8px;
      padding: 8px 10px;
      background: #ffffff;
    }
    QListWidget {
      border: 1px solid #e5e7eb;
      border-radius: 10px;
      background: #ffffff;
      padding: 6px;
    }
    QPushButton {
      background: #111111;
      color: #ffffff;
      border: 1px solid #111111;
      border-radius: 8px;
      padding: 8px 14px;
    }
    QPushButton:hover {
      background: #222222;
      border-color: #222222;
    }
    QPushButton:disabled {
      background: #c4c4c4;
      border-color: #c4c4c4;
      color: #f7f7f8;
    }
    QFrame#line {
      background: #e5e7eb;
      min-height: 1px;
      max-height: 1px;
    }
  )";
  app.setStyleSheet(qss);
}
