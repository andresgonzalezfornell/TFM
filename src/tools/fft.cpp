/**
 * @name	fft.h
 * @author	Cooley-Tukey
 * @brief	Fast Fourier Transform.
 */

#include "fft.h"

CArray fft(CArray &x) {
	const double pi = 3.141592653589793238460;
	// DFT
	unsigned int N = x.size(), k = N, n;
	double thetaT = pi / N;
	Complex phiT = Complex(cos(thetaT), sin(thetaT)), T;
	CArray y = x;
	while (k > 1) {
		n = k;
		k >>= 1;
		phiT = phiT * phiT;
		T = 1.0L;
		for (unsigned int l = 0; l < k; l++) {
			for (unsigned int a = l; a < N; a += n) {
				unsigned int b = a + k;
				Complex t = y[a] - y[b];
				y[a] += y[b];
				y[b] = t * T;
			}
			T *= phiT;
		}
	}
	// Decimate
	unsigned int m = (unsigned int) log2(N);
	for (unsigned int a = 0; a < N; a++) {
		unsigned int b = a;
		// Reverse bits
		b = (((b & 0xaaaaaaaa) >> 1) | ((b & 0x55555555) << 1));
		b = (((b & 0xcccccccc) >> 2) | ((b & 0x33333333) << 2));
		b = (((b & 0xf0f0f0f0) >> 4) | ((b & 0x0f0f0f0f) << 4));
		b = (((b & 0xff00ff00) >> 8) | ((b & 0x00ff00ff) << 8));
		b = ((b >> 16) | (b << 16)) >> (32 - m);
		if (b > a) {
			Complex t = y[a];
			y[a] = y[b];
			y[b] = t;
		}
	}
	//// Normalize (This section make it not working correctly)
	//Complex f = 1.0 / sqrt(N);
	//for (unsigned int i = 0; i < N; i++)
	//	y[i] *= f;
	return y;
}

// inverse fft (in-place)
void ifft(CArray& x) {
	// conjugate the complex numbers
	x = x.apply(std::conj);
	// forward fft
	fft(x);
	// conjugate the complex numbers again
	x = x.apply(std::conj);
	// scale the numbers
	x /= x.size();
}
