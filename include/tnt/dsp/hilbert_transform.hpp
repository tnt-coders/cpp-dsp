#pragma once

#include "fourier_transform.hpp"

#include <complex>

namespace tnt::dsp
{

/*!
\brief Calculates the analytical signal of a real input signal
\param[in] x - Real input signal
\return Analytical signal representing the Hilbert transform of the input signal
*/
template <typename T>
Signal<std::complex<T>> hilbert_transform(const Signal<T>& x)
{
    const auto sampleRate = x.sample_rate();
    const auto N          = x.size();

    // Take the Fourier transform
    const auto X = fourier_transform(x);

    Signal<std::complex<T>> X_c(sampleRate, N);

    // The DC component does not get doubled
    X_c[0] = X[0];

    // Set all real to 2x the Fourier transform
    // Zero out imaginary components (past N/2)
    for (size_t n = 1; n <= N / 2; ++n)
    {
        X_c[n] = static_cast<T>(2) * X[n];
    }

    // Take the inverse Fourier transform
    const auto x_c = inverse_fourier_transform(X_c);

    return x_c;
}

} /* namespace tnt::dsp */
