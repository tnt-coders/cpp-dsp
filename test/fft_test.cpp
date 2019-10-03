#include <cmath>
#include <gtest/gtest.h>
#include <tnt/dsp/dft.h>
#include <tnt/dsp/fft.h>

using namespace tnt;

using std::complex;
using std::cos;
using std::sin;
using std::vector;

class FFTTest : public testing::Test
{
protected:
    double m_epsilon = std::pow(10.0, -4);
};

class FFTSizeTest : public FFTTest, public testing::WithParamInterface<size_t>
{};

TEST_P(FFTSizeTest, FFTSizeTest)
{
    auto N = GetParam();

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

    for (size_t k = 0; k < N; ++k)
    {
        EXPECT_NEAR(X[k].real(), X2[k].real(), m_epsilon);
        EXPECT_NEAR(X[k].imag(), X2[k].imag(), m_epsilon);
    }
}

INSTANTIATE_TEST_CASE_P(NIsEven, FFTSizeTest, testing::Values(2, 4, 6, 8, 10));
INSTANTIATE_TEST_CASE_P(NIsOdd, FFTSizeTest, testing::Values(1, 3, 5, 7, 9));

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

        for (size_t k = 0; k < N; ++k)
        {
            EXPECT_NEAR(X[k].real(), X2[k].real(), epsilon);
            EXPECT_NEAR(X[k].imag(), X2[k].imag(), epsilon);
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

        for (size_t k = 0; k < N; ++k)
        {
            EXPECT_NEAR(X[k].real(), X2[k].real(), epsilon);
            EXPECT_NEAR(X[k].imag(), X2[k].imag(), epsilon);
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

        for (size_t k = 0; k < N; ++k)
        {
            EXPECT_NEAR(x[k], x2[k].real(), epsilon);
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

        for (size_t k = 0; k < N; ++k)
        {
            EXPECT_NEAR(x[k].real(), x2[k].real(), epsilon);
            EXPECT_NEAR(x[k].imag(), x2[k].imag(), epsilon);
        }
    }
}