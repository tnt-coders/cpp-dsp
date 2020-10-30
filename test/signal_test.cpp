#include "constants.hpp"
#include <algorithm>
#include <complex>
#include <gtest/gtest.h>
#include <tnt/dsp/signal.hpp>

using namespace tnt;

template <typename T>
class SignalTest : public ::testing::Test
{
protected:
    void Signal_ConstructRealSignal_SampleRate() const
    {
        dsp::Signal<T> x(1000);

        EXPECT_EQ(x.GetSampleRate(), 1000);
        EXPECT_EQ(x.size(), 0);
    }

    void Signal_ConstructComplexSignal_SampleRate() const
    {
        dsp::Signal<std::complex<T>> x(1000);

        EXPECT_EQ(x.GetSampleRate(), 1000);
        EXPECT_EQ(x.size(), 0);
    }

    void Signal_ConstructRealSignal_SampleRate_Size() const
    {
        dsp::Signal<T> x(1000, 10);

        EXPECT_EQ(x.GetSampleRate(), 1000);
        EXPECT_EQ(x.size(), 10);
    }

    void Signal_ConstructComplexSignal_SampleRate_Size() const
    {
        dsp::Signal<std::complex<T>> x(1000, 10);

        EXPECT_EQ(x.GetSampleRate(), 1000);
        EXPECT_EQ(x.size(), 10);
    }

    void Signal_ConstructRealSignal_Signal() const
    {
        const size_t f_s = 1000;
        const size_t N = 10;
        const auto t_s = T{ 1.0 } / f_s;
        dsp::Signal<T> x1(f_s, N);

        // Put some data in it
        for (size_t n = 0; n < N; ++n)
        {
            x1[n] = std::cos(2 * T{ M_PI } * 100 * n * t_s);
        }

        dsp::Signal<T> x2(x1);

        EXPECT_EQ(x2.GetSampleRate(), x1.GetSampleRate());
        EXPECT_EQ(x2.size(), x1.size());

        for (size_t n = 0; n < N; ++n)
        {
            EXPECT_NEAR(x2[n], x1[n], constants::EPSILON);
        }
    }

    void Signal_ConstructComplexSignal_Signal() const
    {
        const size_t f_s = 1000;
        const size_t N = 10;
        const auto t_s = T{ 1.0 } / f_s;
        dsp::Signal<std::complex<T>> x1(f_s, N);

        // Put some data in it
        for (size_t n = 0; n < N; ++n)
        {
            x1[n] = {
                std::cos(2 * T{ M_PI } * 100 * n * t_s),
                std::sin(2 * T{ M_PI } * 100 * n * t_s)
            };
        }

        dsp::Signal<std::complex<T>> x2(x1);

        EXPECT_EQ(x2.GetSampleRate(), x1.GetSampleRate());
        EXPECT_EQ(x2.size(), x1.size());

        for (size_t n = 0; n < N; ++n)
        {
            EXPECT_NEAR(x2[n].real(), x1[n].real(), constants::EPSILON);
            EXPECT_NEAR(x2[n].imag(), x1[n].imag(), constants::EPSILON);
        }
    }

    void Signal_ConstructComplexSignal_RealSignal_RealSignal() const
    {
        const size_t f_s = 1000;
        const size_t N = 10;
        const auto t_s = T{ 1.0 } / f_s;
        dsp::Signal<T> x1_real(f_s, N);
        dsp::Signal<T> x1_imag(f_s, N);

        // Put some data in it
        for (size_t n = 0; n < N; ++n)
        {
            x1_real[n] = std::cos(2 * T{ M_PI } * 100 * n * t_s);
            x1_imag[n] = std::sin(2 * T{ M_PI } * 100 * n * t_s);
        }

        dsp::Signal<std::complex<T>> x2(x1_real, x1_imag);

        EXPECT_EQ(x2.GetSampleRate(), x1_real.GetSampleRate());
        EXPECT_EQ(x2.GetSampleRate(), x1_imag.GetSampleRate());
        EXPECT_EQ(x2.size(), x1_real.size());
        EXPECT_EQ(x2.size(), x1_imag.size());


        for (size_t n = 0; n < N; ++n)
        {
            EXPECT_NEAR(x2[n].real(), x1_real[n], constants::EPSILON);
            EXPECT_NEAR(x2[n].imag(), x1_imag[n], constants::EPSILON);
        }
    }

    void Signal_GetDuration() const
    {
        dsp::Signal<T> x(1000, 2500);
        EXPECT_NEAR(x.GetDuration(), 2.5, constants::EPSILON);
    }

    void Signal_GetSampleRate() const
    {
        dsp::Signal<T> x(1000);
        EXPECT_EQ(x.GetSampleRate(), 1000);
    }

    void Signal_Iterators() const
    {
        const size_t f_s = 1000;
        const size_t N = 10;
        const auto t_s = T{ 1.0 } / f_s;
        dsp::Signal<T> x(f_s, N);

        // Put some data in it
        for (size_t n = 0; n < N; ++n)
        {
            x[n] = std::cos(2 * T{ M_PI } * 100 * n * t_s);
        }

        // non-const begin, end
        std::for_each(x.begin(), x.end(), [](auto& sample) {
            sample = 1;
            });

        // const begin, end
        std::for_each(x.begin(), x.end(), [](const auto& sample) {
            EXPECT_NEAR(sample, 1.0, constants::EPSILON);
            });

        // cbegin, cend
        std::for_each(x.cbegin(), x.cend(), [](const auto& sample) {
            EXPECT_NEAR(sample, 1.0, constants::EPSILON);
            });
    }

    void Signal_capacity_reserve() const
    {
        dsp::Signal<T> x(1000);
        x.reserve(100);
        EXPECT_EQ(x.capacity(), 100);

        // Reserving a smaller capacity does not drop previously reserved capacity
        x.reserve(10);
        EXPECT_EQ(x.capacity(), 100);
    }

    void Signal_size_resize() const
    {
        dsp::Signal<T> x(1000, 10);
        EXPECT_EQ(x.size(), 10);
        x.resize(100);
        EXPECT_EQ(x.size(), 100);

        // Shrinking the size does not reduce the capacity
        x.resize(10);
        EXPECT_EQ(x.size(), 10);
        EXPECT_EQ(x.capacity(), 100);
    }

    void Signal_push_back() const
    {
        dsp::Signal<T> x(1000);
        x.push_back(1.0);
        x.push_back(2.0);
        x.push_back(3.0);

        EXPECT_EQ(x.size(), 3);
        EXPECT_NEAR(x[0], 1.0, constants::EPSILON);
        EXPECT_NEAR(x[1], 2.0, constants::EPSILON);
        EXPECT_NEAR(x[2], 3.0, constants::EPSILON);
    }

    void Signal_DataAccessOperator() const
    {
        const size_t f_s = 1000;
        const size_t N = 10;
        const auto t_s = T{ 1.0 } / f_s;
        dsp::Signal<T> x1(f_s, N);

        // non-const data access
        for (size_t n = 0; n < N; ++n)
        {
            x1[n] = std::cos(2 * T{ M_PI } * 100 * n * t_s);
        }

        const dsp::Signal<T> x2(x1);

        // const data access
        for (size_t n = 0; n < N; ++n)
        {
            EXPECT_NEAR(x2[n], x1[n], constants::EPSILON);
        }
    }

    void Signal_swap() const
    {
        const size_t f_s = 1000;
        const size_t N = 10;
        const auto t_s = T{ 1.0 } / f_s;
        dsp::Signal<T> cosine(f_s, N);
        dsp::Signal<T> sine(f_s, N);

        for (size_t n = 0; n < N; ++n)
        {
            cosine[n] = std::cos(2 * T{ M_PI } * 100 * n * t_s);
            sine[n] = std::sin(2 * T{ M_PI } * 100 * n * t_s);
        }

        auto x1 = cosine;
        auto x2 = sine;

        swap(x1, x2);

        for (size_t n = 0; n < N; ++n)
        {
            EXPECT_NEAR(x1[n], sine[n], constants::EPSILON);
            EXPECT_NEAR(x2[n], cosine[n], constants::EPSILON);
        }
    }
};

using SignalTestTypes = ::testing::Types<double, float>;

TYPED_TEST_SUITE(SignalTest, SignalTestTypes);

TYPED_TEST(SignalTest, Signal_ConstructRealSignal_SampleRate)
{
    this->Signal_ConstructRealSignal_SampleRate();
}

TYPED_TEST(SignalTest, Signal_ConstructComplexSignal_SampleRate)
{
    this->Signal_ConstructComplexSignal_SampleRate();
}

TYPED_TEST(SignalTest, Signal_ConstructRealSignal_SampleRate_Size)
{
    this->Signal_ConstructRealSignal_SampleRate_Size();
}

TYPED_TEST(SignalTest, Signal_ConstructComplexSignal_SampleRate_Size)
{
    this->Signal_ConstructComplexSignal_SampleRate_Size();
}

TYPED_TEST(SignalTest, Signal_ConstructRealSignal_Signal)
{
    this->Signal_ConstructRealSignal_Signal();
}

TYPED_TEST(SignalTest, Signal_ConstructComplexSignal_Signal)
{
    this->Signal_ConstructComplexSignal_Signal();
}

TYPED_TEST(SignalTest, Signal_ConstructComplexSignal_RealSignal_RealSignal)
{
    this->Signal_ConstructComplexSignal_RealSignal_RealSignal();
}

TYPED_TEST(SignalTest, Signal_GetSampleRate)
{
    this->Signal_GetSampleRate();
}

TYPED_TEST(SignalTest, Signal_GetDuration)
{
    this->Signal_GetDuration();
}

TYPED_TEST(SignalTest, Signal_Iterators)
{
    this->Signal_Iterators();
}

TYPED_TEST(SignalTest, Signal_capacity_reserve)
{
    this->Signal_capacity_reserve();
}

TYPED_TEST(SignalTest, Signal_size_resize)
{
    this->Signal_size_resize();
}

TYPED_TEST(SignalTest, Signal_push_back)
{
    this->Signal_push_back();
}

TYPED_TEST(SignalTest, Signal_DataAccessOperator)
{
    this->Signal_DataAccessOperator();
}

TYPED_TEST(SignalTest, Signal_swap)
{
    this->Signal_swap();
}