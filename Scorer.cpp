#include "Scorer.h"

/*
License for gTune (c) 1999 by Michael Celli and Harold C.L. Baur.

The following are the terms which the program "gTune" including all its source
files and the resultant binary files fall under:

	- You may distribute gTune and all or any piece of it so long as this 
license is included and remains unchanged and credit to all the authors 
remains.  A fee may be charged for distribution of gTune so long as the above 
terms are met.  Distribution of gTune or any piece of gTune in any way shape or
form must be under this license.

	- You may modify gTune and distribute any modifications as long as the 
source code to your modifications are included in any distribution of these 
modifications, and the terms outlined in the above section are met.  This 
means that any modified distribution of gTune or any piece of gTune must 
contain this license with no modifications to the license, be released under 
this license, include the source code to the modifications, and credit to the 
original authors must be stated explicitly.

	- gTune has no warranty whatsoever.  gTune is provided "as is" without 
warranty of any kind.  Should gTune malfunction or cause dammage, you assume 
the cost of servicing or dammages caused.  The authors of gTune or any 
party who may distribute or modify gTune cannot be held accountable for any of
its actions as all accountability is assumed by the user.  Liability for 
dammages caused by this program is assumed entirely by the user.

	- If distribution or usage of this program under this license is 
restricted in any country for any reason including patent or copyright 
interferences, then this software may not be distributed or used in such a 
country.

End of Terms and Restrictions.
*/

char *note[]={
    "A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A"
};

const int sampleBits = 11;	// changing this may affect frequency calculations

Scorer::Scorer(Capture* c) : 
samples(2 << sampleBits)	
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

	chrom[0] = 7;
	chrom[1] = 0;
	chrom[2] = 5;
	chrom[3] = 10;
	chrom[4] = 2;
	chrom[5] = 7;

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
	float* range = new float[3];

	range[0] = tuning[f.s] * freqcoeffs[f.f * 2];
	range[1] = tuning[f.s] * freqcoeffs[f.f * 2 + 1];
	range[2] = tuning[f.s] * freqcoeffs[f.f * 2 + 2];

	return range;
}

int Scorer::Chroma(Fret &f)
{
	int c = chrom[f.s] + f.f;
	if(c > 12) c -= 12;
	return c;
}

int Scorer::Test(list<Fret> &frets)
{
	double* buf = cap->readLast(samples);
	
	/*
	// use for test tones
	double f1 = 600.0;
	double f2 = 1600.0;
	for(int k=0; k < samples; k++)
	{
		buf[k] = 0.6 * sin(2.0 * PI * (k * f1 / SAMPLE_RATE)) +
				 0.4 * sin(2.0 * PI * (k * f2 / SAMPLE_RATE)) ;
	}
	*/

	// compute frequency spectrum
	realfft (buf, samples, spec);

	// do the tests

	// for now, just check the bass note
	double ffreq = 0.0; 
	double norm;
	double maxnorm = -100.0; 
	float li = (int)(samples - 1)/2;

	// compare
	float* bass = 0;	
	int bassn = 0;
	float* freq;
	list<Fret>::iterator it;
	for(it = frets.begin(); it != frets.end(); it++)
	{
		freq = Tune(*it);
		if(!bass || freq[1] < bass[1])
		{
			bass = freq;
			bassn = Chroma(*it);
		}
		else
			delete[] freq;
	}
	float target = bass[1];
	double frequency;

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

	frequency = 2.0 * ffreq * (double)SAMPLE_RATE / (double)samples;

	double a,r; 
	int o,n;

	// determine note
	a=log(frequency/440.0)/log(2.0); 
	o=(int)floor(a); 
	r=(a-o)*12.0; 
	n=(int)(r+0.5);

	lastfreq = frequency;
	lastnote = string(note[n]);
	
	delete[] bass;

	if(n == bassn)
	{
		for(it = frets.begin(); it != frets.end(); it++)
		{
			it->hit = true;
		}
		return 1;
	}

	return 0;
}
