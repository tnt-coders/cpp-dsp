#include "constants.hpp"
#include <complex>
#include <cmath>
#include <gtest/gtest.h>
#include <memory>
#include <tnt/dsp/analysis.hpp>
#include <tnt/dsp/signal.hpp>
#include <tnt/dsp/signal_generator.hpp>

using namespace tnt;

template <typename T>
class AnalysisTest : public ::testing::Test
{
protected:
    void Magnitude_RealSample() const
    {
        EXPECT_NEAR(dsp::Magnitude(static_cast<T>(1)), 1.0, constants::EPSILON);
        EXPECT_NEAR(dsp::Magnitude(static_cast<T>(-1)), 1.0, constants::EPSILON);
    }

    void Magnitude_ComplexSample() const
    {
        EXPECT_NEAR(dsp::Magnitude(std::complex<T>{ 3.0, 4.0 }), 5.0, constants::EPSILON);
        EXPECT_NEAR(dsp::Magnitude(std::complex<T>{ -3.0, 4.0 }), 5.0, constants::EPSILON);
        EXPECT_NEAR(dsp::Magnitude(std::complex<T>{ 3.0, -4.0 }), 5.0, constants::EPSILON);
        EXPECT_NEAR(dsp::Magnitude(std::complex<T>{ -3.0, -4.0 }), 5.0, constants::EPSILON);
    }

    void Magnitude_RealSignal() const
    {
        const dsp::SignalGenerator<T> g{ 4000, 4 };
        const auto x = g.Cosine(1000);
        const auto x_magnitude = dsp::Magnitude(x);

        EXPECT_NEAR(x_magnitude[0], 1.0, constants::EPSILON);
        EXPECT_NEAR(x_magnitude[1], 0.0, constants::EPSILON);
        EXPECT_NEAR(x_magnitude[2], 1.0, constants::EPSILON);
        EXPECT_NEAR(x_magnitude[3], 0.0, constants::EPSILON);
    }

    void Magnitude_ComplexSignal() const
    {
        const dsp::SignalGenerator<T> g{ 4000, 4 };
        const dsp::Signal<std::complex<T>> x{ g.Cosine(1000), g.Sine(1000) };
        const auto x_magnitude = dsp::Magnitude(x);

        EXPECT_NEAR(x_magnitude[0], 1.0, constants::EPSILON);
        EXPECT_NEAR(x_magnitude[1], 1.0, constants::EPSILON);
        EXPECT_NEAR(x_magnitude[2], 1.0, constants::EPSILON);
        EXPECT_NEAR(x_magnitude[3], 1.0, constants::EPSILON);
    }

    void Phase_RealSample() const
    {
        EXPECT_NEAR(dsp::Phase(static_cast<T>(1)), 0.0, constants::EPSILON);
        EXPECT_NEAR(dsp::Phase(static_cast<T>(-1)), M_PI, constants::EPSILON);
    }

    void Phase_ComplexSample() const
    {
        EXPECT_NEAR(dsp::Phase(std::complex<T>{ 1.0, 1.0 }), M_PI / 4, constants::EPSILON);
        EXPECT_NEAR(dsp::Phase(std::complex<T>{ -1.0, 1.0 }), 3 * M_PI / 4, constants::EPSILON);
        EXPECT_NEAR(dsp::Phase(std::complex<T>{ 1.0, -1.0 }), -M_PI / 4, constants::EPSILON);
        EXPECT_NEAR(dsp::Phase(std::complex<T>{ -1.0, -1.0 }), -3 * M_PI / 4, constants::EPSILON);
    }

    void Phase_RealSignal() const
    {
        const dsp::SignalGenerator<T> g{ 4000, 4 };
        auto x = g.Cosine(1000);

        // Phase doesn't calculate accurately for very small values
        // If the magnitude is below EPSILON assume the value is 0
        for (auto& sample : x)
        {
            if (dsp::Magnitude(sample) < constants::EPSILON)
            {
                sample = 0;
            }
        }

        const auto x_phase = dsp::Phase(x);

        EXPECT_NEAR(x_phase[0], 0.0, constants::EPSILON);
        EXPECT_NEAR(x_phase[1], 0.0, constants::EPSILON);
        EXPECT_NEAR(x_phase[2], M_PI, constants::EPSILON);
        EXPECT_NEAR(x_phase[3], 0.0, constants::EPSILON);
    }

    void Phase_ComplexSignal() const
    {
        const dsp::SignalGenerator<T> g{ 4000, 4 };
        dsp::Signal<std::complex<T>> x{ g.Cosine(1000), g.Sine(1000) };

        // Phase doesn't calculate accurately for very small values
        // If the magnitude is below EPSILON assume the value is 0
        for (auto& sample : x)
        {
            if (dsp::Magnitude(sample.real()) < constants::EPSILON)
            {
                sample.real(0);
            }
            if (dsp::Magnitude(sample.imag()) < constants::EPSILON)
            {
                sample.imag(0);
            }
        }

        const auto x_phase = dsp::Phase(x);

        EXPECT_NEAR(x_phase[0], 0.0, constants::EPSILON);
        EXPECT_NEAR(x_phase[1], M_PI / 2, constants::EPSILON);
        EXPECT_NEAR(x_phase[2], M_PI, constants::EPSILON);
        EXPECT_NEAR(x_phase[3], -M_PI / 2, constants::EPSILON);
    }

    void Power_RealSample() const
    {
        EXPECT_NEAR(dsp::Power(static_cast<T>(1)), 1.0, constants::EPSILON);
        EXPECT_NEAR(dsp::Power(static_cast<T>(-1)), 1.0, constants::EPSILON);
        EXPECT_NEAR(dsp::Power(static_cast<T>(2)), 4.0, constants::EPSILON);
        EXPECT_NEAR(dsp::Power(static_cast<T>(-2)), 4.0, constants::EPSILON);
    }

    void Power_ComplexSample() const
    {
        EXPECT_NEAR(dsp::Power(std::complex<T>{ 3.0, 4.0 }), 25.0, constants::EPSILON);
        EXPECT_NEAR(dsp::Power(std::complex<T>{ -3.0, 4.0 }), 25.0, constants::EPSILON);
        EXPECT_NEAR(dsp::Power(std::complex<T>{ 3.0, -4.0 }), 25.0, constants::EPSILON);
        EXPECT_NEAR(dsp::Power(std::complex<T>{ -3.0, -4.0 }), 25.0, constants::EPSILON);
    }

    void Power_RealSignal() const
    {
        const dsp::SignalGenerator<T> g{ 4000, 4 };
        const auto x1 = g.Cosine(1000);
        const auto x2 = g.Cosine(1000, 2);
        const auto x1_power = dsp::Power(x1);
        const auto x2_power = dsp::Power(x2);

        EXPECT_NEAR(x1_power[0], 1.0, constants::EPSILON);
        EXPECT_NEAR(x1_power[1], 0.0, constants::EPSILON);
        EXPECT_NEAR(x1_power[2], 1.0, constants::EPSILON);
        EXPECT_NEAR(x1_power[3], 0.0, constants::EPSILON);
        EXPECT_NEAR(x2_power[0], 4.0, constants::EPSILON);
        EXPECT_NEAR(x2_power[1], 0.0, constants::EPSILON);
        EXPECT_NEAR(x2_power[2], 4.0, constants::EPSILON);
        EXPECT_NEAR(x2_power[3], 0.0, constants::EPSILON);
    }

    void Power_ComplexSignal() const
    {
        const dsp::SignalGenerator<T> g{ 4000, 4 };
        const dsp::Signal<std::complex<T>> x1{ g.Cosine(1000), g.Sine(1000) };
        const dsp::Signal<std::complex<T>> x2{ g.Cosine(1000, 2), g.Sine(1000, 2) };
        const auto x1_power = dsp::Power(x1);
        const auto x2_power = dsp::Power(x2);

        EXPECT_NEAR(x1_power[0], 1.0, constants::EPSILON);
        EXPECT_NEAR(x1_power[1], 1.0, constants::EPSILON);
        EXPECT_NEAR(x1_power[2], 1.0, constants::EPSILON);
        EXPECT_NEAR(x1_power[3], 1.0, constants::EPSILON);
        EXPECT_NEAR(x2_power[0], 4.0, constants::EPSILON);
        EXPECT_NEAR(x2_power[1], 4.0, constants::EPSILON);
        EXPECT_NEAR(x2_power[2], 4.0, constants::EPSILON);
        EXPECT_NEAR(x2_power[3], 4.0, constants::EPSILON);
    }
};

using AnalysisTestTypes = ::testing::Types<double, float>;

TYPED_TEST_SUITE(AnalysisTest, AnalysisTestTypes);

TYPED_TEST(AnalysisTest, Magnitude_RealSample)
{
    this->Magnitude_RealSample();
}

TYPED_TEST(AnalysisTest, Magnitude_ComplexSample)
{
    this->Magnitude_ComplexSample();
}

TYPED_TEST(AnalysisTest, Magnitude_RealSignal)
{
    this->Magnitude_RealSignal();
}

TYPED_TEST(AnalysisTest, Magnitude_ComplexSignal)
{
    this->Magnitude_ComplexSignal();
}

TYPED_TEST(AnalysisTest, Phase_RealSample)
{
    this->Phase_RealSample();
}

TYPED_TEST(AnalysisTest, Phase_ComplexSample)
{
    this->Phase_ComplexSample();
}

TYPED_TEST(AnalysisTest, Phase_RealSignal)
{
    this->Phase_RealSignal();
}

TYPED_TEST(AnalysisTest, Phase_ComplexSignal)
{
    this->Phase_ComplexSignal();
}

TYPED_TEST(AnalysisTest, Power_RealSample)
{
    this->Power_RealSample();
}

TYPED_TEST(AnalysisTest, Power_ComplexSample)
{
    this->Power_ComplexSample();
}

TYPED_TEST(AnalysisTest, Power_RealSignal)
{
    this->Power_RealSignal();
}

TYPED_TEST(AnalysisTest, Power_ComplexSignal)
{
    this->Power_ComplexSignal();
}