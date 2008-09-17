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
#ifndef AUDIO
#define AUDIO

#include "pa_asio.h"

#include <OpenThreads/Thread>
#include <iostream>
#include <math.h>
#include "Options.h"
#include "Backing.h"

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

class Audio
{
public:
	// initialization
	static void init();

	// audio capture
	static void startCapture();	// begin capture and playback
	static void stopCapture();	// stop capture

	// mp3 playback
	static int openMusic(string n);		// play an mp3, continue current song if no argument
	static int playMusic(string n="");	// play an mp3, continue current song if no argument
	static void pauseMusic();			// pauses playback, call playMusic() to resume
	static void advanceMusic(float s);	// advance specific number of seconds
	static float timeMusic();			// return current playback time in seconds
	static void stopMusic();			// stops playback		

	// data read
	static double* readLast(int l);	// get the last l samples captured
	static int readVol();				// get the current volume

	// metronome
	static void tick();

protected:
	Audio(){}

    static PaStream* stream;
	static double* last;
	static string music;
	static bool initialized;
	static bool playing;

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
