#pragma once

#include "signal.hpp"
#include <cmath>

namespace tnt::dsp
{

/*!
\brief Generates signals of similar sample rates/sizes
*/
template <typename T>
class SignalGenerator final
{
public:

    /*!
    \brief Constructor
    \param[in] sampleRate Sample rate to use for generated signals
    \param[in] size Size to use for generated signals
    */
    SignalGenerator(size_t sampleRate, size_t size)
        : m_sampleRate(sampleRate)
        , m_sampleInterval(T{ 1.0 } / m_sampleRate)
        , m_size(size)
    {}

    /*!
    \brief Generates a cosine wave
    \param[in] frequency Frequency in Hz
    \param[in] amplitude Amplitude
    \param[in] phaseShift Phase shift
    \param[in] verticalShift Vertical shift
    */
    Signal<T> Cosine(T frequency, T amplitude = 1, T phaseShift = 0, T verticalShift = 0) const
    {
        Signal<T> signal(m_sampleRate, m_size);

        for (size_t n = 0; n < signal.size(); ++n)
        {
            signal[n] = amplitude * std::cos(2 * T{ M_PI } *frequency * n * m_sampleInterval + phaseShift) + verticalShift;
        }

        return signal;
    }

    /*!
    \brief Generates a sine wave
    \param[in] frequency Frequency in Hz
    \param[in] amplitude Amplitude
    \param[in] phaseShift Phase shift
    \param[in] verticalShift Vertical shift
    */
    Signal<T> Sine(T frequency, T amplitude = 1, T phaseShift = 0, T verticalShift = 0) const
    {
        Signal<T> signal(m_sampleRate, m_size);

        for (size_t n = 0; n < signal.size(); ++n)
        {
            signal[n] = amplitude * std::sin(2 * T{ M_PI } * frequency * n * m_sampleInterval + phaseShift) + verticalShift;
        }

        return signal;
    }

private:
    size_t m_sampleRate;
    T m_sampleInterval;
    size_t m_size;
};

} /* namespace tnt::dsp */