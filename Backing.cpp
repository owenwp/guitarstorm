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
#include "Backing.h"

int Backing::sampleRate = 48000;

double scale = 0.00005;

void Backing::Open(string f)
{
	name = f.substr(f.find_last_of("/")+1);
	if(valid)
		Close();

	file.open(f.c_str(),ios::binary);
	valid = !file.fail();
	int err = 0;

	if(!valid)
		return;

	mad_stream_init(&Stream);
	mad_frame_init(&Frame);
	mad_synth_init(&Synth);
	mad_timer_reset(&Timer);
	Sampler = src_new(SRC_LINEAR, 2, &err);

	GuardPtr=NULL;
	FrameCount=0;
	Status=0;
	position = 0;
	end = 0;
	time = 0;
}

void Backing::Close()
{
	if(valid)
	{
		file.close();
		file.clear();

		mad_synth_finish(&Synth);
		mad_frame_finish(&Frame);
		mad_stream_finish(&Stream);
		src_delete(Sampler);
	}
	valid = false;
}

void Backing::GetSamples(int buf[], int num)
{
	if(!valid)
		return;

	for(int j=0; j<num; j++)
	{
		if(position >= end)
			DecodeFrame();

		buf[j] = position >= end ? 0 : (int)OutputBuffer[position++];
	}
	time += num / (double) sampleRate;
}

double Backing::GetTime()
{
	return time;
}

void Backing::Advance(float t)
{
	if(!valid)
		return;

	int num = 2 * t * sampleRate;

	for(int j=0; j<num; j++)
	{
		if(position >= end)
			DecodeFrame();

		if(position < end)
			position++;
	}
	time += (double)t;
}

void Backing::DecodeFrame()
{
	if(Stream.buffer==NULL || Stream.error==MAD_ERROR_BUFLEN)
	{
		size_t			ReadSize,
						Remaining;
		unsigned char	*ReadStart;
	
		if(Stream.next_frame!=NULL)
		{
			Remaining=Stream.bufend-Stream.next_frame;
			memmove(InputBuffer,Stream.next_frame,Remaining);
			ReadStart=InputBuffer+Remaining;
			ReadSize=INPUT_BUFFER_SIZE-Remaining;
		}
		else
		{
			ReadSize=INPUT_BUFFER_SIZE;
			ReadStart=InputBuffer;
			Remaining=0;
		}

		unsigned char *read = ReadStart;
		for(int j=0; j<ReadSize; j++)
		{
			char c;
			file.read(&c,1);
			*read = (unsigned char)c;
			read++;
		}

		if(ReadSize<=0)
		{
			return;
		}

		file.peek();
		if(file.eof())
		{
			GuardPtr=ReadStart+ReadSize;
			memset(GuardPtr,0,MAD_BUFFER_GUARD);
			ReadSize+=MAD_BUFFER_GUARD;
		}

		mad_stream_buffer(&Stream,InputBuffer,ReadSize+Remaining);
		Stream.error=MAD_ERROR_NONE;
	}

	double sr = sampleRate;

	// decode
	if(mad_frame_decode(&Frame,&Stream))
	{
		if(MAD_RECOVERABLE(Stream.error))
		{
			if(Stream.error!=MAD_ERROR_LOSTSYNC ||
			   Stream.this_frame!=GuardPtr)
			{
			}
			return;
		}
		else
			if(Stream.error==MAD_ERROR_BUFLEN)
				DecodeFrame();
			else
			{
				Status=1;
				valid = false;
				return;
			}
	}

	if(FrameCount==0)
	{
	}

	FrameCount++;
	mad_timer_add(&Timer,Frame.header.duration);

	mad_synth_frame(&Synth,&Frame);

	sr = (double)Synth.pcm.samplerate;
	position = 0;
	end = Synth.pcm.length * 2;
	int j = 0;
	 
	for(int i=0;i<Synth.pcm.length;i++)
	{
		double	Sample;

		/* Left channel */
		Sample=Synth.pcm.samples[0][i];
		TempBuffer[j++]=Sample*scale;

		/* Right channel. If the decoded stream is monophonic then
		 * the right output channel is the same as the left one.
		 */
		if(MAD_NCHANNELS(&Frame.header)==2)
			Sample=Synth.pcm.samples[1][i];
		TempBuffer[j++]=Sample*scale;
	}

	if(sr != sampleRate)
	{
		SRC_DATA data;
		data.data_in = TempBuffer;
		data.data_out = OutputBuffer;
		data.input_frames = end;
		data.output_frames = OUTPUT_BUFFER_SIZE;
		data.src_ratio = sampleRate / sr;
		data.end_of_input = 0;

		src_process(Sampler, &data);

		end = data.output_frames_gen;
	}
	else
	{
		for(int k=0; k<end; k++)
			OutputBuffer[k] = TempBuffer[k];
	}
}
