cmake_minimum_required(VERSION 3.16)
project(DevToolsManager VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Lista todos os arquivos fonte
set(SOURCES
    scr/main.cpp
    scr/MainWindow.cpp
    scr/Theme.cpp
    scr/Product.cpp
    scr/AddProductDialog.cpp
	scr/ProductManager.cpp
)

# Lista todos os headers
set(HEADERS
    include/MainWindow.h
    include/Theme.h
    include/Product.h
    include/AddProductDialog.h
	include/ProductManager.h
)

add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
)

target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Widgets)

# Para Windows, remover console
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()
