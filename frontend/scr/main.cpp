#include <QApplication>
#include <QFile>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Aplica o estilo QSS global, se existir
    QFile styleFile("frontend/resources/styles.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        qApp->setStyleSheet(styleFile.readAll());
    }

    MainWindow window;
    window.setMinimumSize(1200, 800); // Define o tamanho padrão semelhante ao design
    window.show();

    return app.exec();
}
