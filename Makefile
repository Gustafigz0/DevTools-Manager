CXX = /usr/bin/c++
MOC = /opt/homebrew/share/qt/libexec/moc
CXXFLAGS = -std=c++17 -I include -I scr -I /opt/homebrew/lib/QtCore.framework/Headers -I /opt/homebrew/lib/QtGui.framework/Headers -I /opt/homebrew/lib/QtWidgets.framework/Headers -F /opt/homebrew/lib -DQT_CORE_LIB -DQT_GUI_LIB -DQT_WIDGETS_LIB
LDFLAGS = -F /opt/homebrew/lib -framework QtCore -framework QtGui -framework QtWidgets

SOURCES = scr/main.cpp scr/MainWindow.cpp scr/AddProductDialog.cpp scr/Product.cpp scr/ProductManager.cpp scr/Theme.cpp scr/ToastNotification.cpp
HEADERS = include/MainWindow.h include/AddProductDialog.h include/Product.h include/ProductManager.h include/Theme.h include/ToastNotification.h
MOC_SOURCES = moc_MainWindow.cpp moc_AddProductDialog.cpp moc_ProductManager.cpp moc_ToastNotification.cpp
OBJECTS = $(SOURCES:.cpp=.o) $(MOC_SOURCES:.cpp=.o)
TARGET = DevToolsManager

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

moc_%.cpp: include/%.h
	$(MOC) -I include -I scr -I /opt/homebrew/lib/QtCore.framework/Headers -I /opt/homebrew/lib/QtGui.framework/Headers -I /opt/homebrew/lib/QtWidgets.framework/Headers -F /opt/homebrew/lib -DQT_CORE_LIB -DQT_GUI_LIB -DQT_WIDGETS_LIB $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET) $(MOC_SOURCES)

.PHONY: all clean