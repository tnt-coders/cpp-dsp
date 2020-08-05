cmake_minimum_required(VERSION 3.11.4)
project(TestPackage LANGUAGES CXX)

include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
find_package(cpp-dsp REQUIRED)

add_executable(dsp_package_test main.cpp)
target_link_libraries(dsp_package_test tnt::dsp)