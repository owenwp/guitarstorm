/*
 * "ft.c", Pjotr '87.
 */

#include	"complex.h"
#include	"w.h"

/*
 * Forward Fast Fourier Transform on the n samples of complex array in.
 * The result is placed in out.  The number of samples, n, is arbitrary.
 * The W-factors are calculated in advance.
 */
int fft (in, n, out)
COMPLEX *in;
unsigned n;
COMPLEX *out;
{
	unsigned i;

	for (i = 0; i < n; i++)
		c_conj (in [i]);
	
	if (W_init (n) == -1)
		return -1;

	Fourier (in, n, out);

	for (i = 0; i < n; i++) {
		c_conj (out [i]);
		c_realdiv (out [i], n);
	}

	return 0;
}

/*
 * Reverse Fast Fourier Transform on the n complex samples of array in.
 * The result is placed in out.  The number of samples, n, is arbitrary.
 * The W-factors are calculated in advance.
 */
rft (in, n, out)
COMPLEX *in;
unsigned n;
COMPLEX *out;
{
	if (W_init (n) == -1)
		return -1;

	Fourier (in, n, out);

	return 0;
}
