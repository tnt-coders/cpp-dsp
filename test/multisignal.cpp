#include "approx.hpp"
#include <algorithm>
#include <catch2/catch_template_test_macros.hpp>
#include <complex>
#include <tnt/dsp/multisignal.hpp>
#include <tnt/dsp/signal.hpp>
#include <tnt/dsp/signal_generator.hpp>

using namespace tnt;

TEMPLATE_TEST_CASE("multisignal construction", "[multisignal][construction]", double, float)
{
    const dsp::signal_generator<TestType> g(1000, 10);

    SECTION("Construct a real signal given the sample rate")
    {
        const dsp::multisignal<TestType> x(1000);

        CHECK(x.sample_rate() == 1000);
        CHECK(x.size() == 0);
    }

    SECTION("Construct a complex signal given the sample rate")
    {
        const dsp::multisignal<std::complex<TestType>> x(1000);

        CHECK(x.sample_rate() == 1000);
        CHECK(x.size() == 0);
    }

    SECTION("Construct a real signal given the sample rate and size")
    {
        const dsp::multisignal<TestType> x(1000, 10);

        CHECK(x.sample_rate() == 1000);
        CHECK(x.size() == 10);
    }

    SECTION("Construct a complex signal given the sample rate and size")
    {
        const dsp::multisignal<std::complex<TestType>> x(1000, 10);

        CHECK(x.sample_rate() == 1000);
        CHECK(x.size() == 10);
    }

    SECTION("Construct a real signal given the sample rate, size, and number of channels")
    {
        const dsp::multisignal<TestType> x(1000, 10, 2);

        CHECK(x.sample_rate() == 1000);
        CHECK(x.size() == 10);
        CHECK(x.channels() == 2);
    }

    SECTION("Construct a complex signal given the sample rate, size, and number of channels")
    {
        const dsp::multisignal<std::complex<TestType>> x(1000, 10, 2);

        CHECK(x.sample_rate() == 1000);
        CHECK(x.size() == 10);
        CHECK(x.channels() == 2);
    }

    SECTION("Construct a real multisignal from multiple real signals")
    {
        const dsp::multisignal<TestType> x{
            g.cosine(100),
            g.sine(100),
        };

        CHECK(x.sample_rate() == 1000);
        CHECK(x.size() == 10);
        CHECK(x.channels() == 2);
    }

    SECTION("Construct a comple multisignal from multiple complex signals")
    {
        const dsp::multisignal<std::complex<TestType>> x{
            { g.cosine(1000), g.sine(100) },
            { g.sine(1000), g.cosine(100) },
        };

        CHECK(x.sample_rate() == 1000);
        CHECK(x.size() == 10);
        CHECK(x.channels() == 2);
    }

    SECTION("Copy constructor")
    {
        const dsp::multisignal<TestType> x1{
            g.cosine(100),
            g.sine(100),
        };

        const dsp::multisignal<TestType> x2(x1);

        CHECK(x2.sample_rate() == x1.sample_rate());
        REQUIRE(x2.size() == x1.size());
        REQUIRE(x2.channels() == x1.channels());
        
        for (size_t n = 0; n < x2.size(); ++n)
        {
            for (size_t c = 0; c < x2.channels(); ++c)
            {
                CHECK(x2[n][c] == approx(x1[n][c]));
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
        const dsp::multisignal<TestType> x2(std::move(x1));

        CHECK(x2.sample_rate() == x1_copy.sample_rate());
        REQUIRE(x2.size() == x1_copy.size());
        REQUIRE(x2.channels() == x1_copy.channels());

        for (size_t n = 0; n < x2.size(); ++n)
        {
            for (size_t c = 0; c < x2.channels(); ++c)
            {
                CHECK(x2[n][c] == approx(x1_copy[n][c]));
            }
        }
    }
}

TEMPLATE_TEST_CASE("multisignal assignment", "[multisignal][assignment]", double, float)
{
    const dsp::signal_generator<TestType> g(1000, 10);

    SECTION("Copy assignment")
    {
        const dsp::multisignal<TestType> x1{
            g.cosine(100),
            g.sine(100),
        };

        const auto x2 = x1;

        CHECK(x2.sample_rate() == x1.sample_rate());
        CHECK(x2.size() == x1.size());
        CHECK(x2.channels() == x1.channels());

        for (size_t n = 0; n < x2.size(); ++n)
        {
            for (size_t c = 0; c < x2.channels(); ++c)
            {
                CHECK(x2[n][c] == approx(x1[n][c]));
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
        CHECK(x2.size() == x1_copy.size());
        CHECK(x2.channels() == x1_copy.channels());

        for (size_t n = 0; n < x2.size(); ++n)
        {
            for (size_t c = 0; c < x2.channels(); ++c)
            {
                CHECK(x2[n][c] == approx(x1_copy[n][c]));
            }
        }
    }
}

TEMPLATE_TEST_CASE("multisignal accessors", "[multisignal][accessors]", double, float)
{
    const dsp::signal_generator<TestType> g(1000, 10);

    SECTION("channel")
    {
        const auto c0 = g.cosine(100);
        const auto c1 = g.sine(100);
        const dsp::multisignal<TestType> x{
             c0,
             c1,
        };

        REQUIRE(x.size() == c0.size());
        REQUIRE(x.size() == c1.size());
        REQUIRE(x.channels() == 2);

        const auto x_c0 = x.channel(0);
        const auto x_c1 = x.channel(1);

        REQUIRE(x_c0.size() == x.size());
        REQUIRE(x_c1.size() == x.size());

        for (size_t n = 0; n < x.size(); ++n)
        {
            CHECK(x_c0[n] == approx(c0[n]));
            CHECK(x_c1[n] == approx(c1[n]));
        }
    }

    SECTION("duration")
    {
        const dsp::multisignal<TestType> x(1000, 2500);

        CHECK(x.duration() == approx(2.5));
    }

    SECTION("sample_rate")
    {
        const dsp::multisignal<TestType> x(1000);

        CHECK(x.sample_rate() == 1000);
    }
}

TEMPLATE_TEST_CASE("multisignal data access", "[multisignal][data access]", double, float)
{
     const dsp::signal_generator<TestType> g(1000, 10);
     const auto c0 = g.cosine(100);
     const auto c1 = g.sine(100);

     SECTION("Constant data access")
     {
         const dsp::multisignal<TestType> x{
             c0,
             c1,
         };

         REQUIRE(x.size() == c0.size());
         REQUIRE(x.size() == c1.size());

         // const data access
         for (size_t n = 0; n < x.size(); ++n)
         {
             CHECK(x[n][0] == approx(c0[n]));
             CHECK(x[n][1] == approx(c1[n]));
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
             CHECK(x[n][0] == approx(c0[n]));
             CHECK(x[n][1] == approx(c1[n]));
             x[n][0] = c1[n];
             x[n][1] = c0[n];
             CHECK(x[n][0] == approx(c1[n]));
             CHECK(x[n][1] == approx(c0[n]));
         }
     }
}

TEMPLATE_TEST_CASE("miltisignal iterators", "[multisignal][iterators]", double, float)
{
    const dsp::signal_generator<TestType> g(1000, 10);

    SECTION("Constant iterators")
    {
        const dsp::multisignal<TestType> x{
            g.cosine(100),
            g.sine(1000),
        };

        // Iterate through x, and also capture a copy of x to compare to
        std::for_each(x.begin(), x.end(), [x, n = 0](const auto& samples) mutable {

            REQUIRE(samples.size() == x.channels());

            const auto equal = std::equal(samples.begin(), samples.end(), x[n++].begin(), [](const auto& sample1, const auto& sample2) {
                return sample1 == approx(sample2);
            });

            CHECK(equal);
        });

        // Iterate through x, and also capture a copy of x to compare to
        std::for_each(x.cbegin(), x.cend(), [x, n = 0](const auto& samples) mutable {

            REQUIRE(samples.size() == x.channels());

            const auto equal = std::equal(samples.begin(), samples.end(), x[n++].begin(), [](const auto& sample1, const auto& sample2) {
                return sample1 == approx(sample2);
            });

            CHECK(equal);
        });
    }

    SECTION("Mutable iterators")
    {
        dsp::multisignal<TestType> x{
            g.cosine(100),
            g.sine(1000),
        };

        std::for_each(x.begin(), x.end(), [](auto& samples) {
            std::for_each(samples.begin(), samples.end(), [](auto& sample) {
                sample = 1;
            });
        });

        std::for_each(x.begin(), x.end(), [](const auto& samples) {
            std::for_each(samples.begin(), samples.end(), [](const auto& sample) {
                CHECK(sample == approx(1));
            });
        });
    }
}

TEMPLATE_TEST_CASE("multisignal capacity", "[multisignal][capacity]", double, float)
{
    SECTION("size")
    {
        const dsp::multisignal<TestType> x(1000, 10);

        CHECK(x.size() == 10);
    }

    SECTION("channels")
    {
        const dsp::multisignal<TestType> x(1000, 10, 2);

        CHECK(x.channels() == 2);
    }
}

TEMPLATE_TEST_CASE("multisignal modifiers", "[multisignal][modifiers]", double, float)
{
    const dsp::signal_generator<TestType> g(1000, 10);

    SECTION("add_channel")
    {
        dsp::multisignal<TestType> x(g.sample_rate(), g.size());
        x.add_channel(g.cosine(100));
        const auto data = g.cosine(100);

        REQUIRE(x.channels() == 1);
        REQUIRE(x.size() == data.size());

        for (size_t n = 0; n < x.size(); ++n)
        {
            CHECK(x[n][0] == approx(data[n]));
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

        REQUIRE(x1.size() == x2.size());
        REQUIRE(x1.size() == x1_copy.size());
        REQUIRE(x1.size() == x2_copy.size());
        REQUIRE(x1.channels() == x2.channels());
        REQUIRE(x1.channels() == x1_copy.channels());
        REQUIRE(x1.channels() == x2_copy.channels());

        for (size_t n = 0; n < x1.size(); ++n)
        {
            for (size_t c = 0; c < x1.channels(); ++c)
            {
                CHECK(x1[n][c] == approx(x2_copy[n][c]));
                CHECK(x2[n][c] == approx(x1_copy[n][c]));
            }
        }
    }
}
