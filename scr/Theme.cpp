#include "Theme.h"
#include <QPalette>
#include <QColor>
#include <QFont>
void Theme::apply(QApplication& app) {
    app.setStyle("Fusion");
    QPalette pal;
    pal.setColor(QPalette::Window, QColor("#202020"));
    pal.setColor(QPalette::WindowText, QColor("#ffffff"));
    pal.setColor(QPalette::Base, QColor("#2d2d2d"));
    pal.setColor(QPalette::AlternateBase, QColor("#252526"));
    pal.setColor(QPalette::Text, QColor("#ffffff"));
    pal.setColor(QPalette::Button, QColor("#2d2d2d"));
    pal.setColor(QPalette::ButtonText, QColor("#ffffff"));
    pal.setColor(QPalette::Highlight, QColor("#0078d4"));
    pal.setColor(QPalette::HighlightedText, QColor("#ffffff"));
    pal.setColor(QPalette::Disabled, QPalette::Text, QColor("#a6a6a6"));
    pal.setColor(QPalette::Disabled, QPalette::WindowText, QColor("#a6a6a6"));
    pal.setColor(QPalette::Disabled, QPalette::ButtonText, QColor("#a6a6a6"));
    app.setPalette(pal);
    QFont font;
    font.setPointSize(12);
    font.setFamily("Segoe UI");
    app.setFont(font);
    const char* qss = R"(
      QWidget {
        background: #202020;
        color: #ffffff;
        font-size: 13px;
      }
      QLabel[role=\"title\"] {
        font-size: 18px;
        font-weight: 600;
        color: #ffffff;
      }
    )";
    app.setStyleSheet(qss);
}
