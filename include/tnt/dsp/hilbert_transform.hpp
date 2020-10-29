#pragma once

#include "fourier_transform.hpp"
#include <complex>

namespace tnt::dsp
{

template <typename T>
Signal<std::complex<T>> HilbertTransform(const Signal<T>& x)
{
    const auto sampleRate = x.GetSampleRate();
    const auto N = x.size();

    // Take the Fourier transform
    const auto X = FourierTransform(x);

    Signal<std::complex<T>> X_c(sampleRate, N);
    
    // The DC component does not get doubled
    X_c[0] = X[0];

    // Set all real to 2x the Fourier transform
    // Zero out imaginary components (past N/2)
    for(size_t n = 1; n <= N / 2; ++n)
    {
        X_c[n] = T{ 2.0 } * X[n];
    }

    // Take the inverse Fourier transform
    const auto x_c = InverseFourierTransform(X_c);

    return x_c;
}

} /* namespace tnt::dsp */