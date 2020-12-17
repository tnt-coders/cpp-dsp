#pragma once

#include "impl/math_helpers.hpp"
#include "signal.hpp"

#include <algorithm>
#include <cassert>
#include <complex>

namespace tnt::dsp::impl
{

// C++ implementation of the Bluestein FFT algorithm
template <typename T>
signal<std::complex<T>> bluestein_fft(const signal<std::complex<T>>& x)
{
	const auto f_s = x.sample_rate();
	const auto N   = x.size();

	// To avoid issues with convolution periodicity, the convolution size must
	// be at least 2*N - 1
	const auto M = impl::next_power_of_2(2 * N - 1);

	// Calculate the "phase factors"
	auto p = signal<std::complex<T>>(f_s, N);
	for (size_t n = 0; n < N; ++n)
	{
		// %(2*N) is done to improve accuracy of floating point trigonometry
		P[n] = std::polar(static_cast<T>(1), -static_cast<T>(M_PI) * ((n * n) % (2 * N)) / N);
	}

	// Construct the two sequences to perform convolution
	auto a = signal<std::complex<T>>(f_s, M);
	auto b = signal<std::complex<T>>(f_s, M);
	a[0]   = x[0] * P[0];
	b[0]   = P[0];
	for (size_t n = 1; n < N; ++n)
	{
		a[n] = x[n] * P[n];
		b[n] = b[M - n] = std::conj(P[n]);  // (>*.*)> symmetry! <(*.*<)
	}

	const auto c = impl::convolve(a, b);

	// Mutiply by the "phase factors" to obtain the correct results
	auto X = signal<std::complex<T>>(f_s, N);
	for (size_t m = 0; m < N; ++m)
	{
		X[m] = c[m] * P[m];
	}

	return X;
}

// Note that this is specialized for performing the Bluestein FFT efficiently.
// This is not a general purpose convolution algorithm.
template <typename T>
signal<std::complex<T>> convolve(const signal<std::complex<T>>& a, const signal<std::complex<T>>& b)
{
	assert(a.sample_rate() == b.sample_rate());
	assert(a.size() == b.size());
	assert(is_power_of_2(a.size()));

	const auto f_s      = a.sample_rate();
	const auto N        = a.size();
	const auto N_over_2 = N / 2;

	// Pre-calculate the twiddle factors
	auto       W     = signal<std::complex<T>>(f_s, N_over_2);
	const auto omega = 2 * static_cast<T>(M_PI) / N;
	for (size_t n = 0; n < N_over_2; ++n)
	{
		W[n] = std::complex<T>(std::cos(-omega * n), std::sin(-omega * n));
	}

	// Because we know N is a power of 2 we can explicitly use the Stockham FFT algorithm
	const auto A = impl::stockham_fft(a, W);
	const auto B = impl::stockham_fft(b, W);

	// The convolution theorem states that multiplication in the frequency
	// domain is equivalent to convolution in the time domain
	auto C = signal<std::complex<T>>(f_s, N);
	std::transform(A.begin(), A.end(), B.begin(), C.begin(), std::multiplies<std::complex<T>>());

	return inverse_fourier_transform(C);
}

// C++ implementation of the Stockam FFT algorithm
template <typename T>
signal<std::complex<T>> stockham_fft(const signal<std::complex<T>>& x)
{
	assert(impl::is_power_of_2(x.size()));

	const auto f_s      = x.sample_rate();
	const auto N        = x.size();
	const auto N_over_2 = N / 2;

	// Pre-calculate the twiddle factors
	auto       W     = signal<std::complex<T>>(f_s, N_over_2);
	const auto omega = 2 * static_cast<T>(M_PI) / N;
	for (size_t n = 0; n < N_over_2; ++n)
	{
		W[n] = std::complex<T>(std::cos(-omega * n), std::sin(-omega * n));
	}

	return impl::stockham_fft(x, W);
}

// C++ implementation of the Stockam FFT algorithm
template <typename T>
signal<std::complex<T>> stockham_fft(const signal<std::complex<T>>& x,
                                     const signal<std::complex<T>>& W)
{
	assert(impl::is_power_of_2(x.size()));

	const auto f_s      = x.sample_rate();
	const auto N        = x.size();
	const auto N_over_2 = N / 2;

	// The Stockham algorithm requires one vector for input/output data and
	// another as a temporary workspace
	auto a = signal<std::complex<T>>(x);
	auto b = signal<std::complex<T>>(f_s, N);

	// Set the spacing between twiddle factors used at the first stage
	auto W_stride = N / 2;

	// Loop through each stage of the FFT
	for (size_t stride = 1; stride < N; stride *= 2)
	{
		// Loop through the individual FFTs of each stage
		for (size_t m = 0; m < N_over_2; m += stride)
		{
			const auto m_times_2 = m * 2;

			// Perform each individual FFT
			for (size_t n = 0; n < stride; ++n)
			{
				// Calculate the input indexes
				const auto a_index_1 = n + m;
				const auto a_index_2 = a_index_1 + N_over_2;

				// Calculate the output indexes
				const auto b_index_1 = n + m_times_2;
				const auto b_index_2 = b_index_1 + stride;

				// Perform the FFT
				const auto tmp1 = a[a_index_1];
				const auto tmp2 = W[n * W_stride] * a[a_index_2];

				// Sum the results
				b[b_index_1] = tmp1 + tmp2;
				b[b_index_2] = tmp1 - tmp2;  // (>*.*)> symmetry! <(*.*<)
			}
		}

		// Spacing between twiddle factors is half for the next stage
		W_stride /= 2;

		// Swap the data (output of this stage is input of the next)
		swap(a, b);
	}

	return a;
}

} /* namespace tnt::dsp::impl */