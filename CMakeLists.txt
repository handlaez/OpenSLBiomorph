cmake_minimum_required(VERSION 3.10)
project(OpenSLBiomorph LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

list(APPEND CMAKE_PREFIX_PATH "C:/Qt/6.11.2/msvc2022_64")
find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(OpenCL REQUIRED)

qt_add_executable(OpenSLBiomorph
    main.cpp

    src/BiomorphGenerator.cpp
    src/BiomorphGenerator.hpp

    src/BiomorphWorker.cpp
    src/BiomorphWorker.hpp

    src/BiomorphParameters.hpp
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
        OpenCL::OpenCL
)

target_include_directories(OpenSLBiomorph PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# copy kernel.cl to the build directory so the binary can find it at runtime
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel.cl 
    ${CMAKE_CURRENT_BINARY_DIR}/kernel.cl 
    COPYONLY
)