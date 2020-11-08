#include "constants.hpp"
#include <complex>
#include <gtest/gtest.h>
#include <tnt/dsp/multi_signal.hpp>
#include <tnt/dsp/signal.hpp>
#include <tnt/dsp/signal_generator.hpp>

using namespace tnt;

template <typename T>
class MultiSignalTest : public ::testing::Test
{
protected:
    void MultiSignal_ConstructRealSignal_SampleRate() const
    {
        dsp::MultiSignal<T> x{ 1000 };
        EXPECT_EQ(x.GetSampleRate(), 1000);
        EXPECT_EQ(x.channels(), 0);
        EXPECT_EQ(x.size(), 0);
    }

    void MultiSignal_ConstructComplexSignal_SampleRate() const
    {
        dsp::MultiSignal<std::complex<T>> x{ 1000 };
        EXPECT_EQ(x.GetSampleRate(), 1000);
        EXPECT_EQ(x.channels(), 0);
        EXPECT_EQ(x.size(), 0);
    }

    void MultiSignal_ConstructRealSignal_SampleRate_Size() const
    {
        dsp::MultiSignal<T> x{ 1000, 10 };
        EXPECT_EQ(x.GetSampleRate(), 1000);
        EXPECT_EQ(x.channels(), 0);
        EXPECT_EQ(x.size(), 10);
    }

    void MultiSignal_ConstructComplexSignal_SampleRate_Size() const
    {
        dsp::MultiSignal<std::complex<T>> x{ 1000, 10 };
        EXPECT_EQ(x.GetSampleRate(), 1000);
        EXPECT_EQ(x.channels(), 0);
        EXPECT_EQ(x.size(), 10);
    }

    void MultiSignal_ConstructRealSignal_SampleRate_Size_Channels() const
    {
        dsp::MultiSignal<T> x{ 1000, 10, 2 };
        EXPECT_EQ(x.GetSampleRate(), 1000);
        EXPECT_EQ(x.channels(), 2);
        EXPECT_EQ(x.size(), 10);
    }

    void MultiSignal_ConstructComplexSignal_SampleRate_Size_Channels() const
    {
        dsp::MultiSignal<std::complex<T>> x{ 1000, 10, 2 };
        EXPECT_EQ(x.GetSampleRate(), 1000);
        EXPECT_EQ(x.channels(), 2);
        EXPECT_EQ(x.size(), 10);
    }

    void MultiSignal_ConstructRealSignal_Signals() const
    {
        const size_t f_s = 1000;
        const size_t N = 10;

        dsp::SignalGenerator<T> g{ f_s, N };
        dsp::MultiSignal<T> x{
            g.Cosine(100),
            g.Sine(100),
        };

        EXPECT_EQ(x.channels(), 2);
        EXPECT_EQ(x.GetSampleRate(), f_s);
        EXPECT_EQ(x.size(), N);
    }

    void MultiSignal_ConstructComplexSignal_Signals() const
    {
        const size_t f_s = 1000;
        const size_t N = 10;

        dsp::SignalGenerator<T> g{ f_s, N };
        dsp::MultiSignal<std::complex<T>> x{
            { g.Cosine(1000), g.Sine(100) },
            { g.Sine(1000), g.Cosine(100) },
        };

        EXPECT_EQ(x.channels(), 2);
        EXPECT_EQ(x.GetSampleRate(), f_s);
        EXPECT_EQ(x.size(), N);
    }

    void MultiSignal_CopyConstructor() const
    {
        const size_t f_s = 1000;
        const size_t N = 10;

        dsp::SignalGenerator<T> g{ f_s, N };
        dsp::MultiSignal<T> x1{
            g.Cosine(100),
            g.Sine(100),
        };

        const dsp::MultiSignal<T> x2{ x1 };

        EXPECT_EQ(x2.channels(), x1.channels());
        EXPECT_EQ(x2.GetSampleRate(), x1.GetSampleRate());
        EXPECT_EQ(x2.size(), x1.size());

        for (size_t c = 0; c < x1.channels(); ++c)
        {
            for (size_t n = 0; n < N; ++n)
            {
                EXPECT_NEAR(x2[c][n], x1[c][n], constants::EPSILON);
            }
        }
    }

    void MultiSignal_MoveConstructor() const
    {
        const size_t f_s = 1000;
        const size_t N = 10;

        dsp::SignalGenerator<T> g{ f_s, N };
        dsp::Signal<T> c0 = g.Cosine(100);
        dsp::Signal<T> c1 = g.Sine(100);
        dsp::MultiSignal<T> x1{
            c0,
            c1,
        };

        const dsp::MultiSignal<T> x2{ std::move(x1) };

        EXPECT_EQ(x2.channels(), 2);
        EXPECT_EQ(x2.GetSampleRate(), f_s);
        EXPECT_EQ(x2.size(), N);

        for (size_t n = 0; n < N; ++n)
        {
            EXPECT_NEAR(x2[0][n], c0[n], constants::EPSILON);
            EXPECT_NEAR(x2[1][n], c1[n], constants::EPSILON);
        }
    }

    void MultiSignal_CopyAssignmentOperator() const
    {
        const size_t f_s = 1000;
        const size_t N = 10;

        dsp::SignalGenerator<T> g{ f_s, N };
        dsp::MultiSignal<T> x1{
            g.Cosine(100),
            g.Sine(100),
        };

        const auto x2 = x1;

        EXPECT_EQ(x2.channels(), x1.channels());
        EXPECT_EQ(x2.GetSampleRate(), x1.GetSampleRate());
        EXPECT_EQ(x2.size(), x1.size());

        std::cout << x2[0][0] << std::endl;

        for (size_t c = 0; c < x1.channels(); ++c)
        {
            for (size_t n = 0; n < N; ++n)
            {
                EXPECT_NEAR(x2[c][n], x1[c][n], constants::EPSILON);
            }
        }
    }

    void MultiSignal_MoveAssignmentOperator() const
    {
        const size_t f_s = 1000;
        const size_t N = 10;

        dsp::SignalGenerator<T> g{ f_s, N };
        dsp::Signal<T> c0 = g.Cosine(100);
        dsp::Signal<T> c1 = g.Sine(100);
        dsp::MultiSignal<T> x1{
            c0,
            c1,
        };

        const auto x2 = std::move(x1);

        EXPECT_EQ(x2.channels(), 2);
        EXPECT_EQ(x2.GetSampleRate(), f_s);
        EXPECT_EQ(x2.size(), N);

        for (size_t n = 0; n < N; ++n)
        {
            EXPECT_NEAR(x2[0][n], c0[n], constants::EPSILON);
            EXPECT_NEAR(x2[1][n], c1[n], constants::EPSILON);
        }
    }

    void MultiSignal_GetChannel() const
    {
        const size_t f_s = 1000;
        const size_t N = 10;

        dsp::SignalGenerator<T> g{ f_s, N };
        dsp::MultiSignal<T> x{
            g.Cosine(100),
            g.Sine(100),
        };

        const auto c0 = x.GetChannel(0);
        const auto c1 = x.GetChannel(1);

        for (size_t n = 0; n < N; ++n)
        {
            EXPECT_NEAR(c0[n], x[0][n], constants::EPSILON);
            EXPECT_NEAR(c1[n], x[1][n], constants::EPSILON);
        }
    }

    void MultiSignal_GetDuration() const
    {
        dsp::MultiSignal<T> x{ 1000, 2500 };
        EXPECT_NEAR(x.GetDuration(), 2.5, constants::EPSILON);
    }

    void MultiSignal_GetSampleRate() const
    {
        dsp::MultiSignal<T> x{ 1000 };
        EXPECT_EQ(x.GetSampleRate(), 1000);
    }

    void MultiSignal_AddChannel() const
    {
        const size_t f_s = 1000;
        const size_t N = 10;

        dsp::SignalGenerator<T> g{ f_s, N };
        dsp::MultiSignal<T> x{ f_s };
        x.AddChannel(g.Cosine(100));

        const auto data = g.Cosine(100);

        for (size_t n = 0; n < N; ++n)
        {
            EXPECT_NEAR(x[0][n], data[n], constants::EPSILON);
        }
    }

    void MultiSignal_Iterators() const
    {
        const size_t f_s = 1000;
        const size_t N = 10;

        dsp::SignalGenerator<T> g{ f_s, N };
        dsp::MultiSignal<T> x{
            g.Cosine(100),
            g.Sine(100)
        };

        // non-const begin, end
        std::for_each(x.begin(), x.end(), [](auto& channel) {
            std::for_each(channel.begin(), channel.end(), [](auto& sample) {
                sample = 1;
                });
            });

        // const begin, end
        std::for_each(x.begin(), x.end(), [](const auto& channel) {
            std::for_each(channel.begin(), channel.end(), [](const auto& sample) {
                EXPECT_NEAR(sample, 1.0, constants::EPSILON);
                });
            });

        // cbegin, cend
        std::for_each(x.cbegin(), x.cend(), [](const auto& channel) {
            std::for_each(channel.begin(), channel.end(), [](const auto& sample) {
                EXPECT_NEAR(sample, 1.0, constants::EPSILON);
                });
            });
    }
};

using MultiSignalTestTypes = ::testing::Types<double, float>;

TYPED_TEST_SUITE(MultiSignalTest, MultiSignalTestTypes);

TYPED_TEST(MultiSignalTest, MultiSignal_ConstructRealSignal_SampleRate)
{
    this->MultiSignal_ConstructRealSignal_SampleRate();
}

TYPED_TEST(MultiSignalTest, MultiSignal_ConstructComplexSignal_SampleRate)
{
    this->MultiSignal_ConstructComplexSignal_SampleRate();
}

TYPED_TEST(MultiSignalTest, MultiSignal_ConstructRealSignal_SampleRate_Size)
{
    this->MultiSignal_ConstructRealSignal_SampleRate_Size();
}

TYPED_TEST(MultiSignalTest, MultiSignal_ConstructComplexSignal_SampleRate_Size)
{
    this->MultiSignal_ConstructComplexSignal_SampleRate_Size();
}

TYPED_TEST(MultiSignalTest, MultiSignal_ConstructRealSignal_SampleRate_Size_Channels)
{
    this->MultiSignal_ConstructRealSignal_SampleRate_Size_Channels();
}

TYPED_TEST(MultiSignalTest, MultiSignal_ConstructComplexSignal_SampleRate_Size_Channels)
{
    this->MultiSignal_ConstructComplexSignal_SampleRate_Size_Channels();
}

TYPED_TEST(MultiSignalTest, MultiSignal_ConstructRealSignal_Signals)
{
    this->MultiSignal_ConstructRealSignal_Signals();
}

TYPED_TEST(MultiSignalTest, MultiSignal_ConstructComplexSignal_Signals)
{
    this->MultiSignal_ConstructComplexSignal_Signals();
}

TYPED_TEST(MultiSignalTest, MultiSignal_CopyConstructor)
{
    this->MultiSignal_CopyConstructor();
}

TYPED_TEST(MultiSignalTest, MultiSignal_MoveConstructor)
{
    this->MultiSignal_MoveConstructor();
}

TYPED_TEST(MultiSignalTest, MultiSignal_CopyAssignmentOperator)
{
    this->MultiSignal_CopyAssignmentOperator();
}

TYPED_TEST(MultiSignalTest, MultiSignal_MoveAssignmentOperator)
{
    this->MultiSignal_MoveAssignmentOperator();
}

TYPED_TEST(MultiSignalTest, MultiSignal_GetChannel)
{
    this->MultiSignal_GetChannel();
}

TYPED_TEST(MultiSignalTest, MultiSignal_GetDuration)
{
    this->MultiSignal_GetDuration();
}

TYPED_TEST(MultiSignalTest, MultiSignal_GetSampleRate)
{
    this->MultiSignal_GetSampleRate();
}

TYPED_TEST(MultiSignalTest, MultiSignal_AddChannel)
{
    this->MultiSignal_AddChannel();
}

TYPED_TEST(MultiSignalTest, MultiSignal_Iterators)
{
    this->MultiSignal_Iterators();
}

//TYPED_TEST(MultiSignalTest, MultiSignal_channels)
//{
//    this->MultiSignal_channels();
//}
//
//TYPED_TEST(MultiSignalTest, MultiSignal_size)
//{
//    this->MultiSignal_size();
//}
//
//TYPED_TEST(MultiSignalTest, MultiSignal_DataAccessOperator)
//{
//    this->MultiSignal_DataAccessOperator();
//}
//
//TYPED_TEST(MultiSignalTest, MultiSignal_swap)
//{
//    this->MultiSignal_swap();
//}