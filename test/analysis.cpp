#include <catch2/catch_approx.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <complex>
#include <tnt/dsp/analysis.hpp>
#include <tnt/dsp/signal.hpp>
#include <tnt/dsp/signal_generator.hpp>

using namespace tnt;

using Catch::Approx;

TEMPLATE_TEST_CASE("analysis", "[analysis]", double, float)
{
    // Margin of error when testing 0 values
    constexpr auto epsilon = std::numeric_limits<TestType>::epsilon() * 100;

    SECTION("magnitude")
    {
        SECTION("Magnitude of a real sample")
        {
            CHECK(dsp::magnitude(static_cast<TestType>(1)) == Approx(static_cast<TestType>(1.0)));
            CHECK(dsp::magnitude(static_cast<TestType>(-1)) == Approx(static_cast<TestType>(1.0)));
        }

        SECTION("Magnitude of a complex sample")
        {
            CHECK(dsp::magnitude(std::complex<TestType>{ 3.0, 4.0 }) == Approx(static_cast<TestType>(5.0)));
            CHECK(dsp::magnitude(std::complex<TestType>{ -3.0, 4.0 }) == Approx(static_cast<TestType>(5.0)));
            CHECK(dsp::magnitude(std::complex<TestType>{ 3.0, -4.0 }) == Approx(static_cast<TestType>(5.0)));
            CHECK(dsp::magnitude(std::complex<TestType>{ -3.0, -4.0 }) == Approx(static_cast<TestType>(5.0)));
        }

        SECTION("Magnitude of a real signal")
        {
            const dsp::signal_generator<TestType> g{ 4000, 4 };
            const auto x = g.cosine(1000);
            const auto x_magnitude = dsp::magnitude(x);

            CHECK(x_magnitude[0] == Approx(static_cast<TestType>(1.0)));
            CHECK(x_magnitude[1] == Approx(static_cast<TestType>(0.0)).margin(epsilon));
            CHECK(x_magnitude[2] == Approx(static_cast<TestType>(1.0)));
            CHECK(x_magnitude[3] == Approx(static_cast<TestType>(0.0)).margin(epsilon));
        }

        SECTION("Magnitude of a complex signal")
        {
            const dsp::signal_generator<TestType> g{ 4000, 4 };
            const auto x = dsp::complex_signal(g.cosine(1000), g.sine(1000));
            const auto x_magnitude = dsp::magnitude(x);

            CHECK(x_magnitude[0] == Approx(static_cast<TestType>(1.0)));
            CHECK(x_magnitude[1] == Approx(static_cast<TestType>(1.0)));
            CHECK(x_magnitude[2] == Approx(static_cast<TestType>(1.0)));
            CHECK(x_magnitude[3] == Approx(static_cast<TestType>(1.0)));
        }
    }

    SECTION("phase")
    {
        SECTION("Phase of a real sample")
        {
            CHECK(dsp::phase(static_cast<TestType>(1)) == Approx(static_cast<TestType>(0.0)).margin(epsilon));
            CHECK(dsp::phase(static_cast<TestType>(-1)) == Approx(static_cast<TestType>(M_PI)));
        }

        SECTION("Phase of a complex sample")
        {
            CHECK(dsp::phase(std::complex<TestType>{ 1.0, 1.0 }) == Approx(static_cast<TestType>(M_PI / 4)));
            CHECK(dsp::phase(std::complex<TestType>{ -1.0, 1.0 }) == Approx(static_cast<TestType>(3 * M_PI / 4)));
            CHECK(dsp::phase(std::complex<TestType>{ 1.0, -1.0 }) == Approx(static_cast<TestType>(-M_PI / 4)));
            CHECK(dsp::phase(std::complex<TestType>{ -1.0, -1.0 }) == Approx(static_cast<TestType>(-3 * M_PI / 4)));
        }

        SECTION("Phase of a real signal")
        {
            const dsp::signal_generator<TestType> g{ 4000, 4 };
            auto x = g.cosine(1000);

            const auto x_phase = dsp::phase(x);

            CHECK(x_phase[0] == Approx(static_cast<TestType>(0.0)).margin(epsilon));
            CHECK(x_phase[1] == Approx(static_cast<TestType>(0.0)).margin(epsilon));
            CHECK(x_phase[2] == Approx(static_cast<TestType>(M_PI)));
            CHECK(x_phase[3] == Approx(static_cast<TestType>(0.0)).margin(epsilon));
        }

        SECTION("Phase of a complex signal")
        {
            const dsp::signal_generator<TestType> g{ 4000, 4 };
            auto x = dsp::complex_signal(g.cosine(1000), g.sine(1000));

            const auto x_phase = dsp::phase(x);

            CHECK(x_phase[0] == Approx(static_cast<TestType>(0.0)).margin(epsilon));
            CHECK(x_phase[1] == Approx(M_PI / 2));
            CHECK(x_phase[2] == Approx(M_PI));
            CHECK(x_phase[3] == Approx(-M_PI / 2));
        }
    }
}
//
//    void complex_signal() const
//    {
//        const dsp::signal_generator<T> g{ 4000, 4 };
//        dsp::signal<std::complex<T>> x{ g.cosine(1000), g.sine(1000) };
//
//        // Phase doesn't calculate accurately for very small values
//        // If the magnitude is below EPSILON assume the value is 0
//        for (auto& sample : x)
//        {
//            if (dsp::magnitude(sample.real()) < constants::EPSILON)
//            {
//                sample.real(0);
//            }
//            if (dsp::magnitude(sample.imag()) < constants::EPSILON)
//            {
//                sample.imag(0);
//            }
//        }
//
//        const auto x_phase = dsp::phase(x);
//
//        EXPECT_NEAR(x_phase[0], 0.0, constants::EPSILON);
//        EXPECT_NEAR(x_phase[1], M_PI / 2, constants::EPSILON);
//        EXPECT_NEAR(x_phase[2], M_PI, constants::EPSILON);
//        EXPECT_NEAR(x_phase[3], -M_PI / 2, constants::EPSILON);
//    }
//};
//
//TYPED_TEST_SUITE(magnitude, test_types);
//
//TYPED_TEST(magnitude, real_sample)
//{
//    this->real_sample();
//}
//
//TYPED_TEST(magnitude, complex_sample)
//{
//    this->complex_sample();
//}
//
//TYPED_TEST(magnitude, real_signal)
//{
//    this->real_signal();
//}
//
//TYPED_TEST(magnitude, complex_signal)
//{
//    this->complex_signal();
//}
//
//TYPED_TEST_SUITE(phase, test_types);
//
//TYPED_TEST(phase, real_sample)
//{
//    this->real_sample();
//}
//
//TYPED_TEST(phase, complex_sample)
//{
//    this->complex_sample();
//}
//
//TYPED_TEST(phase, real_signal)
//{
//    this->real_signal();
//}
//
//TYPED_TEST(phase, complex_signal)
//{
//    this->complex_signal();
//}

    //void Power_RealSample() const
    //{
    //    EXPECT_NEAR(dsp::power(static_cast<T>(1)), 1.0, constants::EPSILON);
    //    EXPECT_NEAR(dsp::power(static_cast<T>(-1)), 1.0, constants::EPSILON);
    //    EXPECT_NEAR(dsp::power(static_cast<T>(2)), 4.0, constants::EPSILON);
    //    EXPECT_NEAR(dsp::power(static_cast<T>(-2)), 4.0, constants::EPSILON);
    //}

    //void Power_ComplexSample() const
    //{
    //    EXPECT_NEAR(dsp::power(std::complex<T>{ 3.0, 4.0 }), 25.0, constants::EPSILON);
    //    EXPECT_NEAR(dsp::power(std::complex<T>{ -3.0, 4.0 }), 25.0, constants::EPSILON);
    //    EXPECT_NEAR(dsp::power(std::complex<T>{ 3.0, -4.0 }), 25.0, constants::EPSILON);
    //    EXPECT_NEAR(dsp::power(std::complex<T>{ -3.0, -4.0 }), 25.0, constants::EPSILON);
    //}

    //void Power_RealSignal() const
    //{
    //    const dsp::signal_generator<T> g{ 4000, 4 };
    //    const auto x1 = g.cosine(1000);
    //    const auto x2 = g.cosine(1000, 2);
    //    const auto x1_power = dsp::power(x1);
    //    const auto x2_power = dsp::power(x2);

    //    EXPECT_NEAR(x1_power[0], 1.0, constants::EPSILON);
    //    EXPECT_NEAR(x1_power[1], 0.0, constants::EPSILON);
    //    EXPECT_NEAR(x1_power[2], 1.0, constants::EPSILON);
    //    EXPECT_NEAR(x1_power[3], 0.0, constants::EPSILON);
    //    EXPECT_NEAR(x2_power[0], 4.0, constants::EPSILON);
    //    EXPECT_NEAR(x2_power[1], 0.0, constants::EPSILON);
    //    EXPECT_NEAR(x2_power[2], 4.0, constants::EPSILON);
    //    EXPECT_NEAR(x2_power[3], 0.0, constants::EPSILON);
    //}

    //void Power_ComplexSignal() const
    //{
    //    const dsp::signal_generator<T> g{ 4000, 4 };
    //    const dsp::signal<std::complex<T>> x1{ g.cosine(1000), g.sine(1000) };
    //    const dsp::signal<std::complex<T>> x2{ g.cosine(1000, 2), g.sine(1000, 2) };
    //    const auto x1_power = dsp::power(x1);
    //    const auto x2_power = dsp::power(x2);

    //    EXPECT_NEAR(x1_power[0], 1.0, constants::EPSILON);
    //    EXPECT_NEAR(x1_power[1], 1.0, constants::EPSILON);
    //    EXPECT_NEAR(x1_power[2], 1.0, constants::EPSILON);
    //    EXPECT_NEAR(x1_power[3], 1.0, constants::EPSILON);
    //    EXPECT_NEAR(x2_power[0], 4.0, constants::EPSILON);
    //    EXPECT_NEAR(x2_power[1], 4.0, constants::EPSILON);
    //    EXPECT_NEAR(x2_power[2], 4.0, constants::EPSILON);
    //    EXPECT_NEAR(x2_power[3], 4.0, constants::EPSILON);
    //}




//
//TYPED_TEST(analysis_test, Power_RealSample)
//{
//    this->Power_RealSample();
//}
//
//TYPED_TEST(analysis_test, Power_ComplexSample)
//{
//    this->Power_ComplexSample();
//}
//
//TYPED_TEST(analysis_test, Power_RealSignal)
//{
//    this->Power_RealSignal();
//}
//
//TYPED_TEST(analysis_test, Power_ComplexSignal)
//{
//    this->Power_ComplexSignal();
//}