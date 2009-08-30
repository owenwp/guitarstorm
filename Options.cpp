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
#include "yaml/yaml.h"

using namespace YAML;

#if MACOSX
string optionsfile = string(getenv("HOME")) + "/Library/Preferences/GuitarStorm.dat";
string optionsymlfile = string(getenv("HOME")) + "/Library/Preferences/GuitarStorm.yml";
#else 
string optionsfile = "options.dat";
string optionsfile = "options.yml";
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
#if MACOSX
	songDir = string(getenv("HOME")) + "/Library/Application Support/Guitar Storm/songs";
	tabDir = string(getenv("HOME")) + "/Library/Application Support/Guitar Storm/tabs";
	fxDir = string(getenv("HOME")) + "/Library/Application Support/Guitar Storm/effects";
	backingDir = string(getenv("HOME")) + "/Music";
#else
	songDir = "songs";
	tabDir = "tabs";
	fxDir = "effects";
	backingDir = "backing";
#endif

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

	resolutions[1] = "1024x640";
	resolutionx[1] = 1024;
	resolutiony[1] = 640;
	
	resolutions[2] = "1024x768";
	resolutionx[2] = 1024;
	resolutiony[2] = 768;

	resolutions[3] = "1280x720";
	resolutionx[3] = 1280;
	resolutiony[3] = 720;

	resolutions[4] = "1280x800";
	resolutionx[4] = 1280;
	resolutiony[4] = 800;

	resolutions[5] = "1280x1024";
	resolutionx[5] = 1280;
	resolutiony[5] = 1024;

	resolutions[6] = "1920x1080";
	resolutionx[6] = 1920;
	resolutiony[6] = 1080;

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
	devices = NULL;

	// load settings
	Load();
}

void Options::Save()
{
	Emitter out;

	out << BeginMap;
	
	out << Key << "Audio" << Value << BeginMap;
	{
		out << Key << "GuitarVolume" << Value << guitarVolume;
		out << Key << "BackingVolume" << Value << backingVolume;
		out << Key << "MetronomeVolume" << Value << metronomeVolume;
		out << Key << "InputDevice" << Value << inputDevice;
		out << Key << "OutputDevice" << Value << outputDevice;
		out << Key << "BufferSize" << Value << bufferSize;
		out << Key << "SampleRate" << Value << sampleRate;
	}
	out << EndMap;

	out << Key << "Video" << Value << BeginMap;
	{
		out << Key << "Resolution" << Value << screenResolution;
		out << Key << "Fullscreen" << Value << fullScreen;
		out << Key << "Detail" << Value << detailLevel;
	}
	out << EndMap;

	out << Key << "Game" << Value << BeginMap;
	{
		out << Key << "StringColors" << Value << Flow << BeginSeq;
		{
			out << stringColors[0] << stringColors[1] << stringColors[2];
			out << stringColors[3] << stringColors[4] << stringColors[5];
		}
		out << EndSeq;
		//out << Key << "DefaultDistortion" << Value << defaultDistortion;
		//out << Key << "DefaultStompBox" << Value << defaultStompbox;
	}
	out << EndMap;

	// directories
	out << Key << "Directories" << Value << BeginMap;
	{
		out << Key << "Songs" << Value << songDir;
		out << Key << "Tabs" << Value << tabDir;
		out << Key << "FX" << Value << fxDir;
		out << Key << "Backing" << Value << backingDir;
	}
	out << EndMap;

	// player
	out << Key << "Player" << Value << BeginMap;
	{
		out << Key << "UserName" << Value << userName;
		out << Key << "PasswordHash" << Value << Flow << BeginSeq;
		{
			out << passHash[0] << passHash[1] << passHash[2] << passHash[3];
		}
		out << EndSeq;
		out << Key << "SavePassword" << Value << savePassword;
		out << Key << "AutoLogin" << Value << autoLogin;
	}
	out << EndMap;

	out << EndMap;
	
	if(out.good());
	{
		ofstream o(optionsymlfile.c_str(), ios::binary);
		
		if(!o.fail())
		{
			o.write(out.c_str(), out.size());
		}
	}
}

void Options::Load()
{
	ifstream i(optionsymlfile.c_str(), ios::binary);
	
	if(i.fail())
		return;
	
	YAML::Node in;
	try
	{
		Parser parser(i);
		parser.GetNextDocument(in);
	} 
	catch(ParserException& e) 
	{
		cout << "Error Loading Options: " << e.what() << "\n";
		return;
	}
	
	// audio
	in["Audio"]["GuitarVolume"] >> guitarVolume;
	in["Audio"]["BackingVolume"] >> backingVolume;
	in["Audio"]["MetronomeVolume"] >> metronomeVolume;
	in["Audio"]["InputDevice"] >> inputDevice;
	in["Audio"]["OutputDevice"] >> outputDevice;
	in["Audio"]["BufferSize"] >> bufferSize;
	in["Audio"]["SampleRate"] >> sampleRate;

	// video
	in["Video"]["Resolution"] >> screenResolution;
	in["Video"]["Fullscreen"] >> fullScreen;
	in["Video"]["Detail"] >> detailLevel;

	// game
	in["Game"]["StringColors"][0] >> stringColors[0];
	in["Game"]["StringColors"][1] >> stringColors[1];
	in["Game"]["StringColors"][2] >> stringColors[2];
	in["Game"]["StringColors"][3] >> stringColors[3];
	in["Game"]["StringColors"][4] >> stringColors[4];
	in["Game"]["StringColors"][5] >> stringColors[5];
	//in["Game"]["DefaultDistortion"] >> defaultDistortion;
	//in["Game"]["DefaultStompBox"] >> defaultStompbox;

	// directories
	in["Directories"]["Songs"] >> songDir;
	in["Directories"]["Tabs"] >> tabDir;
	in["Directories"]["FX"] >> fxDir;
	in["Directories"]["Backing"] >> backingDir;

	// player
	in["Player"]["UserName"] >> userName;
	in["Player"]["PasswordHash"][0] >> passHash[0];
	in["Player"]["PasswordHash"][1] >> passHash[1];
	in["Player"]["PasswordHash"][2] >> passHash[2];
	in["Player"]["PasswordHash"][3] >> passHash[3];
	in["Player"]["SavePassword"] >> savePassword;
	in["Player"]["AutoLogin"] >> autoLogin;
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
			if(!Options::instance->fullScreen)
			{
				int resx = Options::instance->resolutionx[Options::instance->screenResolution];
				int resy = Options::instance->resolutiony[Options::instance->screenResolution];
				
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				
				gluPerspective(45, (float)resx / (float)resy, 1, 20);
				
				glMatrixMode(GL_MODELVIEW);
				glutReshapeWindow(resx, resy);
			}
			Options::instance->Save();
			if(applyMethod) applyMethod();
		}
	}
}
