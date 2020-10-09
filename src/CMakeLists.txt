tnt_project_AddLibrary(${PROJECT_NAME}
  TARGET dsp
  SOURCES
    analysis.cpp
    convolution.cpp
    dft.cpp
    fft.cpp
    signal.cpp
)

target_compile_features(dsp PUBLIC cxx_std_17)

target_compile_options(dsp PRIVATE -D_USE_MATH_DEFINES)