include(Catch)
# TODO: Add "include(CTest)" and configure it properly

add_executable(${PROJECT_NAME}_test
    main.cpp
    analysis.cpp
    
    convolution.cpp
    fourier_transform.cpp
    hilbert_transform.cpp
    multisignal.cpp
    signal.cpp
    signal_generator.cpp
)

target_compile_options(${PROJECT_NAME}_test PRIVATE -D_USE_MATH_DEFINES)

target_link_libraries(${PROJECT_NAME}_test
    tnt::${PROJECT_NAME}
    tnt::math
    Catch2::Catch2WithMain
)

catch_discover_tests(${PROJECT_NAME}_test)