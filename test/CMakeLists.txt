include(GoogleTest)

add_executable(dsp_test
    main.cpp
    analysis_test.cpp
    convolution_test.cpp
    dft_test.cpp
    fft_test.cpp
)

target_compile_options(dsp_test PRIVATE -D_USE_MATH_DEFINES)

find_package(GTest REQUIRED)
target_link_libraries(dsp_test
    tnt::dsp
    GTest::GTest
)

gtest_discover_tests(dsp_test)