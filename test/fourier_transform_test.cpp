#include "constants.hpp"
#include <complex>
#include <gtest/gtest.h>
#include <tnt/dsp/fourier_transform.hpp>
#include <tnt/dsp/signal.hpp>
#include <tnt/dsp/signal_generator.hpp>

// TODO: Test FourierTransform/InverseFourierTransform of larger sizes for speed

using namespace tnt;

template <typename T>
class FourierTransformTest : public ::testing::Test
{
protected:
    void FourierTransform_RealSignal() const
    {
        for (size_t N = 1; N <= 100; ++N)
        {
            dsp::SignalGenerator<T> g{ 1000, N };
            const auto x = g.Cosine(100);
            const auto X = DFT(x);
            const auto X2 = dsp::FourierTransform(x);

            ASSERT_EQ(X.size(), N);
            ASSERT_EQ(X2.size(), N);

            for (size_t m = 0; m < N; ++m)
            {
                EXPECT_NEAR(X[m].real(), X2[m].real(), constants::EPSILON);
                EXPECT_NEAR(X[m].imag(), X2[m].imag(), constants::EPSILON);
            }
        }
    }

    void FourierTransform_ComplexSignal() const
    {
        for (size_t N = 1; N <= 100; ++N)
        {
            dsp::SignalGenerator<T> g{ 1000, N };
            dsp::Signal<std::complex<T>> x{ g.Cosine(100), g.Sine(100) };
            const auto X = DFT(x);
            const auto X2 = dsp::FourierTransform(x);

            ASSERT_EQ(X.size(), N);
            ASSERT_EQ(X2.size(), N);

            for (size_t m = 0; m < N; ++m)
            {
                EXPECT_NEAR(X[m].real(), X2[m].real(), constants::EPSILON);
                EXPECT_NEAR(X[m].imag(), X2[m].imag(), constants::EPSILON);
            }
        }
    }

    void InverseFourierTransform_RealSignal() const
    {
        for (size_t N = 1; N <= 100; ++N)
        {
            dsp::SignalGenerator<T> g{ 1000, N };
            const auto x = g.Cosine(100);
            auto X = dsp::FourierTransform(x);
            auto x2 = dsp::InverseFourierTransform(X);

            ASSERT_EQ(x.size(), N);
            ASSERT_EQ(x2.size(), N);

            for (size_t n = 0; n < N; ++n)
            {
                EXPECT_NEAR(x[n], x2[n].real(), constants::EPSILON);
            }
        }
    }

    void InverseFourierTransform_ComplexSignal() const
    {
        for (size_t N = 1; N <= 100; ++N)
        {
            dsp::SignalGenerator<T> g{ 1000, N };
            dsp::Signal<std::complex<T>> x{ g.Cosine(100), g.Sine(100) };
            const auto X = dsp::FourierTransform(x);
            const auto x2 = dsp::InverseFourierTransform(X);

            ASSERT_EQ(x.size(), N);
            ASSERT_EQ(x2.size(), N);

            for (size_t m = 0; m < N; ++m)
            {
                EXPECT_NEAR(x[m].real(), x2[m].real(), constants::EPSILON);
                EXPECT_NEAR(x[m].imag(), x2[m].imag(), constants::EPSILON);
            }
        }
    }
};

using FourierTransformTestTypes = ::testing::Types<double, float>;

TYPED_TEST_SUITE(FourierTransformTest, FourierTransformTestTypes);

TYPED_TEST(FourierTransformTest, FourierTransform_RealSignal)
{
    this->FourierTransform_RealSignal();
}

TYPED_TEST(FourierTransformTest, FourierTransform_ComplexSignal)
{
    this->FourierTransform_ComplexSignal();
}

TYPED_TEST(FourierTransformTest, InverseFourierTransform_RealSignal)
{
    this->InverseFourierTransform_RealSignal();
}

TYPED_TEST(FourierTransformTest, InverseFourierTransform_ComplexSignal)
{
    this->InverseFourierTransform_ComplexSignal();
}

// Implementation of slow Fourier transform to compare against
template <typename T>
dsp::Signal<std::complex<T>> DFT(const dsp::Signal<T>& x)
{
    auto f_s = x.GetSampleRate();
    auto N = x.size();

    dsp::Signal<std::complex<T>> X{ f_s, N };

    // Take advantage of DFT symmetry when dealing with real input signals
    // Only the first N/2 + 1 outputs are unique
    for (size_t k = 0; k < N / 2 + 1; ++k)
    {
        for (size_t n = 0; n < N; ++n)
        {
            X[k] += x[n] * std::polar(static_cast<T>(1), -2 * static_cast<T>(M_PI) * n * k / N);
        }

        // X(N-k) = X(k)* for k = 1 -> N/2
        if (k != 0)
        {
            X[N - k] = std::conj(X[k]);
        }
    }

    return X;
}

// Implementation of slow fourier transform to compare against
template <typename T>
dsp::Signal<std::complex<T>> DFT(const dsp::Signal<std::complex<T>>& x)
{
    auto f_s = x.GetSampleRate();
    auto N = x.size();

    dsp::Signal<std::complex<T>> X{ f_s, N };

    for (size_t k = 0; k < N; ++k)
    {
        for (size_t n = 0; n < N; ++n)
        {
            X[k] += x[n] * std::polar(static_cast<T>(1), -2 * static_cast<T>(M_PI) * n * k / N);
        }
    }

    return X;
}