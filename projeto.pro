QT += widgets
CONFIG += c++17

# Diretórios principais
INCLUDEPATH += frontend/include
INCLUDEPATH += frontend/widgets
INCLUDEPATH += frontend/dialogs

# Arquivos fonte
SOURCES += \
    frontend/scr/main.cpp \
    frontend/scr/mainwindow.cpp \
    frontend/widgets/ProductWidget.cpp \
    frontend/dialogs/AddProductDialog.cpp \
    frontend/scr/sidebarwidget.cpp \
    # Adicione outros .cpp conforme criar

HEADERS += \
    frontend/include/mainwindow.h \
    frontend/widgets/ProductWidget.h \
    frontend/dialogs/AddProductDialog.h \
    frontend/include/productwidget.h \
    frontend/include/sidebarwidget.h \
    # Adicione outros .h conforme criar

