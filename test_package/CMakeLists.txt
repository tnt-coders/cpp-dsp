cmake_minimum_required(VERSION 3.11.4)
project(test_package LANGUAGES CXX)

# Sets CMake variables required by Conan
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# Sets up paths required for find_package
include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
find_package(cpp-dsp REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} tnt::dsp)