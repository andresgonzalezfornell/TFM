#ifndef SRC_TOOLS_FFT_H_
#define SRC_TOOLS_FFT_H_

#include "complex"
#include "valarray"
#include "stdio.h"
#include "string"
#include "iostream"

using namespace std;

typedef std::complex<double> Complex;
typedef std::valarray<Complex> CArray;

CArray fft(CArray &x);

void ifft(CArray& x);

#endif /* SRC_TOOLS_FFT_H_ */
