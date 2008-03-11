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

	// standard tuning
	tuning[0] = 82.4f;
	tuning[1] = 110.0f;
	tuning[2] = 146.8f;
	tuning[3] = 196.0f;
	tuning[4] = 246.92f;
	tuning[5] = 329.6f;
}

float Scorer::Tune(Fret &f)
{
	return tuning[f.s] * pow(twelfthroot, f.f);
}

int Scorer::Test(list<Fret> &frets)
{
	double* buf = cap->readLast(samples);
	double* spec = new double[samples];

	// compute frequency spectrum
	realfft (buf, samples, spec);

	// do the tests

	// for now, just check the bass note
	double ffreq=0.0; 
	double norm;
	double maxnorm=0.0; 
	int li=samples-2;

	// find the dominant frequency
    for (int i=1; i<li; i+=2) 
	{
		norm = (spec[i] * spec[i]) + (spec[i+1] * spec[i+1]);

		if (norm>maxnorm)
		{
			maxnorm = norm; 
			ffreq = i;
		}
	}
	double frequency;
	if(maxnorm == 0.0) 
		frequency = 0.0;
	else if(maxnorm < 1.1) 
		frequency = -1.0;
	else
		frequency = 2.0 * ffreq * (double)SAMPLE_RATE / (double)samples;

	// cleanup
	delete spec;

	// compare
	double bass = 100000.0;
	double freq;
	list<Fret>::iterator it;
	for(it = frets.begin(); it != frets.end(); it++)
	{
		freq = Tune(*it);
		if(freq < bass)
			bass = freq;
	}

	double delta = fabs(bass - frequency);
	if(delta < 50.0)
	{
		for(it = frets.begin(); it != frets.end(); it++)
		{
			it->hit = true;
		}
		return 1;
	}

	return 0;
}
