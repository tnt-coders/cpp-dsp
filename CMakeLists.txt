cmake_minimum_required(
  VERSION 3.14.0
)

project(dsp
  LANGUAGES CXX
  VERSION 0.0.1
)

include(FetchContent)
FetchContent_Declare(CMakeModules
  GIT_REPOSITORY https://github.com/tnt-coders/cmake-modules.git
)
FetchContent_MakeAvailable(CMakeModules)

include(Conan)
conan_install()

add_subdirectory(docs)
add_subdirectory(src)
add_subdirectory(test)

include(InstallProject)
install_project(
  NAMESPACE tnt
  TARGETS dsp
)