#include "constants.hpp"
#include <cmath>
#include <complex>
#include <gtest/gtest.h>
#include <tnt/dsp/dft.hpp>
#include <tnt/dsp/signal.hpp>
#include <vector>

// TODO: Add tests for types other than double and complex<double>

using namespace tnt;

using std::complex;
using std::pow;
using std::sin;
using std::vector;

TEST(DFT, DFT_RealSignal)
{
    size_t f_s = 4000;
    size_t N = 4;

    dsp::Signal<double> x(f_s, N);

    auto t_s = 1.0 / f_s;

    for (size_t n = 0; n < N; ++n)
    {
        x[n] = cos(2 * M_PI * 1000 * n * t_s);
    }

    auto X = dsp::DFT(x);

    ASSERT_EQ(X.size(), x.size());

    EXPECT_NEAR(X[0].real(), 0.0, constants::EPSILON);
    EXPECT_NEAR(X[0].imag(), 0.0, constants::EPSILON);
    EXPECT_NEAR(X[1].real(), 2.0, constants::EPSILON);
    EXPECT_NEAR(X[1].imag(), 0.0, constants::EPSILON);
    EXPECT_NEAR(X[2].real(), 0.0, constants::EPSILON);
    EXPECT_NEAR(X[2].imag(), 0.0, constants::EPSILON);
    EXPECT_NEAR(X[3].real(), 2.0, constants::EPSILON);
    EXPECT_NEAR(X[3].imag(), 0.0, constants::EPSILON);
}

TEST(DFT, DFT_ComplexSignal)
{
    size_t f_s = 4000;
    size_t N = 4;

    dsp::Signal<complex<double>> x(f_s, N);

    auto t_s = 1.0 / f_s;

    for (size_t n = 0; n < N; ++n)
    {
        x[n] = { cos(2 * M_PI * 1000 * n * t_s), sin(2 * M_PI * 1000 * n * t_s) };
    }

    auto X = dsp::DFT(x);

    ASSERT_EQ(X.size(), x.size());

    EXPECT_NEAR(X[0].real(), 0.0, constants::EPSILON);
    EXPECT_NEAR(X[0].imag(), 0.0, constants::EPSILON);
    EXPECT_NEAR(X[1].real(), 4.0, constants::EPSILON);
    EXPECT_NEAR(X[1].imag(), 0.0, constants::EPSILON);
    EXPECT_NEAR(X[2].real(), 0.0, constants::EPSILON);
    EXPECT_NEAR(X[2].imag(), 0.0, constants::EPSILON);
    EXPECT_NEAR(X[3].real(), 0.0, constants::EPSILON);
    EXPECT_NEAR(X[3].imag(), 0.0, constants::EPSILON);
}