#include "constants.hpp"
#include <gtest/gtest.h>
#include <tnt/dsp/signal.hpp>
#include <tnt/dsp/signal_generator.hpp>

using namespace tnt;

template <typename T>
class SignalGeneratorTest : public ::testing::Test
{
protected:
    void SignalGenerator_Constructor()
    {
        // Nothing definitive to test
    }

    void SignalGenerator_Cosine()
    {
        const size_t f_s = 1000;
        const size_t N = 10;
        const auto t_s = 1 / static_cast<T>(f_s);

        const T frequency = 100;
        const T amplitude = 2;
        const T phaseShift = static_cast<T>(M_PI) / 2;
        const T verticalShift = 2;

        dsp::signal<T> cosine{ f_s, N };

        for (size_t n = 0; n < N; ++n)
        {
            cosine[n] = amplitude * std::cos(2 * static_cast<T>(M_PI) * frequency * n * t_s + phaseShift) + verticalShift;
        }

        dsp::signal_generator<T> g{ f_s, N };
        const auto x = g.cosine(frequency, amplitude, phaseShift, verticalShift);

        ASSERT_EQ(x.size(), cosine.size());
        ASSERT_EQ(x.sample_rate(), cosine.sample_rate());

        for (size_t n = 0; n < N; ++n)
        {
            EXPECT_NEAR(x[n], cosine[n], constants::EPSILON);
        }
    }

    void SignalGenerator_Sine()
    {
        const size_t f_s = 1000;
        const size_t N = 10;
        const auto t_s = 1 / static_cast<T>(f_s);

        const T frequency = 100;
        const T amplitude = 2;
        const T phaseShift = static_cast<T>(M_PI) / 2;
        const T verticalShift = 2;

        dsp::signal<T> sine{ f_s, N };

        for (size_t n = 0; n < N; ++n)
        {
            sine[n] = amplitude * std::sin(2 * static_cast<T>(M_PI) * frequency * n * t_s + phaseShift) + verticalShift;
        }

        dsp::signal_generator<T> g{ f_s, N };
        const auto x = g.sine(frequency, amplitude, phaseShift, verticalShift);

        ASSERT_EQ(x.size(), sine.size());
        ASSERT_EQ(x.sample_rate(), sine.sample_rate());

        for (size_t n = 0; n < N; ++n)
        {
            EXPECT_NEAR(x[n], sine[n], constants::EPSILON);
        }
    }
};

using SignalGeneratorTestTypes = ::testing::Types<double, float>;

TYPED_TEST_SUITE(SignalGeneratorTest, SignalGeneratorTestTypes);

TYPED_TEST(SignalGeneratorTest, SignalGenerator_Constructor)
{
    this->SignalGenerator_Constructor();
}

TYPED_TEST(SignalGeneratorTest, SignalGenerator_Cosine)
{
    this->SignalGenerator_Cosine();
}

TYPED_TEST(SignalGeneratorTest, SignalGenerator_Sine)
{
    this->SignalGenerator_Sine();
}