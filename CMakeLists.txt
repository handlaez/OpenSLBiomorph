cmake_minimum_required(VERSION 3.10)
project(OpenSLBiomorph CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(OpenCL REQUIRED)

add_executable(OpenSLBiomorph Main.cpp)

target_link_libraries(OpenSLBiomorph PRIVATE OpenCL::OpenCL)

target_include_directories(OpenSLBiomorph PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# copy kernel.cl to the build directory so the binary can find it at runtime
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel.cl 
    ${CMAKE_CURRENT_BINARY_DIR}/kernel.cl 
    COPYONLY
)