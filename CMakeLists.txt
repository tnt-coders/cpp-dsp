cmake_minimum_required(VERSION 3.11.4)

# Define the project
project(cpp-dsp LANGUAGES CXX)

# Get CMake modules from Git
include(FetchContent)
FetchContent_Declare(CMakeModules GIT_REPOSITORY https://github.com/tnt-coders/cmake-modules.git)
FetchContent_MakeAvailable(CMakeModules)

# Include the required modules
include(tnt/project/Project)

# Initialize the project
tnt_project_New(${PROJECT_NAME}
  NAMESPACE tnt
  CONANFILE conanfile.py
)

# Find and install dependencies
tnt_project_ConanInstall(${PROJECT_NAME})

# Enable testing for the project
# Note: must be in top level CMakeLists.txt
enable_testing()

add_subdirectory(docs)
add_subdirectory(src)
add_subdirectory(test)

# Install project targets
tnt_project_Install(${PROJECT_NAME})