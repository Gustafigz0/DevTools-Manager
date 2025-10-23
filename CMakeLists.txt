# CMakeLists.txt (na raiz)
cmake_minimum_required(VERSION 3.21)
project(MinimalQtWidgets LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

add_subdirectory(scr)

# Provide a friendly warning if the found Qt appears to be MinGW while MSVC is used.
if(MSVC AND Qt6_DIR)
	string(TOLOWER "${Qt6_DIR}" _qt6_dir_lower)
	if(_qt6_dir_lower MATCHES "mingw")
		message(WARNING "Detected Qt in: ${Qt6_DIR}\nIt looks like this Qt build was made for MinGW but you're generating for MSVC.\nThis mismatch causes unresolved linker symbols.\nFix by installing a Qt build for MSVC (e.g. msvc_64) and re-run CMake with -D CMAKE_PREFIX_PATH=\"C:/Qt/<version>/msvc_64/lib/cmake\" or by using a MinGW generator/toolchain to match your Qt.")
	endif()
endif()
