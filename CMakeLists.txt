cmake_minimum_Required(VERSION 3.10)
project(OpenSLBiomorph LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

list(APPEND CMAKE_PREFIX_PATH "C:/Qt/6.11.2/msvc2022_64")
find_package(Qt6 REQUIRED COMPONENTS Quick QuickControls2)
find_package(OpenCL REQUIRED)

qt_add_executable(OpenSLBiomorph
    main.cpp
    src/BiomorphGenerator.cpp
    src/BiomorphGenerator.hpp
    src/BiomorphWorker.cpp
    src/BiomorphWorker.hpp
    src/BiomorphImage.hpp
    src/BiomorphParameters.hpp
    src/BiomorphController.cpp
    src/BiomorphController.hpp
    src/stb_image_write.cpp
)

qt_add_qml_module(OpenSLBiomorph
    URI Biomorph
    VERSION 1.0
    QML_FILES
        qml/Main.qml
)

target_link_libraries(OpenSLBiomorph
    PRIVATE
        Qt6::Quick
        Qt6::QuickControls2
        OpenCL::OpenCL
)

target_include_directories(OpenSLBiomorph PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# copy kernel.cl to the build directory so the binary can find it at runtime
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel.cl 
    ${CMAKE_CURRENT_BINARY_DIR}/kernel.cl 
    COPYONLY
)

# deploy Qt runtime dependencies next to the .exe
if(WIN32)
    get_filename_component(QT_ROOT "${Qt6_DIR}/../../../" ABSOLUTE)
    set(WINDEPLOYQT "${QT_ROOT}/bin/windeployqt.exe")

    add_custom_command(TARGET OpenSLBiomorph POST_BUILD
        COMMAND "${WINDEPLOYQT}"
                --debug
                --qmldir "${CMAKE_CURRENT_SOURCE_DIR}/qml"
                "$<TARGET_FILE:OpenSLBiomorph>"
        COMMENT "Deploying Qt runtime dependencies..."
    )
endif()