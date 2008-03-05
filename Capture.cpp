/*
 * Copyright (c) 2006, Creative Labs Inc.
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided
 * that the following conditions are met:
 * 
 *     * Redistributions of source code must retain the above copyright notice, this list of conditions and
 * 	     the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright notice, this list of conditions
 * 	     and the following disclaimer in the documentation and/or other materials provided with the distribution.
 *     * Neither the name of Creative Labs Inc. nor the names of its contributors may be used to endorse or
 * 	     promote products derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "Capture.h"

#define BUFFERSIZE				22050

void Capture::stop()
{
	stopped = true;
}

void Capture::run()
{
	ALCdevice		*pDevice;
	ALCcontext		*pContext;
	ALCdevice		*pCaptureDevice;
	const ALCchar	*szDefaultCaptureDevice;
	ALint			iSamplesAvailable;
	ALchar			Data[BUFFERSIZE];
	ALuint			freq = 22050;
	const ALuint	bcount = 4;

	ALuint		    uiSource;
	ALuint			uiBuffer[bcount];

	// Initialize Framework
	ALFWInit();
	if (!ALFWInitOpenAL())
	{
		ALFWprintf("Failed to initialize OpenAL\n");
		ALFWShutdown();
		return;
	}
	
    // Generate some AL Buffers for streaming
	alGenBuffers( bcount, uiBuffer );
	alGenSources( 1, &uiSource );

	// Check for Capture Extension support
	pContext = alcGetCurrentContext();
	pDevice = alcGetContextsDevice(pContext);
	if (alcIsExtensionPresent(pDevice, "ALC_EXT_CAPTURE") == AL_FALSE)
	{
		ALFWprintf("Failed to detect Capture Extension\n");
		ALFWShutdownOpenAL();
		ALFWShutdown();
		return;
	}

	// Get the name of the 'default' capture device
	szDefaultCaptureDevice = alcGetString(NULL, ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER);
	ALFWprintf("\nDefault Capture Device is '%s'\n\n", szDefaultCaptureDevice);
	pCaptureDevice = alcCaptureOpenDevice(szDefaultCaptureDevice, freq, AL_FORMAT_MONO16, BUFFERSIZE*2);
	if (pCaptureDevice)
	{
		ALFWprintf("Opened '%s' Capture Device\n\n", alcGetString(pCaptureDevice, ALC_CAPTURE_DEVICE_SPECIFIER));

		/* Setup some initial silent data to play out of the source */
		memset(Data, 0, BUFFERSIZE);
		for(int j=0; j<bcount; j++)
		{
			alBufferData(uiBuffer[j], AL_FORMAT_MONO16, Data, 10, freq);
		}
		alSourceQueueBuffers(uiSource, bcount, uiBuffer);
		alSourcePlay(uiSource);

		// Start audio capture
		alcCaptureStart(pCaptureDevice);
		stopped = false;

		// Record and play back
		while (!stopped)
		{
			// Find out how many samples have been captured
			alcGetIntegerv(pCaptureDevice, ALC_CAPTURE_SAMPLES, 1, &iSamplesAvailable);
			iSamplesAvailable = min(BUFFERSIZE, iSamplesAvailable);

			if(iSamplesAvailable <= 0)
				continue;

			// Consume Samples
			alcCaptureSamples(pCaptureDevice, Data, iSamplesAvailable);

			// buffer data
			ALuint buf;
			alSourceUnqueueBuffers(uiSource, 1, &buf);
			alBufferData(buf, AL_FORMAT_MONO16, Data, iSamplesAvailable, freq);
			alSourceQueueBuffers(uiSource, 1, &buf);

			 /* Make sure the source is still playing */
			 ALint val;
			 alGetSourcei(uiSource, AL_SOURCE_STATE, &val);
			 if(val != AL_PLAYING)
				   alSourcePlay(uiSource);;
		}

		// Stop capture
		alcCaptureStop(pCaptureDevice);

		// Close the Capture Device
		alcCaptureCloseDevice(pCaptureDevice);

		// Stop the Source and clear the Queue
		alSourceStop(uiSource);
		alSourcei(uiSource, AL_BUFFER, 0);
	}

	// Clean up buffers and sources
	alDeleteSources( 1, &uiSource );
	alDeleteBuffers( bcount, uiBuffer );

	// Close down OpenAL
	ALFWShutdownOpenAL();

	// Close down the Framework
	ALFWShutdown();
}