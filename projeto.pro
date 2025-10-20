QT += widgets
CONFIG += c++17

# Diretórios principais
INCLUDEPATH += frontend/include
INCLUDEPATH += frontend/widgets

# Arquivos fonte
SOURCES += \
    frontend/scr/main.cpp \
    frontend/scr/mainwindow.cpp \
    frontend/scr/productlistwidget.cpp \
    frontend/scr/sidebarwidget.cpp \
    frontend/scr/addproductdialog.cpp \
    frontend/scr/productwidget.cpp \
    # Adicione outros .cpp conforme criar

HEADERS += \
    frontend/include/mainwindow.h \
    frontend/include/productlistwidget.h \
    frontend/include/productwidget.h \
    frontend/include/sidebarwidget.h \
    frontend/include/addproductdialog.h \
    frontend/include/product.h \
    frontend/widgets/ProductWidget.h \
    # Adicione outros .h conforme criar

