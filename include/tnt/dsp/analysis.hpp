#pragma once

#include "signal.hpp"

#include <algorithm>
#include <complex>
#include <limits>

namespace tnt::dsp
{

/*!
\brief Calculates the magnitude of a real sample
\param[in] sample - Real sample
\return Magnitude of the sample
*/
template <typename T>
T magnitude(const T& sample)
{
    return std::abs(sample);
}

/*!
\brief Calculates the magnitude of a complex sample
\param[in] sample - Complex sample
\return Magnitude of the sample
*/
template <typename T>
T magnitude(const std::complex<T>& sample)
{
    return std::abs(sample);
}

/*!
\brief Calculates the magnitude spectrum of a real signal
\param[in] x - Real signal
\return Magnitude of the signal
*/
template <typename T>
signal<T> magnitude(const signal<T>& x)
{
    signal<T> x_magnitude(x.sample_rate(), x.size());
    std::transform(x.begin(), x.end(), x_magnitude.begin(), [](const auto& sample) {
        return magnitude(sample);
    });

    return x_magnitude;
}

/*!
\brief Calculates the magnitude spectrum of a complex signal
\param[in] x - Complex signal
\return Magnitude of the signal
*/
template <typename T>
signal<T> magnitude(const signal<std::complex<T>>& x)
{
    signal<T> x_magnitude(x.sample_rate(), x.size());
    std::transform(x.begin(), x.end(), x_magnitude.begin(), [](const auto& sample) {
        return magnitude(sample);
    });

    return x_magnitude;
}

/*!
\brief Calculates the phase angle (in radians) of a real sample
\param[in] sample Real sample
\param[in] epsilon Values below epsilon will be treated as 0
\return Phase angle (in radians) of the sample
*/
template <typename T>
T phase(const T& sample, const T& epsilon)
{
    const auto x = std::abs(sample) < epsilon ? 0 : sample;
    return std::arg(x);
}

/*!
\brief Calculates the phase angle (in radians) of a real sample

Phase does not behave well around 0. Values close to 0 will be treated as 0.

\param[in] sample - Real sample
\return Phase angle (in radians) of the sample
*/
template <typename T>
T phase(const T& sample)
{
    return phase(sample, std::numeric_limits<T>::epsilon() * 100);
}

/*!
\brief Calculates the phase angle (in radians) of a complex sample
\param[in] sample Complex sample
\param[in] epsilon Real/imaginary components below epsilon will be treated as 0
\return Phase angle (in radians) of the sample
*/
template <typename T>
T phase(const std::complex<T>& sample, const T& epsilon)
{
    const auto real      = std::abs(sample.real()) < epsilon ? 0 : sample.real();
    const auto imaginary = std::abs(sample.imag()) < epsilon ? 0 : sample.imag();

    const std::complex<T> x = {real, imaginary};

    return std::arg(x);
}

/*!
\brief Calculates the phase angle (in radians) of a complex sample

Phase does not behave well around 0. Real/imaginary components close to 0 will be treated as 0.

\param[in] sample - Complex sample
\return Phase angle (in radians) of the sample
*/
template <typename T>
T phase(const std::complex<T>& sample)
{
    return phase(sample, std::numeric_limits<T>::epsilon() * 100);
}

/*!
\brief Calculates the phase spectrum (in radians) of a real signal
\param[in] x - Real signal
\return Phase spectrum (in radians) of the signal
*/
template <typename T>
signal<T> phase(const signal<T>& x)
{
    signal<T> x_phase(x.sample_rate(), x.size());
    std::transform(x.begin(), x.end(), x_phase.begin(), [](const auto& sample) {
        return phase(sample);
    });

    return x_phase;
}

/*!
\brief Calculates the phase spectrum (in radians) of a complex signal
\param[in] x - Complex signal
\return Phase spectrum (in radians) of the signal
*/
template <typename T>
signal<T> phase(const signal<std::complex<T>>& x)
{
    signal<T> x_phase(x.sample_rate(), x.size());
    std::transform(x.begin(), x.end(), x_phase.begin(), [](const auto& sample) {
        return phase(sample);
    });

    return x_phase;
}

/*!
\brief Calculates the power of a real sample
\param[in] sample - Real sample
\return Power of the sample
*/
template <typename T>
T power(const T& sample)
{
    return std::norm(sample);
}

/*!
\brief Calculates the power of a complex sample
\param[in] sample - Complex sample
\return Power of the sample
*/
template <typename T>
T power(const std::complex<T>& sample)
{
    return std::norm(sample);
}

/*!
\brief Calculates the power spectrum of a real signal
\param[in] x - Real signal
\return Power spectrum of the signal
*/
template <typename T>
signal<T> power(const signal<T>& x)
{
    signal<T> x_power(x.sample_rate(), x.size());
    std::transform(x.begin(), x.end(), x_power.begin(), [](const auto& sample) {
        return power(sample);
    });

    return x_power;
}

/*!
\brief Calculates the power spectrum of a complex signal
\param[in] x - Complex signal
\return Power spectrum of the signal
*/
template <typename T>
signal<T> power(const signal<std::complex<T>>& x)
{
    signal<T> x_power(x.sample_rate(), x.size());
    std::transform(x.begin(), x.end(), x_power.begin(), [](const auto& sample) {
        return power(sample);
    });

    return x_power;
}

}  // namespace tnt::dsp
