#pragma once

#include "impl/fourier_transform.hpp"
#include "impl/math_helpers.hpp"
#include "signal.hpp"
#include <algorithm>
#include <cmath>
#include <complex>
#include <utility>

namespace tnt::dsp
{



/*!
\brief Calculates the fast Fourier transform of a real signal
\param[in] x - Real input signal
\return Signal representing the FFT of the input data
*/
template <typename T>
Signal<std::complex<T>> FourierTransform(const Signal<T>& x)
{
    const auto f_s = x.GetSampleRate();
    const auto N = x.size();

    // TODO: See if further optimization can be done for odd values of N
    // If N is odd, just perform the complex FFT
    if (!impl::IsEven(N))
    {
        Signal<std::complex<T>> x_p{ f_s, N };
        std::transform(x.begin(), x.end(), x_p.begin(), [](const auto& sample) {
            return std::complex<T>{ sample };
            });

        return FourierTransform(x_p);
    }

    const auto NOver2 = N / 2;

    // Taking advantage of symmetry the FFT of a real signal can be computed
    // using a single N/2-point complex FFT. Split the input signal into its
    // even and odd components and load the data into a single complex vector.
    Signal<std::complex<T>> x_p{ f_s, NOver2 };
    for (size_t n = 0; n < NOver2; ++n)
    {
        const auto nTimes2 = n * 2;

        // x_p[n] = x[2n] + jx[2n + 1]
        x_p[n] = std::complex<T>{ x[nTimes2], x[nTimes2 + 1] };
    }

    // Perform the complex FFT
    auto X_p = impl::IsPowerOf2(N)
        ? impl::StockhamFFT(x_p)
        : impl::BluesteinFFT(x_p);

    // The FFT is periodic so it is valid append X_p[0] to the end. This is
    // required to avoid a buffer overflow in the next section.
    X_p.push_back(X_p[0]);

    // Extract the real FFT from the output of the complex FFT
    Signal<std::complex<T>> X{ f_s, N };
    for (size_t m = 0; m < NOver2; ++m)
    {
        const auto NOver2MinusM = NOver2 - m;

        const auto X_r = std::make_pair(
            (X_p[m].real() + X_p[NOver2MinusM].real()) / 2,
            (X_p[m].real() - X_p[NOver2MinusM].real()) / 2
        );

        const auto X_i = std::make_pair(
            (X_p[m].imag() + X_p[NOver2MinusM].imag()) / 2,
            (X_p[m].imag() - X_p[NOver2MinusM].imag()) / 2
        );

        const auto a = std::cos(static_cast<T>(M_PI) * m / NOver2);
        const auto b = std::sin(static_cast<T>(M_PI) * m / NOver2);

        X[m] = std::complex<T>{
            X_r.first + a * X_i.first - b * X_r.second,
            X_i.second - b * X_i.first - a * X_r.second
        };
    }

    // X[m] = X*[N-m] where 1 <= m <= N/2 - 1
    for (size_t m = 1; m < NOver2; ++m)
    {
        X[N - m] = std::conj(X[m]); // (>*.*)> symmetry! <(*.*<)
    }

    // X[NOver2] is a special case
    X[NOver2] = X_p[0].real() - X_p[0].imag();

    return X;
}

/*!
\brief Calculates the fast Fourier transform of a complex signal
\param[in] x - Complex input signal
\return Signal representing the FFT of the input data
*/
template <typename T>
Signal<std::complex<T>> FourierTransform(const Signal<std::complex<T>>& x)
{
    const auto N = x.size();

    return impl::IsPowerOf2(N)
        ? impl::StockhamFFT(x)
        : impl::BluesteinFFT(x);
}

/*!
\brief Calculates the inverse fast Fourier transform of a complex signal
\param[in] X - Complex input signal
\return Signal representing the IFFT of the input data
*/
template <typename T>
Signal<std::complex<T>> InverseFourierTransform(const Signal<std::complex<T>>& X)
{
    const auto f_s = X.GetSampleRate();
    const auto N = X.size();

    Signal<std::complex<T>> X_p{ f_s, N };
    std::transform(X.begin(), X.end(), X_p.begin(), [](const auto& sample) {
        return std::conj(sample);
        });

    const auto x_p = FourierTransform(X_p);

    Signal<std::complex<T>> x{ f_s, N };
    std::transform(x_p.begin(), x_p.end(), x.begin(), [=](const auto& sample) {
        return std::conj(sample) / static_cast<T>(N);
        });

    return x;
}

} /* namespace tnt::dsp */