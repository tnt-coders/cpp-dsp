# Overview

cpp-dsp is an open source, cross platform, C++ implementation of common digital
signal processing algorithms.

## Documentation

[![Documentation](https://codedocs.xyz/tnt-coders/cpp-dsp.svg)](https://codedocs.xyz/tnt-coders/cpp-dsp/)

Documentation can be found at <https://codedocs.xyz/tnt-coders/cpp-dsp/>.

## Disclaimer

This library is still a work in progress, but since it can already calculate
fast Fourier transforms of arbitrary sizes relatively quickly I am choosing to
make it public now because others may find it useful.

# Build Instructions

cpp-dsp can be built as a standalone CMake project.

    cmake -H. -Bbuild
    cmake --build build

To test the project, run CTest from the build directory.

    cd build
    ctest

## Build Requirements

* CMake v3.11.4 (or later)
* C++17 compliant compiler (or later)
* Conan package manager v1.29.2 (or later)

## Dependencies

This project requires the Conan package manager to automatically manage all of
its dependencies. To install Conan see:
https://docs.conan.io/en/latest/installation.html
