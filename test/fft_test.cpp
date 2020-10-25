#include "constants.hpp"
#include <complex>
#include <gtest/gtest.h>
#include <tnt/dsp/dft.hpp>
#include <tnt/dsp/fft.hpp>
#include <tnt/dsp/signal.hpp>
#include <tnt/dsp/signal_generator.hpp>

// TODO: Test FFT/IFFT of larger sizes for speed

using namespace tnt;

template <typename T>
class FFTTest : public ::testing::Test
{
protected:
    void FFT_RealSignal() const
    {
        for (size_t N = 1; N <= 100; ++N)
        {
            dsp::SignalGenerator<T> g(1000, N);
            const auto x = g.Cos(100);
            const auto X = dsp::DFT(x);
            const auto X2 = dsp::FFT(x);

            ASSERT_EQ(X.size(), N);
            ASSERT_EQ(X2.size(), N);

            for (size_t m = 0; m < N; ++m)
            {
                EXPECT_NEAR(X[m].real(), X2[m].real(), constants::EPSILON);
                EXPECT_NEAR(X[m].imag(), X2[m].imag(), constants::EPSILON);
            }
        }
    }

    void FFT_ComplexSignal() const
    {
        for (size_t N = 1; N <= 100; ++N)
        {
            dsp::SignalGenerator<T> g(1000, N);
            dsp::Signal<std::complex<T>> x = { g.Cos(100), g.Sin(100) };
            const auto X = dsp::DFT(x);
            const auto X2 = dsp::FFT(x);

            ASSERT_EQ(X.size(), N);
            ASSERT_EQ(X2.size(), N);

            for (size_t m = 0; m < N; ++m)
            {
                EXPECT_NEAR(X[m].real(), X2[m].real(), constants::EPSILON);
                EXPECT_NEAR(X[m].imag(), X2[m].imag(), constants::EPSILON);
            }
        }
    }

    void IFFT_RealSignal() const
    {
        for (size_t N = 1; N <= 100; ++N)
        {
            dsp::SignalGenerator<T> g(1000, N);
            const auto x = g.Cos(100);
            auto X = dsp::FFT(x);
            auto x2 = dsp::IFFT(X);

            ASSERT_EQ(x.size(), N);
            ASSERT_EQ(x2.size(), N);

            for (size_t n = 0; n < N; ++n)
            {
                EXPECT_NEAR(x[n], x2[n].real(), constants::EPSILON);
            }
        }
    }

    void IFFT_ComplexSignal() const
    {
        for (size_t N = 1; N <= 100; ++N)
        {
            dsp::SignalGenerator<T> g(1000, N);
            dsp::Signal<std::complex<T>> x = { g.Cos(100), g.Sin(100) };
            const auto X = dsp::FFT(x);
            const auto x2 = dsp::IFFT(X);

            ASSERT_EQ(x.size(), N);
            ASSERT_EQ(x2.size(), N);

            for (size_t m = 0; m < N; ++m)
            {
                EXPECT_NEAR(x[m].real(), x2[m].real(), constants::EPSILON);
                EXPECT_NEAR(x[m].imag(), x2[m].imag(), constants::EPSILON);
            }
        }
    }
};

using FFTTestTypes = ::testing::Types<double, float>;

TYPED_TEST_SUITE(FFTTest, FFTTestTypes);

TYPED_TEST(FFTTest, FFT_RealSignal)
{
    this->FFT_RealSignal();
}

TYPED_TEST(FFTTest, FFT_ComplexSignal)
{
    this->FFT_ComplexSignal();
}

TYPED_TEST(FFTTest, IFFT_RealSignal)
{
    this->IFFT_RealSignal();
}

TYPED_TEST(FFTTest, IFFT_ComplexSignal)
{
    this->IFFT_ComplexSignal();
}