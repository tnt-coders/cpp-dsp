#include "constants.hpp"
#include <complex>
#include <gtest/gtest.h>
#include <tnt/dsp/dft.hpp>
#include <tnt/dsp/signal.hpp>
#include <tnt/dsp/signal_generator.hpp>

using namespace tnt;

template <typename T>
class DFTTest : public ::testing::Test
{
protected:
    void DFT_RealSignal() const
    {
        dsp::SignalGenerator<T> g(4000, 4);
        const auto x = g.Cos(1000);
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

    void DFT_ComplexSignal() const
    {
        dsp::SignalGenerator<T> g(4000, 4);
        dsp::Signal<std::complex<T>> x = { g.Cos(1000), g.Sin(1000) };
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
};

using DFTTestTypes = ::testing::Types<double, float>;

TYPED_TEST_SUITE(DFTTest, DFTTestTypes);

TYPED_TEST(DFTTest, DFT_RealSignal)
{
    this->DFT_RealSignal();
}

TYPED_TEST(DFTTest, DFT_ComplexSignal)
{
    this->DFT_ComplexSignal();
}