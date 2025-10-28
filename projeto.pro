TEMPLATE = app
TARGET = catalogo

CONFIG += cmdline c++17 warn_on
CONFIG -= qt

INCLUDEPATH += $$PWD/include

SOURCES += $$PWD/scr/main.cpp \
           $$PWD/scr/Ansi.cpp \
           $$PWD/scr/CatalogUI.cpp \
           $$PWD/scr/CSV.cpp \
           $$PWD/scr/Product.cpp \
           $$PWD/scr/ProductRepository.cpp

HEADERS += $$PWD/include/Ansi.hpp \
           $$PWD/include/CatalogUI.hpp \
           $$PWD/include/CSV.hpp \
           $$PWD/include/Product.hpp \
           $$PWD/include/ProductRepository.hpp

DESTDIR     = $$OUT_PWD/bin
OBJECTS_DIR = $$OUT_PWD/obj

# Força C++17 caso o kit não respeite só CONFIG
win32:!msvc: QMAKE_CXXFLAGS += -std=c++17
unix|macx:   QMAKE_CXXFLAGS += -std=c++17
win32:msvc:  QMAKE_CXXFLAGS += /std:c++17

# Remove execução automática inválida após build
