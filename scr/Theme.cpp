// scr/Theme.cpp
#include "Theme.h"
#include <QPalette>
#include <QColor>
#include <QFont>

void Theme::apply(QApplication& app) {
    app.setStyle("Fusion");

    QPalette pal;
    pal.setColor(QPalette::Window, QColor("#202020"));              // Fundo principal
    pal.setColor(QPalette::WindowText, QColor("#ffffff"));          // Texto principal
    pal.setColor(QPalette::Base, QColor("#2d2d2d"));                // Fundo de widget
    pal.setColor(QPalette::AlternateBase, QColor("#252526"));       // Alternância de fundo
    pal.setColor(QPalette::Text, QColor("#ffffff"));                // Texto
    pal.setColor(QPalette::Button, QColor("#2d2d2d"));              // Fundo do botão
    pal.setColor(QPalette::ButtonText, QColor("#ffffff"));          // Texto do botão
    pal.setColor(QPalette::Highlight, QColor("#0078d4"));           // Azul padrão Windows
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
        QLineEdit {
            border: 1px solid #2d2d2d;
            border-radius: 8px;
            padding: 8px 10px;
            background: #252526;
            color: #ffffff;
        }
        QListWidget {
            border: 1px solid #2d2d2d;
            border-radius: 10px;
            background: #252526;
            padding: 6px;
            color: #ffffff;
        }
        QPushButton {
            background: #0078d4;
            color: #fff;
            border: none;
            border-radius: 8px;
            padding: 8px 14px;
            font-weight: 500;
        }
        QPushButton:hover {
            background: #005a9e;
        }
        QPushButton:disabled {
            background: #232323;
            color: #a6a6a6;
        }
        QFrame#line {
            background: #232323;
            min-height: 1px;
            max-height: 1px;
        }
        QLabel[role=\"title\"] {
            font-size: 18px;
            font-weight: 600;
            color: #ffffff;
        }
    )";
    app.setStyleSheet(qss);
}
