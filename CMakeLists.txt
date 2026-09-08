cmake_minimum_required(VERSION 3.16)
project(OpenCLBiomorph LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Fallback path for local builds if CMAKE_PREFIX_PATH isn't passed via command line
if(NOT CMAKE_PREFIX_PATH)
    list(APPEND CMAKE_PREFIX_PATH "C:/Qt/6.11.2/msvc2022_64")
endif()

find_package(Qt6 REQUIRED COMPONENTS Quick QuickControls2)
find_package(OpenCL REQUIRED)

qt_add_executable(OpenCLBiomorph
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

set_target_properties(OpenCLBiomorph PROPERTIES
    WIN32_EXECUTABLE TRUE
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}"
)

qt_add_qml_module(OpenCLBiomorph
    URI Biomorph
    VERSION 1.0
    QML_FILES
        qml/Main.qml
)

target_link_libraries(OpenCLBiomorph
    PRIVATE
        Qt6::Quick
        Qt6::QuickControls2
        OpenCL::OpenCL
)

target_include_directories(OpenCLBiomorph PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Copy kernel.cl to build directory for runtime availability
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel.cl 
    ${CMAKE_CURRENT_BINARY_DIR}/kernel.cl 
    COPYONLY
)

# Deploy Qt dependencies next to the binary
if(WIN32)
    get_filename_component(QT_BIN_DIR "${Qt6_DIR}/../../../bin" ABSOLUTE)
    set(WINDEPLOYQT "${QT_BIN_DIR}/windeployqt.exe")

    add_custom_command(TARGET OpenCLBiomorph POST_BUILD
        COMMAND "${WINDEPLOYQT}"
                $<$<CONFIG:Debug>:--debug>$<$<CONFIG:Release>:--release>
                --qmldir "${CMAKE_CURRENT_SOURCE_DIR}/qml"
                "$<TARGET_FILE:OpenCLBiomorph>"
        COMMENT "Deploying Qt runtime dependencies..."
    )
endif()