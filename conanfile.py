from conans import ConanFile, CMake, tools

class CppDsp(ConanFile):
    author = "TNT Coders <tnt-coders@googlegroups.com>"
    build_requires = "gtest/1.8.1"
    default_options = {"shared": False}
    description = "C++ Implementation of common digital signal processing algorithms"
    exports_sources = "CMakeLists.txt", "docs/*", "include/*", "src/*", "test/*"
    generators = "cmake"
    license = "GNU Lesser General Public License v3.0"
    name = "cpp-dsp"
    options = {"shared": [True, False]}
    settings = "os", "compiler", "build_type", "arch"
    topics = ("dsp")
    url = "https://github.com/tnt-coders/cpp-dsp"
    version = "0.0.1"

    def configure(self):
        tools.check_min_cppstd(self, "17")

    def build(self):
        cmake = self._configure_cmake()
        cmake.build()
        cmake.test()

    def package(self):
        cmake = self._configure_cmake()
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = self.collect_libs()

    def _configure_cmake(self):
        cmake = CMake(self)
        cmake.configure()
        return cmake