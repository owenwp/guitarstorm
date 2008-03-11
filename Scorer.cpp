#include "Scorer.h"

Scorer::Scorer(Capture* c)
{
	cap = c;
	sstates[0] = -1;
	sstates[1] = -1;
	sstates[2] = -1;
	sstates[3] = -1;
	sstates[4] = -1;
	sstates[5] = -1;
}

int Scorer::Test(list<Fret> &frets)
{
	double* buf = cap->readLast(1000);
	double* spec = new double[1000];

	//realfft (buf, 1000, spec);

	return 0;
}
