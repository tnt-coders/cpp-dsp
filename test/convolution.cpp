#include "approx.hpp"

#include <catch2/catch_template_test_macros.hpp>
#include <tnt/dsp/convolution.hpp>
#include <tnt/dsp/signal.hpp>
#include <tnt/dsp/signal_generator.hpp>

using namespace tnt;

TEMPLATE_TEST_CASE("convolve", "[convolve]", double, float)
{
    const auto g = dsp::signal_generator<TestType>(4000, 4);

    SECTION("Convolve a real signal with a real signal")
    {
        const auto a = g.cosine(1000);
        const auto b = g.sine(1000);
        const auto c = dsp::convolve(a, b);

        REQUIRE(c.size() == a.size());
        REQUIRE(c.size() == b.size());

        CHECK(c[0] == approx(0));
        CHECK(c[1] == approx(2));
        CHECK(c[2] == approx(0));
        CHECK(c[3] == approx(-2));
    }

    SECTION("Convolve a real signal with a complex signal")
    {
        const auto a = g.cosine(1000);
        const auto b = dsp::complex_signal(g.cosine(1000), g.sine(1000));
        const auto c = dsp::convolve(a, b);

        REQUIRE(c.size() == a.size());
        REQUIRE(c.size() == b.size());

        CHECK(c[0].real() == approx(2));
        CHECK(c[0].imag() == approx(0));
        CHECK(c[1].real() == approx(0));
        CHECK(c[1].imag() == approx(2));
        CHECK(c[2].real() == approx(-2));
        CHECK(c[2].imag() == approx(0));
        CHECK(c[3].real() == approx(0));
        CHECK(c[3].imag() == approx(-2));
    }

    SECTION("Convolve a complex signal with a real signal")
    {
        const auto a = dsp::complex_signal(g.cosine(1000), g.sine(1000));
        const auto b = g.cosine(1000);
        const auto c = dsp::convolve(a, b);

        REQUIRE(c.size() == a.size());
        REQUIRE(c.size() == b.size());

        CHECK(c[0].real() == approx(2));
        CHECK(c[0].imag() == approx(0));
        CHECK(c[1].real() == approx(0));
        CHECK(c[1].imag() == approx(2));
        CHECK(c[2].real() == approx(-2));
        CHECK(c[2].imag() == approx(0));
        CHECK(c[3].real() == approx(0));
        CHECK(c[3].imag() == approx(-2));
    }

    SECTION("Convolve a complex signal with a complex signal")
    {
        const auto a = dsp::complex_signal(g.cosine(1000), g.sine(1000));
        const auto b = dsp::complex_signal(g.cosine(1000), g.sine(1000));
        const auto c = dsp::convolve(a, b);

        REQUIRE(c.size() == a.size());
        REQUIRE(c.size() == b.size());

        CHECK(c[0].real() == approx(4));
        CHECK(c[0].imag() == approx(0));
        CHECK(c[1].real() == approx(0));
        CHECK(c[1].imag() == approx(4));
        CHECK(c[2].real() == approx(-4));
        CHECK(c[2].imag() == approx(0));
        CHECK(c[3].real() == approx(0));
        CHECK(c[3].imag() == approx(-4));
    }
}