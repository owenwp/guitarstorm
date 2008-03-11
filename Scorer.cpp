#include "Scorer.h"

Scorer::Scorer(Capture* c)
{
	cap = c;
	samples = 1000;
	sstates[0] = -1;
	sstates[1] = -1;
	sstates[2] = -1;
	sstates[3] = -1;
	sstates[4] = -1;
	sstates[5] = -1;
}

int Scorer::Test(list<Fret> &frets)
{
	double* buf = cap->readLast(samples);
	double* spec = new double[samples];

	list<Fret>::iterator it;
	for(it = frets.begin(); it != frets.end(); it++)
	{
		it->hit = true;
	}

	//realfft (buf, samples, spec);

	return 1;
}
