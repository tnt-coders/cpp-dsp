#include <catch2/catch_template_test_macros.hpp>
#include <cmath>
#include <complex>
#include <tnt/dsp/analysis.hpp>
#include <tnt/dsp/signal.hpp>
#include <tnt/dsp/signal_generator.hpp>
#include <tnt/math/comparison.hpp>

using namespace tnt;

TEMPLATE_TEST_CASE("magnitude", "[magnitude]", double, float)
{
    const dsp::SignalGenerator<TestType> g(4000, 4);

    SECTION("Magnitude of a real sample")
    {
        CHECK(math::near(dsp::magnitude<TestType>(1), 1));
        CHECK(math::near(dsp::magnitude<TestType>(-1), 1));
    }

    SECTION("Magnitude of a complex sample")
    {
        CHECK(math::near(dsp::magnitude(std::complex<TestType>(3, 4)), 5));
        CHECK(math::near(dsp::magnitude(std::complex<TestType>(-3, 4)), 5));
        CHECK(math::near(dsp::magnitude(std::complex<TestType>(3, -4)), 5));
        CHECK(math::near(dsp::magnitude(std::complex<TestType>(-3, -4)), 5));
    }

    SECTION("Magnitude of a real signal")
    {
        const auto x           = g.cosine(1000);
        const auto x_magnitude = dsp::magnitude(x);

        CHECK(math::near(x_magnitude[0], 1));
        CHECK(math::near(x_magnitude[1], 0));
        CHECK(math::near(x_magnitude[2], 1));
        CHECK(math::near(x_magnitude[3], 0));
    }

    SECTION("Magnitude of a complex signal")
    {
        const auto x           = dsp::complex_signal(g.cosine(1000), g.sine(1000));
        const auto x_magnitude = dsp::magnitude(x);

        CHECK(math::near(x_magnitude[0], 1));
        CHECK(math::near(x_magnitude[1], 1));
        CHECK(math::near(x_magnitude[2], 1));
        CHECK(math::near(x_magnitude[3], 1));
    }
}

TEMPLATE_TEST_CASE("phase", "[phase]", double, float)
{
    const dsp::SignalGenerator<TestType> g(4000, 4);

    SECTION("Phase of a real sample")
    {
        CHECK(math::near(dsp::phase<TestType>(1), 0));
        CHECK(math::near(dsp::phase<TestType>(-1), M_PI));
    }

    SECTION("Phase of a complex sample")
    {
        CHECK(math::near(dsp::phase(std::complex<TestType>(1, 1)), M_PI / 4));
        CHECK(math::near(dsp::phase(std::complex<TestType>(-1, 1)), 3 * M_PI / 4));
        CHECK(math::near(dsp::phase(std::complex<TestType>(1, -1)), -M_PI / 4));
        CHECK(math::near(dsp::phase(std::complex<TestType>(-1, -1)), -3 * M_PI / 4));
    }

    SECTION("Phase of a real signal")
    {
        auto       x       = g.cosine(1000);
        const auto x_phase = dsp::phase(x);

        CHECK(math::near(x_phase[0], 0));
        CHECK(math::near(x_phase[1], 0));
        CHECK(math::near(x_phase[2], M_PI));
        CHECK(math::near(x_phase[3], 0));
    }

    SECTION("Phase of a complex signal")
    {
        auto       x       = dsp::complex_signal(g.cosine(1000), g.sine(1000));
        const auto x_phase = dsp::phase(x);

        CHECK(math::near(x_phase[0], 0));
        CHECK(math::near(x_phase[1], M_PI / 2));
        CHECK(math::near(x_phase[2], M_PI));
        CHECK(math::near(x_phase[3], -M_PI / 2));
    }
}

TEMPLATE_TEST_CASE("power", "[power]", double, float)
{
    const dsp::SignalGenerator<TestType> g(4000, 4);

    SECTION("Power of a real sample")
    {
        CHECK(math::near(dsp::power<TestType>(1), 1));
        CHECK(math::near(dsp::power<TestType>(-1), 1));
        CHECK(math::near(dsp::power<TestType>(2), 4));
        CHECK(math::near(dsp::power<TestType>(-2), 4));
    }

    SECTION("Power of a complex sample")
    {
        CHECK(math::near(dsp::power(std::complex<TestType>(3, 4)), 25));
        CHECK(math::near(dsp::power(std::complex<TestType>(-3, 4)), 25));
        CHECK(math::near(dsp::power(std::complex<TestType>(3, -4)), 25));
        CHECK(math::near(dsp::power(std::complex<TestType>(-3, -4)), 25));
    }

    SECTION("Power of a real signal")
    {
        const auto x1       = g.cosine(1000);
        const auto x2       = g.cosine(1000, 2);
        const auto x1_power = dsp::power(x1);
        const auto x2_power = dsp::power(x2);

        CHECK(math::near(x1_power[0], 1));
        CHECK(math::near(x1_power[1], 0));
        CHECK(math::near(x1_power[2], 1));
        CHECK(math::near(x1_power[3], 0));
        CHECK(math::near(x2_power[0], 4));
        CHECK(math::near(x2_power[1], 0));
        CHECK(math::near(x2_power[2], 4));
        CHECK(math::near(x2_power[3], 0));
    }

    SECTION("Power of a complex signal")
    {
        const auto x1       = dsp::complex_signal(g.cosine(1000), g.sine(1000));
        const auto x2       = dsp::complex_signal(g.cosine(1000, 2), g.sine(1000, 2));
        const auto x1_power = dsp::power(x1);
        const auto x2_power = dsp::power(x2);

        CHECK(math::near(x1_power[0], 1));
        CHECK(math::near(x1_power[1], 1));
        CHECK(math::near(x1_power[2], 1));
        CHECK(math::near(x1_power[3], 1));
        CHECK(math::near(x2_power[0], 4));
        CHECK(math::near(x2_power[1], 4));
        CHECK(math::near(x2_power[2], 4));
        CHECK(math::near(x2_power[3], 4));
    }
}
