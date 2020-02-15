#include "fft.hpp"
#include "math_helpers.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <complex>
#include <functional>
#include <vector>

namespace tnt::dsp
{

using std::array;
using std::complex;
using std::conj;
using std::cos;
using std::multiplies;
using std::polar;
using std::sin;
using std::transform;
using std::vector;

template <size_t N>
consteval array<complex<double>, N/2> CalculateTwiddleFactors()
{
    array<complex<double>, N/2> W;
    const auto omega = 2.0*M_PI/N;
    for (size_t n = 0; n < N/2; ++n)
    {
        W[n] = complex{cos(-omega * n), sin(-omega * n)};
    }

    return W;
}

constexpr auto Twiddle = CalculateTwiddleFactors<65536>();

static vector<complex<double>> BluesteinFFT(const vector<complex<double>>& x);
static vector<complex<double>> Convolve(const vector<complex<double>>& a, const vector<complex<double>>& b);
static vector<complex<double>> StockhamFFT(const vector<double>& x);
static vector<complex<double>> StockhamFFT(const vector<complex<double>>& x);
static vector<complex<double>> StockhamFFT(const vector<complex<double>>& x, const vector<complex<double>>& W);

// The FFT of a real input signal is much more complicated because this function
// takes advantage of symmetry in the output in order to drastically improve
// performance
vector<complex<double>> FFT(const vector<double>& x)
{
    const size_t N = x.size();

    // TODO: See if further optimization can be done for odd values of N
    // If N is odd, just perform the complex FFT
    if (!IsEven(N))
    {
        vector<complex<double>> x_p(N);
        transform(x.begin(), x.end(), x_p.begin(), [](const double value) {
            return complex<double>(value);
            });

        return FFT(x_p);
    }

    const size_t NOver2 = N / 2;

    // Taking advantage of symmetry the FFT of a real signal can be computed
    // using a single N/2-point complex FFT. Split the input signal into its
    // even and odd components and load the data into a single complex vector.
    vector<complex<double>> x_p(NOver2);
    for (size_t n = 0; n < NOver2; ++n)
    {
        const auto nTimes2 = n * 2;

        // x_p[n] = x[2n] + jx[2n + 1]
        x_p[n] = complex<double>(x[nTimes2], x[nTimes2 + 1]);
    }

    // Pre-calculate twiddle factors
    vector<complex<double>> W(NOver2);
    const auto omega = 2.0 * M_PI / N;
    for (size_t n = 0; n < NOver2; ++n)
    {
        W[n] = polar(1.0, -omega * n);
    }

    // TODO: Make even faster by pre-calculating twiddle factors and passing
    //       them to the Stockham and Bluestein algorithms
    // Perform the complex FFT
    auto X_p = IsPowerOf2(N)
        ? StockhamFFT(x_p)
        : BluesteinFFT(x_p);

    // The FFT is periodic so it is valid append X_p[0] to the end. This is
    // required to avoid a buffer overflow in the next section.
    X_p.push_back(X_p[0]);

    // Extract the real FFT from the output of the complex FFT
    vector<complex<double>> X(N);
    for (size_t k = 0; k < NOver2; ++k)
    {
        const auto NOver2MinusK = NOver2 - k;

        // Extract the FFT of the even components
        const auto A = complex<double>(
            (X_p[k].real() + X_p[NOver2MinusK].real()) / 2,
            (X_p[k].imag() - X_p[NOver2MinusK].imag()) / 2);

        // Extract the FFT of the odd components
        const auto B = complex<double>(
            (X_p[NOver2MinusK].imag() + X_p[k].imag()) / 2,
            (X_p[NOver2MinusK].real() - X_p[k].real()) / 2);

        const auto tmp1 = A;
        const auto tmp2 = W[k] * B;

        // Sum the results
        X[k] = tmp1 + tmp2;
        X[k + NOver2] = tmp1 - tmp2; // (>*.*)> symmetry! <(*.*<)
    }

    return X;
}

// Performs the FFT of a complex input signal
vector<complex<double>> FFT(const vector<complex<double>>& x)
{
    const size_t N = x.size();

    return IsPowerOf2(N)
        ? StockhamFFT(x)
        : BluesteinFFT(x);
}

// TODO: Investigate whether there is a more optimized implement the IFFT when
//       the result is a real signal
// Performs the inverse FFT of a complex input signal
vector<complex<double>> IFFT(const vector<complex<double>>& X)
{
    const size_t N = X.size();

    vector<complex<double>> X_p(N);
    transform(X.begin(), X.end(), X_p.begin(), [](const complex<double>& value) {
        return conj(value);
        });

    const auto x_p = FFT(X_p);

    vector<complex<double>> x(N);
    transform(x_p.begin(), x_p.end(), x.begin(), [=](const complex<double>& value) {
        return conj(value) / static_cast<double>(N);
        });

    return x;
}

// C++ implementation of the Bluestein FFT algorithm
static vector<complex<double>> BluesteinFFT(const vector<complex<double>>& x)
{
    const size_t N = x.size();

    // To avoid issues with convolution periodicity, the convolution size must
    // be at least 2 * N - 1
    const size_t M = NextPowerOf2(2 * N - 1);

    // Calculate the "phase factors"
    vector<complex<double>> P(N);
    for (size_t n = 0; n < N; ++n)
    {
        // % (2 * N) is done to improve accuracy of floating point trigonometry
        P[n] = polar(1.0, -M_PI * ((n * n) % (2 * N)) / N);
    }

    // Construct the two sequences to perform convolution
    vector<complex<double>> a(M);
    vector<complex<double>> b(M);
    a[0] = x[0] * P[0];
    b[0] = P[0];
    for (size_t n = 1; n < N; ++n)
    {
        a[n] = x[n] * P[n];
        b[n] = b[M - n] = conj(P[n]); // (>*.*)> symmetry! <(*.*<)
    }

    const auto c = Convolve(a, b);

    // Mutiply by the "phase factors" to obtain the correct results
    vector<complex<double>> X(N);
    for (size_t k = 0; k < N; ++k)
    {
        X[k] = c[k] * P[k];
    }

    return X;
}

// Note that this is specialized for performing the Bluestein FFT efficiently.
// This is not a general purpose convolution algorithm.
static vector<complex<double>> Convolve(const vector<complex<double>>& a, const vector<complex<double>>& b)
{
    assert(IsPowerOf2(a.size()));
    assert(a.size() == b.size());
    const size_t N = a.size();
    const size_t NOver2 = N / 2;

    // Pre-calculate the twiddle factors
    vector<complex<double>> W(NOver2);
    const auto omega = 2.0 * M_PI / N;
    for (size_t n = 0; n < NOver2; ++n)
    {
        W[n] = polar(1.0, -omega * n);
    }

    const auto A = StockhamFFT(a, W);
    const auto B = StockhamFFT(b, W);

    // The convolution theorem states that multiplication in the frequency
    // domain is equivalent to convolution in the time domain
    vector<complex<double>> C(N);
    transform(A.begin(), A.end(), B.begin(), C.begin(), multiplies<complex<double>>());

    return IFFT(C);
}

// C++ implementation of the Stockam FFT algorithm
static vector<complex<double>> StockhamFFT(const vector<complex<double>>& x)
{
    assert(IsPowerOf2(x.size()));
    const size_t N = x.size();
    const size_t NOver2 = N / 2;

    // Pre-calculate twiddle factors
    vector<complex<double>> W(NOver2);
    const auto omega = 2.0 * M_PI / N;
    for (size_t n = 0; n < NOver2; ++n)
    {
        W[n] = polar(1.0, -omega * n);
    }

    return StockhamFFT(x, W);
}

// C++ implementation of the Stockam FFT algorithm
static vector<complex<double>> StockhamFFT(const vector<complex<double>>& x, const vector<complex<double>>& W)
{
    assert(IsPowerOf2(x.size()));
    const size_t N = x.size();
    const size_t NOver2 = N / 2;

    // The Stockham algorithm requires one vector for input/output data and
    // another as a temporary workspace
    vector<complex<double>> a(x);
    vector<complex<double>> b(N);

    // Set the spacing between twiddle factors used at the first stage
    size_t WStride = NOver2;

    // Loop through each stage of the FFT
    for (size_t stride = 1; stride < N; stride *= 2)
    {
        // Loop through the individual FFTs of each stage
        for (size_t k = 0; k < NOver2; k += stride)
        {
            const auto kTimes2 = k * 2;

            // Perform each individual FFT
            for (size_t n = 0; n < stride; ++n)
            {
                // Calculate the input indexes
                const auto aIndex1 = n + k;
                const auto aIndex2 = aIndex1 + NOver2;

                // Calculate the output indexes
                const auto bIndex1 = n + kTimes2;
                const auto bIndex2 = bIndex1 + stride;

                // Perform the FFT
                const auto tmp1 = a[aIndex1];
                const auto tmp2 = W[n * WStride] * a[aIndex2];

                // Sum the results
                b[bIndex1] = tmp1 + tmp2;
                b[bIndex2] = tmp1 - tmp2; // (>*.*)> symmetry! <(*.*<)
            }
        }

        // Spacing between twiddle factors is half for the next stage
        WStride /= 2;

        // Swap the data (output of this stage is input of the next)
        a.swap(b);
    }

    return a;
}

} /* namespace tnt::dsp */