#ifndef CAPTURE
#define CAPTURE

#include "Framework/Framework.h"
//#include "playstream.cpp"

#include <OpenThreads/Thread>

class Capture : public OpenThreads::Thread
{
public:
	void run();

	void stop();

protected:
	bool stopped;
};

#endif
