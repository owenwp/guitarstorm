/*
 * "realfft.c", Pjotr '87
 */

/*
 * Bevat funkties realfft en realrft die resp. forward en reverse fast fourier
 * transform op reele samples doen.  Gebruikt pakket fft(3).
 */

#include "fft.h"

COMPLEX *W_factors = 0;		/* array of W-factors */
unsigned Nfactors = 0;		/* number of entries in W-factors */

int W_init (int n)
{
	//char *malloc ();
#	define pi	3.1415926535897932384626434
	unsigned k;

	if (n == Nfactors)
		return 0;
	if (Nfactors != 0 && W_factors != 0)
		free ((char *) W_factors);
	if ((Nfactors = n) == 0)
		return 0;
	if ((W_factors = (COMPLEX *) malloc (n * sizeof (COMPLEX))) == 0)
		return -1;

	for (k = 0; k < n; k++) {
		c_re (W_factors [k]) = cos (2 * pi * k / n);
		c_im (W_factors [k]) = sin (2 * pi * k / n);
	}

	return 0;
}

void realfft (double *in, int n, double *out)
{
	COMPLEX *c_in, *c_out;
	unsigned int i;

	if (n == 0 ||
	    (c_in = (COMPLEX *) malloc (n * sizeof (COMPLEX))) == 0 ||
	    (c_out = (COMPLEX *) malloc (n * sizeof (COMPLEX))) == 0)
		return;
	
	for (i = 0; i < n; i++) {
		c_re (c_in [i]) = in [i];
		c_im (c_in [i]) = 0;
	}

	fft (c_in, n, c_out);

	out [0] = c_re (c_out [0]);		/* cos van dc */
	for (i = 1; i < (n + 1) / 2; i++) {	/* cos/sin i-de harmonische */
		out [2 * i - 1] = c_re (c_out [i]) * 2;
		out [2 * i] = c_im (c_out [i]) * -2;
	}
	if (n % 2 == 0)				/* cos van Nyquist */
		out [n - 1] = c_re (c_out [n / 2]);

	free ((char *) c_in);
	free ((char *) c_out);
}

void realrft (double *in, int n, double *out)
{
	COMPLEX *c_in, *c_out;
	unsigned i;

	if (n == 0 ||
	    (c_in = (COMPLEX *) malloc (n * sizeof (COMPLEX))) == 0 ||
	    (c_out = (COMPLEX *) malloc (n * sizeof (COMPLEX))) == 0)
		return;
	
	c_re (c_in [0]) = in [0];		/* dc */
	c_im (c_in [0]) = 0;
	for (i = 1; i < (n + 1) / 2; i++) {	/* geconj. symm. harmonischen */
		c_re (c_in [i]) = in [2 * i - 1] / 2;
		c_im (c_in [i]) = in [2 * i] / -2;
		c_re (c_in [n - i]) = in [2 * i - 1] / 2;
		c_im (c_in [n - i]) = in [2 * i] / 2;
	}
	if (n % 2 == 0) {			/* Nyquist */
		c_re (c_in [n / 2]) = in [n - 1];
		c_im (c_in [n / 2]) = 0;
	}

	rft (c_in, n, c_out);

	for (i = 0; i < n; i++)
		out [i] = c_re (c_out [i]);

	free ((char *) c_in);
	free ((char *) c_out);
}

int fft (COMPLEX *in, int n, COMPLEX *out)
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

int rft (COMPLEX *in, int n, COMPLEX *out)
{
	if (W_init (n) == -1)
		return -1;

	Fourier (in, n, out);

	return 0;
}
