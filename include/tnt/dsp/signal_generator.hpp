#pragma once

#include "signal.hpp"
#include <cmath>

namespace tnt::dsp
{

template <typename T>
class SignalGenerator
{
public:
    SignalGenerator(size_t sampleRate, size_t size)
        : m_sampleRate(sampleRate)
        , m_size(size)
    {
        m_step = 1.0f / m_sampleRate;
    }

    Signal<T> Sin(T frequency, T amplitude = 1, T phaseShift = 0, T verticalShift = 0) const
    {
        Signal<T> signal(m_sampleRate, m_size);
        for (auto index = 0; index < signal.size(); ++index)
        {
            signal[index] = amplitude * std::sin(2 * T{ M_PI } * frequency * index * m_step + phaseShift) + verticalShift;
        }

        return signal;
    }

    Signal<T> Cos(T frequency, T amplitude = 1, T phaseShift = 0, T verticalShift = 0) const
    {
        Signal<T> signal(m_sampleRate, m_size);
        for (auto index = 0; index < signal.size(); ++index)
        {
            signal[index] = amplitude * std::cos(2 * T{ M_PI } * frequency * index * m_step + phaseShift) + verticalShift;
        }

        return signal;
    }

private:
    size_t m_sampleRate;
    size_t m_size;
    T m_step;
};

} /* namespace tnt::dsp */