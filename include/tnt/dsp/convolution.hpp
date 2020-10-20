#pragma once

#include "fft.hpp"
#include <algorithm>
#include <cassert>
#include <complex>
#include <functional>

namespace tnt::dsp
{

/*!
 \brief Calculates the convolution of two real input signals
 \param[in] a - Vector of real input data
 \param[in] b - Vector of real input data
 \return Vector of real values representing \a a * \a b
 */
template <typename T>
Signal<T> Convolve(const Signal<T>& a, const Signal<T>& b)
{
    assert(a.GetSampleRate() == b.GetSampleRate());
    const auto f_s = a.GetSampleRate();
    assert(a.size() == b.size());
    const auto N = a.size();

    // TODO: Cache FFT twiddle factors so they aren't calculated twice
    const auto A = FFT(a);
    const auto B = FFT(b);

    // The convolution theorem states that multiplication in the frequency
    // domain is equivalent to convolution in the time domain
    Signal<std::complex<T>> C(f_s, N);
    std::transform(A.begin(), A.end(), B.begin(), C.begin(), std::multiplies<std::complex<T>>());

    const auto c = IFFT(C);

    // Strip off the complex portion of the result since we are dealing
    // with only real input signals
    Signal<T> x(f_s, N);
    std::transform(c.begin(), c.end(), x.begin(), [](const auto& sample) {
        return sample.real();
        });

    return x;
}

/*!
 \brief Calculates the convolution of a real, and a complex input signal
 \param[in] a - Vector of real input data
 \param[in] b - Vector of complex input data
 \return Vector of complex values representing \a a * \a b
 */
template<typename T>
Signal<std::complex<T>> Convolve(const Signal<T>& a, const Signal<std::complex<T>>& b)
{
    assert(a.GetSampleRate() == b.GetSampleRate());
    const auto f_s = a.GetSampleRate();
    assert(a.size() == b.size());
    const auto N = a.size();

    // Convert "a" into a complex signal
    Signal<std::complex<T>> a_p(f_s, N);
    std::transform(a.begin(), a.end(), a_p.begin(), [](const auto& sample) {
        return std::complex<T>(sample);
        });

    return Convolve(a_p, b);
}

/*!
 \brief Calculates the convolution of a complex, and a real input signal
 \param[in] a - Vector of complex input data
 \param[in] b - Vector of real input data
 \return Vector of complex values representing \a a * \a b
 */
template <typename T>
Signal<std::complex<T>> Convolve(const Signal<std::complex<T>>& a, const Signal<T>& b)
{
    assert(a.GetSampleRate() == b.GetSampleRate());
    const auto f_s = a.GetSampleRate();
    assert(a.size() == b.size());
    const auto N = a.size();

    // Convert "b" into a complex signal
    Signal<std::complex<T>> b_p(f_s, N);
    std::transform(b.begin(), b.end(), b_p.begin(), [](const auto& sample) {
        return std::complex<T>(sample);
        });

    return Convolve(a, b_p);
}

/**
 \brief Calculates the convolution of two complex input signals
 \param[in] a - Vector of complex input data
 \param[in] b - Vector of complex input data
 \return Vector of complex values representing \a a * \a b
 */
template <typename T>
Signal<std::complex<T>> Convolve(const Signal<std::complex<T>>& a, const Signal<std::complex<T>>& b)
{
    assert(a.GetSampleRate() == b.GetSampleRate());
    const auto f_s = a.GetSampleRate();
    assert(a.size() == b.size());
    const auto N = a.size();

    // TODO: Cache FFT twiddle factors so they aren't calculated twice
    const auto A = FFT(a);
    const auto B = FFT(b);

    // The convolution theorem states that multiplication in the frequency
    // domain is equivalent to convolution in the time domain
    Signal<std::complex<T>> C(f_s, N);
    std::transform(A.begin(), A.end(), B.begin(), C.begin(), std::multiplies<std::complex<T>>());

    return IFFT(C);
}

} /* namespace tnt::dsp */