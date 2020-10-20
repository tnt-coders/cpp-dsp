#include "constants.hpp"

#include <complex>
#include <cmath>
#include <gtest/gtest.h>
#include <memory>
#include <tnt/dsp/analysis.hpp>
#include <tnt/dsp/signal.hpp>
#include <vector>

using namespace tnt;

template <typename T>
class AnalysisTest : public ::testing::Test
{
protected:
    void Magnitude_RealSample()
    {
        EXPECT_NEAR(dsp::Magnitude(T{ 1.0 }), 1.0, constants::EPSILON);
        EXPECT_NEAR(dsp::Magnitude(T{ -1.0 }), 1.0, constants::EPSILON);
    }

    void Magnitude_ComplexSample()
    {
        EXPECT_NEAR(dsp::Magnitude(std::complex<T>{ 3.0, 4.0 }), 5.0, constants::EPSILON);
        EXPECT_NEAR(dsp::Magnitude(std::complex<T>{ -3.0, 4.0 }), 5.0, constants::EPSILON);
        EXPECT_NEAR(dsp::Magnitude(std::complex<T>{ 3.0, -4.0 }), 5.0, constants::EPSILON);
        EXPECT_NEAR(dsp::Magnitude(std::complex<T>{ -3.0, -4.0 }), 5.0, constants::EPSILON);
    }

    void Magnitude_RealSignal()
    {
        const auto f_s = 4000;
        const auto N = 4;

        dsp::Signal<T> x(f_s, N);

        const auto t_s = 1.0f / f_s;

        for (auto n = 0; n < N; ++n)
        {
            x[n] = T{ cos(2 * T{ M_PI } * 1000 * n * t_s) };
        }

        const auto x_magnitude = dsp::Magnitude(x);

        EXPECT_NEAR(x_magnitude[0], 1.0, constants::EPSILON);
        EXPECT_NEAR(x_magnitude[1], 0.0, constants::EPSILON);
        EXPECT_NEAR(x_magnitude[2], 1.0, constants::EPSILON);
        EXPECT_NEAR(x_magnitude[3], 0.0, constants::EPSILON);
    }

    void Magnitude_ComplexSignal()
    {
        const auto f_s = 4000;
        const auto N = 4;

        dsp::Signal<std::complex<T>> x(f_s, N);

        const auto t_s = 1.0f / f_s;

        for (auto n = 0; n < N; ++n)
        {
            x[n] = std::complex<T>{ cos(2 * T{ M_PI } * 1000 * n * t_s), sin(2 * T{ M_PI } * 1000 * n * t_s) };
        }

        const auto x_magnitude = dsp::Magnitude(x);

        EXPECT_NEAR(x_magnitude[0], 1.0, constants::EPSILON);
        EXPECT_NEAR(x_magnitude[1], 1.0, constants::EPSILON);
        EXPECT_NEAR(x_magnitude[2], 1.0, constants::EPSILON);
        EXPECT_NEAR(x_magnitude[3], 1.0, constants::EPSILON);
    }

    void Phase_RealSample()
    {
        EXPECT_NEAR(dsp::Phase(T{ 1.0 }), 0.0, constants::EPSILON);
        EXPECT_NEAR(dsp::Phase(T{ -1.0 }), M_PI, constants::EPSILON);
    }

    void Phase_ComplexSample()
    {
        EXPECT_NEAR(dsp::Phase(std::complex<T>{ 1.0, 1.0 }), M_PI / 4, constants::EPSILON);
        EXPECT_NEAR(dsp::Phase(std::complex<T>{ -1.0, 1.0 }), 3 * M_PI / 4, constants::EPSILON);
        EXPECT_NEAR(dsp::Phase(std::complex<T>{ 1.0, -1.0 }), -M_PI / 4, constants::EPSILON);
        EXPECT_NEAR(dsp::Phase(std::complex<T>{ -1.0, -1.0 }), -3 * M_PI / 4, constants::EPSILON);
    }

    void Phase_RealSignal()
    {
        const auto f_s = 4000;
        const auto N = 4;

        dsp::Signal<T> x(f_s, N);

        const auto t_s = 1.0f / f_s;

        for (auto n = 0; n < N; ++n)
        {
            x[n] = T{ cos(2 * T{ M_PI } * 1000 * n * t_s) };

            // Phase doesn't calculate accurately for very small values
            // If the magnitude is below EPSILON assume the value is 0
            if (dsp::Magnitude(x[n]) < constants::EPSILON)
            {
                x[n] = T{ 0.0 };
            }
        }

        const auto x_phase = dsp::Phase(x);

        EXPECT_NEAR(x_phase[0], 0.0, constants::EPSILON);
        EXPECT_NEAR(x_phase[1], 0.0, constants::EPSILON);
        EXPECT_NEAR(x_phase[2], M_PI, constants::EPSILON);
        EXPECT_NEAR(x_phase[3], 0.0, constants::EPSILON);
    }

    void Phase_ComplexSignal()
    {
        const auto f_s = 4000;
        const auto N = 4;

        dsp::Signal<std::complex<T>> x(f_s, N);

        const auto t_s = 1.0f / f_s;

        for (auto n = 0; n < N; ++n)
        {
            auto real = T{ cos(2 * T{ M_PI } * 1000 * n * t_s) };
            auto imag = T{ sin(2 * T{ M_PI } * 1000 * n * t_s) };

            // Phase doesn't calculate accurately for very small values
            // If the magnitude of the real or imag part is below EPSILON, assume it is 0
            if (dsp::Magnitude(real) < constants::EPSILON)
            {
                real = 0;
            }
            if (dsp::Magnitude(imag) < constants::EPSILON)
            {
                imag = 0;
            }

            x[n] = std::complex<T>{ real, imag };
        }

        const auto x_phase = dsp::Phase(x);

        EXPECT_NEAR(x_phase[0], 0.0, constants::EPSILON);
        EXPECT_NEAR(x_phase[1], M_PI / 2, constants::EPSILON);
        EXPECT_NEAR(x_phase[2], M_PI, constants::EPSILON);
        EXPECT_NEAR(x_phase[3], -M_PI / 2, constants::EPSILON);
    }

    void Power_RealSample()
    {
        EXPECT_NEAR(dsp::Power(T{ 1.0 }), 1.0, constants::EPSILON);
        EXPECT_NEAR(dsp::Power(T{ -1.0 }), 1.0, constants::EPSILON);
        EXPECT_NEAR(dsp::Power(T{ 2.0 }), 4.0, constants::EPSILON);
        EXPECT_NEAR(dsp::Power(T{ -2.0 }), 4.0, constants::EPSILON);
    }

    void Power_ComplexSample()
    {
        EXPECT_NEAR(dsp::Power(std::complex<T>{ 3.0, 4.0 }), 25.0, constants::EPSILON);
        EXPECT_NEAR(dsp::Power(std::complex<T>{ -3.0, 4.0 }), 25.0, constants::EPSILON);
        EXPECT_NEAR(dsp::Power(std::complex<T>{ 3.0, -4.0 }), 25.0, constants::EPSILON);
        EXPECT_NEAR(dsp::Power(std::complex<T>{ -3.0, -4.0 }), 25.0, constants::EPSILON);
    }

    void Power_RealSignal()
    {
        const auto f_s = 4000;
        const auto N = 4;

        dsp::Signal<T> x1(f_s, N);
        dsp::Signal<T> x2(f_s, N);

        const auto t_s = 1.0f / f_s;

        for (auto n = 0; n < N; ++n)
        {
            x1[n] = T{ cos(2 * T{ M_PI } * 1000 * n * t_s) };
            x2[n] = T{ 2 * cos(2 * T{ M_PI } * 1000 * n * t_s) };
        }

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

    void Power_ComplexSignal()
    {
        const auto f_s = 4000;
        const auto N = 4;

        dsp::Signal<std::complex<double>> x1(f_s, N);
        dsp::Signal<std::complex<double>> x2(f_s, N);

        const auto t_s = 1.0f / f_s;

        for (auto n = 0; n < N; ++n)
        {
            x1[n] = std::complex<T>{ cos(2 * T{ M_PI } * 1000 * n * t_s), sin(2 * T{ M_PI } * 1000 * n * t_s) };
            x2[n] = std::complex<T>{ 2 * cos(2 * T{ M_PI } * 1000 * n * t_s), 2 * sin(2 * T{ M_PI } * 1000 * n * t_s) };
        }

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

TYPED_TEST_CASE(AnalysisTest, AnalysisTestTypes);

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