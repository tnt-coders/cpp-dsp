#include "constants.hpp"
#include <cmath>
#include <gtest/gtest.h>
#include <tnt/dsp/dft.hpp>
#include <tnt/dsp/fft.hpp>
#include <vector>

using namespace tnt;

// TODO: Test FFT of larger sizes for speed

TEST(FFT, FFT_RealSignal)
{
    for (auto N = 1; N <= 100; ++N)
    {
        const auto f_s = 1000;

        dsp::Signal<double> x(f_s, N);

        const auto t_s = 1.0 / f_s;

        for (auto n = 0; n < N; ++n)
        {
            x[n] = std::cos(2 * M_PI * 100 * n * t_s);
        }

        const auto X = dsp::DFT(x);
        const auto X2 = dsp::FFT(x);

        ASSERT_EQ(X.size(), N);
        ASSERT_EQ(X2.size(), N);

        for (auto m = 0; m < N; ++m)
        {
            EXPECT_NEAR(X[m].real(), X2[m].real(), constants::EPSILON);
            EXPECT_NEAR(X[m].imag(), X2[m].imag(), constants::EPSILON);
        }
    }
}

TEST(FFT, FFT_ComplexSignal)
{
    for (auto N = 1; N <= 100; ++N)
    {
        const auto f_s = 1000;

        dsp::Signal<std::complex<double>> x(f_s, N);

        const auto t_s = 1.0 / f_s;

        for (auto n = 0; n < N; ++n)
        {
            x[n] = { std::cos(2 * M_PI * 100 * n * t_s), std::sin(2 * M_PI * 100 * n * t_s) };
        }

        const auto X = dsp::DFT(x);
        const auto X2 = dsp::FFT(x);

        ASSERT_EQ(X.size(), N);
        ASSERT_EQ(X2.size(), N);

        for (auto m = 0; m < N; ++m)
        {
            EXPECT_NEAR(X[m].real(), X2[m].real(), constants::EPSILON);
            EXPECT_NEAR(X[m].imag(), X2[m].imag(), constants::EPSILON);
        }
    }
}

// TODO: Test IFFT of larger sizes for speed

TEST(IFFT, IFFT_RealSignal)
{
    for (auto N = 1; N <= 100; ++N)
    {
        const auto f_s = 1000;

        dsp::Signal<double> x(f_s, N);

        const auto t_s = 1.0 / f_s;

        for (auto n = 0; n < N; ++n)
        {
            x[n] = std::cos(2 * M_PI * 100 * n * t_s);
        }

        auto X = dsp::FFT(x);
        auto x2 = dsp::IFFT(X);

        ASSERT_EQ(x.size(), N);
        ASSERT_EQ(x2.size(), N);

        for (auto n = 0; n < N; ++n)
        {
            EXPECT_NEAR(x[n], x2[n].real(), constants::EPSILON);
        }
    }
}

TEST(IFFT, IFFT_ComplexSignal)
{
    for (auto N = 1; N <= 100; ++N)
    {
        const auto f_s = 1000;

        dsp::Signal<std::complex<double>> x(f_s, N);

        const auto t_s = 1.0 / f_s;

        for (auto n = 0; n < N; ++n)
        {
            x[n] = { std::cos(2 * M_PI * 100 * n * t_s), std::sin(2 * M_PI * 100 * n * t_s) };
        }

        const auto X = dsp::FFT(x);
        const auto x2 = dsp::IFFT(X);

        ASSERT_EQ(x.size(), N);
        ASSERT_EQ(x2.size(), N);

        for (auto m = 0; m < N; ++m)
        {
            EXPECT_NEAR(x[m].real(), x2[m].real(), constants::EPSILON);
            EXPECT_NEAR(x[m].imag(), x2[m].imag(), constants::EPSILON);
        }
    }
}