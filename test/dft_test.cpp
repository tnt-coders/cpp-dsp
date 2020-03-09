#include <cmath>
#include <gtest/gtest.h>
#include <tnt/dsp/dft.hpp>

using namespace tnt;

using std::pow;
using std::sin;
using std::vector;

TEST(DFT, RealInputSignal)
{
    double epsilon = pow(10.0, -4);

    size_t N = 4;
    vector<double> x(N);

    int f_s = 4000;
    double t_s = 1.0 / f_s;

    for (size_t n = 0; n < N; ++n)
    {
        x[n] = sin(2 * M_PI * 1000 * n * t_s);
    }

    auto X = dsp::DFT(x);

    ASSERT_EQ(X.size(), x.size());

    EXPECT_NEAR(X[0].real(), 0.0, epsilon);
    EXPECT_NEAR(X[0].imag(), 0.0, epsilon);
    EXPECT_NEAR(X[1].real(), 0.0, epsilon);
    EXPECT_NEAR(X[1].imag(), -2.0, epsilon);
    EXPECT_NEAR(X[2].real(), 0.0, epsilon);
    EXPECT_NEAR(X[2].imag(), 0.0, epsilon);
    EXPECT_NEAR(X[3].real(), 0.0, epsilon);
    EXPECT_NEAR(X[3].imag(), 2.0, epsilon);
}

TEST(DFT, ComplexInputSignal)
{
    // TODO: Test complex input signals
    FAIL();
}