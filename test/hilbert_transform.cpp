#include <catch2/catch_template_test_macros.hpp>
#include <complex>
#include <tnt/dsp/hilbert_transform.hpp>
#include <tnt/dsp/signal.hpp>
#include <tnt/dsp/signal_generator.hpp>
#include <tnt/math/comparison.hpp>

using namespace tnt;

TEMPLATE_TEST_CASE("hilbert_transform", "[hilbert_transform]", double, float)
{
    SECTION("Hilbert transform of an even length signal")
    {
        const dsp::SignalGenerator<TestType> g(4000, 4);

        const auto x   = g.cosine(1000);
        const auto x_c = dsp::hilbert_transform(x);

        CHECK(math::near(x_c[0].real(), 1));
        CHECK(math::near(x_c[0].imag(), 0));
        CHECK(math::near(x_c[1].real(), 0));
        CHECK(math::near(x_c[1].imag(), 1));
        CHECK(math::near(x_c[2].real(), -1));
        CHECK(math::near(x_c[2].imag(), 0));
        CHECK(math::near(x_c[3].real(), 0));
        CHECK(math::near(x_c[3].imag(), -1));
    }

    SECTION("Hilbert transform of an odd length signal")
    {
        const dsp::SignalGenerator<TestType> g(4000, 5);

        const auto x   = g.cosine(1000);
        const auto x_c = dsp::hilbert_transform(x);

        CHECK(math::near(x_c[0].real(), 1));
        CHECK(math::near(x_c[0].imag(), 0.47023));
        CHECK(math::near(x_c[1].real(), 0));
        CHECK(math::near(x_c[1].imag(), 1.08577));
        CHECK(math::near(x_c[2].real(), -1));
        CHECK(math::near(x_c[2].imag(), 0));
        CHECK(math::near(x_c[3].real(), 0));
        CHECK(math::near(x_c[3].imag(), -1.08577));
        CHECK(math::near(x_c[4].real(), 1));
        CHECK(math::near(x_c[4].imag(), -0.47023));
    }
}
