// Global defines

#define MAX_STRINGS   12
#define MAX_FRETS     24
#define NUMFRETS      5

// Global utility functions

//#include <qglobal.h>
//#include <config.h>
//#include <kdebug.h>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osg/io_utils>

#include <osg/MatrixTransform>
#include <osg/Geode>
#include <osg/Group>
#include <osg/Switch>
#include <osg/Notify>
#include <osg/Geometry>

#include <OpenThreads/Thread>

#include <osgText/Text>

#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

class QString;

QString midi_patch_name(int);

extern QString drum_abbr[128];
