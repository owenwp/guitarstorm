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

class Scorer
{
public:
	Scorer(Capture* c);

	int Test(list<Fret> &frets);

	float lastfreq;

private:
	// returns an array consisting of the lower range, perfect pitch, and upper range of the note
	float* Tune(Fret &f);

	float freqcoeffs[50];

	Capture* cap;
	int sstates[6];
	float tuning[6];
	const int samples;

	double *spec;
};

#endif
