#include "approx.hpp"
#include <catch2/catch_template_test_macros.hpp>
#include <complex>
#include <tnt/dsp/multisignal.hpp>
#include <tnt/dsp/signal.hpp>
#include <tnt/dsp/signal_generator.hpp>

using namespace tnt;

TEMPLATE_TEST_CASE("multisignal construction", "[multisignal][construction]", double, float)
{
    const dsp::signal_generator<TestType> g{ 1000, 10 };

    SECTION("Construct a real signal given the sample rate")
    {
        const dsp::multisignal<TestType> x{ 1000 };

        CHECK(x.sample_rate() == 1000);
        CHECK(x.size() == 0);
    }

    SECTION("Construct a complex signal given the sample rate")
    {
        const dsp::multisignal<std::complex<TestType>> x{ 1000 };

        CHECK(x.sample_rate() == 1000);
        CHECK(x.size() == 0);
    }

    SECTION("Construct a real signal given the sample rate and size")
    {
        const dsp::multisignal<TestType> x{ 1000, 10 };

        CHECK(x.sample_rate() == 1000);
        CHECK(x.size() == 10);
    }

    SECTION("Construct a complex signal given the sample rate and size")
    {
        const dsp::multisignal<std::complex<TestType>> x{ 1000, 10 };

        CHECK(x.sample_rate() == 1000);
        CHECK(x.size() == 10);
    }

    SECTION("Construct a real signal given the sample rate, size, and number of channels")
    {
        const dsp::multisignal<TestType> x{ 1000, 10, 2 };

        CHECK(x.sample_rate() == 1000);
        CHECK(x.channels() == 2);
        CHECK(x.size() == 10);
    }

    SECTION("Construct a complex signal given the sample rate, size, and number of channels")
    {
        const dsp::multisignal<std::complex<TestType>> x{ 1000, 10, 2 };

        CHECK(x.sample_rate() == 1000);
        CHECK(x.channels() == 2);
        CHECK(x.size() == 10);
    }

    SECTION("Construct a real multisignal from multiple real signals")
    {
        const dsp::multisignal<TestType> x{
            g.cosine(100),
            g.sine(100),
        };

        CHECK(x.sample_rate() == 1000);
        CHECK(x.channels() == 2);
        CHECK(x.size() == 10);
    }

    SECTION("Construct a comple multisignal from multiple complex signals")
    {
        const dsp::multisignal<std::complex<TestType>> x{
            { g.cosine(1000), g.sine(100) },
            { g.sine(1000), g.cosine(100) },
        };

        CHECK(x.sample_rate() == 1000);
        CHECK(x.channels() == 2);
        CHECK(x.size() == 10);
    }

    SECTION("Copy constructor")
    {
        const dsp::multisignal<TestType> x1{
            g.cosine(100),
            g.sine(100),
        };

        const dsp::multisignal<TestType> x2{ x1 };

        CHECK(x2.sample_rate() == x1.sample_rate());
        REQUIRE(x2.channels() == x1.channels());
        REQUIRE(x2.size() == x1.size());
        
        for (size_t c = 0; c < x2.channels(); ++c)
        {
            for (size_t n = 0; n < x2.size(); ++n)
            {
                CHECK(x2[c][n] == approx(x1[c][n]));
            }
        }
    }

    SECTION("Move constructor")
    {
        const dsp::multisignal<TestType> x1{
            g.cosine(100),
            g.sine(100),
        };

        const auto x1_copy = x1;
        const dsp::multisignal<TestType> x2{ std::move(x1) };

        CHECK(x2.sample_rate() == x1_copy.sample_rate());
        REQUIRE(x2.channels() == x1_copy.channels());
        REQUIRE(x2.size() == x1_copy.size());

        for (size_t c = 0; c < x2.channels(); ++c)
        {
            for (size_t n = 0; n < x2.size(); ++n)
            {
                CHECK(x2[c][n] == approx(x1_copy[c][n]));
            }
        }
    }
}

TEMPLATE_TEST_CASE("multisignal assignment", "[multisignal][assignment]", double, float)
{
    const dsp::signal_generator<TestType> g{ 1000, 10 };

    SECTION("Copy assignment")
    {
        const dsp::multisignal<TestType> x1{
            g.cosine(100),
            g.sine(100),
        };

        const auto x2 = x1;

        CHECK(x2.sample_rate() == x1.sample_rate());
        CHECK(x2.channels() == x1.channels());
        CHECK(x2.size() == x1.size());

        for (size_t c = 0; c < x2.channels(); ++c)
        {
            for (size_t n = 0; n < x2.size(); ++n)
            {
                CHECK(x2[c][n] == approx(x1[c][n]));
            }
        }
    }

    SECTION("Move assignment")
    {
        dsp::multisignal<TestType> x1{
            g.cosine(100),
            g.sine(100),
        };

        const auto x1_copy = x1;
        const auto x2 = std::move(x1);

        CHECK(x2.sample_rate() == x1_copy.sample_rate());
        CHECK(x2.channels() == x1_copy.channels());
        CHECK(x2.size() == x1_copy.size());

        for (size_t c = 0; c < x2.channels(); ++c)
        {
            for (size_t n = 0; n < x2.size(); ++n)
            {
                CHECK(x2[c][n] == approx(x1_copy[c][n]));
            }
        }
    }
}

TEMPLATE_TEST_CASE("multisignal accessors", "[multisignal][accessors]", double, float)
{
    SECTION("duration")
    {
        const dsp::multisignal<TestType> x{ 1000, 2500 };

        CHECK(x.duration() == approx(2.5));
    }

    SECTION("sample_rate")
    {
        const dsp::multisignal<TestType> x{ 1000 };

        CHECK(x.sample_rate() == 1000);
    }
}

TEMPLATE_TEST_CASE("multisignal data access", "[multisignal][data access]", double, float)
{
     // Signal generator used to generate test signals
     const dsp::signal_generator<TestType> g{ 1000, 10 };
     const auto c0 = g.cosine(100);
     const auto c1 = g.sine(100);

     SECTION("Constant data access")
     {
         const dsp::multisignal<TestType> x{
             c0,
             c1,
         };

         // const data access
         for (size_t n = 0; n < x.size(); ++n)
         {
             CHECK(x[0][n] == approx(c0[n]));
             CHECK(x[1][n] == approx(c1[n]));
         }
     }

     SECTION("Mutable data access")
     {
         dsp::multisignal<TestType> x{
             c0,
             c1,
         };

         REQUIRE(x.size() == c0.size());
         REQUIRE(x.size() == c1.size());

         for (size_t n = 0; n < x.size(); ++n)
         {
             CHECK(x[0][n] == approx(c0[n]));
             CHECK(x[1][n] == approx(c1[n]));
             x[0][n] = c1[n];
             x[1][n] = c0[n];
             CHECK(x[0][n] == approx(c1[n]));
             CHECK(x[1][n] == approx(c0[n]));
         }
     }
}

TEMPLATE_TEST_CASE("miltisignal iterators", "[multisignal][iterators]", double, float)
{
    // Nothing to test, the multisignal class does not currently support iterators
}

TEMPLATE_TEST_CASE("multisignal capacity", "[multisignal][capacity]", double, float)
{
    SECTION("channels")
    {
        const dsp::multisignal<TestType> x{ 1000, 10, 2 };

        CHECK(x.channels() == 2);
    }

    SECTION("size")
    {
        const dsp::multisignal<TestType> x{ 1000, 10 };

        CHECK(x.size() == 10);
    }
}

TEMPLATE_TEST_CASE("multisignal modifiers", "[multisignal][modifiers]", double, float)
{
    const dsp::signal_generator<TestType> g{ 1000, 10 };

    SECTION("add_channel")
    {
        dsp::multisignal<TestType> x{ g.sample_rate(), g.size() };
        x.add_channel(g.cosine(100));
        const auto data = g.cosine(100);

        REQUIRE(x.channels() == 1);
        REQUIRE(x.size() == data.size());

        for (size_t n = 0; n < x.size(); ++n)
        {
            CHECK(x[0][n] == approx(data[n]));
        }
    }

    SECTION("swap")
    {
        dsp::multisignal<TestType> x1{
            g.cosine(100),
            g.sine(100),
        };
        dsp::multisignal<TestType> x2{
            g.sine(100),
            g.cosine(100),
        };

        const auto x1_copy = x1;
        const auto x2_copy = x2;

        swap(x1, x2);

        REQUIRE(x1.channels() == x2.channels());
        REQUIRE(x1.channels() == x1_copy.channels());
        REQUIRE(x1.channels() == x2_copy.channels());
        REQUIRE(x1.size() == x2.size());
        REQUIRE(x1.size() == x1_copy.size());
        REQUIRE(x1.size() == x2_copy.size());

        for (size_t c = 0; c < x1.channels(); ++c)
        {
            for (size_t n = 0; n < x1.size(); ++n)
            {
                CHECK(x1[c][n] == approx(x2_copy[c][n]));
                CHECK(x2[c][n] == approx(x1_copy[c][n]));
            }
        }
    }
}
