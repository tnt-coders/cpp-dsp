#pragma once

#include <complex>
#include <vector>

namespace tnt::dsp
{

/**
 * @brief Calculates the magnitude of a real value
 * @param[in] value - Real input value
 * @return Magnitude of the input
 */
double Magnitude(const double value);

/**
 * @brief Calculates the magnitude of a complex value
 * @param[in] value - Complex input value
 * @return Magnitude of the input
 */
double Magnitude(const std::complex<double>& value);

/**
 * @brief Calculates the magnitude spectrum of a real input vector
 * @param[in] x - Real input vector
 * @return Magnitude spectrum of the input
 */
std::vector<double> Magnitude(const std::vector<double>& x);

/**
 * @brief Calculates the magnitude spectrum of a complex input vector
 * @param[in] x - Complex input vector
 * @return Magnitude spectrum of the input
 */
std::vector<double> Magnitude(const std::vector<std::complex<double>>& x);

// Phase calculations don't make much sense for real input values so no phase
// functions are provided for real inputs

/**
 * @brief Calculates the phase angle (in radians) of a complex value
 * @param[in] value - Complex input value
 * @return Phase angle (in radians) of the input
 */
double Phase(const std::complex<double>& value);

/**
 * @brief Calculates the phase spectrum (in radians) of a complex input vector
 * @param[in] x - Complex input vector
 * @return Phase spectrum (in radians) of the input
 */
std::vector<double> Phase(const std::vector<std::complex<double>>& x);

/**
 * @brief Calculates the power of a real value
 * @param[in] value - Real input value
 * @return Power of the input
 */
double Power(const double value);

/**
 * @brief Calculates the power of a complex value
 * @param[in] value - Complex input value
 * @return Power of the input
 */
double Power(const std::complex<double>& value);

/**
 * @brief Calculates the power spectrum of a real input vector
 * @param[in] x - Real input vector
 * @return Power spectrum of the input
 */
std::vector<double> Power(const std::vector<double>& x);

/**
 * @brief Calculates the power spectrum of a complex input vector
 * @param[in] x - Complex input vector
 * @return Power spectrum of the input
 */
std::vector<double> Power(const std::vector<std::complex<double>>& x);

} /* namespace tnt::dsp */
