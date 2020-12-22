#include <algorithm>
#include <catch2/catch_template_test_macros.hpp>
#include <complex>
#include <tnt/dsp/signal.hpp>
#include <tnt/dsp/signal_generator.hpp>
#include <tnt/math/comparison.hpp>

using namespace tnt;

TEMPLATE_TEST_CASE("Signal construction", "[Signal][construction]", double, float)
{
    const dsp::SignalGenerator<TestType> g(1000, 10);

    SECTION("Construct a real signal given the sample rate")
    {
        const dsp::Signal<TestType> x(1000);

        CHECK(x.sample_rate() == 1000);
        CHECK(x.size() == 0);
    }

    SECTION("Construct a complex signal given the sample rate")
    {
        const dsp::Signal<std::complex<TestType>> x(1000);

        CHECK(x.sample_rate() == 1000);
        CHECK(x.size() == 0);
    }

    SECTION("Construct a real signal given the sample rate and size")
    {
        const dsp::Signal<TestType> x(1000, 10);

        CHECK(x.sample_rate() == 1000);
        CHECK(x.size() == 10);
    }

    SECTION("Construct a complex signal given the sample rate and size")
    {
        const dsp::Signal<std::complex<TestType>> x(1000, 10);

        CHECK(x.sample_rate() == 1000);
        CHECK(x.size() == 10);
    }

    SECTION("Construct a complex signal from two real signals")
    {
        const auto x1_real = g.cosine(100);
        const auto x1_imag = g.sine(100);

        const dsp::Signal<std::complex<TestType>> x2(x1_real, x1_imag);

        CHECK(x2.sample_rate() == x1_real.sample_rate());
        CHECK(x2.sample_rate() == x1_imag.sample_rate());
        REQUIRE(x2.size() == x1_real.size());
        REQUIRE(x2.size() == x1_imag.size());

        for (size_t n = 0; n < x2.size(); ++n)
        {
            CHECK(math::near(x2[n].real(), x1_real[n]));
            CHECK(math::near(x2[n].imag(), x1_imag[n]));
        }
    }

    SECTION("Copy constructor")
    {
        const auto x1 = g.cosine(100);

        const dsp::Signal<TestType> x2(x1);

        CHECK(x2.sample_rate() == x1.sample_rate());
        REQUIRE(x2.size() == x1.size());

        for (size_t n = 0; n < x2.size(); ++n)
        {
            CHECK(math::near(x2[n], x1[n]));
        }
    }

    SECTION("Move constructor")
    {
        auto       x1      = g.cosine(100);
        const auto x1_copy = x1;

        const dsp::Signal<TestType> x2(std::move(x1));

        CHECK(x2.sample_rate() == x1_copy.sample_rate());
        REQUIRE(x2.size() == x1_copy.size());

        for (size_t n = 0; n < x2.size(); ++n)
        {
            CHECK(math::near(x2[n], x1_copy[n]));
        }
    }
}

TEMPLATE_TEST_CASE("Signal assignment", "[Signal][assignment]", double, float)
{
    const dsp::SignalGenerator<TestType> g(1000, 10);

    SECTION("Copy assignment")
    {
        const auto x1 = g.cosine(100);
        const auto x2 = x1;

        CHECK(x2.sample_rate() == x1.sample_rate());
        REQUIRE(x2.size() == x1.size());

        for (size_t n = 0; n < x2.size(); ++n)
        {
            CHECK(math::near(x2[n], x1[n]));
        }
    }

    SECTION("Move assignment")
    {
        const auto x1      = g.cosine(100);
        const auto x1_copy = x1;
        const auto x2      = std::move(x1);

        CHECK(x2.sample_rate() == x1_copy.sample_rate());
        REQUIRE(x2.size() == x1_copy.size());

        for (size_t n = 0; n < x2.size(); ++n)
        {
            CHECK(math::near(x2[n], x1_copy[n]));
        }
    }
}

TEMPLATE_TEST_CASE("Signal accessors", "[Signal][accessors]", double, float)
{
    SECTION("duration")
    {
        const dsp::Signal<TestType> x(1000, 2500);
        CHECK(math::near(x.duration(), 2.5));
    }

    SECTION("sample_rate")
    {
        const dsp::Signal<TestType> x(1000);
        CHECK(x.sample_rate() == 1000);
    }
}

TEMPLATE_TEST_CASE("Signal data access", "[Signal][data access]", double, float)
{
    const dsp::SignalGenerator<TestType> g(1000, 10);

    SECTION("Constant data access")
    {
        const auto x1 = g.cosine(100);
        const auto x2 = x1;

        CHECK(x2.sample_rate() == x1.sample_rate());
        REQUIRE(x2.size() == x1.size());

        for (size_t n = 0; n < x2.size(); ++n)
        {
            CHECK(math::near(x2[n], x1[n]));
        }
    }

    SECTION("Mutable data access")
    {
        auto x = g.cosine(100);

        for (size_t n = 0; n < x.size(); ++n)
        {
            x[n] = 1;
            CHECK(math::near(x[n], 1));
        }
    }
}

TEMPLATE_TEST_CASE("Signal iterators", "[Signal][iterators]", double, float)
{
    const dsp::SignalGenerator<TestType> g(1000, 10);

    SECTION("Constant iterators")
    {
        const auto x = g.cosine(100);

        // Iterate through x, and also capture a copy of x to compare to
        std::for_each(x.begin(), x.end(), [x, n = 0](const auto& sample) mutable {
            CHECK(math::near(sample, x[n++]));
        });

        // Iterate through x, and also capture a copy of x to compare to
        std::for_each(x.cbegin(), x.cend(), [x, n = 0](const auto& sample) mutable {
            CHECK(math::near(sample, x[n++]));
        });
    }

    SECTION("Mutable iterators")
    {
        auto x = g.cosine(100);

        std::for_each(x.begin(), x.end(), [](auto& sample) {
            sample = 1;
        });

        std::for_each(x.begin(), x.end(), [](const auto& sample) {
            CHECK(math::near(sample, 1));
        });
    }
}

TEMPLATE_TEST_CASE("Signal capacity", "[Signal][capacity]", double, float)
{
    SECTION("capacity and reserve")
    {
        dsp::Signal<TestType> x(1000);
        x.reserve(100);

        CHECK(x.capacity() == 100);

        // Reserving a smaller capacity should not drop previously reserved capacity
        x.reserve(10);

        CHECK(x.capacity() == 100);
    }

    SECTION("size and resize")
    {
        dsp::Signal<TestType> x(1000, 10);

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

TEMPLATE_TEST_CASE("Signal modifiers", "[Signal][modifiers]", double, float)
{
    const dsp::SignalGenerator<TestType> g(1000, 10);

    SECTION("push_back")
    {
        dsp::Signal<TestType> x(1000);
        x.push_back(1);
        x.push_back(2);
        x.push_back(3);

        REQUIRE(x.size() == 3);
        CHECK(math::near(x[0], 1));
        CHECK(math::near(x[1], 2));
        CHECK(math::near(x[2], 3));
    }

    SECTION("swap")
    {
        const auto cosine = g.cosine(100);
        const auto sine   = g.sine(100);
        auto       x1     = cosine;
        auto       x2     = sine;

        // Do not use the qualified name (use ADL to find the function)
        swap(x1, x2);

        REQUIRE(x1.size() == x2.size());
        for (size_t n = 0; n < x1.size(); ++n)
        {
            CHECK(math::near(x1[n], sine[n]));
            CHECK(math::near(x2[n], cosine[n]));
        }
    }
}

TEMPLATE_TEST_CASE("complex_signal", "[complex_signal]", double, float)
{
    const dsp::SignalGenerator<TestType> g(1000, 10);

    const auto x1_real = g.cosine(100);
    const auto x1_imag = g.sine(100);
    const auto x2      = dsp::complex_signal(x1_real, x1_imag);

    CHECK(x2.sample_rate() == x1_real.sample_rate());
    CHECK(x2.sample_rate() == x1_imag.sample_rate());
    REQUIRE(x2.size() == x1_real.size());
    REQUIRE(x2.size() == x1_imag.size());

    for (size_t n = 0; n < x2.size(); ++n)
    {
        CHECK(math::near(x2[n].real(), x1_real[n]));
        CHECK(math::near(x2[n].imag(), x1_imag[n]));
    }
}
