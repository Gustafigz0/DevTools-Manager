set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Busca todos os arquivos da pasta src/scr automaticamente
file(GLOB SRC_FILES *.cpp)
file(GLOB HEADER_FILES ../include/*.h)

# Nome do executável será herdado do projeto raiz
add_executable(${PROJECT_NAME}
    ${SRC_FILES}
    ${HEADER_FILES}
)

# Inclui o diretório de headers
target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Linka com Qt6
find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets Qt6::Core Qt6::Gui)

# Garante C++17 em todos
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
