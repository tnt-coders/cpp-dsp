#pragma once

#include <complex>
#include <vector>

namespace tnt::dsp
{

/**
 * @brief Computes the discrete Fourier transform of a real input signal.
 *
 * This function calculates the DFT of a real input signal using the standard
 * DFT algorithm:
 *
 * @image html "DFT Equation.png"
 *
 * When calculating the DFT of a real input signal only the first N/2 + 1 ouputs
 * are unique due to symmetry. This DFT implementation takes advantage of that
 * symmetry to improve efficiency.
 *
 * @param[in] x - Discrete sequence of real time-domain samples
 * @return DFT of the input sequence
 */
std::vector<std::complex<double>> DFT(const std::vector<double>& x);

/**
 * @brief Computes the discrete Fourier transform of a complex input signal.
 *
 * This function calculates the DFT of a complex input signal using the standard
 * DFT algorithm:
 *
 * @image html "DFT Equation.png"
 *
 * @param[in] x - Discrete sequence of complex time-domain samples
 * @return DFT of the input sequence
 */
std::vector<std::complex<double>> DFT(const std::vector<std::complex<double>>& x);

} /* namespace tnt::dsp */
