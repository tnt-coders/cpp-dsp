#pragma once

#include "signal.hpp"
#include <algorithm>
#include <complex>

namespace tnt::dsp
{

/*!
\brief Calculates the magnitude of a real sample
\param[in] sample - Real sample
\return Magnitude of the sample
*/
template <typename T>
T Magnitude(const T& sample)
{
    return std::abs(sample);
}

/*!
\brief Calculates the magnitude of a complex sample
\param[in] sample - Complex sample
\return Magnitude of the sample
*/
template <typename T>
T Magnitude(const std::complex<T>& sample)
{
    return std::abs(sample);
}

/*!
\brief Calculates the magnitude spectrum of a real signal
\param[in] x - Real signal
\return Magnitude of the signal
*/
template <typename T>
Signal<T> Magnitude(const Signal<T>& x)
{
    Signal<T> x_magnitude{ x.GetSampleRate(), x.size() };
    std::transform(x.begin(), x.end(), x_magnitude.begin(), [](const auto& sample) {
        return Magnitude(sample);
        });

    return x_magnitude;
}

/*!
\brief Calculates the magnitude spectrum of a complex signal
\param[in] x - Complex signal
\return Magnitude of the signal
*/
template <typename T>
Signal<T> Magnitude(const Signal<std::complex<T>>& x)
{
    Signal<T> x_magnitude{ x.GetSampleRate(), x.size() };
    std::transform(x.begin(), x.end(), x_magnitude.begin(), [](const auto& sample) {
        return Magnitude(sample);
        });

    return x_magnitude;
}

/*!
\brief Calculates the phase angle (in radians) of a real sample
\param[in] sample - Real sample
\return Phase angle (in radians) of the sample
*/
template <typename T>
T Phase(const T& sample)
{
    return std::arg(sample);
}

/*!
\brief Calculates the phase angle (in radians) of a complex sample
\param[in] sample - Complex sample
\return Phase angle (in radians) of the sample
*/
template <typename T>
T Phase(const std::complex<T>& sample)
{
    return std::arg(sample);
}

/*!
\brief Calculates the phase spectrum (in radians) of a real signal
\param[in] x - Real signal
\return Phase spectrum (in radians) of the signal
*/
template <typename T>
Signal<T> Phase(const Signal<T>& x)
{
    Signal<T> x_phase{ x.GetSampleRate(), x.size() };
    std::transform(x.begin(), x.end(), x_phase.begin(), [](const auto& sample) {
        return Phase(sample);
        });

    return x_phase;
}

/*!
\brief Calculates the phase spectrum (in radians) of a complex signal
\param[in] x - Complex signal
\return Phase spectrum (in radians) of the signal
*/
template <typename T>
Signal<T> Phase(const Signal<std::complex<T>>& x)
{
    Signal<T> x_phase{ x.GetSampleRate(), x.size() };
    std::transform(x.begin(), x.end(), x_phase.begin(), [](const auto& sample) {
        return Phase(sample);
        });

    return x_phase;
}

/*!
\brief Calculates the power of a real sample
\param[in] sample - Real sample
\return Power of the sample
*/
template <typename T>
T Power(const T& sample)
{
    return std::norm(sample);
}

/*!
\brief Calculates the power of a complex sample
\param[in] sample - Complex sample
\return Power of the sample
*/
template <typename T>
T Power(const std::complex<T>& sample)
{
    return std::norm(sample);
}

/*!
\brief Calculates the power spectrum of a real signal
\param[in] x - Real signal
\return Power spectrum of the signal
*/
template <typename T>
Signal<T> Power(const Signal<T>& x)
{
    Signal<T> x_power{ x.GetSampleRate(), x.size() };
    std::transform(x.begin(), x.end(), x_power.begin(), [](const auto& sample) {
        return Power(sample);
        });

    return x_power;
}

/*!
\brief Calculates the power spectrum of a complex signal
\param[in] x - Complex signal
\return Power spectrum of the signal
*/
template <typename T>
Signal<T> Power(const Signal<std::complex<T>>& x)
{
    Signal<T> x_power{ x.GetSampleRate(), x.size() };
    std::transform(x.begin(), x.end(), x_power.begin(), [](const auto& sample) {
        return Power(sample);
        });

    return x_power;
}

} /* namespace tnt::dsp */