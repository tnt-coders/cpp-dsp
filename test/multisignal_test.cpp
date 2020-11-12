#include "constants.hpp"
#include <complex>
#include <gtest/gtest.h>
#include <tnt/dsp/multisignal.hpp>
#include <tnt/dsp/signal.hpp>
#include <tnt/dsp/signal_generator.hpp>
#include <utility>

using namespace tnt;

template <typename T>
class MultisignalTest : public ::testing::Test
{
protected:
    void Multisignal_ConstructRealSignal_SampleRate() const
    {
        dsp::Multisignal<T> x{ 1000 };
        EXPECT_EQ(x.GetSampleRate(), 1000);
        EXPECT_EQ(x.channels(), 0);
        EXPECT_EQ(x.size(), 0);
    }

    void Multisignal_ConstructComplexSignal_SampleRate() const
    {
        dsp::Multisignal<std::complex<T>> x{ 1000 };
        EXPECT_EQ(x.GetSampleRate(), 1000);
        EXPECT_EQ(x.channels(), 0);
        EXPECT_EQ(x.size(), 0);
    }

    void Multisignal_ConstructRealSignal_SampleRate_Size() const
    {
        dsp::Multisignal<T> x{ 1000, 10 };
        EXPECT_EQ(x.GetSampleRate(), 1000);
        EXPECT_EQ(x.channels(), 0);
        EXPECT_EQ(x.size(), 10);
    }

    void Multisignal_ConstructComplexSignal_SampleRate_Size() const
    {
        dsp::Multisignal<std::complex<T>> x{ 1000, 10 };
        EXPECT_EQ(x.GetSampleRate(), 1000);
        EXPECT_EQ(x.channels(), 0);
        EXPECT_EQ(x.size(), 10);
    }

    void Multisignal_ConstructRealSignal_SampleRate_Size_Channels() const
    {
        dsp::Multisignal<T> x{ 1000, 10, 2 };
        EXPECT_EQ(x.GetSampleRate(), 1000);
        EXPECT_EQ(x.channels(), 2);
        EXPECT_EQ(x.size(), 10);
    }

    void Multisignal_ConstructComplexSignal_SampleRate_Size_Channels() const
    {
        dsp::Multisignal<std::complex<T>> x{ 1000, 10, 2 };
        EXPECT_EQ(x.GetSampleRate(), 1000);
        EXPECT_EQ(x.channels(), 2);
        EXPECT_EQ(x.size(), 10);
    }

    void Multisignal_ConstructRealSignal_Signals() const
    {
        const size_t f_s = 1000;
        const size_t N = 10;

        dsp::SignalGenerator<T> g{ f_s, N };
        dsp::Multisignal<T> x{
            g.Cosine(100),
            g.Sine(100),
        };

        EXPECT_EQ(x.channels(), 2);
        EXPECT_EQ(x.GetSampleRate(), f_s);
        EXPECT_EQ(x.size(), N);
    }

    void Multisignal_ConstructComplexSignal_Signals() const
    {
        const size_t f_s = 1000;
        const size_t N = 10;

        dsp::SignalGenerator<T> g{ f_s, N };
        dsp::Multisignal<std::complex<T>> x{
            { g.Cosine(1000), g.Sine(100) },
            { g.Sine(1000), g.Cosine(100) },
        };

        EXPECT_EQ(x.channels(), 2);
        EXPECT_EQ(x.GetSampleRate(), f_s);
        EXPECT_EQ(x.size(), N);
    }

    void Multisignal_CopyConstructor() const
    {
        const size_t f_s = 1000;
        const size_t N = 10;

        dsp::SignalGenerator<T> g{ f_s, N };
        dsp::Multisignal<T> x1{
            g.Cosine(100),
            g.Sine(100),
        };

        const dsp::Multisignal<T> x2{ x1 };

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

    void Multisignal_MoveConstructor() const
    {
        const size_t f_s = 1000;
        const size_t N = 10;

        dsp::SignalGenerator<T> g{ f_s, N };
        dsp::Signal<T> c0 = g.Cosine(100);
        dsp::Signal<T> c1 = g.Sine(100);
        dsp::Multisignal<T> x1{
            c0,
            c1,
        };

        const dsp::Multisignal<T> x2{ std::move(x1) };

        EXPECT_EQ(x2.channels(), 2);
        EXPECT_EQ(x2.GetSampleRate(), f_s);
        EXPECT_EQ(x2.size(), N);

        for (size_t n = 0; n < N; ++n)
        {
            EXPECT_NEAR(x2[0][n], c0[n], constants::EPSILON);
            EXPECT_NEAR(x2[1][n], c1[n], constants::EPSILON);
        }
    }

    void Multisignal_CopyAssignmentOperator() const
    {
        const size_t f_s = 1000;
        const size_t N = 10;

        dsp::SignalGenerator<T> g{ f_s, N };
        dsp::Multisignal<T> x1{
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

    void Multisignal_MoveAssignmentOperator() const
    {
        const size_t f_s = 1000;
        const size_t N = 10;

        dsp::SignalGenerator<T> g{ f_s, N };
        dsp::Signal<T> c0 = g.Cosine(100);
        dsp::Signal<T> c1 = g.Sine(100);
        dsp::Multisignal<T> x1{
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

    void Multisignal_GetDuration() const
    {
        dsp::Multisignal<T> x{ 1000, 2500 };
        EXPECT_NEAR(x.GetDuration(), 2.5, constants::EPSILON);
    }

    void Multisignal_GetSampleRate() const
    {
        dsp::Multisignal<T> x{ 1000 };
        EXPECT_EQ(x.GetSampleRate(), 1000);
    }

    void Multisignal_AddChannel() const
    {
        const size_t f_s = 1000;
        const size_t N = 10;

        dsp::SignalGenerator<T> g{ f_s, N };
        dsp::Multisignal<T> x{ f_s };
        x.AddChannel(g.Cosine(100));

        const auto data = g.Cosine(100);

        for (size_t n = 0; n < N; ++n)
        {
            EXPECT_NEAR(x[0][n], data[n], constants::EPSILON);
        }
    }

    void Multisignal_channels() const
    {
        dsp::Multisignal<T> x{ 1000, 10, 2 };
        EXPECT_EQ(x.channels(), 2);
    }

    void Multisignal_size() const
    {
        dsp::Multisignal<T> x{ 1000, 10 };
        EXPECT_EQ(x.size(), 10);
    }

    void Multisignal_DataAccessOperator() const
    {
        const size_t f_s = 1000;
        const size_t N = 10;

        dsp::SignalGenerator<T> g{ f_s, N };
        dsp::Signal<T> c0 = g.Cosine(100);
        dsp::Signal<T> c1 = g.Sine(100);
        dsp::Multisignal<T> x1{
            c0,
            c1,
        };

        for (size_t n = 0; n < N; ++n)
        {
            EXPECT_NEAR(x1[0][n], c0[n], constants::EPSILON);
            EXPECT_NEAR(x1[1][n], c1[n], constants::EPSILON);
        }

        // non-const data access
        for (size_t n = 0; n < N; ++n)
        {
            x1[0][n] = c1[n];
            x1[1][n] = c0[n];
        }

        const dsp::Multisignal<T> x2{ x1 };

        // const data access
        for (size_t n = 0; n < N; ++n)
        {
            EXPECT_NEAR(x2[0][n], c1[n], constants::EPSILON);
            EXPECT_NEAR(x2[1][n], c0[n], constants::EPSILON);
        }
    }

    void Multisignal_swap() const
    {
        const size_t f_s = 1000;
        const size_t N = 10;

        dsp::SignalGenerator<T> g{ f_s, N };
        dsp::Multisignal<T> x1{
            g.Cosine(100),
            g.Sine(100),
        };
        dsp::Multisignal<T> x2{
            g.Sine(100),
            g.Cosine(100),
        };
        const auto x1_c = x1;
        const auto x2_c = x2;

        std::swap(x1, x2);

        for (size_t c = 0; c < 2; ++c)
        {
            for (size_t n = 0; n < N; ++n)
            {
                EXPECT_NEAR(x1[c][n], x2_c[c][n], constants::EPSILON);
                EXPECT_NEAR(x2[c][n], x1_c[c][n], constants::EPSILON);
            }
        }
    }
};

using MultisignalTestTypes = ::testing::Types<double, float>;

TYPED_TEST_SUITE(MultisignalTest, MultisignalTestTypes);

TYPED_TEST(MultisignalTest, Multisignal_ConstructRealSignal_SampleRate)
{
    this->Multisignal_ConstructRealSignal_SampleRate();
}

TYPED_TEST(MultisignalTest, Multisignal_ConstructComplexSignal_SampleRate)
{
    this->Multisignal_ConstructComplexSignal_SampleRate();
}

TYPED_TEST(MultisignalTest, Multisignal_ConstructRealSignal_SampleRate_Size)
{
    this->Multisignal_ConstructRealSignal_SampleRate_Size();
}

TYPED_TEST(MultisignalTest, Multisignal_ConstructComplexSignal_SampleRate_Size)
{
    this->Multisignal_ConstructComplexSignal_SampleRate_Size();
}

TYPED_TEST(MultisignalTest, Multisignal_ConstructRealSignal_SampleRate_Size_Channels)
{
    this->Multisignal_ConstructRealSignal_SampleRate_Size_Channels();
}

TYPED_TEST(MultisignalTest, Multisignal_ConstructComplexSignal_SampleRate_Size_Channels)
{
    this->Multisignal_ConstructComplexSignal_SampleRate_Size_Channels();
}

TYPED_TEST(MultisignalTest, Multisignal_ConstructRealSignal_Signals)
{
    this->Multisignal_ConstructRealSignal_Signals();
}

TYPED_TEST(MultisignalTest, Multisignal_ConstructComplexSignal_Signals)
{
    this->Multisignal_ConstructComplexSignal_Signals();
}

TYPED_TEST(MultisignalTest, Multisignal_CopyConstructor)
{
    this->Multisignal_CopyConstructor();
}

TYPED_TEST(MultisignalTest, Multisignal_MoveConstructor)
{
    this->Multisignal_MoveConstructor();
}

TYPED_TEST(MultisignalTest, Multisignal_CopyAssignmentOperator)
{
    this->Multisignal_CopyAssignmentOperator();
}

TYPED_TEST(MultisignalTest, Multisignal_MoveAssignmentOperator)
{
    this->Multisignal_MoveAssignmentOperator();
}

TYPED_TEST(MultisignalTest, Multisignal_GetDuration)
{
    this->Multisignal_GetDuration();
}

TYPED_TEST(MultisignalTest, Multisignal_GetSampleRate)
{
    this->Multisignal_GetSampleRate();
}

TYPED_TEST(MultisignalTest, Multisignal_AddChannel)
{
    this->Multisignal_AddChannel();
}

TYPED_TEST(MultisignalTest, Multisignal_channels)
{
    this->Multisignal_channels();
}

TYPED_TEST(MultisignalTest, Multisignal_size)
{
    this->Multisignal_size();
}

TYPED_TEST(MultisignalTest, Multisignal_DataAccessOperator)
{
    this->Multisignal_DataAccessOperator();
}

TYPED_TEST(MultisignalTest, Multisignal_swap)
{
    this->Multisignal_swap();
}