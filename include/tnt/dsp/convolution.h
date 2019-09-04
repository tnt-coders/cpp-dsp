#pragma once

#include <complex>
#include <vector>

namespace tnt::dsp
{

/**
 * @brief Calculates the convolution of two real input signals
 * @param[in] a - Vector of real input data
 * @param[in] b - Vector of real input data
 * @result Vector of real values representing \a a * \a b
 */
std::vector<double> Convolve(const std::vector<double>& a, const std::vector<double>& b);

/**
 * @brief Calculates the convolution of a real, and a complex input signal
 * @param[in] a - Vector of real input data
 * @param[in] b - Vector of complex input data
 * @result Vector of complex values representing \a a * \a b
 */
std::vector<std::complex<double>> Convolve(const std::vector<double>& a, const std::vector<std::complex<double>>& b);

/**
 * @brief Calculates the convolution of a complex, and a real input signal
 * @param[in] a - Vector of complex input data
 * @param[in] b - Vector of real input data
 * @result Vector of complex values representing \a a * \a b
 */
std::vector<std::complex<double>> Convolve(const std::vector<std::complex<double>>& a, const std::vector<double>& b);

/**
 * @brief Calculates the convolution of two complex input signals
 * @param[in] a - Vector of complex input data
 * @param[in] b - Vector of complex input data
 * @result Vector of complex values representing \a a * \a b
 */
std::vector<std::complex<double>> Convolve(const std::vector<std::complex<double>>& a, const std::vector<std::complex<double>>& b);

} /* namespace tnt::dsp */