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
#include "Audio.h"

double gInOutScaler = 1.0;
#define CONVERT_IN_TO_OUT(in)  ((OUTPUT_SAMPLE) ((in) * gInOutScaler))
#define PI (3.141592653589793)

// these have to be global, because the callback can't access the object's memory space
WireConfig_t config;
FILE* outfile;
const int rblen = SAMPLE_RATE;
int rbi = 0;
int rblast = 0;
bool mtick = false;
double tickfreq = 2400.0;
double tickvol = 8000.0;
const int ticklen = 128;
double backvol = 1.0f;
int currvol = 0;
float ringBuffer[rblen];
int ticksamp[ticklen];
int backBuffer[rblen];
Backing back;

// options
int samples = 128;
int srate = 48000;
int guitarOptVol = 100;
int backingOptVol = 100;
int metronomeOptVol = 50;
int inputDevice = 0;
int outputDevice = 0;
bool recordRaw = false;

// static members
PaStream* Audio::stream = NULL;
double* Audio::last = NULL;
string Audio::music;
bool Audio::initialized = false;
bool Audio::playing = false;

void Audio::init()
{
	for(int i=0; i<ticklen; i++)
	{
		ticksamp[i] = sin(2.0 * PI * (i * tickfreq / SAMPLE_RATE));
	}

	PaError err = Pa_Initialize();
    if( err != paNoError ) 
		throw string("Audio failed to initialize");

	const   PaDeviceInfo *deviceInfo;
	int numDevices = Pa_GetDeviceCount();
	if (numDevices > 9) numDevices = 9;
	Options::instance->devices = new string[10];
    for(int i=0; i<numDevices; i++)
    {
        deviceInfo = Pa_GetDeviceInfo( i );
		Options::instance->devices[i] = deviceInfo->name;
    }

	initialized = true;
}

int Audio::readVol()
{
	return currvol;
}

double* Audio::readLast(int l)
{
	last = new double[l];

	double min =  10000000000.0;
	double max = -10000000000.0;

	int i = rblast-1, j;
	for(j=l-1; j>=0; j--)
	{
		if(i<0) 
			i = rblen-1;

		last[j] = ringBuffer[i--];

		if(last[j] > max)
			max = last[j];
		if(last[j] < min)
			min = last[j];
	}
	double dif = max - min;
	// normalize array
	for(j=0; j<l; j++)
	{
		last[j] = (2 * last[j] - dif) / dif;
	}
	return last;
}

void Audio::tick()
{
	mtick = true;
}

int Audio::wireCallback( const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void *userData )
{
    INPUT_SAMPLE *in;
    OUTPUT_SAMPLE *out;
    int inStride;
    int outStride;
	int backStride = 2;
    int inDone = 0;
    int outDone = 0;
    unsigned int i;
    int inChannel, outChannel, backChannel;

	// adjust volume
	guitarOptVol = Options::instance->guitarVolume;
	backingOptVol = Options::instance->backingVolume;
	metronomeOptVol = Options::instance->metronomeVolume;

    /* This may get called with NULL inputBuffer during initial setup. */
    if( inputBuffer == NULL) return 0;

	if(!recordRaw && playing && back.Valid())
		back.GetSamples(backBuffer, framesPerBuffer * 2);

    inChannel=0, outChannel=0, backChannel = 0;
    while( !(inDone && outDone) )
    {
        if( config.isInputInterleaved )
        {
            in = ((INPUT_SAMPLE*)inputBuffer) + inChannel;
            inStride = config.numInputChannels;
        }
        else
        {
            in = ((INPUT_SAMPLE**)inputBuffer)[inChannel];
            inStride = 1;
        }

        if( config.isOutputInterleaved )
        {
            out = ((OUTPUT_SAMPLE*)outputBuffer) + outChannel;
            outStride = config.numOutputChannels;
        }
        else
        {
            out = ((OUTPUT_SAMPLE**)outputBuffer)[outChannel];
            outStride = 1;
        }

		currvol = 0;
        for( i=0; i<framesPerBuffer; i++ )
        {
			if(rbi >= rblen)
				rbi = 0;

			if(*in > currvol)
				currvol = *in;

			// create output
            *out = CONVERT_IN_TO_OUT( *in * (guitarOptVol/100.0) );

			if(mtick && i<ticklen)
				*out += tickvol * (metronomeOptVol/100.0) * ticksamp[i];

			if(!recordRaw && playing && back.Valid())
				*out += backvol * (backingOptVol/100.0) * backBuffer[backChannel + i * backStride];

			if(inChannel == 0)
			{
				ringBuffer[rbi++] = *out;

				if(recordRaw)
					fwrite( out, sizeof(OUTPUT_SAMPLE), 1, outfile );
			}

            out += outStride;
            in += inStride;
			rblast = rbi;
        }

        if(inChannel < (config.numInputChannels - 1)) inChannel++;
        else inDone = 1;
		if(outChannel < (config.numOutputChannels - 1)) {outChannel++; backChannel++;}
        else outDone = 1;
    }

	mtick = false;
    return 0;
}

void Audio::stopCapture()
{
	if(recordRaw)
		fclose(outfile);
	if(stream) Pa_CloseStream( stream );
	Pa_Terminate();
}

void Audio::startCapture()
{
	if(!initialized)
		return;

	PaError err = paNoError;
    //WireConfig_t CONFIG;
    //config = &CONFIG;
    int configIndex = 0;
	playing = false;

	// read options
	char str[10];
	strncpy(str, (char*)Options::instance->bsizes[Options::instance->bufferSize].c_str(), 10);
	samples = atoi(str);
	strncpy(str, (char*)Options::instance->srates[Options::instance->sampleRate].c_str(), 10);
	srate = atoi(str);
	
	inputDevice = Options::instance->inputDevice;
	outputDevice = Options::instance->outputDevice;

	memset(ringBuffer, 0, rblen);

    if( INPUT_FORMAT == OUTPUT_FORMAT )
    {
        gInOutScaler = 1.0;
    }
    else if( (INPUT_FORMAT == paInt16) && (OUTPUT_FORMAT == paFloat32) )
    {
        gInOutScaler = 1.0/32768.0;
    }
    else if( (INPUT_FORMAT == paFloat32) && (OUTPUT_FORMAT == paInt16) )
    {
        gInOutScaler = 32768.0;
    }
     
	config.isInputInterleaved = 0; 
    config.isOutputInterleaved = 0;
    config.numInputChannels = 2; 
    config.numOutputChannels = 2;
    config.framesPerCallback = samples;
                
	int c;
    err = paNoError;
    PaStreamParameters inputParameters, outputParameters;
   
    inputParameters.device = inputDevice;           
    if (inputParameters.device == paNoDevice) 
        throw string("No ASIO Input Device Detected");
    
    inputParameters.channelCount = config.numInputChannels;
    inputParameters.sampleFormat = INPUT_FORMAT | (config.isInputInterleaved ? 0 : paNonInterleaved);
    inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;

    outputParameters.device = outputDevice;          
    if (outputParameters.device == paNoDevice) 
       throw string("No ASIO Output Device Detected");
    
    outputParameters.channelCount = config.numOutputChannels;
    outputParameters.sampleFormat = OUTPUT_FORMAT | (config.isOutputInterleaved ? 0 : paNonInterleaved);
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

	Backing::SampleRate(srate);

	if(recordRaw)
	{
		stringstream fname;
		fname	<< "rec " << sizeof(OUTPUT_SAMPLE)*8 << "bit " 
				<< srate << "hz mono.raw";
		outfile = fopen(fname.str().c_str(), "wb");
	}

    err = Pa_OpenStream(
              &stream,
              &inputParameters,
              &outputParameters,
              srate,
              config.framesPerCallback, /* frames per buffer */
              paClipOff, /* we won't output out of range samples so don't bother clipping them */
			  wireCallback,
              NULL );
    if( err != paNoError )
		throw string("Failed to open stream");
    
    err = Pa_StartStream( stream );
}

// mp3 playback
int Audio::openMusic(string n)
{
	if(!initialized)
		return -1;

	music = n;
	back.Open(n);

	playing = false;

	return !back.Valid();
}

int Audio::playMusic(string n)
{
	if(n.length())
	{
		openMusic(n);
	}
	
	if(back.Valid())
		playing = true;

	return !back.Valid();
}

void Audio::pauseMusic()
{
	playing = false;
}

void Audio::advanceMusic(float s)
{
	if(!recordRaw) 
		back.Advance(s);
}

float Audio::timeMusic()
{
	return back.GetTime();
}

void Audio::stopMusic()
{
	playing = false;
	back.Close();
}
