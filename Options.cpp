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
#include "Options.h"

#if MACOSX
string optionsfile = string(getenv("HOME")) + "/Library/Preferences/GuitarStorm.dat";
#else 
string optionsfile = "options.dat";
#endif

Options* Options::instance = new Options;

// default values
Options::Options()
{
	// audio
	guitarVolume = 100;
	backingVolume = 75;
	metronomeVolume = 50;
	inputDevice = 0;
	outputDevice = 0;
	bufferSize = 2;
	sampleRate = 2;

	// video
	screenResolution = 1;
	fullScreen = 0;
	detailLevel = 1;

	// game
	stringColors[0] = 0;
	stringColors[1] = 0;
	stringColors[2] = 0;
	stringColors[3] = 0;
	stringColors[4] = 0;
	stringColors[5] = 0;
	//defaultDistortion = NULL;
	//defaultStompbox = NULL;

	// directories
	songDir = "songs";
	tabDir = "tabs";
	fxDir = "effects";
	backingDir = "backing";

	// player
	userName = "default";
	savePassword = 0;
	autoLogin = 0;

	// enumerations
	resolutions = new string[10];
	resolutionx = new int[10];
	resolutiony = new int[10];
	resolutions[0] = "800x600";
	resolutionx[0] = 800;
	resolutiony[0] = 600;

	resolutions[1] = "1024x768";
	resolutionx[1] = 1024;
	resolutiony[1] = 768;

	resolutions[2] = "1280x720";
	resolutionx[2] = 1280;
	resolutiony[2] = 720;

	resolutions[3] = "1280x986";
	resolutionx[3] = 1280;
	resolutiony[3] = 986;

	resolutions[4] = "1280x1024";
	resolutionx[4] = 1280;
	resolutiony[4] = 1024;

	resolutions[5] = "1920x1080";
	resolutionx[5] = 1920;
	resolutiony[5] = 1080;

	noyes = new string[10];
	noyes[0] = "No";
	noyes[1] = "Yes";
	srates = new string[10];
	srates[0] = "22400";
	srates[1] = "44100";
	srates[2] = "48000";
	srates[3] = "96000";
	bsizes = new string[10];
	bsizes[0] = "32";
	bsizes[1] = "64";
	bsizes[2] = "128";
	bsizes[3] = "256";
	bsizes[4] = "512";
	bsizes[5] = "1024";
	details = new string[10];
	details[0] = "Low";
	details[1] = "Normal";
	details[2] = "High";
	details[3] = "Ultra";
	devices = NULL;

	// load settings
	Load();
}

void Options::Save()
{
	ofstream out;
	out.open(optionsfile.c_str(), ios_base::binary);

	if(out.fail())
		throw string("Failed to open options.dat for writing\n");

	out << "Options";
	out << endl << versionMaj << endl << versionMin;

	// audio
	out << endl << guitarVolume;
	out << endl << backingVolume;
	out << endl << metronomeVolume;
	out << endl << inputDevice;
	out << endl << outputDevice;
	out << endl << bufferSize;
	out << endl << sampleRate;

	// video
	out << endl << screenResolution;
	out << endl << fullScreen;
	out << endl << detailLevel;

	// game
	out << endl << stringColors[0];
	out << endl << stringColors[1];
	out << endl << stringColors[2];
	out << endl << stringColors[3];
	out << endl << stringColors[4];
	out << endl << stringColors[5];
	//out << endl << defaultDistortion;
	//out << endl << defaultStompbox;

	// directories
	out << endl << songDir.c_str();
	out << endl << tabDir.c_str();
	out << endl << fxDir.c_str();
	out << endl << backingDir.c_str();

	// player
	out << endl << userName.c_str();
	out << endl << passHash[0];
	out << endl << passHash[1];
	out << endl << passHash[2];
	out << endl << passHash[3];
	out << endl << savePassword;
	out << endl << autoLogin;

	out.close();
}

void Options::Load()
{
	ifstream in;
	in.open(optionsfile.c_str(), ios_base::binary);

	if(in.fail())
		return;

	char opt[10];
	in >> opt;
	if(strncmp(opt, "Options", 10) != 0)
		return;

	// must use different loading routines for different versions
	int vMaj, vMin;
	in >> vMaj >> vMin;
	if(vMaj == 0 && vMin == 11)
	{
		// audio
		in >> guitarVolume;
		in >> backingVolume;
		in >> metronomeVolume;
		in >> inputDevice;
		in >> outputDevice;
		in >> bufferSize;
		in >> sampleRate;

		// video
		in >> screenResolution;
		in >> fullScreen;
		in >> detailLevel;

		// game
		in >> stringColors[0];
		in >> stringColors[1];
		in >> stringColors[2];
		in >> stringColors[3];
		in >> stringColors[4];
		in >> stringColors[5];
		//in >> defaultDistortion;
		//in >> defaultStompbox;

		// directories
		in.ignore();
		char dir[100];
		in.getline(dir,100);
		songDir = dir;
		in.getline(dir,100);
		tabDir = dir;
		in.getline(dir,100);
		fxDir = dir;
		in.getline(dir,100);
		backingDir = dir;

		// player
		char name[100];
		in >> name;
		userName = name;
		in >> passHash[0];
		in >> passHash[1];
		in >> passHash[2];
		in >> passHash[3];
		in >> savePassword;
		in >> autoLogin;
	}

	in.close();
}

void OptionsMenu::OnOpen()
{
	changed = false;
	if(firstOpen && parent)
	{
		Add(new BackItem("Cancel", -2, -4));
		Add(new BackItem("OK", -2, -4.5));
		//Add(new NullItem("Apply", -5, -12));
	}
}

void OptionsMenu::OnSelect()
{
	if(changed)
	{
		if(items[select]->name == "Cancel")
			Options::instance->Load();
		else if(items[select]->name == "OK" || items[select]->name == "Apply")
		{
			Options::instance->Save();
			if(applyMethod) applyMethod();
		}
	}
}
