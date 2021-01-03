cmake_minimum_required(VERSION 3.11.4)

# Define the project
project(dsp LANGUAGES CXX)

# Get CMake modules from Git
include(FetchContent)
FetchContent_Declare(cmake-modules
  GIT_REPOSITORY https://github.com/tnt-coders/cmake-modules.git
  GIT_TAG master
)
FetchContent_MakeAvailable(cmake-modules)

# Include the required modules
include(tnt/project/Project)

# Initialize the project
tnt_project_New(${PROJECT_NAME}
  NAMESPACE tnt
  CONANFILE conanfile.py
  VERSION_FROM_GIT
)

# Find and install dependencies
tnt_project_ConanInstall(${PROJECT_NAME})
include(${CMAKE_CURRENT_BINARY_DIR}/conan_paths.cmake)
find_package(Catch2 REQUIRED)
find_package(math REQUIRED)

# Enable testing for the project
# Note: must be in top level CMakeLists.txt
enable_testing()

add_subdirectory(src)
add_subdirectory(test)

# Install project targets
tnt_project_Install(${PROJECT_NAME})

# Create a conan package from the project
tnt_project_Package(${PROJECT_NAME}
  USER tnt-coders
)