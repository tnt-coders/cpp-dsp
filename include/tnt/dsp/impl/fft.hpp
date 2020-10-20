#pragma once

#include "impl/math_helpers.hpp"
#include "signal.hpp"
#include <algorithm>
#include <cassert>
#include <complex>

namespace tnt::dsp::impl
{

// C++ implementation of the Bluestein FFT algorithm
template <typename T>
static Signal<std::complex<T>> BluesteinFFT(const Signal<std::complex<T>>& x)
{
    const auto f_s = x.GetSampleRate();
    const auto N = x.size();

    // To avoid issues with convolution periodicity, the convolution size must
    // be at least 2*N - 1
    const auto M = impl::NextPowerOf2(2 * N - 1);

    // Calculate the "phase factors"
    Signal<std::complex<T>> P(f_s, N);
    for (auto n = 0; n < N; ++n)
    {
        // %(2*N) is done to improve accuracy of floating point trigonometry
        P[n] = std::polar(1.0, -M_PI * ((n * n) % (2 * N)) / N);
    }

    // Construct the two sequences to perform convolution
    Signal<std::complex<T>> a(f_s, M);
    Signal<std::complex<T>> b(f_s, M);
    a[0] = x[0] * P[0];
    b[0] = P[0];
    for (auto n = 1; n < N; ++n)
    {
        a[n] = x[n] * P[n];
        b[n] = b[M - n] = std::conj(P[n]); // (>*.*)> symmetry! <(*.*<)
    }

    const auto c = impl::Convolve(a, b);

    // Mutiply by the "phase factors" to obtain the correct results
    Signal<std::complex<T>> X(f_s, N);
    for (auto m = 0; m < N; ++m)
    {
        X[m] = c[m] * P[m];
    }

    return X;
}

// Note that this is specialized for performing the Bluestein FFT efficiently.
// This is not a general purpose convolution algorithm.
template <typename T>
static Signal<std::complex<T>> Convolve(const Signal<std::complex<T>>& a, const Signal<std::complex<T>>& b)
{
    assert(a.GetSampleRate() == b.GetSampleRate());
    const auto f_s = a.GetSampleRate();
    assert(a.size() == b.size());
    const auto N = a.size();
    assert(IsPowerOf2(N));
    const auto NOver2 = N / 2;

    // Pre-calculate the twiddle factors
    Signal<std::complex<T>> W(f_s, NOver2);
    const auto omega = 2.0 * M_PI / N;
    for (auto n = 0; n < NOver2; ++n)
    {
        W[n] = { std::cos(-omega * n), std::sin(-omega * n) };
    }

    // Because we know N is a power of 2 we can explicitly use the Stockham FFT algorithm
    const auto A = impl::StockhamFFT(a, W);
    const auto B = impl::StockhamFFT(b, W);

    // The convolution theorem states that multiplication in the frequency
    // domain is equivalent to convolution in the time domain
    Signal<std::complex<T>> C(f_s, N);
    std::transform(A.begin(), A.end(), B.begin(), C.begin(), std::multiplies<std::complex<T>>());

    return IFFT(C);
}

// C++ implementation of the Stockam FFT algorithm
template <typename T>
static Signal<std::complex<T>> StockhamFFT(const Signal<std::complex<T>>& x)
{
    const auto f_s = x.GetSampleRate();
    const auto N = x.size();
    assert(impl::IsPowerOf2(N));
    const auto NOver2 = N / 2;

    // Pre-calculate the twiddle factors
    Signal<std::complex<T>> W(f_s, NOver2);
    const auto omega = 2.0 * M_PI / N;
    for (auto n = 0; n < NOver2; ++n)
    {
        W[n] = { std::cos(-omega * n), std::sin(-omega * n) };
    }

    return impl::StockhamFFT(x, W);
}

// C++ implementation of the Stockam FFT algorithm
template <typename T>
static Signal<std::complex<T>> StockhamFFT(const Signal<std::complex<T>>& x, const Signal<std::complex<T>>& W)
{
    const auto f_s = x.GetSampleRate();
    const auto N = x.size();
    assert(impl::IsPowerOf2(N));
    const auto NOver2 = N / 2;

    // The Stockham algorithm requires one vector for input/output data and
    // another as a temporary workspace
    Signal<std::complex<T>> a(x);
    Signal<std::complex<T>> b(f_s, N);

    // Set the spacing between twiddle factors used at the first stage
    auto WStride = N / 2;

    // Loop through each stage of the FFT
    for (auto stride = 1; stride < N; stride *= 2)
    {
        // Loop through the individual FFTs of each stage
        for (auto m = 0; m < NOver2; m += stride)
        {
            const auto mTimes2 = m * 2;

            // Perform each individual FFT
            for (auto n = 0; n < stride; ++n)
            {
                // Calculate the input indexes
                const auto aIndex1 = n + m;
                const auto aIndex2 = aIndex1 + NOver2;

                // Calculate the output indexes
                const auto bIndex1 = n + mTimes2;
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
        swap(a, b);
    }

    return a;
}

} /* namespace tnt::dsp::impl */