#include "approx.hpp"

#include <catch2/catch_template_test_macros.hpp>
#include <tnt/dsp/signal.hpp>
#include <tnt/dsp/signal_generator.hpp>

using namespace tnt;

TEMPLATE_TEST_CASE("SignalGenerator construction", "[SignalGenerator][construction]", double, float)
{
    const dsp::SignalGenerator<TestType> g(1000, 10);

    CHECK(g.sample_rate() == 1000);
    CHECK(g.size() == 10);
}

TEMPLATE_TEST_CASE("SignalGenerator accessors", "[SignalGenerator][accessors]", double, float)
{
    const dsp::SignalGenerator<TestType> g(1000, 10);

    SECTION("sample_rate")
    {
        CHECK(g.sample_rate() == 1000);
    }

    SECTION("size")
    {
        CHECK(g.size() == 10);
    }
}

TEMPLATE_TEST_CASE("SignalGenerator::cosine", "[SignalGenerator][cosine]", double, float)
{
    const size_t f_s = 1000;
    const size_t N   = 10;
    const auto   t_s = 1 / static_cast<TestType>(f_s);

    const TestType frequency      = 100;
    const TestType amplitude      = 2;
    const TestType phase_shift    = static_cast<TestType>(M_PI) / 2;
    const TestType vertical_shift = 2;

    dsp::Signal<TestType> cosine(f_s, N);

    for (size_t n = 0; n < N; ++n)
    {
        cosine[n] = amplitude
                      * std::cos(2 * static_cast<TestType>(M_PI) * frequency * n * t_s
                                 + phase_shift)
                  + vertical_shift;
    }

    const dsp::SignalGenerator<TestType> g(f_s, N);

    const auto x = g.cosine(frequency, amplitude, phase_shift, vertical_shift);

    REQUIRE(x.size() == cosine.size());
    REQUIRE(x.sample_rate() == cosine.sample_rate());

    for (size_t n = 0; n < N; ++n)
    {
        CHECK(x[n] == approx(cosine[n]));
    }
}

TEMPLATE_TEST_CASE("SignalGenerator::sine", "[SignalGenerator][sine]", double, float)
{
    const size_t f_s = 1000;
    const size_t N   = 10;
    const auto   t_s = 1 / static_cast<TestType>(f_s);

    const TestType frequency      = 100;
    const TestType amplitude      = 2;
    const TestType phase_shift    = static_cast<TestType>(M_PI) / 2;
    const TestType vertical_shift = 2;

    dsp::Signal<TestType> sine(f_s, N);

    for (size_t n = 0; n < N; ++n)
    {
        sine[n] = amplitude
                    * std::sin(2 * static_cast<TestType>(M_PI) * frequency * n * t_s + phase_shift)
                + vertical_shift;
    }

    const dsp::SignalGenerator<TestType> g(f_s, N);

    const auto x = g.sine(frequency, amplitude, phase_shift, vertical_shift);

    REQUIRE(x.size() == sine.size());
    REQUIRE(x.sample_rate() == sine.sample_rate());

    for (size_t n = 0; n < N; ++n)
    {
        CHECK(x[n] == approx(sine[n]));
    }
}
