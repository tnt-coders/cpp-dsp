#pragma once

#include "signal.hpp"
#include <cmath>
#include <complex>

namespace tnt::dsp
{

/*!
\brief Computes the discrete Fourier transform of a real input signal.

This function calculates the DFT of a real input signal using the standard
DFT algorithm:

\image html "DFT Equation.png"

When calculating the DFT of a real input signal only the first N/2 + 1 ouputs
are unique due to symmetry. This DFT implementation takes advantage of that
symmetry to improve efficiency.

\param[in] x - Discrete sequence of real time-domain samples
\return DFT of the input sequence
*/
template <typename T>
Signal<std::complex<T>> DFT(const Signal<T>& x)
{
    auto f_s = x.GetSampleRate();
    auto N = x.size();

    Signal<std::complex<T>> X(f_s, N);

    // Take advantage of DFT symmetry when dealing with real input signals
    // Only the first N/2 + 1 outputs are unique
    for (auto k = 0; k < N / 2 + 1; ++k)
    {
        for (auto n = 0; n < N; ++n)
        {
            X[k] += x[n] * std::polar(1.0, -2 * M_PI * n * k / N);
        }

        // X(N-k) = X(k)* for k = 1 -> N/2
        if (k != 0)
        {
            X[N - k] = std::conj(X[k]);
        }
    }

    return X;
}

/*!
\brief Computes the discrete Fourier transform of a complex input signal.

This function calculates the DFT of a complex input signal using the standard
DFT algorithm:

\image html "DFT Equation.png"

\param[in] x - Discrete sequence of complex time-domain samples
\return DFT of the input sequence
*/
template <typename T>
Signal<std::complex<T>> DFT(const Signal<std::complex<T>>& x)
{
    auto f_s = x.GetSampleRate();
    auto N = x.size();

    Signal<std::complex<T>> X(f_s, N);

    for (auto k = 0; k < N; ++k)
    {
        for (auto n = 0; n < N; ++n)
        {
            X[k] += x[n] * std::polar(1.0, -2 * M_PI * n * k / N);
        }
    }

    return X;
}

} /* namespace tnt::dsp */