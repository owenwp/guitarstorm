/*
 * "fourier.c", Pjotr '87.
 */

#include	"fourier.h"

void Fourier (COMPLEX *in, int n, COMPLEX *out)
{
	unsigned r;

	if ((r = Radix (n)) < n)
		split (in, r, n / r, out);
	join (in, n / r, n, out);
}

static int Radix (int n)
{
	unsigned r;

	if (n < 2)
		return 1;

	for (r = 2; r < n; r++)
		if (n % r == 0)
			break;
	return r;
}

static void split (COMPLEX *in, int r, int m, COMPLEX *out)
{
	register unsigned k, s, i, j;

	for (k = 0, j = 0; k < r; k++)
		for (s = 0, i = k; s < m; s++, i += r, j++)
			out [j] = in [i];

	for (k = 0; k < r; k++, out += m, in += m)
		Fourier (out, m, in);
}

static void join (COMPLEX *in, int m, int n, COMPLEX *out)
{
	register unsigned i, j, jk, s;

	for (s = 0; s < m; s++)
		for (j = s; j < n; j += m) {
			out [j] = in [s];
			for (i = s + m, jk = j; i < n; i += m, jk += j)
				c_add_mul (out [j], in [i], W (n, jk));
		}
}
