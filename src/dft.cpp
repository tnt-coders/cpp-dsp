#include "dft.h"

#include <cmath>
#include <complex>
#include <vector>

namespace tnt::dsp
{

using std::complex;
using std::conj;
using std::polar;
using std::vector;

vector<complex<double>> DFT(const vector<double>& x)
{
    size_t N = x.size();

    vector<complex<double>> X(N);

    // Take advantage of DFT symmetry when dealing with real input signals
    // Only the first N/2 + 1 outputs are unique
    for (size_t k = 0; k < N / 2 + 1; ++k)
    {
        for (size_t n = 0; n < N; ++n)
        {
            X[k] += x[n] * polar(1.0, -2 * M_PI * n * k / N);
        }

        // X(N-k) = X(k)* for k = 1 -> N/2
        if (k != 0)
        {
            X[N - k] = conj(X[k]);
        }
    }

    return X;
}

vector<complex<double>> DFT(const vector<complex<double>>& x)
{
    size_t N = x.size();
    vector<complex<double>> X(N);

    for (size_t k = 0; k < N; ++k)
    {
        for (size_t n = 0; n < N; ++n)
        {
            X[k] += x[n] * polar(1.0, -2 * M_PI * n * k / N);
        }
    }

    return X;
}

} /* namespace tnt::dsp */