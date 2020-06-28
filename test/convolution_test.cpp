#include "constants.hpp"

#include <gtest/gtest.h>
#include <tnt/dsp/convolution.hpp>
#include <vector>

using namespace tnt;

using std::cos;
using std::sin;
using std::vector;

TEST(Convolution, Convolve)
{
    size_t N = 4;
    vector<double> a(N);
    vector<double> b(N);

    int f_s = 4000;
    double t_s = 1.0 / f_s;

    for (size_t n = 0; n < N; ++n)
    {
        a[n] = cos(2 * M_PI * 1000 * n * t_s);
        b[n] = sin(2 * M_PI * 1000 * n * t_s);
    }

    auto c = dsp::Convolve(a, b);

    ASSERT_EQ(c.size(), a.size());
    ASSERT_EQ(c.size(), b.size());

    EXPECT_NEAR(c[0], 0.0, constants::EPSILON);
    EXPECT_NEAR(c[1], 2.0, constants::EPSILON);
    EXPECT_NEAR(c[2], 0.0, constants::EPSILON);
    EXPECT_NEAR(c[3], -2.0, constants::EPSILON);
 }