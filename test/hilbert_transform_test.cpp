#include "constants.hpp"
#include <complex>
#include <gtest/gtest.h>
#include <tnt/dsp/hilbert_transform.hpp>
#include <tnt/dsp/signal.hpp>
#include <tnt/dsp/signal_generator.hpp>

using namespace tnt;

template <typename T>
class HilbertTransformTest : public ::testing::Test
{
protected:
    void HilbertTransform_EvenLength() const
    {
        dsp::SignalGenerator<T> g(4000, 4);
        const auto x = g.Cosine(1000);

        const auto x_c = dsp::HilbertTransform(x);

        EXPECT_NEAR(x_c[0].real(), 1.0, constants::EPSILON);
        EXPECT_NEAR(x_c[0].imag(), 0.0, constants::EPSILON);
        EXPECT_NEAR(x_c[1].real(), 0.0, constants::EPSILON);
        EXPECT_NEAR(x_c[1].imag(), 1.0, constants::EPSILON);
        EXPECT_NEAR(x_c[2].real(), -1.0, constants::EPSILON);
        EXPECT_NEAR(x_c[2].imag(), 0.0, constants::EPSILON);
        EXPECT_NEAR(x_c[3].real(), 0.0, constants::EPSILON);
        EXPECT_NEAR(x_c[3].imag(), -1.0, constants::EPSILON);
    }
    
    void HilbertTransform_OddLength() const
    {
        dsp::SignalGenerator<T> g(4000, 5);
        const auto x = g.Cosine(1000);
        const auto x_c = dsp::HilbertTransform(x);

        EXPECT_NEAR(x_c[0].real(), 1.0, constants::EPSILON);
        EXPECT_NEAR(x_c[0].imag(), 0.470, constants::EPSILON);
        EXPECT_NEAR(x_c[1].real(), 0.0, constants::EPSILON);
        EXPECT_NEAR(x_c[1].imag(), 1.086, constants::EPSILON);
        EXPECT_NEAR(x_c[2].real(), -1.0, constants::EPSILON);
        EXPECT_NEAR(x_c[2].imag(), 0.0, constants::EPSILON);
        EXPECT_NEAR(x_c[3].real(), 0.0, constants::EPSILON);
        EXPECT_NEAR(x_c[3].imag(), -1.086, constants::EPSILON);
        EXPECT_NEAR(x_c[4].real(), 1.0, constants::EPSILON);
        EXPECT_NEAR(x_c[4].imag(), -0.470, constants::EPSILON);
    }
};

using HilbertTransformTestTypes = ::testing::Types<double, float>;

TYPED_TEST_SUITE(HilbertTransformTest, HilbertTransformTestTypes);

TYPED_TEST(HilbertTransformTest, HilbertTransform_EvenLength)
{
    this->HilbertTransform_EvenLength();
}

TYPED_TEST(HilbertTransformTest, HilbertTransform_OddLength)
{
    this->HilbertTransform_OddLength();
}