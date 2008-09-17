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
#ifndef BACKING
#define BACKING

#include "mad.h"
#include "samplerate/samplerate.h"
#include <fstream>
#include <string>

using namespace std;

class Backing
{
public:
	Backing(string f) {Open(f);}
	Backing():valid(false){}

	void Open(string f);
	void Close();
	bool Valid(){return valid;}
	void GetSamples(int buf[], const int num);
	double GetTime();
	void Advance(float t);
	static void SampleRate(int s) {sampleRate = s;}

private:
	void DecodeFrame();
	string name;
	bool valid;
	double time;

	mad_stream		Stream;
	mad_frame		Frame;
	mad_synth		Synth;
	mad_timer_t		Timer;
	ifstream		file;
	unsigned long	FrameCount;
	int				Status;
	SRC_STATE*		Sampler;

	const static int INPUT_BUFFER_SIZE = (5*8192);
	const static int TEMP_BUFFER_SIZE = 8192;
	const static int OUTPUT_BUFFER_SIZE = (5*8192);

	unsigned char	InputBuffer[INPUT_BUFFER_SIZE+MAD_BUFFER_GUARD];
	float			TempBuffer[TEMP_BUFFER_SIZE];
	float			OutputBuffer[OUTPUT_BUFFER_SIZE];
	unsigned char	*GuardPtr;

	static int sampleRate;
	int position;
	int end;
};

#endif
