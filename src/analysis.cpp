#include "analysis.hpp"

#include <algorithm>
#include <complex>
#include <vector>

namespace tnt::dsp
{

using std::abs;
using std::arg;
using std::complex;
using std::norm;
using std::transform;
using std::vector;

double Magnitude(const double value)
{
    return abs(value);
}

double Magnitude(const complex<double>& value)
{
    return abs(value);
}

vector<double> Magnitude(const vector<double>& x)
{
    vector<double> x_magnitude(x.size());
    transform(x.begin(), x.end(), x_magnitude.begin(), [](const double value) {
        return Magnitude(value);
        });

    return x_magnitude;
}

vector<double> Magnitude(const vector<complex<double>>& x)
{
    vector<double> x_magnitude(x.size());
    transform(x.begin(), x.end(), x_magnitude.begin(), [](const complex<double>& value) {
        return Magnitude(value);
        });

    return x_magnitude;
}

double Phase(const double value)
{
    // Due to a bug in the MSVC implementation of std::arg it doesn't handle real inputs correctly
    // Explicitly convert the value to std::complex<double> to get around this.
    return arg(static_cast<complex<double>>(value));
}

double Phase(const complex<double>& value)
{
    return arg(value);
}

vector<double> Phase(const vector<double>& x)
{
    vector<double> x_phase(x.size());
    transform(x.begin(), x.end(), x_phase.begin(), [](const complex<double>& value) {
        return Phase(value);
        });

    return x_phase;
}

vector<double> Phase(const vector<complex<double>>& x)
{
    vector<double> x_phase(x.size());
    transform(x.begin(), x.end(), x_phase.begin(), [](const complex<double>& value) {
        return Phase(value);
        });

    return x_phase;
}

double Power(const double value)
{
    return norm(value);
}

double Power(const complex<double>& value)
{
    return norm(value);
}

vector<double> Power(const vector<double>& x)
{
    vector<double> x_power(x.size());
    transform(x.begin(), x.end(), x_power.begin(), [](const double value) {
        return Power(value);
        });

    return x_power;
}

vector<double> Power(const vector<complex<double>>& x)
{
    vector<double> x_power(x.size());
    transform(x.begin(), x.end(), x_power.begin(), [](const complex<double>& value) {
        return Power(value);
        });

    return x_power;
}

} /* namespace tnt::dsp */