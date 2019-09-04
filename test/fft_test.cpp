#include <cmath>
#include <gtest/gtest.h>
#include <tnt/dsp/dft.h>
#include <tnt/dsp/fft.h>

using namespace tnt;

TEST(FFT, FFT)
{
    for (size_t N = 1; N <= 100; ++N)
    {
        std::vector<double> x(N);

        int f_s = 8000;
        double t_s = 1.0 / f_s;

        for (size_t n = 0; n < N; ++n)
        {
            x[n] = std::sin(2 * M_PI * 1000 * n * t_s)
                + 0.5 * std::sin(2 * M_PI * 2000 * n * t_s + 3 * M_PI / 4);
        }

        auto X = dsp::DFT(x);
        auto X2 = dsp::FFT(x);

        ASSERT_EQ(X.size(), N);
        ASSERT_EQ(X2.size(), N);

        double epsilon = std::pow(10.0, -4);

        for (size_t k = 0; k < N; ++k)
        {
            EXPECT_NEAR(X[k].real(), X2[k].real(), epsilon);
            EXPECT_NEAR(X[k].imag(), X2[k].imag(), epsilon);
        }
    }
}

TEST(FFT, IFFT)
{
    //TODO: Implement tests
    FAIL();
}