#include "Capture.h"

double gInOutScaler = 1.0;
#define CONVERT_IN_TO_OUT(in)  ((OUTPUT_SAMPLE) ((in) * gInOutScaler))

// these have to be global, because the callback can't access the object's memory space
WireConfig_t config;
const int rblen = SAMPLE_RATE/10;
int rbi = 0;
int rblast = 0;
float ringBuffer[rblen];

double* Capture::readLast(int l)
{
	last = new double[l];

	double min =  10000000000.0;
	double max = -10000000000.0;

	int i = rblast-1, j;
	for(j=l-1; j>=0; j--)
	{
		if(i<0) 
			i = rblen-1;

		last[j] = ringBuffer[i++];

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

int Capture::wireCallback( const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void *userData )
{
    INPUT_SAMPLE *in;
    OUTPUT_SAMPLE *out;
    int inStride;
    int outStride;
    int inDone = 0;
    int outDone = 0;
	Capture* cap = static_cast<Capture*>(userData);
	// pointer does not evaluate correctly, possible this callback is in a separate memory space
	//WireConfig_t *config = cap->config;  
    unsigned int i;
    int inChannel, outChannel;

    /* This may get called with NULL inputBuffer during initial setup. */
    if( inputBuffer == NULL) return 0;

    inChannel=0, outChannel=0;
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

        for( i=0; i<framesPerBuffer; i++ )
        {
			if(rbi >= rblen)
				rbi = 0;

            *out = CONVERT_IN_TO_OUT( *in );

			if(outChannel == 0)
				ringBuffer[rbi++] = *out;

            out += outStride;
            in += inStride;
			rblast = rbi;
        }

        if(inChannel < (config.numInputChannels - 1)) inChannel++;
        else inDone = 1;
        if(outChannel < (config.numOutputChannels - 1)) outChannel++;
        else outDone = 1;
    }
    return 0;
}

void Capture::stop()
{
	Pa_CloseStream( stream );
	Pa_Terminate();
}

void Capture::start()
{
	PaError err = paNoError;
    //WireConfig_t CONFIG;
    //config = &CONFIG;
    int configIndex = 0;

	memset(ringBuffer, 0, rblen);

    err = Pa_Initialize();
    if( err != paNoError ) return;

    //printf("Please connect audio signal to input and listen for it on output!\n");
    //printf("input format = %lu\n", INPUT_FORMAT );
    //printf("output format = %lu\n", OUTPUT_FORMAT );
    //printf("input device ID  = %d\n", INPUT_DEVICE );
    //printf("output device ID = %d\n", OUTPUT_DEVICE );

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
     
	config.isInputInterleaved = 1; 
    config.isOutputInterleaved = 1;
    config.numInputChannels = 1; 
    config.numOutputChannels = 2;
    config.framesPerCallback = 128;
                
	int c;
    err = paNoError;
    PaStreamParameters inputParameters, outputParameters;
   
    inputParameters.device = INPUT_DEVICE;              /* default input device */
    if (inputParameters.device == paNoDevice) {
        return;
    }
    inputParameters.channelCount = config.numInputChannels;
    inputParameters.sampleFormat = INPUT_FORMAT | (config.isInputInterleaved ? 0 : paNonInterleaved);
    inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;

    outputParameters.device = OUTPUT_DEVICE;            /* default output device */
    if (outputParameters.device == paNoDevice) {
        return;
    }
    outputParameters.channelCount = config.numOutputChannels;
    outputParameters.sampleFormat = OUTPUT_FORMAT | (config.isOutputInterleaved ? 0 : paNonInterleaved);
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    err = Pa_OpenStream(
              &stream,
              &inputParameters,
              &outputParameters,
              SAMPLE_RATE,
              config.framesPerCallback, /* frames per buffer */
              paClipOff, /* we won't output out of range samples so don't bother clipping them */
			  this->wireCallback,
              this );
    if( err != paNoError ) return;
    
    err = Pa_StartStream( stream );
}