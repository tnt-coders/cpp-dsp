#pragma once

#include "signal.hpp"

#include <cmath>
#include <complex>

namespace tnt::dsp
{

/*!
\brief Generates signals of similar sample rates/sizes
*/
template <typename T>
class signal_generator final
{
public:
	/*!
	\brief Constructor
	\param[in] sample_rate Sample rate to use for generated signals
	\param[in] size Size to use for generated signals
	*/
	signal_generator(size_t sample_rate, size_t size)
		: m_sample_rate(sample_rate)
		, m_sample_interval(1 / static_cast<T>(m_sample_rate))
		, m_size(size)
	{}

	/*!
	\brief Gets the sample rate
	\return Sample rate
	*/
	size_t sample_rate() const
	{
		return m_sample_rate;
	}

	/*!
	\brief Gets the size
	\return Size
	*/
	size_t size() const
	{
		return m_size;
	}

	/*!
	\brief Generates a cosine wave
	\param[in] frequency Frequency in Hz
	\param[in] amplitude Amplitude
	\param[in] phase_shift Phase shift
	\param[in] vertical_shift Vertical shift
	*/
	signal<T> cosine(T frequency, T amplitude = 1, T phase_shift = 0, T vertical_shift = 0) const
	{
		auto signal = signal<T>(m_sample_rate, m_size);
		for (size_t n = 0; n < signal.size(); ++n)
		{
			signal[n] =
			amplitude
			* std::cos(2 * static_cast<T>(M_PI) * frequency * n * m_sample_interval + phase_shift)
			+ vertical_shift;
		}

		return signal;
	}

	/*!
	\brief Generates a sine wave
	\param[in] frequency Frequency in Hz
	\param[in] amplitude Amplitude
	\param[in] phase_shift Phase shift
	\param[in] vertical_shift Vertical shift
	*/
	signal<T> sine(T frequency, T amplitude = 1, T phase_shift = 0, T vertical_shift = 0) const
	{
		auto signal = signal<T>(m_sample_rate, m_size);
		for (size_t n = 0; n < signal.size(); ++n)
		{
			signal[n] =
			amplitude
			* std::sin(2 * static_cast<T>(M_PI) * frequency * n * m_sample_interval + phase_shift)
			+ vertical_shift;
		}

		return signal;
	}

private:
	size_t m_sample_rate;
	T      m_sample_interval;
	size_t m_size;
};

} /* namespace tnt::dsp */