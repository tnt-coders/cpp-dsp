#include <cmath>
#include <gtest/gtest.h>
#include <tnt/dsp/dft.hpp>
#include <tnt/dsp/fft.hpp>

using namespace tnt;

using std::complex;
using std::cos;
using std::sin;
using std::vector;

// TODO: Test larger sizes for speed
TEST(FFT, FFT)
{
    double epsilon = std::pow(10.0, -4);

    // Test real input signals
    for (size_t N = 1; N <= 100; ++N)
    {
        vector<double> x(N);

        int f_s = 1000;
        double t_s = 1.0 / f_s;

        for (size_t n = 0; n < N; ++n)
        {
            x[n] = sin(2 * M_PI * 100 * n * t_s);
        }

        auto X = dsp::DFT(x);
        auto X2 = dsp::FFT(x);

        ASSERT_EQ(X.size(), N);
        ASSERT_EQ(X2.size(), N);

        for (size_t m = 0; m < N; ++m)
        {
            EXPECT_NEAR(X[m].real(), X2[m].real(), epsilon);
            EXPECT_NEAR(X[m].imag(), X2[m].imag(), epsilon);
        }
    }

    // Test complex input signals
    for (size_t N = 1; N <= 100; ++N)
    {
        vector<complex<double>> x(N);

        int f_s = 1000;
        double t_s = 1.0 / f_s;

        for (size_t n = 0; n < N; ++n)
        {
            x[n] = complex<double>(sin(2 * M_PI * 100 * n * t_s), cos(2 * M_PI * 100 * n * t_s));
        }

        auto X = dsp::DFT(x);
        auto X2 = dsp::FFT(x);

        ASSERT_EQ(X.size(), N);
        ASSERT_EQ(X2.size(), N);

        for (size_t m = 0; m < N; ++m)
        {
            EXPECT_NEAR(X[m].real(), X2[m].real(), epsilon);
            EXPECT_NEAR(X[m].imag(), X2[m].imag(), epsilon);
        }
    }
}

// TODO: Test larger sizes for speed
TEST(FFT, IFFT)
{
    double epsilon = std::pow(10.0, -4);

    // Test real input signals
    for (size_t N = 1; N <= 100; ++N)
    {
        vector<double> x(N);

        int f_s = 1000;
        double t_s = 1.0 / f_s;

        for (size_t n = 0; n < N; ++n)
        {
            x[n] = sin(2 * M_PI * 100 * n * t_s);
        }

        auto X = dsp::FFT(x);
        auto x2 = dsp::IFFT(X);

        ASSERT_EQ(x.size(), N);
        ASSERT_EQ(x2.size(), N);

        for (size_t n = 0; n < N; ++n)
        {
            EXPECT_NEAR(x[n], x2[n].real(), epsilon);
        }
    }

    // Test complex input signals
    for (size_t N = 1; N <= 100; ++N)
    {
        vector<complex<double>> x(N);

        int f_s = 1000;
        double t_s = 1.0 / f_s;

        for (size_t n = 0; n < N; ++n)
        {
            x[n] = complex<double>(sin(2 * M_PI * 100 * n * t_s), cos(2 * M_PI * 100 * n * t_s));
        }

        auto X = dsp::FFT(x);
        auto x2 = dsp::IFFT(X);

        ASSERT_EQ(x.size(), N);
        ASSERT_EQ(x2.size(), N);

        for (size_t m = 0; m < N; ++m)
        {
            EXPECT_NEAR(x[m].real(), x2[m].real(), epsilon);
            EXPECT_NEAR(x[m].imag(), x2[m].imag(), epsilon);
        }
    }
}