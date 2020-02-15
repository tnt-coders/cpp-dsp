#pragma once

#include <complex>
#include <vector>

namespace tnt::dsp
{

/**
 * @brief Calculates the fast Fourier transform of a real signal
 * @param[in] x - Vector of real input data
 * @return Vector of complex values representing the FFT of the input data
 */
std::vector<std::complex<double>> FFT(const std::vector<double>& x);

/**
 * @brief Calculates the fast Fourier transform of a complex signal
 * @param[in] x - Vector of complex input data
 * @return Vector of complex values representing the FFT of the input data
 */
std::vector<std::complex<double>> FFT(const std::vector<std::complex<double>>& x);


/**
 * @brief Calculates the inverse fast Fourier transform of a complex signal
 * @param[in] X - Vector of complex input data
 * @return Vector of complex values representing the IFFT of the input data
 */
std::vector<std::complex<double>> IFFT(const std::vector<std::complex<double>>& X);

} /* namespace tnt::dsp */