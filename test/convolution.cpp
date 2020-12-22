#include <catch2/catch_template_test_macros.hpp>
#include <tnt/dsp/convolution.hpp>
#include <tnt/dsp/signal.hpp>
#include <tnt/dsp/signal_generator.hpp>
#include <tnt/math/comparison.hpp>

using namespace tnt;

TEMPLATE_TEST_CASE("convolve", "[convolve]", double, float)
{
    const dsp::SignalGenerator<TestType> g(4000, 4);

    SECTION("Convolve a real signal with a real signal")
    {
        const auto a = g.cosine(1000);
        const auto b = g.sine(1000);
        const auto c = dsp::convolve(a, b);

        REQUIRE(c.size() == a.size());
        REQUIRE(c.size() == b.size());

        CHECK(math::near(c[0], 0));
        CHECK(math::near(c[1], 2));
        CHECK(math::near(c[2], 0));
        CHECK(math::near(c[3], -2));
    }

    SECTION("Convolve a real signal with a complex signal")
    {
        const auto a = g.cosine(1000);
        const auto b = dsp::complex_signal(g.cosine(1000), g.sine(1000));
        const auto c = dsp::convolve(a, b);

        REQUIRE(c.size() == a.size());
        REQUIRE(c.size() == b.size());

        CHECK(math::near(c[0].real(), 2));
        CHECK(math::near(c[0].imag(), 0));
        CHECK(math::near(c[1].real(), 0));
        CHECK(math::near(c[1].imag(), 2));
        CHECK(math::near(c[2].real(), -2));
        CHECK(math::near(c[2].imag(), 0));
        CHECK(math::near(c[3].real(), 0));
        CHECK(math::near(c[3].imag(), -2));
    }

    SECTION("Convolve a complex signal with a real signal")
    {
        const auto a = dsp::complex_signal(g.cosine(1000), g.sine(1000));
        const auto b = g.cosine(1000);
        const auto c = dsp::convolve(a, b);

        REQUIRE(c.size() == a.size());
        REQUIRE(c.size() == b.size());

        CHECK(math::near(c[0].real(), 2));
        CHECK(math::near(c[0].imag(), 0));
        CHECK(math::near(c[1].real(), 0));
        CHECK(math::near(c[1].imag(), 2));
        CHECK(math::near(c[2].real(), -2));
        CHECK(math::near(c[2].imag(), 0));
        CHECK(math::near(c[3].real(), 0));
        CHECK(math::near(c[3].imag(), -2));
    }

    SECTION("Convolve a complex signal with a complex signal")
    {
        const auto a = dsp::complex_signal(g.cosine(1000), g.sine(1000));
        const auto b = dsp::complex_signal(g.cosine(1000), g.sine(1000));
        const auto c = dsp::convolve(a, b);

        REQUIRE(c.size() == a.size());
        REQUIRE(c.size() == b.size());

        CHECK(math::near(c[0].real(), 4));
        CHECK(math::near(c[0].imag(), 0));
        CHECK(math::near(c[1].real(), 0));
        CHECK(math::near(c[1].imag(), 4));
        CHECK(math::near(c[2].real(), -4));
        CHECK(math::near(c[2].imag(), 0));
        CHECK(math::near(c[3].real(), 0));
        CHECK(math::near(c[3].imag(), -4));
    }
}
