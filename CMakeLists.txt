cmake_minimum_required(VERSION 3.16)
project(DevToolsManager VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Find Qt6
set(CMAKE_PREFIX_PATH "/usr/local/lib/cmake/Qt6" ${CMAKE_PREFIX_PATH})
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui Charts)

# Source files
set(SOURCES
    scr/main.cpp
    scr/MainWindow.cpp
    scr/AddProductDialog.cpp
    scr/DashboardWidget.cpp
    scr/Product.cpp
    scr/ProductManager.cpp
    scr/ToastNotification.cpp
    scr/UserAuthDialog.cpp
    scr/UserAuthManager.cpp
)

# Header files
set(HEADERS
    include/MainWindow.h
    include/AddProductDialog.h
    include/DashboardWidget.h
    include/Product.h
    include/ProductManager.h
    include/ToastNotification.h
    include/UserAuthManager.h
)

# Create executable
add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
    resources.qrc
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE include scr)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Charts
)
# Install rules
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

# Install data files (JSON) so the installer packages them
install(DIRECTORY ${CMAKE_SOURCE_DIR}/dados/
    DESTINATION share/${PROJECT_NAME}/dados
    FILES_MATCHING PATTERN "*.json"
)

if(WIN32)
    # Allow user to pass Qt bin directory (where windeployqt.exe lives)
    set(QT_BIN_DIR "" CACHE PATH "Path to Qt bin directory containing windeployqt.exe")

    # Try to find windeployqt (use QT_BIN_DIR as hint if provided)
    find_program(WINDEPLOYQT_EXECUTABLE
        NAMES windeployqt.exe windeployqt
        HINTS ${QT_BIN_DIR} ${QT_BIN_DIR}/bin $ENV{QTDIR} $ENV{QT_HOME}
        PATHS $ENV{PATH}
    )

    if(WINDEPLOYQT_EXECUTABLE)
        message(STATUS "windeployqt found: ${WINDEPLOYQT_EXECUTABLE}")
        set(WINDEPLOY_DIR "${CMAKE_BINARY_DIR}/windeploy")

        # Run windeployqt as a post-build step of the executable so files are collected during build
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND "${WINDEPLOYQT_EXECUTABLE}" --dir "${WINDEPLOY_DIR}" "$<TARGET_FILE:${PROJECT_NAME}>"
            COMMENT "Running windeployqt to collect Qt runtimes and plugins into ${WINDEPLOY_DIR}"
            VERBATIM
        )

        # Install everything produced by windeployqt into the package 'bin' so plugins sit next to the executable
        install(DIRECTORY ${WINDEPLOY_DIR}/ DESTINATION bin)
    else()
        message(WARNING "windeployqt not found. Will try to copy Qt plugins manually...")
        
        # Tenta copiar plugins Qt manualmente se windeployqt não estiver disponível
        if(QT_BIN_DIR)
            get_filename_component(QT_ROOT "${QT_BIN_DIR}" DIRECTORY)
            set(QT_PLUGINS_DIR "${QT_ROOT}/plugins")
            if(EXISTS "${QT_PLUGINS_DIR}")
                # Plugins essenciais
                install(DIRECTORY
                    "${QT_PLUGINS_DIR}/platforms"
                    "${QT_PLUGINS_DIR}/styles"
                    "${QT_PLUGINS_DIR}/imageformats"
                    "${QT_PLUGINS_DIR}/sqldrivers"
                    "${QT_PLUGINS_DIR}/iconengines"
                    DESTINATION bin
                    OPTIONAL
                )
            endif()
        endif()
    endif()

    # Try to include common MinGW runtime DLLs if present on the build machine
    set(MINGW_DLLS 
        # Básicas do MinGW
        libstdc++-6.dll
        libgcc_s_seh-1.dll
        libwinpthread-1.dll
        libssp-0.dll
        libgomp-1.dll
        
        # ICU (Unicode/Internacionalização)
        libicuin77.dll
        libicuuc77.dll
        libicudt77.dll
        libicuio77.dll
        libicule77.dll
        libiculx77.dll
        libicutest77.dll
        libicutu77.dll
        
        # SSL e Crypto
        libb2-1.dll
        libcrypto-1_1-x64.dll
        libssl-1_1-x64.dll
        libgmp-10.dll
        libgnutls-30.dll
        libnettle-8.dll
        
        # Compressão e arquivos
        libbrotlicommon.dll
        libbrotlidec.dll
        libbrotlienc.dll
        libzstd.dll
        zlib1.dll
        libbz2-1.dll
        liblzma-5.dll
        
        # Renderização de texto, fontes e imagens
        libharfbuzz-0.dll
        libfreetype-6.dll
        libgraphite2.dll
        libpng16-16.dll
        libjpeg-8.dll
        libtiff-6.dll
        libwebp-7.dll
        libdbus-1-3.dll
        
        # PCRE e outras libs base
        libpcre2-16-0.dll
        libpcre2-8-0.dll
        libpcre2-posix-3.dll
        libpcre-1.dll
        
        # Suporte a XML e dados
        libxml2-2.dll
        libxslt-1.dll
        libdouble-conversion.dll
        libmd4c.dll
        libyaml-0-2.dll
        
        # Banco de dados
        libsqlite3-0.dll
        libmariadb.dll
        libmysql.dll
        libpq.dll
        libodbc32.dll
        
        # Internacionalização e sistema
        libiconv-2.dll
        libintl-8.dll
        libglib-2.0-0.dll
        libffi-8.dll
        
        # Extras do sistema
        libatomic-1.dll
        libgmodule-2.0-0.dll
        libgobject-2.0-0.dll
        libgthread-2.0-0.dll
        
        # Qt plugins (além dos já copiados pelo windeployqt)
        # Nota: windeployqt já copia qwindows.dll e outros plugins
        
        # Codecs e formatos adicionais
        libasprintf-0.dll
        libbrotlidec.dll
        libdeflate.dll
        libeay32.dll
        libexpat-1.dll
        libfam-0.dll
        libfribidi-0.dll
        libgettextlib-0-21.dll
        libgettextpo-0.dll
        libgettextsrc-0-21.dll
        libgio-2.0-0.dll
        libjasper-4.dll
        libjbig-0.dll
        libjpeg-8.dll
        libLerc.dll
        libmng-2.dll
        libthai-0.dll
        libunistring-5.dll
        libzstd.dll
    )
    set(found_mingw_dlls)
    foreach(dll ${MINGW_DLLS})
        find_program(MINGW_${dll}_PATH NAMES ${dll} PATHS ${QT_BIN_DIR} $ENV{PATH})
        if(MINGW_${dll}_PATH)
            list(APPEND found_mingw_dlls ${MINGW_${dll}_PATH})
            # Clear the variable for the next find_program call
            unset(MINGW_${dll}_PATH CACHE)
        endif()
    endforeach()
    if(found_mingw_dlls)
        message(STATUS "Including MinGW runtime DLLs: ${found_mingw_dlls}")
        install(FILES ${found_mingw_dlls} DESTINATION bin OPTIONAL)
    endif()
endif()

# Qt deployment: copy dependent DLLs (for Windows)
if(WIN32)
    include(InstallRequiredSystemLibraries)
    install(FILES 
        $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>
        DESTINATION bin
        OPTIONAL
    )
endif()

# CPack WiX configuration for MSI generation
set(CPACK_GENERATOR "WIX")
set(CPACK_PACKAGE_NAME "DevToolsManager")
set(CPACK_PACKAGE_VENDOR "DevTools Inc.")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Software para gerenciar ferramentas de desenvolvimento")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "DevToolsManager")

# Nome do arquivo de pacote (ex.: DevToolsManager-1.0.0.msi)
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")

# WiX specific settings
set(CPACK_WIX_UPGRADE_GUID "1f2e3d4c-5b6a-4c7d-8e9f-0a1b2c3d4e5f") # Use a unique GUID (format: 8-4-4-4-12 hex)
set(CPACK_WIX_PRODUCT_GUID "a1b2c3d4-e5f6-4728-9abc-def012345678") # Use a unique GUID (format: 8-4-4-4-12 hex)
if(EXISTS "${CMAKE_SOURCE_DIR}/LICENSE.rtf")
    set(CPACK_WIX_LICENSE_RTF "${CMAKE_SOURCE_DIR}/LICENSE.rtf") # Optional: Include if you have a license file
else()
    message(STATUS "CPack: LICENSE.rtf not found in source dir; MSI will be built without a license dialog.")
endif()

include(CPack)

