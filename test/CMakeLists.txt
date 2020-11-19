include(Catch)
include(CTest)

add_executable(dsp_test
    main.cpp
    analysis.cpp
    approx.cpp
    convolution.cpp
    fourier_transform.cpp
    hilbert_transform.cpp
    multisignal.cpp
    signal.cpp
    signal_generator.cpp
)

target_compile_options(dsp_test PRIVATE -D_USE_MATH_DEFINES)

target_link_libraries(dsp_test
    tnt::dsp
    Catch2::Catch2WithMain
)

catch_discover_tests(dsp_test)