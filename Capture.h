#ifndef CAPTURE
#define CAPTURE

#include "pa_asio.h"

#include <OpenThreads/Thread>
#include <iostream>
#include <math.h>

using namespace std;

#define SAMPLE_RATE            (48000)

typedef struct WireConfig_s
{
    int isInputInterleaved;
    int isOutputInterleaved;
    int numInputChannels;
    int numOutputChannels;
    int framesPerCallback;
} WireConfig_t;

#define USE_FLOAT_INPUT        (0)
#define USE_FLOAT_OUTPUT       (0)

/* Latencies set to defaults. */

#if USE_FLOAT_INPUT
    #define INPUT_FORMAT  paFloat32
    typedef float INPUT_SAMPLE;
#else
    #define INPUT_FORMAT  paInt16
    typedef short INPUT_SAMPLE;
#endif

#if USE_FLOAT_OUTPUT
    #define OUTPUT_FORMAT  paFloat32
    typedef float OUTPUT_SAMPLE;
#else
    #define OUTPUT_FORMAT  paInt16
    typedef short OUTPUT_SAMPLE;
#endif

#define INPUT_DEVICE           (Pa_GetDefaultInputDevice())
#define OUTPUT_DEVICE          (Pa_GetDefaultOutputDevice())

class Capture
{
public:
	Capture(int s);

	void start();	// begin capture and playback

	void stop();	// stop capture

	double* readLast(int l);	// get the last l samples captured
	int readVol();				// get the current volume

	void tick();

protected:
    PaStream *stream;

	double* last;

	/* This routine will be called by the PortAudio engine when audio is needed.
	** It may be called at interrupt level on some machines so don't do anything
	** that could mess up the system like calling malloc() or free().
	*/
	static int wireCallback( const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void *userData );
};

#endif
