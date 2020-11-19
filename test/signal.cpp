#include "approx.hpp"
#include <algorithm>
#include <catch2/catch_template_test_macros.hpp>
#include <complex>
#include <tnt/dsp/signal.hpp>
#include <tnt/dsp/signal_generator.hpp>

using namespace tnt;

TEMPLATE_TEST_CASE("signal construction", "[signal][construction]", double, float)
{
    const dsp::signal_generator<TestType> g{ 1000, 10 };

    SECTION("Construct a real signal given the sample rate")
    {
        const dsp::signal<TestType> x{ 1000 };

        CHECK(x.sample_rate() == 1000);
        CHECK(x.size() == 0);
    }

    SECTION("Construct a complex signal given the sample rate")
    {
        const dsp::signal<std::complex<TestType>> x{ 1000 };

        CHECK(x.sample_rate() == 1000);
        CHECK(x.size() == 0);
    }

    SECTION("Construct a real signal given the sample rate and size")
    {
        const dsp::signal<TestType> x{ 1000, 10 };

        CHECK(x.sample_rate() == 1000);
        CHECK(x.size() == 10);
    }

    SECTION("Construct a complex signal given the sample rate and size")
    {
        const dsp::signal<std::complex<TestType>> x{ 1000, 10 };

        CHECK(x.sample_rate() == 1000);
        CHECK(x.size() == 10);
    }

    SECTION("Construct a complex signal from two real signals")
    {
        const auto x1_real = g.cosine(100);
        const auto x1_imag = g.sine(100);
        const dsp::signal<std::complex<TestType>> x2{ x1_real, x1_imag };

        CHECK(x2.sample_rate() == x1_real.sample_rate());
        CHECK(x2.sample_rate() == x1_imag.sample_rate());
        REQUIRE(x2.size() == x1_real.size());
        REQUIRE(x2.size() == x1_imag.size());

        for (size_t n = 0; n < x2.size(); ++n)
        {
            CHECK(x2[n].real() == approx(x1_real[n]));
            CHECK(x2[n].imag() == approx(x1_imag[n]));
        }
    }

    SECTION("Copy constructor")
    {
        const auto x1 = g.cosine(100);
        const dsp::signal<TestType> x2{ x1 };
        
        CHECK(x2.sample_rate() == x1.sample_rate());
        REQUIRE(x2.size() == x1.size());
        
        for (size_t n = 0; n < x2.size(); ++n)
        {
            CHECK(x2[n] == approx(x1[n]));
        }
    }

    SECTION("Move constructor")
    {
        auto x1 = g.cosine(100);
        const auto x1_copy = x1;
        const dsp::signal<TestType> x2{ std::move(x1) };

        CHECK(x2.sample_rate() == x1_copy.sample_rate());
        REQUIRE(x2.size() == x1_copy.size());

        for (size_t n = 0; n < x2.size(); ++n)
        {
            CHECK(x2[n] == approx(x1_copy[n]));
        }
    }
}

TEMPLATE_TEST_CASE("signal assignment", "[signal][assignment]", double, float)
{
    const dsp::signal_generator<TestType> g{ 1000, 10 };

    SECTION("Copy assignment")
    {
        const auto x1 = g.cosine(100);
        const auto x2 = x1;

        CHECK(x2.sample_rate() == x1.sample_rate());
        REQUIRE(x2.size() == x1.size());

        for (size_t n = 0; n < x2.size(); ++n)
        {
            CHECK(x2[n] == approx(x1[n]));
        }
    }

    SECTION("Move assignment")
    {
        const auto x1 = g.cosine(100);
        const auto x1_copy = x1;
        const auto x2 = std::move(x1);

        CHECK(x2.sample_rate() == x1_copy.sample_rate());
        REQUIRE(x2.size() == x1_copy.size());

        for (size_t n = 0; n < x2.size(); ++n)
        {
            CHECK(x2[n] == approx(x1_copy[n]));
        }
    }
}

TEMPLATE_TEST_CASE("signal accessors", "[signal][accessors]", double, float)
{
    SECTION("duration")
    {
        const dsp::signal<TestType> x{ 1000, 2500 };
        CHECK(x.duration() == approx(2.5));
    }

    SECTION("sample_rate")
    {
        const dsp::signal<TestType> x{ 1000 };
        CHECK(x.sample_rate() == 1000);
    }
}

TEMPLATE_TEST_CASE("signal data access", "[signal][data access]", double, float)
{
    const dsp::signal_generator<TestType> g{ 1000, 10 };

    SECTION("Constant data access")
    {
        const auto x1 = g.cosine(100);
        const auto x2 = x1;

        CHECK(x2.sample_rate() == x1.sample_rate());
        REQUIRE(x2.size() == x1.size());

        for (size_t n = 0; n < x2.size(); ++n)
        {
            CHECK(x2[n] == approx(x1[n]));
        }
    }

    SECTION("Mutable data access")
    {
        auto x = g.cosine(100);

        for (size_t n = 0; n < x.size(); ++n)
        {
            x[n] = 1;
            CHECK(x[n] == approx(1));
        }
    }
}

TEMPLATE_TEST_CASE("signal iterators", "[signal][iterators]", double, float)
{
    const dsp::signal_generator<TestType> g{ 1000, 10 };

    SECTION("Constant iterators")
    {
        const auto x = g.cosine(100);

        // Iterate through x, and also capture a copy of x to compare to
        std::for_each(x.begin(), x.end(), [x, i=0](const auto& sample) mutable {
            CHECK(sample == approx(x[i++]));
        });

        // Iterate through x, and also capture a copy of x to compare to
        std::for_each(x.cbegin(), x.cend(), [x, i = 0](const auto& sample) mutable {
            CHECK(sample == approx(x[i++]));
        });
    }

    SECTION("Mutable iterators")
    {
        auto x = g.cosine(100);

        std::for_each(x.begin(), x.end(), [](auto& sample) {
            sample = 1;
        });

        std::for_each(x.begin(), x.end(), [](const auto& sample) {
            CHECK(sample == approx(1));
        });
    }
}

TEMPLATE_TEST_CASE("signal capacity", "[signal][capacity]", double, float)
{
    SECTION("capacity and reserve")
    {
        dsp::signal<TestType> x{ 1000 };
        x.reserve(100);

        CHECK(x.capacity() == 100);

        // Reserving a smaller capacity should not drop previously reserved capacity
        x.reserve(10);

        CHECK(x.capacity() == 100);
    }

    SECTION("size and resize")
    {
        dsp::signal<TestType> x{ 1000, 10 };

        CHECK(x.size() == 10);

        x.resize(100);

        CHECK(x.size() == 100);

        // Shrinking the size does not reduce the capacity
        const auto capacity = x.capacity();
        x.resize(10);

        CHECK(x.size() == 10);
        CHECK(x.capacity() == capacity);
    }
}

TEMPLATE_TEST_CASE("signal modifiers", "[signal][modifiers]", double, float)
{
    const dsp::signal_generator<TestType> g{ 1000, 10 };

    SECTION("push_back")
    {
        dsp::signal<TestType> x{ 1000 };
        x.push_back(1);
        x.push_back(2);
        x.push_back(3);
        
        REQUIRE(x.size() == 3);
        CHECK(x[0] == approx(1));
        CHECK(x[1] == approx(2));
        CHECK(x[2] == approx(3));
    }

    SECTION("swap")
    {
        const auto cosine = g.cosine(100);
        const auto sine = g.sine(100);
        auto x1 = cosine;
        auto x2 = sine;

        // Do not use the qualified name (use ADL to find the function)
        swap(x1, x2);

        REQUIRE(x1.size() == x2.size());
        for (size_t n = 0; n < x1.size(); ++n)
        {
            CHECK(x1[n] == approx(sine[n]));
            CHECK(x2[n] == approx(cosine[n]));
        }
    }
}

TEMPLATE_TEST_CASE("complex_signal", "[complex_signal]", double, float)
{
    const dsp::signal_generator<TestType> g{ 1000, 10 };

    const auto x1_real = g.cosine(100);
    const auto x1_imag = g.sine(100);
    const auto x2 = dsp::complex_signal(x1_real, x1_imag);

    CHECK(x2.sample_rate() == x1_real.sample_rate());
    CHECK(x2.sample_rate() == x1_imag.sample_rate());
    REQUIRE(x2.size() == x1_real.size());
    REQUIRE(x2.size() == x1_imag.size());

    for (size_t n = 0; n < x2.size(); ++n)
    {
        CHECK(x2[n].real() == approx(x1_real[n]));
        CHECK(x2[n].imag() == approx(x1_imag[n]));
    }
}