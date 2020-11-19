#include "approx.hpp"
#include <catch2/catch_template_test_macros.hpp>
#include <complex>
#include <tnt/dsp/hilbert_transform.hpp>
#include <tnt/dsp/signal.hpp>
#include <tnt/dsp/signal_generator.hpp>

using namespace tnt;

TEMPLATE_TEST_CASE("hilbert_transform", "[hilbert_transform]", double, float)
{
    SECTION("Hilbert transform of an even length signal")
    {
        const dsp::signal_generator<TestType> g{ 4000, 4 };
        const auto x = g.cosine(1000);
        const auto x_c = dsp::hilbert_transform(x);

        CHECK(x_c[0].real() == approx(1));
        CHECK(x_c[0].imag() == approx(0));
        CHECK(x_c[1].real() == approx(0));
        CHECK(x_c[1].imag() == approx(1));
        CHECK(x_c[2].real() == approx(-1));
        CHECK(x_c[2].imag() == approx(0));
        CHECK(x_c[3].real() == approx(0));
        CHECK(x_c[3].imag() == approx(-1));
    }

    SECTION("Hilbert transform of an odd length signal")
    {
        const dsp::signal_generator<TestType> g{ 4000, 5 };
        const auto x = g.cosine(1000);
        const auto x_c = dsp::hilbert_transform(x);
        
        CHECK(x_c[0].real() == approx(1));
        CHECK(x_c[0].imag() == approx(0.47023));
        CHECK(x_c[1].real() == approx(0));
        CHECK(x_c[1].imag() == approx(1.08577));
        CHECK(x_c[2].real() == approx(-1));
        CHECK(x_c[2].imag() == approx(0));
        CHECK(x_c[3].real() == approx(0));
        CHECK(x_c[3].imag() == approx(-1.08577));
        CHECK(x_c[4].real() == approx(1));
        CHECK(x_c[4].imag() == approx(-0.47023));
    }
}
