#include "constants.hpp"
#include <gtest/gtest.h>
#include <tnt/dsp/convolution.hpp>
#include <tnt/dsp/signal.hpp>
#include <tnt/dsp/signal_generator.hpp>
#include <vector>

using namespace tnt;

template <typename T>
class ConvolutionTest : public ::testing::Test
{
protected:
    void Convolve_RealSignal_RealSignal() const
    {
        dsp::SignalGenerator<T> g(4000, 4);
        const auto a = g.Cos(1000);
        const auto b = g.Sin(1000);
        const auto c = dsp::Convolve(a, b);

        ASSERT_EQ(c.size(), a.size());
        ASSERT_EQ(c.size(), b.size());

        EXPECT_NEAR(c[0], 0.0, constants::EPSILON);
        EXPECT_NEAR(c[1], 2.0, constants::EPSILON);
        EXPECT_NEAR(c[2], 0.0, constants::EPSILON);
        EXPECT_NEAR(c[3], -2.0, constants::EPSILON);
    }

    void Convolve_RealSignal_ComplexSignal() const
    {
        dsp::SignalGenerator<T> g(4000, 4);
        const auto a = g.Cos(1000);
        dsp::Signal<std::complex<T>> b = { g.Cos(1000), g.Sin(1000) };
        const auto c = dsp::Convolve(a, b);

        ASSERT_EQ(c.size(), a.size());
        ASSERT_EQ(c.size(), b.size());

        EXPECT_NEAR(c[0].real(), 2.0, constants::EPSILON);
        EXPECT_NEAR(c[0].imag(), 0.0, constants::EPSILON);
        EXPECT_NEAR(c[1].real(), 0.0, constants::EPSILON);
        EXPECT_NEAR(c[1].imag(), 2.0, constants::EPSILON);
        EXPECT_NEAR(c[2].real(), -2.0, constants::EPSILON);
        EXPECT_NEAR(c[2].imag(), 0.0, constants::EPSILON);
        EXPECT_NEAR(c[3].real(), 0.0, constants::EPSILON);
        EXPECT_NEAR(c[3].imag(), -2.0, constants::EPSILON);
    }

    void Convolve_ComplexSignal_RealSignal() const
    {
        dsp::SignalGenerator<T> g(4000, 4);
        dsp::Signal<std::complex<T>> a = { g.Cos(1000), g.Sin(1000) };
        const auto b = g.Cos(1000);
        const auto c = dsp::Convolve(a, b);

        ASSERT_EQ(c.size(), a.size());
        ASSERT_EQ(c.size(), b.size());

        EXPECT_NEAR(c[0].real(), 2.0, constants::EPSILON);
        EXPECT_NEAR(c[0].imag(), 0.0, constants::EPSILON);
        EXPECT_NEAR(c[1].real(), 0.0, constants::EPSILON);
        EXPECT_NEAR(c[1].imag(), 2.0, constants::EPSILON);
        EXPECT_NEAR(c[2].real(), -2.0, constants::EPSILON);
        EXPECT_NEAR(c[2].imag(), 0.0, constants::EPSILON);
        EXPECT_NEAR(c[3].real(), 0.0, constants::EPSILON);
        EXPECT_NEAR(c[3].imag(), -2.0, constants::EPSILON);
    }

    void Convolve_ComplexSignal_ComplexSignal() const
    {
        dsp::SignalGenerator<T> g(4000, 4);
        dsp::Signal<std::complex<T>> a = { g.Cos(1000), g.Sin(1000) };
        dsp::Signal<std::complex<T>> b = { g.Cos(1000), g.Sin(1000) };
        const auto c = dsp::Convolve(a, b);

        EXPECT_NEAR(c[0].real(), 4.0, constants::EPSILON);
        EXPECT_NEAR(c[0].imag(), 0.0, constants::EPSILON);
        EXPECT_NEAR(c[1].real(), 0.0, constants::EPSILON);
        EXPECT_NEAR(c[1].imag(), 4.0, constants::EPSILON);
        EXPECT_NEAR(c[2].real(), -4.0, constants::EPSILON);
        EXPECT_NEAR(c[2].imag(), 0.0, constants::EPSILON);
        EXPECT_NEAR(c[3].real(), 0.0, constants::EPSILON);
        EXPECT_NEAR(c[3].imag(), -4.0, constants::EPSILON);
    }
};

using ConvolutionTestTypes = ::testing::Types<double, float>;

TYPED_TEST_SUITE(ConvolutionTest, ConvolutionTestTypes);

TYPED_TEST(ConvolutionTest, Convolve_RealSignal_RealSignal)
{
    this->Convolve_RealSignal_RealSignal();
}

TYPED_TEST(ConvolutionTest, Convolve_RealSignal_ComplexSignal)
{
    this->Convolve_RealSignal_ComplexSignal();
}

TYPED_TEST(ConvolutionTest, Convolve_ComplexSignal_RealSignal)
{
    this->Convolve_ComplexSignal_RealSignal();
}

TYPED_TEST(ConvolutionTest, Convolve_ComplexSignal_ComplexSignal)
{
    this->Convolve_ComplexSignal_ComplexSignal();
}

//TODO: Add more tests