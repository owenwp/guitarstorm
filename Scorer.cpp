#include "Scorer.h"

Scorer::Scorer(Capture* c) : 
samples(1024)
{
	cap = c;
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

	lastfreq = 0.0f;

	spec = new double[samples];

	// twelvth root lookup table
	for(int i=-1; i<49; i++)
	{
		freqcoeffs[i+1] = pow(twelfthroot, i/2.0);
	}
}

float* Scorer::Tune(Fret &f)
{
	float range[3];

	range[0] = tuning[f.s] * freqcoeffs[f.f * 2];
	range[1] = tuning[f.s] * freqcoeffs[f.f * 2 + 1];
	range[2] = tuning[f.s] * freqcoeffs[f.f * 2 + 2];

	return range;
}

int Scorer::Test(list<Fret> &frets)
{
	double* buf = cap->readLast(samples);
	
	for(int k=0; k < samples; k++)
	{
		//buf[k] = sin(2.0 * PI * (k * 800.0 / SAMPLE_RATE));
	}

	// compute frequency spectrum
	realfft (buf, samples, spec);

	// do the tests

	// for now, just check the bass note
	double ffreq = 0.0; 
	double norm;
	double maxnorm = 0.0; 
	int li = (samples - 1)/2;

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
	//else
		frequency = 0.5 * ffreq * (double)SAMPLE_RATE / (double)samples;

	lastfreq = frequency;

	// compare
	float* bass = 0;
	float* freq;
	list<Fret>::iterator it;
	for(it = frets.begin(); it != frets.end(); it++)
	{
		freq = Tune(*it);
		if(!bass || freq[1] < bass[1])
			bass = freq;
	}

	if(frequency > bass[0] && frequency < bass[2])
	{
		for(it = frets.begin(); it != frets.end(); it++)
		{
			it->hit = true;
		}
		return 1;
	}

	return 0;
}
