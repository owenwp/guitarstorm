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
#include "Song.h"

int vmaj = 0;
int vmin = 0;

ostream& operator<<(ostream &stream, Song ob)
{
	stream << "Song";

	// do serialization
	stream << endl << ob.versionMaj;
	stream << endl << ob.versionMin;

	stream << endl << ob.title.c_str();
	stream << endl << ob.artist.c_str();
	stream << endl << ob.backing.c_str();
	stream << endl << ob.path.c_str();
	stream << endl << ob.difficulty[0];
	stream << endl << ob.difficulty[1];
	stream << endl << ob.difficulty[2];
	stream << endl << ob.difficulty[3];

	return stream;
}

istream& operator>>(istream &stream, Song &ob)
{
	char str[500];
	stream >> str;
	if(strncmp(str, "Song", 30))
		return stream;

	// do serialization
	stream >> vmaj;
	stream >> vmin;

	if(vmaj == Song::versionMaj && vmin == Song::versionMin)
	{
		stream.ignore();
		stream.getline(str, 500);
		ob.title = str;
		stream.getline(str, 500);
		ob.artist = str;
		stream.getline(str, 500);
		ob.backing = str;
		stream.getline(str, 500);
		ob.path = str;

		stream >> ob.difficulty[0];
		stream >> ob.difficulty[1];
		stream >> ob.difficulty[2];
		stream >> ob.difficulty[3];
	}

	return stream;
}

ostream& operator<<(ostream &stream, Difficulty ob)
{
	stream << endl << ob.name.c_str();
	if(ob.used.length())
	{
		stream << endl << 1;
		stream << endl << ob.track1;
		stream << endl << ob.track2;
		stream << endl << ob.offset;
	}
	else
		stream << endl << 0;

	return stream;
}

istream& operator>>(istream &stream, Difficulty &ob)
{
	char str[30];
	int num;
	stream >> str;
	ob.name = str;

	stream >> num;
	if(!num)
		return stream;
	else
	{
		ob.used = "true";
	}
	stream >> ob.track1;
	stream >> ob.track2;
	stream >> ob.offset;
	
	return stream;
}
