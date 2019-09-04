#include "convolution.h"

#include "fft.h"

#include <algorithm>
#include <cassert>
#include <complex>
#include <functional>
#include <vector>

namespace tnt::dsp
{

using std::complex;
using std::multiplies;
using std::transform;
using std::vector;

vector<double> Convolve(const vector<double>& a, const vector<double>& b)
{
    assert(a.size() == b.size());
    const size_t N = a.size();

    // TODO: Cache FFT twiddle factors so they aren't calculated twice
    const auto A = FFT(a);
    const auto B = FFT(b);

    // The convolution theorem states that multiplication in the frequency
    // domain is equivalent to convolution in the time domain
    vector<complex<double>> C(N);
    transform(A.begin(), A.end(), B.begin(), C.begin(), multiplies<complex<double>>());

    const auto c = IFFT(C);

    // Strip off the complex portion of the result since we are dealing
    // with only real input signals
    vector<double> x(N);
    transform(c.begin(), c.end(), x.begin(), [](const complex<double>& value) {
        return value.real();
        });

    return x;
}

vector<complex<double>> Convolve(const vector<double>& a, const vector<complex<double>>& b)
{
    assert(a.size() == b.size());
    const size_t N = a.size();

    // Convert "a" into a complex signal
    vector<complex<double>> a_p(N);
    transform(a.begin(), a.end(), a_p.begin(), [](const double value) {
        return complex<double>(value);
        });

    return Convolve(a_p, b);
}

vector<complex<double>> Convolve(const vector<complex<double>>& a, const vector<double>& b)
{
    assert(a.size() == b.size());
    const size_t N = a.size();

    // Convert "b" into a complex signal
    vector<complex<double>> b_p(N);
    transform(b.begin(), b.end(), b_p.begin(), [](const double value) {
        return complex<double>(value);
        });

    return Convolve(a, b_p);
}


vector<complex<double>> Convolve(const vector<complex<double>>& a, const vector<complex<double>>& b)
{
    assert(a.size() == b.size());
    const size_t N = a.size();

    // TODO: Cache FFT twiddle factors so they aren't calculated twice
    const auto A = FFT(a);
    const auto B = FFT(b);

    // The convolution theorem states that multiplication in the frequency
    // domain is equivalent to convolution in the time domain
    vector<complex<double>> C(N);
    transform(A.begin(), A.end(), B.begin(), C.begin(), multiplies<complex<double>>());

    return IFFT(C);
}

} /* namespace tnt::dsp */