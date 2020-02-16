#TODO: Make this an optional passed parameter when running cmake
set(MAX_FFT_SIZE 262144)
configure_file(fft_constants.hpp.in fft_constants.hpp @ONLY)

add_library(dsp
    analysis.cpp
    convolution.cpp
    dft.cpp
    fft.cpp
)

add_library(TNT::dsp
  ALIAS dsp
)

target_compile_features(dsp
  PUBLIC cxx_std_20
)

target_compile_options(dsp
  PRIVATE -D_USE_MATH_DEFINES
)

target_include_directories(dsp
  PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
         $<INSTALL_INTERFACE:include>
  PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
          $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/tnt/dsp>
          $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
)
