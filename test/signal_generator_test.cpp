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

    void SignalGenerator_Cos()
    {
        const size_t f_s = 1000;
        const size_t N = 10;
        const auto t_s = T{ 1.0 } / f_s;

        const T frequency = 100;
        const T amplitude = 2;
        const T phaseShift = T{ M_PI } / 2;
        const T verticalShift = 2;

        dsp::Signal<T> cosine(f_s, N);

        for (size_t n = 0; n < N; ++n)
        {
            cosine[n] = amplitude * std::cos(2 * T{ M_PI } * frequency * n * t_s + phaseShift) + verticalShift;
        }

        dsp::SignalGenerator<T> g(f_s, N);
        const auto x = g.Cos(frequency, amplitude, phaseShift, verticalShift);

        ASSERT_EQ(x.size(), cosine.size());
        ASSERT_EQ(x.GetSampleRate(), cosine.GetSampleRate());

        for (size_t n = 0; n < N; ++n)
        {
            EXPECT_NEAR(x[n], cosine[n], constants::EPSILON);
        }
    }

    void SignalGenerator_Sin()
    {
        const size_t f_s = 1000;
        const size_t N = 10;
        const auto t_s = T{ 1.0 } / f_s;

        const T frequency = 100;
        const T amplitude = 2;
        const T phaseShift = T{ M_PI } / 2;
        const T verticalShift = 2;

        dsp::Signal<T> sine(f_s, N);

        for (size_t n = 0; n < N; ++n)
        {
            sine[n] = amplitude * std::sin(2 * T{ M_PI } *frequency * n * t_s + phaseShift) + verticalShift;
        }

        dsp::SignalGenerator<T> g(f_s, N);
        const auto x = g.Sin(frequency, amplitude, phaseShift, verticalShift);

        ASSERT_EQ(x.size(), sine.size());
        ASSERT_EQ(x.GetSampleRate(), sine.GetSampleRate());

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

TYPED_TEST(SignalGeneratorTest, SignalGenerator_Cos)
{
    this->SignalGenerator_Cos();
}

TYPED_TEST(SignalGeneratorTest, SignalGenerator_Sin)
{
    this->SignalGenerator_Sin();
}