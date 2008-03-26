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

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osg/io_utils>

#include <osg/MatrixTransform>
#include <osg/Geode>
#include <osg/Group>
#include <osg/Switch>
#include <osg/Notify>
#include <osg/Geometry>

#include <osgText/Text>

#include "fft/fft.h"
#include "Capture.h"

#include <math.h>

using namespace std;

const double twelfthroot = 1.05946309;

#define PI (3.141592653589793)

struct Fret : public osg::Referenced
{
	int s;
	int f;
	osg::ref_ptr<osg::Switch> m;
	bool hit;
};

class Scorer : public OpenThreads::Thread
{
public:
	Scorer(Capture* c);

	void Test(list<Fret> &f);
	bool HasResults() { return active; }
	list<Fret>& GetResult();

	void tick() { cap->tick(); }
	int getVolume() { return cap->readVol(); }

	void run();

	float lastfreq;
	string lastnote;

private:
	// returns an array consisting of the lower range, perfect pitch, and upper range of the note
	float* Tune(Fret &f);
	int Chroma(Fret &f);

	list<Fret> frets;

	float freqcoeffs[50];

	Capture* cap;
	int sstates[6];
	float tuning[6];
	int chrom[6];
	const int samples;

	bool active;
	bool taken;

	double *spec;
};

#endif
