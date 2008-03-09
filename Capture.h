#ifndef CAPTURE
#define CAPTURE

#include "pa_asio.h"

#include <OpenThreads/Thread>
#include <iostream>

/* #define SAMPLE_RATE  (17932) // Test failure to open with this value. */
#define SAMPLE_RATE  (44100)
#define FRAMES_PER_BUFFER (64)
#define NUM_CHANNELS    (1)
/* #define DITHER_FLAG     (paDitherOff)  */
#define DITHER_FLAG     (0) /**/

/* Select sample format. */
#define PA_SAMPLE_TYPE  paFloat32

class Capture : public OpenThreads::Thread
{
public:
	void run();

	void stop();

protected:
	bool stopped;
};

#endif
