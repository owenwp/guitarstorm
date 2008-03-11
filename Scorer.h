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
private:
	float Tune(Fret &f);

	Capture* cap;
	int sstates[6];
	float tuning[6];
	int samples;
};

#endif
