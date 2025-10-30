#include "Theme.h"
#include <QPalette>
#include <QColor>
#include <QFont>
void Theme::apply(QApplication& app) {
    app.setStyle("Fusion");
    QPalette pal;
    pal.setColor(QPalette::Window, QColor("#000000"));
    pal.setColor(QPalette::WindowText, QColor("#ffffff"));
    pal.setColor(QPalette::Base, QColor("#1a1a1a"));
    pal.setColor(QPalette::AlternateBase, QColor("#2a2a2a"));
    pal.setColor(QPalette::Text, QColor("#ffffff"));
    pal.setColor(QPalette::Button, QColor("#333333"));
    pal.setColor(QPalette::ButtonText, QColor("#ffffff"));
    pal.setColor(QPalette::Highlight, QColor("#666666"));
    pal.setColor(QPalette::HighlightedText, QColor("#ffffff"));
    pal.setColor(QPalette::Disabled, QPalette::Text, QColor("#888888"));
    pal.setColor(QPalette::Disabled, QPalette::WindowText, QColor("#888888"));
    pal.setColor(QPalette::Disabled, QPalette::ButtonText, QColor("#888888"));
    app.setPalette(pal);
    QFont font;
    font.setPointSize(12);
    font.setFamily("Segoe UI");
    app.setFont(font);
    const char* qss = R"(
      QWidget {
        background: #000000;
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
