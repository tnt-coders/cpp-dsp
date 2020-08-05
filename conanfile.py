from conans import ConanFile, CMake, tools

class CppDspConan(ConanFile):
    author = "TNT Coders <tnt-coders@googlegroups.com>"
    default_options = {"shared": False}
    description = "C++ Implementation of common digital signal processing algorithms"
    exports_sources = "CMakeLists.txt", "docs/*", "include/*", "src/*", "test/*"
    generators = "cmake_paths"
    license = "GNU Lesser General Public License v3.0"
    name = "cpp-dsp"
    options = {"shared": [True, False]}
    requires = "gtest/1.8.1@bincrafters/stable"
    settings = "os", "compiler", "build_type", "arch"
    topics = ("dsp")
    url = "https://github.com/tnt-coders/cpp-dsp"
    version = "0.0.0"

    def _configure_cmake(self):
        cmake = CMake(self)
        cmake.configure()
        return cmake

    def build(self):
        cmake = self._configure_cmake()
        cmake.build()

    def package(self):
        cmake = self._configure_cmake()
        cmake.install()