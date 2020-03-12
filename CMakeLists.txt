cmake_minimum_required(VERSION 3.14.0)

# Get CMake modules from Git
include(FetchContent)
FetchContent_Declare(CMakeModules GIT_REPOSITORY https://github.com/tnt-coders/cmake-modules.git)
FetchContent_MakeAvailable(CMakeModules)

# Include the required modules
include(tnt/Conan)
include(tnt/Git)
include(tnt/Project)

# Define the project
project(dsp LANGUAGES CXX)

# Define a namespace for the project
tnt_project_SetNamespace(tnt)

# Get the project version info from Git
tnt_git_DefineProjectVersion()

# Install dependencies with Conan
tnt_conan_Install(CONANFILE conanfile.txt)

add_subdirectory(docs)
add_subdirectory(src)
add_subdirectory(test)

# Install project targets
tnt_project_Install()