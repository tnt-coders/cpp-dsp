tnt_project_AddLibrary(${PROJECT_NAME} TARGET dsp INTERFACE)

target_compile_features(dsp INTERFACE cxx_std_17)

target_compile_options(dsp INTERFACE -D_USE_MATH_DEFINES)