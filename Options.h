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
#ifndef OPTIONS
#define OPTIONS

#include <iostream>
#include <fstream>
#include "Menu.h"

using namespace std;

struct Options
{
public:
	// singleton
	static Options* instance;

	// audio
	int guitarVolume;
	int backingVolume;
	int metronomeVolume;
	int inputDevice;
	int outputDevice;
	int bufferSize;
	int sampleRate;

	// video
	int screenResolution;
	int fullScreen;
	int detailLevel;

	// game
	int stringColors[6];
	//Effect* defaultDistortion
	//Effect* defaultStompbox;

	// directories
	string songDir;
	string backingDir;
	string tabDir;
	string fxDir;

	// player
	string userName;
	int passHash[4];
	int savePassword;
	int autoLogin;

	// enumarations
	string *resolutions;
	int    *resolutionx;
	int    *resolutiony;
	string *noyes;
	string *srates;
	string *bsizes;
	string *details;
	string *devices;

	void Save();
	void Load();

private:
	static const int versionMaj = 0;
	static const int versionMin = 11;

	Options();
};

class OptionsMenu : public Menu
{
public:
	OptionsMenu(string n, void (*a)() = NULL):Menu(n),applyMethod(a){}

private:
	void OnOpen();
	void OnSelect();
	void OnValueChange() {changed = true;}

	void (*applyMethod)();
	bool changed;
};

#endif
