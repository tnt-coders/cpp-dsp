cmake_minimum_required(VERSION 2.8.12)
project(TestPackage LANGUAGES CXX)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

add_executable(dsp_package_test main.cpp)
target_link_libraries(dsp_package_test CONAN_PKG::cpp-dsp)