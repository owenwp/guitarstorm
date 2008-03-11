#ifndef REALFFT
#define REALFFT

#include <iostream>
#include	"complex.h"
#include	"w.h"
#include	<math.h>
#include "fourier.h"

/*
 * W_init puts Wn ^ k (= e ^ (2pi * i * k / n)) in W_factors [k], 0 <= k < n.
 * If n is equal to Nfactors then nothing is done, so the same W_factors
 * array can used for several transforms of the same number of samples.
 * Notice the explicit calculation of sines and cosines, an iterative approach
 * introduces substantial errors.
 */
int W_init (int n);

/*
 * Reele forward fast fourier transform van n samples van in naar
 * amplitudes van out.
 * De cosinus komponent van de dc komt in out [0], dan volgen in
 * out [2 * i - 1] en out [2 * i] steeds resp. de cosinus en sinus
 * komponenten van de i-de harmonische.  Bij een even aantal samples
 * bevat out [n - 1] de cosinus komponent van de Nyquist frequentie. 
 * Extraatje: Na afloop is in onaangetast.
 */
void realfft (double *in, int n, double *out);

/*
 * Reele reverse fast fourier transform van amplitudes van in naar
 * n samples van out.
 * De cosinus komponent van de dc staat in in [0], dan volgen in
 * in [2 * i - 1] en in [2 * i] steeds resp. de cosinus en sinus
 * komponenten van de i-de harmonische.  Bij een even aantal samples
 * bevat in [n - 1] de cosinus komponent van de Nyquist frequentie. 
 * Extraatje: Na afloop is in onaangetast.
 */
void realrft (double *in, int n, double *out);

/*
 * Forward Fast Fourier Transform on the n samples of complex array in.
 * The result is placed in out.  The number of samples, n, is arbitrary.
 * The W-factors are calculated in advance.
 */
int fft (COMPLEX *in, int n, COMPLEX *out);

/*
 * Reverse Fast Fourier Transform on the n complex samples of array in.
 * The result is placed in out.  The number of samples, n, is arbitrary.
 * The W-factors are calculated in advance.
 */
int rft (COMPLEX *in, int n, COMPLEX *out);

#endif
