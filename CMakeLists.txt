cmake_minimum_required(VERSION 3.16)
project(DevToolsManager VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Find Qt6
set(CMAKE_PREFIX_PATH "/usr/local/lib/cmake/Qt6" ${CMAKE_PREFIX_PATH})
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)

# Source files
set(SOURCES
    scr/main.cpp
    scr/MainWindow.cpp
    scr/AddProductDialog.cpp
    scr/Product.cpp
    scr/ProductManager.cpp
    scr/Theme.cpp
    scr/ToastNotification.cpp
    scr/UserAuthDialog.cpp
    scr/UserAuthManager.cpp
    scr/ClickableLabel.cpp
)

# Header files
set(HEADERS
    include/MainWindow.h
    include/AddProductDialog.h
    include/Product.h
    include/ProductManager.h
    include/Theme.h
    include/ToastNotification.h
    include/UserAuthManager.h
    include/ClickableLabel.h
)

# Create executable
add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE include scr)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets
)


