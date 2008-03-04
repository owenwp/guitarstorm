/*
 * "realfft.c", Pjotr '87
 */

/*
 * Bevat funkties realfft en realrft die resp. forward en reverse fast fourier
 * transform op reele samples doen.  Gebruikt pakket fft(3).
 */

#include	"complex.h"

char *malloc ();

/*
 * Reele forward fast fourier transform van n samples van in naar
 * amplitudes van out.
 * De cosinus komponent van de dc komt in out [0], dan volgen in
 * out [2 * i - 1] en out [2 * i] steeds resp. de cosinus en sinus
 * komponenten van de i-de harmonische.  Bij een even aantal samples
 * bevat out [n - 1] de cosinus komponent van de Nyquist frequentie. 
 * Extraatje: Na afloop is in onaangetast.
 */
realfft (in, n, out)
double *in;
unsigned n;
double *out;
{
	COMPLEX *c_in, *c_out;
	unsigned i;

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

/*
 * Reele reverse fast fourier transform van amplitudes van in naar
 * n samples van out.
 * De cosinus komponent van de dc staat in in [0], dan volgen in
 * in [2 * i - 1] en in [2 * i] steeds resp. de cosinus en sinus
 * komponenten van de i-de harmonische.  Bij een even aantal samples
 * bevat in [n - 1] de cosinus komponent van de Nyquist frequentie. 
 * Extraatje: Na afloop is in onaangetast.
 */
realrft (in, n, out)
double *in;
unsigned n;
double *out;
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
