include(GoogleTest)

add_executable(dsp_test
    main.cpp
    analysis_test.cpp
    convolution_test.cpp
    fourier_transform_test.cpp
    hilbert_transform_test.cpp
    multisignal_test.cpp
    signal_generator_test.cpp
    signal_test.cpp
)

target_compile_options(dsp_test PRIVATE -D_USE_MATH_DEFINES)

target_link_libraries(dsp_test
    tnt::dsp
    CONAN_PKG::gtest
)

gtest_discover_tests(dsp_test)