/*
	This file is part of Guitar Storm.
	Copyright 2008 Zombie Process, Owen Pedrotti

    Guitar Storm is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Guitar Storm is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Guitar Storm.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef SCORER
#define SCORER

#include "fft/fft.h"
#include "Audio.h"
#include "Sprite.h"

#include <math.h>
#include <list>

using namespace std;

const double twelfthroot = 1.05946309;

#define PI (3.141592653589793)

struct Fret
{
	int s;
	int f;
	double t;
	Sprite* m;
	bool hit;
};

class Scorer
{
public:
	Scorer();

	void Test(list<Fret> &f);
	bool HasResults() { return active && !taken; }
	list<Fret>& GetResult();

	void run();

	float lastfreq;
	string lastnote;

private:
	// returns an array consisting of the lower range, perfect pitch, and upper range of the note
	float* Tune(Fret &f);
	int Chroma(Fret &f);

	list<Fret> frets;

	float freqcoeffs[50];

	int sstates[6];
	float tuning[6];
	int chrom[6];
	const int samples;

	bool active;
	bool taken;

	double *spec;
};

#endif
