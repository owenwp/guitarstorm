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

//#include "fft/realfft.c"
#include "Capture.h"

using namespace std;

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
	Capture* cap;
	int sstates[6];
};

#endif
