// Global defines

#define MAX_STRINGS   12
#define MAX_FRETS     24
#define NUMFRETS      5

// Global utility functions

//#include <qglobal.h>
//#include <config.h>
//#include <kdebug.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>

#include "VectorMath.h"

using namespace std;

class QString;

QString midi_patch_name(int);

extern QString drum_abbr[128];
