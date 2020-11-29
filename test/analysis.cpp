#include "approx.hpp"
#include <catch2/catch_template_test_macros.hpp>
#include <cmath>
#include <complex>
#include <tnt/dsp/analysis.hpp>
#include <tnt/dsp/signal.hpp>
#include <tnt/dsp/signal_generator.hpp>

using namespace tnt;

TEMPLATE_TEST_CASE("magnitude", "[magnitude]", double, float)
{
    const dsp::signal_generator<TestType> g(4000, 4);

    SECTION("Magnitude of a real sample")
    {
        CHECK(dsp::magnitude(static_cast<TestType>(1)) == approx(1));
        CHECK(dsp::magnitude(static_cast<TestType>(-1)) == approx(1));
    }

    SECTION("Magnitude of a complex sample")
    {
        CHECK(dsp::magnitude(std::complex<TestType>(3, 4)) == approx(5));
        CHECK(dsp::magnitude(std::complex<TestType>(-3, 4)) == approx(5));
        CHECK(dsp::magnitude(std::complex<TestType>(3, -4)) == approx(5));
        CHECK(dsp::magnitude(std::complex<TestType>(-3, -4)) == approx(5));
    }

    SECTION("Magnitude of a real signal")
    {
        const auto x = g.cosine(1000);
        const auto x_magnitude = dsp::magnitude(x);

        CHECK(x_magnitude[0] == approx(1));
        CHECK(x_magnitude[1] == approx(0));
        CHECK(x_magnitude[2] == approx(1));
        CHECK(x_magnitude[3] == approx(0));
    }

    SECTION("Magnitude of a complex signal")
    {
        const auto x = dsp::complex_signal(g.cosine(1000), g.sine(1000));
        const auto x_magnitude = dsp::magnitude(x);

        CHECK(x_magnitude[0] == approx(1));
        CHECK(x_magnitude[1] == approx(1));
        CHECK(x_magnitude[2] == approx(1));
        CHECK(x_magnitude[3] == approx(1));
    }
}

TEMPLATE_TEST_CASE("phase", "[phase]", double, float)
{
    const dsp::signal_generator<TestType> g(4000, 4);

    SECTION("Phase of a real sample")
    {
        CHECK(dsp::phase(static_cast<TestType>(1)) == approx(0));
        CHECK(dsp::phase(static_cast<TestType>(-1)) == approx(M_PI));
    }

    SECTION("Phase of a complex sample")
    {
        CHECK(dsp::phase(std::complex<TestType>(1, 1)) == approx(M_PI / 4));
        CHECK(dsp::phase(std::complex<TestType>(-1, 1)) == approx(3 * M_PI / 4));
        CHECK(dsp::phase(std::complex<TestType>(1, -1)) == approx(-M_PI / 4));
        CHECK(dsp::phase(std::complex<TestType>(-1, -1)) == approx(-3 * M_PI / 4));
    }

    SECTION("Phase of a real signal")
    {
        auto x = g.cosine(1000);
        const auto x_phase = dsp::phase(x);

        CHECK(x_phase[0] == approx(0));
        CHECK(x_phase[1] == approx(0));
        CHECK(x_phase[2] == approx(M_PI));
        CHECK(x_phase[3] == approx(0));
    }

    SECTION("Phase of a complex signal")
    {
        auto x = dsp::complex_signal(g.cosine(1000), g.sine(1000));
        const auto x_phase = dsp::phase(x);

        CHECK(x_phase[0] == approx(0));
        CHECK(x_phase[1] == approx(M_PI / 2));
        CHECK(x_phase[2] == approx(M_PI));
        CHECK(x_phase[3] == approx(-M_PI / 2));
    }
}

TEMPLATE_TEST_CASE("power", "[power]", double, float)
{
    const dsp::signal_generator<TestType> g(4000, 4);

    SECTION("Power of a real sample")
    {
        CHECK(dsp::power(static_cast<TestType>(1)) == approx(1));
        CHECK(dsp::power(static_cast<TestType>(-1)) == approx(1));
        CHECK(dsp::power(static_cast<TestType>(2)) == approx(4));
        CHECK(dsp::power(static_cast<TestType>(-2)) == approx(4));
    }

    SECTION("Power of a complex sample")
    {
        CHECK(dsp::power(std::complex<TestType>(3, 4)) == approx(25));
        CHECK(dsp::power(std::complex<TestType>(-3, 4)) == approx(25));
        CHECK(dsp::power(std::complex<TestType>(3, -4)) == approx(25));
        CHECK(dsp::power(std::complex<TestType>(-3, -4)) == approx(25));
    }

    SECTION("Power of a real signal")
    {
        const auto x1 = g.cosine(1000);
        const auto x2 = g.cosine(1000, 2);
        const auto x1_power = dsp::power(x1);
        const auto x2_power = dsp::power(x2);

        CHECK(x1_power[0] == approx(1));
        CHECK(x1_power[1] == approx(0));
        CHECK(x1_power[2] == approx(1));
        CHECK(x1_power[3] == approx(0));
        CHECK(x2_power[0] == approx(4));
        CHECK(x2_power[1] == approx(0));
        CHECK(x2_power[2] == approx(4));
        CHECK(x2_power[3] == approx(0));
    }

    SECTION("Power of a complex signal")
    {
        const auto x1 = dsp::complex_signal(g.cosine(1000), g.sine(1000));
        const auto x2 = dsp::complex_signal(g.cosine(1000, 2), g.sine(1000, 2));
        const auto x1_power = dsp::power(x1);
        const auto x2_power = dsp::power(x2);

        CHECK(x1_power[0] == approx(1));
        CHECK(x1_power[1] == approx(1));
        CHECK(x1_power[2] == approx(1));
        CHECK(x1_power[3] == approx(1));
        CHECK(x2_power[0] == approx(4));
        CHECK(x2_power[1] == approx(4));
        CHECK(x2_power[2] == approx(4));
        CHECK(x2_power[3] == approx(4));
    }
}
