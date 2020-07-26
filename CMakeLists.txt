cmake_minimum_required(VERSION 3.11.4)

# Define the project
project(dsp LANGUAGES CXX)

# Get CMake modules from Git
include(FetchContent)
FetchContent_Declare(CMakeModules GIT_REPOSITORY https://github.com/tnt-coders/cmake-modules.git)
FetchContent_MakeAvailable(CMakeModules)

# Include the required modules
include(tnt/project/Project)

# Define a namespace for the project
tnt_project_New(${PROJECT_NAME}
  NAMESPACE tnt
  CONANFILE conanfile.txt
  VERSION_FROM_GIT
)

# Install and find dependencies
tnt_project_ConanInstall(${PROJECT_NAME} CMAKE_TARGETS)

add_subdirectory(docs)
add_subdirectory(src)
add_subdirectory(test)

# Install project targets
tnt_project_Install(${PROJECT_NAME})