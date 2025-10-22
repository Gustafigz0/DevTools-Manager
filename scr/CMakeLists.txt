# src/CMakeLists.txt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(minimal
  ../include/MainWindow.h
  ../include/Theme.h
  ../include/Product.h
  MainWindow.cpp
  Theme.cpp
  Product.cpp
  main.cpp
)

target_include_directories(minimal PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_link_libraries(minimal PRIVATE Qt6::Widgets)

# Ensure C++17 and MSVC reports __cplusplus correctly
target_compile_features(minimal PRIVATE cxx_std_17)
if(MSVC)
  # Force MSVC to set a correct __cplusplus value as required by modern Qt
  target_compile_options(minimal PRIVATE /Zc:__cplusplus /permissive-)
endif()
