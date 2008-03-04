/*
 * "fourier.c", Pjotr '87.
 */

#include	"complex.h"
#include	"w.h"

/*
 * Recursive (reverse) complex fast Fourier transform on the n
 * complex samples of array in, with the Cooley-Tukey method.
 * The result is placed in out.  The number of samples, n, is arbitrary.
 * The algorithm costs O (n * (r1 + .. + rk)), where k is the number
 * of factors in the prime-decomposition of n (also the maximum
 * depth of the recursion), and ri is the i-th primefactor.
 */
Fourier (in, n, out)
COMPLEX *in;
unsigned n;
COMPLEX *out;
{
	unsigned r;
	unsigned radix ();

	if ((r = radix (n)) < n)
		split (in, r, n / r, out);
	join (in, n / r, n, out);
}

/*
 * Give smallest possible radix for n samples.
 * Determines (in a rude way) the smallest primefactor of n.
 */
static unsigned radix (n)
unsigned n;
{
	unsigned r;

	if (n < 2)
		return 1;

	for (r = 2; r < n; r++)
		if (n % r == 0)
			break;
	return r;
}

/*
 * Split array in of r * m samples in r parts of each m samples,
 * such that in [i] goes to out [(i % r) * m + (i / r)].
 * Then call for each part of out Fourier, so the r recursively
 * transformed parts will go back to in.
 */
static split (in, r, m, out)
COMPLEX *in;
register unsigned r, m;
COMPLEX *out;
{
	register unsigned k, s, i, j;

	for (k = 0, j = 0; k < r; k++)
		for (s = 0, i = k; s < m; s++, i += r, j++)
			out [j] = in [i];

	for (k = 0; k < r; k++, out += m, in += m)
		Fourier (out, m, in);
}

/*
 * Sum the n / m parts of each m samples of in to n samples in out.
 * 		   r - 1
 * Out [j] becomes  sum  in [j % m] * W (j * k).  Here in is the k-th
 * 		   k = 0   k	       n		 k
 * part of in (indices k * m ... (k + 1) * m - 1), and r is the radix.
 * For k = 0, a complex multiplication with W (0) is avoided.
 */
static join (in, m, n, out)
COMPLEX *in;
register unsigned m, n;
COMPLEX *out;
{
	register unsigned i, j, jk, s;

	for (s = 0; s < m; s++)
		for (j = s; j < n; j += m) {
			out [j] = in [s];
			for (i = s + m, jk = j; i < n; i += m, jk += j)
				c_add_mul (out [j], in [i], W (n, jk));
		}
}
