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
Signal<std::complex<T>> fourier_transform(const Signal<T>& x)
{
    const auto f_s = x.sample_rate();
    const auto N   = x.size();

    // TODO: See if further optimization can be done for odd values of N
    // If N is odd, just perform the complex FFT
    if (!impl::is_even(N))
    {
        auto x_p = Signal<std::complex<T>>(f_s, N);
        std::transform(x.begin(), x.end(), x_p.begin(), [](const auto& sample) {
            return std::complex<T>(sample);
        });

        return fourier_transform(x_p);
    }

    const auto N_over_2 = N / 2;

    // Taking advantage of symmetry the FFT of a real signal can be computed
    // using a single N/2-point complex FFT. Split the input signal into its
    // even and odd components and load the data into a single complex vector.
    auto x_p = Signal<std::complex<T>>(f_s, N_over_2);
    for (size_t n = 0; n < N_over_2; ++n)
    {
        const auto n_times_2 = n * 2;

        // x_p[n] = x[2n] + jx[2n + 1]
        x_p[n] = std::complex<T>(x[n_times_2], x[n_times_2 + 1]);
    }

    // Perform the complex FFT
    auto X_p = impl::is_power_of_2(N) ? impl::stockham_fft(x_p) : impl::bluestein_fft(x_p);

    // The FFT is periodic so it is valid append X_p[0] to the end. This is
    // required to avoid a buffer overflow in the next section.
    X_p.push_back(X_p[0]);

    // Extract the real FFT from the output of the complex FFT
    auto X = Signal<std::complex<T>>(f_s, N);
    for (size_t m = 0; m < N_over_2; ++m)
    {
        const auto N_over_2_minus_m = N_over_2 - m;

        const auto X_r = std::make_pair((X_p[m].real() + X_p[N_over_2_minus_m].real()) / 2,
                                        (X_p[m].real() - X_p[N_over_2_minus_m].real()) / 2);

        const auto X_i = std::make_pair((X_p[m].imag() + X_p[N_over_2_minus_m].imag()) / 2,
                                        (X_p[m].imag() - X_p[N_over_2_minus_m].imag()) / 2);

        const auto a = std::cos(static_cast<T>(M_PI) * m / N_over_2);
        const auto b = std::sin(static_cast<T>(M_PI) * m / N_over_2);

        X[m] = std::complex<T>(X_r.first + a * X_i.first - b * X_r.second,
                               X_i.second - b * X_i.first - a * X_r.second);
    }

    // X[m] = X*[N-m] where 1 <= m <= N/2 - 1
    for (size_t m = 1; m < N_over_2; ++m)
    {
        X[N - m] = std::conj(X[m]);  // (>*.*)> symmetry! <(*.*<)
    }

    // X[N_over_2] is a special case
    X[N_over_2] = X_p[0].real() - X_p[0].imag();

    return X;
}

/*!
\brief Calculates the fast Fourier transform of a complex signal
\param[in] x - Complex input signal
\return Signal representing the FFT of the input data
*/
template <typename T>
Signal<std::complex<T>> fourier_transform(const Signal<std::complex<T>>& x)
{
    const auto N = x.size();

    return impl::is_power_of_2(N) ? impl::stockham_fft(x) : impl::bluestein_fft(x);
}

/*!
\brief Calculates the inverse fast Fourier transform of a complex signal
\param[in] X - Complex input signal
\return Signal representing the IFFT of the input data
*/
template <typename T>
Signal<std::complex<T>> inverse_fourier_transform(const Signal<std::complex<T>>& X)
{
    const auto f_s = X.sample_rate();
    const auto N   = X.size();

    auto X_p = Signal<std::complex<T>>(f_s, N);
    std::transform(X.begin(), X.end(), X_p.begin(), [](const auto& sample) {
        return std::conj(sample);
    });

    const auto x_p = fourier_transform(X_p);

    auto x = Signal<std::complex<T>>(f_s, N);
    std::transform(x_p.begin(), x_p.end(), x.begin(), [=](const auto& sample) {
        return std::conj(sample) / static_cast<T>(N);
    });

    return x;
}

} /* namespace tnt::dsp */