#include "approx.hpp"

#include <catch2/catch_template_test_macros.hpp>
#include <cmath>
#include <complex>
#include <tnt/dsp/fourier_transform.hpp>
#include <tnt/dsp/signal.hpp>
#include <tnt/dsp/signal_generator.hpp>

// TODO: Test fourier_transform/inverse_fourier_transform of larger sizes for speed

using namespace tnt;

TEMPLATE_TEST_CASE("fourier_transform", "[fourier_transform]", double, float)
{
	SECTION("Fourier transform of a real signal")
	{
		for (size_t N = 1; N <= 10; ++N)
		{
			const auto g  = dsp::signal_generator<TestType>(1000, N);
			const auto x  = g.cosine(100);
			const auto X  = dft(x);
			const auto X2 = dsp::fourier_transform(x);

			REQUIRE(X.size() == N);
			REQUIRE(X2.size() == N);

			for (size_t m = 0; m < N; ++m)
			{
				CHECK(X[m].real() == approx(X2[m].real()));
				CHECK(X[m].imag() == approx(X2[m].imag()));
			}
		}
	}

	SECTION("Fourier transform of a complex signal")
	{
		for (size_t N = 1; N <= 10; ++N)
		{
			const auto g  = dsp::signal_generator<TestType>(1000, N);
			const auto x  = dsp::complex_signal(g.cosine(100), g.sine(100));
			const auto X  = dft(x);
			const auto X2 = dsp::fourier_transform(x);

			REQUIRE(X.size() == N);
			REQUIRE(X2.size() == N);

			for (size_t m = 0; m < N; ++m)
			{
				CHECK(X[m].real() == approx(X2[m].real()));
				CHECK(X[m].imag() == approx(X2[m].imag()));
			}
		}
	}
}

TEMPLATE_TEST_CASE("inverse_fourier_transform", "[inverse_fourier_transform]", double, float)
{
	SECTION("Inverse fourier transform of a real signal")
	{
		for (size_t N = 1; N <= 10; ++N)
		{
			const auto g  = dsp::signal_generator<TestType>(1000, N);
			const auto x  = g.cosine(100);
			auto       X  = dsp::fourier_transform(x);
			auto       x2 = dsp::inverse_fourier_transform(X);

			REQUIRE(x.size() == N);
			REQUIRE(x2.size() == N);

			for (size_t n = 0; n < N; ++n)
			{
				CHECK(x[n] == approx(x2[n].real()));
			}
		}
	}

	SECTION("Inverse fourier transform of a complex signal")
	{
		for (size_t N = 1; N <= 10; ++N)
		{
			const auto g  = dsp::signal_generator<TestType>(1000, N);
			const auto x  = dsp::complex_signal(g.cosine(100), g.sine(100));
			const auto X  = dsp::fourier_transform(x);
			const auto x2 = dsp::inverse_fourier_transform(X);

			REQUIRE(x.size() == N);
			REQUIRE(x2.size() == N);

			for (size_t m = 0; m < N; ++m)
			{
				CHECK(x[m].real() == approx(x2[m].real()));
				CHECK(x[m].imag() == approx(x2[m].imag()));
			}
		}
	}
}

// Implementation of slow Fourier transform to compare against
template <typename T>
dsp::signal<std::complex<T>> dft(const dsp::signal<T>& x)
{
	const auto f_s = x.sample_rate();
	const auto N   = x.size();

	auto X = dsp::signal<std::complex<T>>(f_s, N);

	// Take advantage of DFT symmetry when dealing with real input signals
	// Only the first N/2 + 1 outputs are unique
	for (size_t k = 0; k < N / 2 + 1; ++k)
	{
		for (size_t n = 0; n < N; ++n)
		{
			X[k] += x[n] * std::polar(static_cast<T>(1), -2 * static_cast<T>(M_PI) * n * k / N);
		}

		// X(N-k) = X(k)* for k = 1 -> N/2
		if (k != 0)
		{
			X[N - k] = std::conj(X[k]);
		}
	}

	return X;
}

// Implementation of slow fourier transform to compare against
template <typename T>
dsp::signal<std::complex<T>> dft(const dsp::signal<std::complex<T>>& x)
{
	const auto f_s = x.sample_rate();
	const auto N   = x.size();

	auto X = dsp::signal<std::complex<T>>(f_s, N);

	for (size_t k = 0; k < N; ++k)
	{
		for (size_t n = 0; n < N; ++n)
		{
			X[k] += x[n] * std::polar(static_cast<T>(1), -2 * static_cast<T>(M_PI) * n * k / N);
		}
	}

	return X;
}