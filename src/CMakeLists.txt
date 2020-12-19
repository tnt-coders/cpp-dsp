tnt_project_AddLibrary(${PROJECT_NAME} TARGET ${PROJECT_NAME} INTERFACE)

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)

target_compile_options(${PROJECT_NAME} INTERFACE -D_USE_MATH_DEFINES)