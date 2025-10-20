#include <QApplication>
#include <QFile>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Aplica o estilo QSS global, se existir
    QFile styleFile("frontend/styles/style.qss");
    if(styleFile.open(QFile::ReadOnly)) {
        QString style = styleFile.readAll();
        app.setStyleSheet(style);
    }

    MainWindow window;
    window.setMinimumSize(1200, 800); // Define o tamanho padrão semelhante ao design
    window.show();

    return app.exec();
}
