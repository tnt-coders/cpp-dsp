#pragma once

#include "fourier_transform.hpp"
#include <algorithm>
#include <cassert>
#include <complex>
#include <functional>

namespace tnt::dsp
{

/*!
 \brief Calculates the convolution of two real input signals
 \param[in] a - Real input signal
 \param[in] b - Real input signal
 \return Signal representing \a a * \a b
 */
template <typename T>
signal<T> convolve(const signal<T>& a, const signal<T>& b)
{
    assert(a.sample_rate() == b.sample_rate());
    const auto f_s = a.sample_rate();
    assert(a.size() == b.size());
    const auto N = a.size();

    // TODO: Cache FFT twiddle factors so they aren't calculated twice
    const auto A = fourier_transform(a);
    const auto B = fourier_transform(b);

    // The convolution theorem states that multiplication in the frequency
    // domain is equivalent to convolution in the time domain
    signal<std::complex<T>> C{ f_s, N };
    std::transform(A.begin(), A.end(), B.begin(), C.begin(), std::multiplies<std::complex<T>>());

    const auto c = inverse_fourier_transform(C);

    // Strip off the complex portion of the result since we are dealing
    // with only real input signals
    signal<T> x{ f_s, N };
    std::transform(c.begin(), c.end(), x.begin(), [](const auto& sample) {
        return sample.real();
        });

    return x;
}

/*!
 \brief Calculates the convolution of a real, and a complex input signal
 \param[in] a - Real input signal
 \param[in] b - Complex input signal
 \return Signal representing \a a * \a b
 */
template<typename T>
signal<std::complex<T>> convolve(const signal<T>& a, const signal<std::complex<T>>& b)
{
    assert(a.sample_rate() == b.sample_rate());
    const auto f_s = a.sample_rate();
    assert(a.size() == b.size());
    const auto N = a.size();

    // Convert "a" into a complex signal
    signal<std::complex<T>> a_p{ f_s, N };
    std::transform(a.begin(), a.end(), a_p.begin(), [](const auto& sample) {
        return std::complex<T>(sample);
        });

    return convolve(a_p, b);
}

/*!
 \brief Calculates the convolution of a complex, and a real input signal
 \param[in] a - Complex input signal
 \param[in] b - Real input signal
 \return Signal representing \a a * \a b
 */
template <typename T>
signal<std::complex<T>> convolve(const signal<std::complex<T>>& a, const signal<T>& b)
{
    assert(a.sample_rate() == b.sample_rate());
    const auto f_s = a.sample_rate();
    assert(a.size() == b.size());
    const auto N = a.size();

    // Convert "b" into a complex signal
    signal<std::complex<T>> b_p{ f_s, N };
    std::transform(b.begin(), b.end(), b_p.begin(), [](const auto& sample) {
        return std::complex<T>{ sample };
        });

    return convolve(a, b_p);
}

/**
 \brief Calculates the convolution of two complex input signals
 \param[in] a - Complex input signal
 \param[in] b - Complex input signal
 \return Signal representing \a a * \a b
 */
template <typename T>
signal<std::complex<T>> convolve(const signal<std::complex<T>>& a, const signal<std::complex<T>>& b)
{
    assert(a.sample_rate() == b.sample_rate());
    const auto f_s = a.sample_rate();
    assert(a.size() == b.size());
    const auto N = a.size();

    // TODO: Cache FFT twiddle factors so they aren't calculated twice
    const auto A = fourier_transform(a);
    const auto B = fourier_transform(b);

    // The convolution theorem states that multiplication in the frequency
    // domain is equivalent to convolution in the time domain
    signal<std::complex<T>> C{ f_s, N };
    std::transform(A.begin(), A.end(), B.begin(), C.begin(), std::multiplies<std::complex<T>>());

    return inverse_fourier_transform(C);
}

} /* namespace tnt::dsp */