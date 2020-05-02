#include "constants.hpp"

#include <complex>
#include <cmath>
#include <gtest/gtest.h>
#include <tnt/dsp/analysis.hpp>
#include <vector>

using namespace tnt;

using std::complex;
using std::cos;
using std::sin;
using std::vector;

TEST(Analysis, Magnitude_SingleRealValue)
{
    EXPECT_NEAR(dsp::Magnitude(1.0), 1.0, constants::EPSILON);
    EXPECT_NEAR(dsp::Magnitude(-1.0), 1.0, constants::EPSILON);
}

TEST(Analysis, Magnitude_SingleComplexValue)
{
    EXPECT_NEAR(dsp::Magnitude(complex<double>{ 3.0, 4.0 }), 5.0, constants::EPSILON);
    EXPECT_NEAR(dsp::Magnitude(complex<double>{ -3.0, 4.0 }), 5.0, constants::EPSILON);
    EXPECT_NEAR(dsp::Magnitude(complex<double>{ 3.0, -4.0 }), 5.0, constants::EPSILON);
    EXPECT_NEAR(dsp::Magnitude(complex<double>{ -3.0, -4.0 }), 5.0, constants::EPSILON);
}

TEST(Analysis, Magnitude_VectorOfRealValues)
{
    size_t N = 4;
    vector<double> x(N);

    int f_s = 4000;
    double t_s = 1.0 / f_s;

    for (size_t n = 0; n < N; ++n)
    {
        x[n] = cos(2 * M_PI * 1000 * n * t_s);
    }

    auto x_magnitude = dsp::Magnitude(x);

    EXPECT_NEAR(x_magnitude[0], 1.0, constants::EPSILON);
    EXPECT_NEAR(x_magnitude[1], 0.0, constants::EPSILON);
    EXPECT_NEAR(x_magnitude[2], 1.0, constants::EPSILON);
    EXPECT_NEAR(x_magnitude[3], 0.0, constants::EPSILON);
}

TEST(Analysis, Magnitude_VectorOfComplexValues)
{
    size_t N = 4;
    vector<complex<double>> x(N);

    int f_s = 4000;
    double t_s = 1.0 / f_s;

    for (size_t n = 0; n < N; ++n)
    {
        x[n] = complex<double>{ cos(2 * M_PI * 1000 * n * t_s), sin(2 * M_PI * 1000 * n * t_s) };
    }

    auto x_magnitude = dsp::Magnitude(x);

    EXPECT_NEAR(x_magnitude[0], 1.0, constants::EPSILON);
    EXPECT_NEAR(x_magnitude[1], 1.0, constants::EPSILON);
    EXPECT_NEAR(x_magnitude[2], 1.0, constants::EPSILON);
    EXPECT_NEAR(x_magnitude[3], 1.0, constants::EPSILON);
}

TEST(Analysis, Phase_SingleRealValue)
{
    EXPECT_NEAR(dsp::Phase(1.0), 0.0, constants::EPSILON);
    EXPECT_NEAR(dsp::Phase(-1.0), M_PI, constants::EPSILON);
}

TEST(Analysis, Phase_SingleComplexValue)
{
    EXPECT_NEAR(dsp::Phase(complex<double>{ 1.0, 1.0 }), M_PI/4, constants::EPSILON);
    EXPECT_NEAR(dsp::Phase(complex<double>{ -1.0, 1.0 }), 3 * M_PI / 4, constants::EPSILON);
    EXPECT_NEAR(dsp::Phase(complex<double>{ 1.0, -1.0 }), -M_PI / 4, constants::EPSILON);
    EXPECT_NEAR(dsp::Phase(complex<double>{ -1.0, -1.0 }), -3 * M_PI / 4, constants::EPSILON);
}

TEST(Analysis, Phase_VectorOfRealValues)
{
    size_t N = 4;
    vector<double> x(N);

    int f_s = 4000;
    double t_s = 1.0 / f_s;

    for (size_t n = 0; n < N; ++n)
    {
        x[n] = cos(2 * M_PI * 1000 * n * t_s);

        // Phase doesn't calculate accurately for very small values
        // If the magnitude is below EPSILON assume the value is 0
        if (dsp::Magnitude(x[n]) < constants::EPSILON)
        {
            x[n] = 0.0;
        }
    }

    auto x_phase = dsp::Phase(x);

    EXPECT_NEAR(x_phase[0], 0.0, constants::EPSILON);
    EXPECT_NEAR(x_phase[1], 0.0, constants::EPSILON);
    EXPECT_NEAR(x_phase[2], M_PI, constants::EPSILON);
    EXPECT_NEAR(x_phase[3], 0.0, constants::EPSILON);
}

TEST(Analysis, Phase_VectorOfComplexValues)
{
    size_t N = 4;
    vector<complex<double>> x(N);

    int f_s = 4000;
    double t_s = 1.0 / f_s;

    for (size_t n = 0; n < N; ++n)
    {
        x[n] = complex<double>{ cos(2 * M_PI * 1000 * n * t_s), sin(2 * M_PI * 1000 * n * t_s) };

        // Phase doesn't calculate accurately for very small values
        // If the magnitude is below EPSILON assume the value is 0
        if (dsp::Magnitude(x[n]) < constants::EPSILON)
        {
            x[n] = 0.0;
        }
    }

    auto x_phase = dsp::Phase(x);

    EXPECT_NEAR(x_phase[0], 0.0, constants::EPSILON);
    EXPECT_NEAR(x_phase[1], M_PI / 2, constants::EPSILON);
    EXPECT_NEAR(x_phase[2], M_PI, constants::EPSILON);
    EXPECT_NEAR(x_phase[3], -M_PI / 2, constants::EPSILON);
}

TEST(Analysis, Power_SingleRealValue)
{
    EXPECT_NEAR(dsp::Power(1.0), 1.0, constants::EPSILON);
    EXPECT_NEAR(dsp::Power(-1.0), 1.0, constants::EPSILON);
    EXPECT_NEAR(dsp::Power(2.0), 4.0, constants::EPSILON);
    EXPECT_NEAR(dsp::Power(-2.0), 4.0, constants::EPSILON);
}

TEST(Analysis, Power_SingleComplexValue)
{
    EXPECT_NEAR(dsp::Power(complex<double>{ 3.0, 4.0 }), 25.0, constants::EPSILON);
    EXPECT_NEAR(dsp::Power(complex<double>{ -3.0, 4.0 }), 25.0, constants::EPSILON);
    EXPECT_NEAR(dsp::Power(complex<double>{ 3.0, -4.0 }), 25.0, constants::EPSILON);
    EXPECT_NEAR(dsp::Power(complex<double>{ -3.0, -4.0 }), 25.0, constants::EPSILON);
}

TEST(Analysis, Power_VectorOfRealValues)
{
    size_t N = 4;
    vector<double> x1(N);
    vector<double> x2(N);

    int f_s = 4000;
    double t_s = 1.0 / f_s;

    for (size_t n = 0; n < N; ++n)
    {
        x1[n] = cos(2 * M_PI * 1000 * n * t_s);
        x2[n] = 2 * cos(2 * M_PI * 1000 * n * t_s);
    }

    auto x1_power = dsp::Power(x1);
    auto x2_power = dsp::Power(x2);

    EXPECT_NEAR(x1_power[0], 1.0, constants::EPSILON);
    EXPECT_NEAR(x1_power[1], 0.0, constants::EPSILON);
    EXPECT_NEAR(x1_power[2], 1.0, constants::EPSILON);
    EXPECT_NEAR(x1_power[3], 0.0, constants::EPSILON);
    EXPECT_NEAR(x2_power[0], 4.0, constants::EPSILON);
    EXPECT_NEAR(x2_power[1], 0.0, constants::EPSILON);
    EXPECT_NEAR(x2_power[2], 4.0, constants::EPSILON);
    EXPECT_NEAR(x2_power[3], 0.0, constants::EPSILON);
}

TEST(Analysis, Power_VectorOfComplexValues)
{
    size_t N = 4;
    vector<complex<double>> x1(N);
    vector<complex<double>> x2(N);

    int f_s = 4000;
    double t_s = 1.0 / f_s;

    for (size_t n = 0; n < N; ++n)
    {
        x1[n] = complex<double>{ cos(2 * M_PI * 1000 * n * t_s), sin(2 * M_PI * 1000 * n * t_s) };
        x2[n] = complex<double>{ 2 * cos(2 * M_PI * 1000 * n * t_s), 2 * sin(2 * M_PI * 1000 * n * t_s) };
    }

    auto x1_power = dsp::Power(x1);
    auto x2_power = dsp::Power(x2);

    EXPECT_NEAR(x1_power[0], 1.0, constants::EPSILON);
    EXPECT_NEAR(x1_power[1], 1.0, constants::EPSILON);
    EXPECT_NEAR(x1_power[2], 1.0, constants::EPSILON);
    EXPECT_NEAR(x1_power[3], 1.0, constants::EPSILON);
    EXPECT_NEAR(x2_power[0], 4.0, constants::EPSILON);
    EXPECT_NEAR(x2_power[1], 4.0, constants::EPSILON);
    EXPECT_NEAR(x2_power[2], 4.0, constants::EPSILON);
    EXPECT_NEAR(x2_power[3], 4.0, constants::EPSILON);
}