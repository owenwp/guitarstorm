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
#ifndef SONG
#define SONG

#include <iostream>
#include <fstream>
#include <string>
#include <map>

using namespace std;

struct Difficulty
{
public:
	Difficulty():offset(0.0f){}
	string name;
	string tab;
	float offset;
	string used;

	int track1;
	int track2;

	// serialization
	friend ostream& operator<<(ostream &stream, Difficulty ob);
	friend istream& operator>>(istream &stream, Difficulty &ob);
};

struct Song
{
public:
	static map<string, Song> list;

	Difficulty difficulty[4];

	string title;
	string artist;

	string backing;
	string path;

	//Effect *distortion1;
	//Effect *distortion2;
	//Effect *stompBox;

	// serialization
	friend ostream& operator<<(ostream &stream, Song ob);
	friend istream& operator>>(istream &stream, Song &ob);

private:
	static const int versionMaj = 0;
	static const int versionMin = 2;
};

ostream& operator<<(ostream &stream, Song ob);
istream& operator>>(istream &stream, Song &ob);

ostream& operator<<(ostream &stream, Difficulty ob);
istream& operator>>(istream &stream, Difficulty &ob);

#endif