#include <QApplication>
#include "Theme.h"
#include "MainWindow.h"
int main(int argc, char** argv) {
    QApplication app(argc, argv);
    Theme::apply(app);
    MainWindow w;
    w.show();
    return app.exec();
}
